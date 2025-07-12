#include "include/memory_helpers.h"
#include "include/error_handler.h"
#include <psapi.h>
#include <tlhelp32.h>
#include <regex>
#include <algorithm>
#include <iomanip>
#include <sstream>

// Pointer finding operations
std::vector<PointerResult> MemoryHelpers::FindPointersTo(HANDLE processHandle, uintptr_t targetAddress, 
                                                         uintptr_t searchStart, uintptr_t searchEnd) {
    std::vector<PointerResult> results;
    
    try {
        auto regions = GetMemoryRegions(processHandle, false, false);
        
        for (const auto& region : regions) {
            uintptr_t start = (searchStart > 0) ? std::max(searchStart, region.startAddress) : region.startAddress;
            uintptr_t end = (searchEnd > 0) ? std::min(searchEnd, region.endAddress) : region.endAddress;
            
            if (start >= end) continue;
            
            size_t regionSize = end - start;
            if (regionSize > MAX_SCAN_SIZE) continue;
            
            auto buffer = ReadMemoryChunk(processHandle, start, regionSize);
            if (buffer.empty()) continue;
            
            // Search for pointers (assuming pointer size is sizeof(uintptr_t))
            for (size_t i = 0; i <= buffer.size() - sizeof(uintptr_t); i += sizeof(uintptr_t)) {
                uintptr_t* ptrValue = reinterpret_cast<uintptr_t*>(&buffer[i]);
                
                if (*ptrValue == targetAddress) {
                    PointerResult result;
                    result.address = start + i;
                    result.pointsTo = targetAddress;
                    result.offset = 0;
                    results.push_back(result);
                    
                    if (results.size() >= MAX_RESULTS) break;
                }
            }
            
            if (results.size() >= MAX_RESULTS) break;
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("FindPointersTo failed: " + std::string(e.what()));
    }
    
    return results;
}

std::vector<PointerResult> MemoryHelpers::FindPointerChain(HANDLE processHandle, uintptr_t finalAddress, 
                                                           int maxDepth, const std::vector<size_t>& offsets) {
    std::vector<PointerResult> results;
    
    try {
        // Start by finding direct pointers to the final address
        auto directPointers = FindPointersTo(processHandle, finalAddress);
        
        for (const auto& directPtr : directPointers) {
            PointerResult result = directPtr;
            results.push_back(result);
        }
        
        // If offsets are provided, search for pointer chains
        if (!offsets.empty() && maxDepth > 1) {
            for (int depth = 1; depth < maxDepth; ++depth) {
                std::vector<PointerResult> newResults;
                
                for (const auto& existingPtr : results) {
                    // Find pointers to this pointer's address
                    auto chainPointers = FindPointersTo(processHandle, existingPtr.address);
                    
                    for (const auto& chainPtr : chainPointers) {
                        PointerResult chainResult;
                        chainResult.address = chainPtr.address;
                        chainResult.pointsTo = finalAddress;
                        chainResult.offset = depth < offsets.size() ? offsets[depth] : 0;
                        newResults.push_back(chainResult);
                    }
                }
                
                results.insert(results.end(), newResults.begin(), newResults.end());
                if (results.size() >= MAX_RESULTS) break;
            }
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("FindPointerChain failed: " + std::string(e.what()));
    }
    
    return results;
}

std::vector<uintptr_t> MemoryHelpers::FindNullPointers(HANDLE processHandle, uintptr_t searchStart, 
                                                       uintptr_t searchEnd) {
    std::vector<uintptr_t> results;
    
    try {
        auto regions = GetMemoryRegions(processHandle, false, false);
        
        for (const auto& region : regions) {
            uintptr_t start = std::max(searchStart, region.startAddress);
            uintptr_t end = std::min(searchEnd, region.endAddress);
            
            if (start >= end) continue;
            
            size_t regionSize = end - start;
            if (regionSize > MAX_SCAN_SIZE) continue;
            
            auto buffer = ReadMemoryChunk(processHandle, start, regionSize);
            if (buffer.empty()) continue;
            
            for (size_t i = 0; i <= buffer.size() - sizeof(uintptr_t); i += sizeof(uintptr_t)) {
                uintptr_t* ptrValue = reinterpret_cast<uintptr_t*>(&buffer[i]);
                
                if (*ptrValue == 0) {
                    results.push_back(start + i);
                    if (results.size() >= MAX_RESULTS) break;
                }
            }
            
            if (results.size() >= MAX_RESULTS) break;
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("FindNullPointers failed: " + std::string(e.what()));
    }
    
    return results;
}

// Enhanced pattern searching
std::vector<PatternMatch> MemoryHelpers::SearchBytePattern(HANDLE processHandle, const std::vector<uint8_t>& pattern, 
                                                          const std::vector<bool>& mask, uintptr_t searchStart, 
                                                          uintptr_t searchEnd) {
    std::vector<PatternMatch> results;
    
    if (pattern.empty() || mask.size() != pattern.size()) {
        return results;
    }
    
    try {
        auto regions = GetMemoryRegions(processHandle, false, false);
        
        for (const auto& region : regions) {
            uintptr_t start = (searchStart > 0) ? std::max(searchStart, region.startAddress) : region.startAddress;
            uintptr_t end = (searchEnd > 0) ? std::min(searchEnd, region.endAddress) : region.endAddress;
            
            if (start >= end) continue;
            
            size_t regionSize = end - start;
            if (regionSize > MAX_SCAN_SIZE) continue;
            
            auto buffer = ReadMemoryChunk(processHandle, start, regionSize);
            if (buffer.empty() || buffer.size() < pattern.size()) continue;
            
            for (size_t i = 0; i <= buffer.size() - pattern.size(); ++i) {
                if (MatchesPattern(&buffer[i], pattern, mask)) {
                    PatternMatch match;
                    match.address = start + i;
                    match.size = pattern.size();
                    match.data.assign(buffer.begin() + i, buffer.begin() + i + pattern.size());
                    results.push_back(match);
                    
                    if (results.size() >= MAX_RESULTS) break;
                }
            }
            
            if (results.size() >= MAX_RESULTS) break;
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("SearchBytePattern failed: " + std::string(e.what()));
    }
    
    return results;
}

std::vector<PatternMatch> MemoryHelpers::SearchStringPattern(HANDLE processHandle, const std::string& pattern, 
                                                            bool caseSensitive, bool unicode,
                                                            uintptr_t searchStart, uintptr_t searchEnd) {
    std::vector<PatternMatch> results;
    
    if (pattern.empty()) return results;
    
    try {
        auto regions = GetMemoryRegions(processHandle, false, false);
        
        for (const auto& region : regions) {
            uintptr_t start = (searchStart > 0) ? std::max(searchStart, region.startAddress) : region.startAddress;
            uintptr_t end = (searchEnd > 0) ? std::min(searchEnd, region.endAddress) : region.endAddress;
            
            if (start >= end) continue;
            
            size_t regionSize = end - start;
            if (regionSize > MAX_SCAN_SIZE) continue;
            
            auto buffer = ReadMemoryChunk(processHandle, start, regionSize);
            if (buffer.empty()) continue;
            
            std::string searchStr = pattern;
            if (!caseSensitive) {
                std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);
            }
            
            if (unicode) {
                // Search for Unicode (UTF-16) strings
                std::wstring widePattern(pattern.begin(), pattern.end());
                const uint8_t* wideBytes = reinterpret_cast<const uint8_t*>(widePattern.c_str());
                size_t wideSize = widePattern.size() * sizeof(wchar_t);
                
                for (size_t i = 0; i <= buffer.size() - wideSize; i += 2) {
                    if (memcmp(&buffer[i], wideBytes, wideSize) == 0) {
                        PatternMatch match;
                        match.address = start + i;
                        match.size = wideSize;
                        match.data.assign(buffer.begin() + i, buffer.begin() + i + wideSize);
                        results.push_back(match);
                        
                        if (results.size() >= MAX_RESULTS) break;
                    }
                }
            } else {
                // Search for ASCII strings
                for (size_t i = 0; i <= buffer.size() - pattern.size(); ++i) {
                    std::string candidate(buffer.begin() + i, buffer.begin() + i + pattern.size());
                    
                    if (!caseSensitive) {
                        std::transform(candidate.begin(), candidate.end(), candidate.begin(), ::tolower);
                    }
                    
                    if (candidate == searchStr) {
                        PatternMatch match;
                        match.address = start + i;
                        match.size = pattern.size();
                        match.data.assign(buffer.begin() + i, buffer.begin() + i + pattern.size());
                        results.push_back(match);
                        
                        if (results.size() >= MAX_RESULTS) break;
                    }
                }
            }
            
            if (results.size() >= MAX_RESULTS) break;
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("SearchStringPattern failed: " + std::string(e.what()));
    }
    
    return results;
}

std::vector<PatternMatch> MemoryHelpers::SearchRegexPattern(HANDLE processHandle, const std::string& regexPattern,
                                                           uintptr_t searchStart, uintptr_t searchEnd) {
    std::vector<PatternMatch> results;
    
    try {
        std::regex regex(regexPattern);
        auto regions = GetMemoryRegions(processHandle, false, false);
        
        for (const auto& region : regions) {
            uintptr_t start = (searchStart > 0) ? std::max(searchStart, region.startAddress) : region.startAddress;
            uintptr_t end = (searchEnd > 0) ? std::min(searchEnd, region.endAddress) : region.endAddress;
            
            if (start >= end) continue;
            
            size_t regionSize = end - start;
            if (regionSize > MAX_SCAN_SIZE) continue;
            
            auto buffer = ReadMemoryChunk(processHandle, start, regionSize);
            if (buffer.empty()) continue;
            
            // Convert buffer to string for regex matching
            std::string text(buffer.begin(), buffer.end());
            std::sregex_iterator iter(text.begin(), text.end(), regex);
            std::sregex_iterator end_iter;
            
            for (; iter != end_iter && results.size() < MAX_RESULTS; ++iter) {
                const std::smatch& match = *iter;
                
                PatternMatch patternMatch;
                patternMatch.address = start + match.position();
                patternMatch.size = match.length();
                patternMatch.data.assign(buffer.begin() + match.position(), 
                                       buffer.begin() + match.position() + match.length());
                results.push_back(patternMatch);
            }
            
            if (results.size() >= MAX_RESULTS) break;
        }
    }
    catch (const std::exception& e) {
        ErrorHandler::logError("SearchRegexPattern failed: " + std::string(e.what()));
    }
    
    return results;
}

// Numeric value searching
std::vector<NumericMatch> MemoryHelpers::SearchInt32(HANDLE processHandle, int32_t value, uintptr_t searchStart, 
                                                     uintptr_t searchEnd, bool aligned) {
    std::vector<NumericMatch> results;
    
    try {
        auto regions = GetMemoryRegions(processHandle, false, false);
        
        for (const auto& region : regions) {
            uintptr_t start = (searchStart > 0) ? std::max(searchStart, region.startAddress) : region.startAddress;
            uintptr_t end = (searchEnd > 0) ? std::min(searchEnd, region.endAddress) : region.endAddress;
            
            if (start >= end) continue;
            
            size_t regionSize = end - start;
            if (regionSize > MAX_SCAN_SIZE) continue;
            
            auto buffer = ReadMemoryChunk(processHandle, start, regionSize);
            if (buffer.empty() || buffer.size() < sizeof(int32_t)) continue;
            
            size_t step = aligned ? sizeof(int32_t) : 1;
            
            for (size_t i = 0; i <= buffer.size() - sizeof(int32_t); i += step) {
                int32_t* candidate = reinterpret_cast<int32_t*>(&buffer[i]);
                
                if (*candidate == value) {
                    NumericMatch match;
                    match.address = start + i;
                    match.dataType = "int32";
                    match.rawData.assign(buffer.begin() + i, buffer.begin() + i + sizeof(int32_t));
                    results.push_back(match);
                    
                    if (results.size() >= MAX_RESULTS) break;
                }
            }
            
            if (results.size() >= MAX_RESULTS) break;
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("SearchInt32 failed: " + std::string(e.what()));
    }
    
    return results;
}

std::vector<NumericMatch> MemoryHelpers::SearchInt64(HANDLE processHandle, int64_t value, uintptr_t searchStart, 
                                                     uintptr_t searchEnd, bool aligned) {
    std::vector<NumericMatch> results;
    
    try {
        auto regions = GetMemoryRegions(processHandle, false, false);
        
        for (const auto& region : regions) {
            uintptr_t start = (searchStart > 0) ? std::max(searchStart, region.startAddress) : region.startAddress;
            uintptr_t end = (searchEnd > 0) ? std::min(searchEnd, region.endAddress) : region.endAddress;
            
            if (start >= end) continue;
            
            size_t regionSize = end - start;
            if (regionSize > MAX_SCAN_SIZE) continue;
            
            auto buffer = ReadMemoryChunk(processHandle, start, regionSize);
            if (buffer.empty() || buffer.size() < sizeof(int64_t)) continue;
            
            size_t step = aligned ? sizeof(int64_t) : 1;
            
            for (size_t i = 0; i <= buffer.size() - sizeof(int64_t); i += step) {
                int64_t* candidate = reinterpret_cast<int64_t*>(&buffer[i]);
                
                if (*candidate == value) {
                    NumericMatch match;
                    match.address = start + i;
                    match.dataType = "int64";
                    match.rawData.assign(buffer.begin() + i, buffer.begin() + i + sizeof(int64_t));
                    results.push_back(match);
                    
                    if (results.size() >= MAX_RESULTS) break;
                }
            }
            
            if (results.size() >= MAX_RESULTS) break;
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("SearchInt64 failed: " + std::string(e.what()));
    }
    
    return results;
}

std::vector<NumericMatch> MemoryHelpers::SearchFloat(HANDLE processHandle, float value, float tolerance,
                                                    uintptr_t searchStart, uintptr_t searchEnd) {
    std::vector<NumericMatch> results;
    
    try {
        auto regions = GetMemoryRegions(processHandle, false, false);
        
        for (const auto& region : regions) {
            uintptr_t start = (searchStart > 0) ? std::max(searchStart, region.startAddress) : region.startAddress;
            uintptr_t end = (searchEnd > 0) ? std::min(searchEnd, region.endAddress) : region.endAddress;
            
            if (start >= end) continue;
            
            size_t regionSize = end - start;
            if (regionSize > MAX_SCAN_SIZE) continue;
            
            auto buffer = ReadMemoryChunk(processHandle, start, regionSize);
            if (buffer.empty() || buffer.size() < sizeof(float)) continue;
            
            for (size_t i = 0; i <= buffer.size() - sizeof(float); i += sizeof(float)) {
                float* candidate = reinterpret_cast<float*>(&buffer[i]);
                
                if (std::abs(*candidate - value) <= tolerance) {
                    NumericMatch match;
                    match.address = start + i;
                    match.dataType = "float";
                    match.rawData.assign(buffer.begin() + i, buffer.begin() + i + sizeof(float));
                    results.push_back(match);
                    
                    if (results.size() >= MAX_RESULTS) break;
                }
            }
            
            if (results.size() >= MAX_RESULTS) break;
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("SearchFloat failed: " + std::string(e.what()));
    }
    
    return results;
}

std::vector<NumericMatch> MemoryHelpers::SearchDouble(HANDLE processHandle, double value, double tolerance,
                                                     uintptr_t searchStart, uintptr_t searchEnd) {
    std::vector<NumericMatch> results;
    
    try {
        auto regions = GetMemoryRegions(processHandle, false, false);
        
        for (const auto& region : regions) {
            uintptr_t start = (searchStart > 0) ? std::max(searchStart, region.startAddress) : region.startAddress;
            uintptr_t end = (searchEnd > 0) ? std::min(searchEnd, region.endAddress) : region.endAddress;
            
            if (start >= end) continue;
            
            size_t regionSize = end - start;
            if (regionSize > MAX_SCAN_SIZE) continue;
            
            auto buffer = ReadMemoryChunk(processHandle, start, regionSize);
            if (buffer.empty() || buffer.size() < sizeof(double)) continue;
            
            for (size_t i = 0; i <= buffer.size() - sizeof(double); i += sizeof(double)) {
                double* candidate = reinterpret_cast<double*>(&buffer[i]);
                
                if (std::abs(*candidate - value) <= tolerance) {
                    NumericMatch match;
                    match.address = start + i;
                    match.dataType = "double";
                    match.rawData.assign(buffer.begin() + i, buffer.begin() + i + sizeof(double));
                    results.push_back(match);
                    
                    if (results.size() >= MAX_RESULTS) break;
                }
            }
            
            if (results.size() >= MAX_RESULTS) break;
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("SearchDouble failed: " + std::string(e.what()));
    }
    
    return results;
}

// Memory region analysis
std::vector<MemoryRegionEx> MemoryHelpers::GetMemoryRegions(HANDLE processHandle, bool executableOnly, bool writableOnly) {
    std::vector<MemoryRegionEx> regions;
    
    try {
        MEMORY_BASIC_INFORMATION mbi;
        uintptr_t address = 0;
        
        while (VirtualQueryEx(processHandle, reinterpret_cast<LPVOID>(address), &mbi, sizeof(mbi)) == sizeof(mbi)) {
            if (mbi.State == MEM_COMMIT) {
                bool includeRegion = true;
                
                if (executableOnly && !(mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
                    includeRegion = false;
                }
                
                if (writableOnly && !(mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE))) {
                    includeRegion = false;
                }
                
                if (includeRegion) {
                    MemoryRegionEx region;
                    region.startAddress = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
                    region.endAddress = region.startAddress + mbi.RegionSize;
                    region.size = mbi.RegionSize;
                    region.protection = mbi.Protect;
                    region.type = mbi.Type;
                    
                    // Try to get module name for this region
                    HMODULE hModule;
                    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, 
                                         reinterpret_cast<LPCTSTR>(mbi.BaseAddress), &hModule)) {
                        char moduleName[MAX_PATH];
                        if (GetModuleFileNameA(hModule, moduleName, MAX_PATH)) {
                            region.moduleName = moduleName;
                        }
                        FreeLibrary(hModule);
                    }
                    
                    regions.push_back(region);
                }
            }
            
            address = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("GetMemoryRegions failed: " + std::string(e.what()));
    }
    
    return regions;
}

std::vector<MemoryRegionEx> MemoryHelpers::GetModuleRegions(HANDLE processHandle, const std::string& moduleName) {
    std::vector<MemoryRegionEx> regions;
    
    try {
        auto allRegions = GetMemoryRegions(processHandle, false, false);
        
        for (const auto& region : allRegions) {
            if (moduleName.empty() || region.moduleName.find(moduleName) != std::string::npos) {
                regions.push_back(region);
            }
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("GetModuleRegions failed: " + std::string(e.what()));
    }
    
    return regions;
}

MemoryRegionEx MemoryHelpers::GetRegionInfo(HANDLE processHandle, uintptr_t address) {
    MemoryRegionEx region = {};
    
    try {
        MEMORY_BASIC_INFORMATION mbi;
        if (VirtualQueryEx(processHandle, reinterpret_cast<LPVOID>(address), &mbi, sizeof(mbi)) == sizeof(mbi)) {
            region.startAddress = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
            region.endAddress = region.startAddress + mbi.RegionSize;
            region.size = mbi.RegionSize;
            region.protection = mbi.Protect;
            region.type = mbi.Type;
            
            // Try to get module name
            HMODULE hModule;
            if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, 
                                 reinterpret_cast<LPCTSTR>(mbi.BaseAddress), &hModule)) {
                char moduleName[MAX_PATH];
                if (GetModuleFileNameA(hModule, moduleName, MAX_PATH)) {
                    region.moduleName = moduleName;
                }
                FreeLibrary(hModule);
            }
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("GetRegionInfo failed: " + std::string(e.what()));
    }
    
    return region;
}

// Advanced scanning utilities
std::vector<PatternMatch> MemoryHelpers::ScanForCode(HANDLE processHandle, const std::vector<uint8_t>& opcodes,
                                                    uintptr_t searchStart, uintptr_t searchEnd) {
    std::vector<bool> mask(opcodes.size(), true); // All bytes must match for code scanning
    return SearchBytePattern(processHandle, opcodes, mask, searchStart, searchEnd);
}

std::vector<uintptr_t> MemoryHelpers::FindFunctionPrologs(HANDLE processHandle, uintptr_t searchStart, uintptr_t searchEnd) {
    std::vector<uintptr_t> results;
    
    try {
        // Common x86/x64 function prologues
        std::vector<std::vector<uint8_t>> prologPatterns = {
            {0x55, 0x8B, 0xEC},                    // push ebp; mov ebp, esp (x86)
            {0x48, 0x89, 0x5C, 0x24},              // mov [rsp+XX], rbx (x64)
            {0x48, 0x83, 0xEC},                    // sub rsp, XX (x64)
            {0x55, 0x48, 0x89, 0xE5}               // push rbp; mov rbp, rsp (x64)
        };
        
        for (const auto& pattern : prologPatterns) {
            std::vector<bool> mask(pattern.size(), true);
            auto matches = SearchBytePattern(processHandle, pattern, mask, searchStart, searchEnd);
            
            for (const auto& match : matches) {
                results.push_back(match.address);
                if (results.size() >= MAX_RESULTS) break;
            }
            
            if (results.size() >= MAX_RESULTS) break;
        }
        
        // Remove duplicates and sort
        std::sort(results.begin(), results.end());
        results.erase(std::unique(results.begin(), results.end()), results.end());
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("FindFunctionPrologs failed: " + std::string(e.what()));
    }
    
    return results;
}

std::vector<uintptr_t> MemoryHelpers::FindCallInstructions(HANDLE processHandle, uintptr_t targetAddress,
                                                          uintptr_t searchStart, uintptr_t searchEnd) {
    std::vector<uintptr_t> results;
    
    try {
        auto regions = GetMemoryRegions(processHandle, true, false); // Executable regions only
        
        for (const auto& region : regions) {
            uintptr_t start = (searchStart > 0) ? std::max(searchStart, region.startAddress) : region.startAddress;
            uintptr_t end = (searchEnd > 0) ? std::min(searchEnd, region.endAddress) : region.endAddress;
            
            if (start >= end) continue;
            
            size_t regionSize = end - start;
            if (regionSize > MAX_SCAN_SIZE) continue;
            
            auto buffer = ReadMemoryChunk(processHandle, start, regionSize);
            if (buffer.empty()) continue;
            
            // Look for call instructions (0xE8 for relative call)
            for (size_t i = 0; i < buffer.size() - 5; ++i) {
                if (buffer[i] == 0xE8) { // Call instruction
                    // Calculate the target address of the call
                    int32_t* offset = reinterpret_cast<int32_t*>(&buffer[i + 1]);
                    uintptr_t callTarget = start + i + 5 + *offset; // instruction pointer + instruction size + offset
                    
                    if (callTarget == targetAddress) {
                        results.push_back(start + i);
                        if (results.size() >= MAX_RESULTS) break;
                    }
                }
            }
            
            if (results.size() >= MAX_RESULTS) break;
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("FindCallInstructions failed: " + std::string(e.what()));
    }
    
    return results;
}

// Memory comparison and diffing
std::vector<uintptr_t> MemoryHelpers::CompareMemoryRegions(HANDLE processHandle, uintptr_t region1Start, 
                                                          uintptr_t region2Start, size_t size) {
    std::vector<uintptr_t> differences;
    
    try {
        if (size > MAX_SCAN_SIZE) {
            throw MemoryHookingException(MemoryHookingError::InvalidSize, "Region size too large for comparison");
        }
        
        auto buffer1 = ReadMemoryChunk(processHandle, region1Start, size);
        auto buffer2 = ReadMemoryChunk(processHandle, region2Start, size);
        
        if (buffer1.size() != buffer2.size()) {
            throw MemoryHookingException(MemoryHookingError::InvalidSize, "Buffer sizes don't match");
        }
        
        for (size_t i = 0; i < buffer1.size(); ++i) {
            if (buffer1[i] != buffer2[i]) {
                differences.push_back(region1Start + i);
                if (differences.size() >= MAX_RESULTS) break;
            }
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("CompareMemoryRegions failed: " + std::string(e.what()));
    }
    
    return differences;
}

std::vector<uintptr_t> MemoryHelpers::FindChangedBytes(HANDLE processHandle, uintptr_t address, 
                                                      const std::vector<uint8_t>& originalData) {
    std::vector<uintptr_t> changes;
    
    try {
        auto currentData = ReadMemoryChunk(processHandle, address, originalData.size());
        
        if (currentData.size() != originalData.size()) {
            throw MemoryHookingException(MemoryHookingError::InvalidSize, "Data size mismatch");
        }
        
        for (size_t i = 0; i < originalData.size(); ++i) {
            if (originalData[i] != currentData[i]) {
                changes.push_back(address + i);
                if (changes.size() >= MAX_RESULTS) break;
            }
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("FindChangedBytes failed: " + std::string(e.what()));
    }
    
    return changes;
}

// Utility functions
std::vector<uint8_t> MemoryHelpers::ParseBytePattern(const std::string& pattern) {
    std::vector<uint8_t> bytes;
    std::istringstream iss(pattern);
    std::string byteStr;
    
    while (iss >> byteStr) {
        if (byteStr == "?" || byteStr == "??") {
            bytes.push_back(0x00); // Placeholder for wildcard
        } else {
            try {
                unsigned int byte = std::stoul(byteStr, nullptr, 16);
                bytes.push_back(static_cast<uint8_t>(byte));
            } catch (const std::exception&) {
                // Invalid byte, skip
                continue;
            }
        }
    }
    
    return bytes;
}

std::vector<bool> MemoryHelpers::ParsePatternMask(const std::string& pattern) {
    std::vector<bool> mask;
    std::istringstream iss(pattern);
    std::string byteStr;
    
    while (iss >> byteStr) {
        if (byteStr == "?" || byteStr == "??") {
            mask.push_back(false); // Don't check this byte
        } else {
            mask.push_back(true); // Check this byte
        }
    }
    
    return mask;
}

std::string MemoryHelpers::FormatAddress(uintptr_t address, bool withPrefix) {
    std::ostringstream oss;
    if (withPrefix) {
        oss << "0x";
    }
    oss << std::hex << std::uppercase << std::setfill('0') << std::setw(sizeof(uintptr_t) * 2) << address;
    return oss.str();
}

bool MemoryHelpers::IsValidCodeAddress(HANDLE processHandle, uintptr_t address) {
    try {
        MEMORY_BASIC_INFORMATION mbi;
        if (VirtualQueryEx(processHandle, reinterpret_cast<LPVOID>(address), &mbi, sizeof(mbi)) == sizeof(mbi)) {
            return (mbi.State == MEM_COMMIT) && 
                   (mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE));
        }
    }
    catch (const MemoryHookingException& e) {
        ErrorHandler::logError("IsValidCodeAddress failed: " + std::string(e.what()));
    }
    
    return false;
}

size_t MemoryHelpers::GetAlignment(uintptr_t address) {
    if (address == 0) return 0;
    
    size_t alignment = 1;
    while ((address & (alignment - 1)) == 0 && alignment <= 4096) {
        alignment <<= 1;
    }
    return alignment >> 1;
}

// Private helper methods
bool MemoryHelpers::IsAddressInRange(uintptr_t address, uintptr_t start, uintptr_t end) {
    return (start == 0 || address >= start) && (end == 0 || address < end);
}

std::vector<uint8_t> MemoryHelpers::ReadMemoryChunk(HANDLE processHandle, uintptr_t address, size_t size) {
    std::vector<uint8_t> buffer(size);
    SIZE_T bytesRead;
    
    if (!ErrorHandler::safeReadMemory(processHandle, reinterpret_cast<LPCVOID>(address), 
                                           buffer.data(), size, &bytesRead)) {
        return std::vector<uint8_t>(); // Return empty vector on failure
    }
    
    buffer.resize(bytesRead);
    return buffer;
}

bool MemoryHelpers::MatchesPattern(const uint8_t* data, const std::vector<uint8_t>& pattern, 
                                  const std::vector<bool>& mask) {
    for (size_t i = 0; i < pattern.size(); ++i) {
        if (mask[i] && data[i] != pattern[i]) {
            return false;
        }
    }
    return true;
} 