#include "include/memory_scanner.h"
#include "include/error_handler.h"
#include <windows.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>

MemoryScanner::MemoryScanner(ProcessManager* processManager) 
    : m_processManager(processManager) {
}

MemoryScanner::~MemoryScanner() {
}

std::vector<LPVOID> MemoryScanner::scanForPattern(const std::string& pattern, const std::string& mask) {
    std::vector<LPVOID> results;
    
    if (!m_processManager->isProcessAttached()) {
        return results;
    }
    
    std::vector<BYTE> patternBytes = patternToBytes(pattern, mask);
    std::vector<bool> maskVector;
    
    for (char c : mask) {
        maskVector.push_back(c == 'x');
    }
    
    auto regions = getMemoryRegions();
    for (const auto& region : regions) {
        if (region.state == MEM_COMMIT && 
            (region.protection & PAGE_GUARD) == 0 &&
            (region.protection & PAGE_NOACCESS) == 0) {
            
            scanMemoryRegion(region.baseAddress, region.size, patternBytes, maskVector, results);
        }
    }
    
    return results;
}

std::vector<LPVOID> MemoryScanner::scanForBytes(const std::vector<BYTE>& bytes) {
    std::vector<LPVOID> results;
    
    if (!m_processManager->isProcessAttached()) {
        return results;
    }
    
    std::vector<bool> mask(bytes.size(), true);
    
    auto regions = getMemoryRegions();
    for (const auto& region : regions) {
        if (region.state == MEM_COMMIT && 
            (region.protection & PAGE_GUARD) == 0 &&
            (region.protection & PAGE_NOACCESS) == 0) {
            
            scanMemoryRegion(region.baseAddress, region.size, bytes, mask, results);
        }
    }
    
    return results;
}

std::vector<LPVOID> MemoryScanner::scanForString(const std::string& text, bool caseSensitive) {
    std::vector<BYTE> bytes;
    std::string searchText = text;
    
    if (!caseSensitive) {
        std::transform(searchText.begin(), searchText.end(), searchText.begin(), ::tolower);
    }
    
    for (char c : searchText) {
        bytes.push_back(static_cast<BYTE>(c));
    }
    
    return scanForBytes(bytes);
}

std::vector<LPVOID> MemoryScanner::scanForValue(int value) {
    std::vector<BYTE> bytes(sizeof(int));
    memcpy(bytes.data(), &value, sizeof(int));
    return scanForBytes(bytes);
}

std::vector<LPVOID> MemoryScanner::scanForValue(float value) {
    std::vector<BYTE> bytes(sizeof(float));
    memcpy(bytes.data(), &value, sizeof(float));
    return scanForBytes(bytes);
}

std::vector<LPVOID> MemoryScanner::scanForValue(double value) {
    std::vector<BYTE> bytes(sizeof(double));
    memcpy(bytes.data(), &value, sizeof(double));
    return scanForBytes(bytes);
}

std::vector<LPVOID> MemoryScanner::scanForValue(const std::string& value) {
    return scanForString(value, true);
}

std::vector<MemoryRegion> MemoryScanner::getMemoryRegions() {
    std::vector<MemoryRegion> regions;
    
    if (!m_processManager->isProcessAttached()) {
        return regions;
    }
    
    MEMORY_BASIC_INFORMATION mbi;
    LPVOID address = nullptr;
    
    // Fix: Use attached process handle instead of GetCurrentProcess()
    while (VirtualQueryEx(m_processManager->getProcessHandle(), address, &mbi, sizeof(mbi))) {
        MemoryRegion region;
        region.baseAddress = mbi.BaseAddress;
        region.size = mbi.RegionSize;
        region.protection = mbi.Protect;
        region.state = mbi.State;
        region.type = mbi.Type;
        
        regions.push_back(region);
        
        address = static_cast<LPBYTE>(mbi.BaseAddress) + mbi.RegionSize;
    }
    
    return regions;
}

std::vector<MemoryRegion> MemoryScanner::getExecutableRegions() {
    auto allRegions = getMemoryRegions();
    std::vector<MemoryRegion> executableRegions;
    
    for (const auto& region : allRegions) {
        if (region.protection & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) {
            executableRegions.push_back(region);
        }
    }
    
    return executableRegions;
}

std::vector<MemoryRegion> MemoryScanner::getWritableRegions() {
    auto allRegions = getMemoryRegions();
    std::vector<MemoryRegion> writableRegions;
    
    for (const auto& region : allRegions) {
        if (region.protection & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_WRITECOPY)) {
            writableRegions.push_back(region);
        }
    }
    
    return writableRegions;
}

bool MemoryScanner::isValidAddress(LPVOID address) {
    MEMORY_BASIC_INFORMATION mbi;
    // Fix: Use attached process handle instead of GetCurrentProcess()
    return VirtualQueryEx(m_processManager->getProcessHandle(), address, &mbi, sizeof(mbi)) &&
           mbi.State == MEM_COMMIT &&
           (mbi.Protect & PAGE_GUARD) == 0 &&
           (mbi.Protect & PAGE_NOACCESS) == 0;
}

bool MemoryScanner::isExecutableAddress(LPVOID address) {
    MEMORY_BASIC_INFORMATION mbi;
    // Fix: Use attached process handle instead of GetCurrentProcess()
    return VirtualQueryEx(m_processManager->getProcessHandle(), address, &mbi, sizeof(mbi)) &&
           (mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY));
}

bool MemoryScanner::isWritableAddress(LPVOID address) {
    MEMORY_BASIC_INFORMATION mbi;
    // Fix: Use attached process handle instead of GetCurrentProcess()
    return VirtualQueryEx(m_processManager->getProcessHandle(), address, &mbi, sizeof(mbi)) &&
           (mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_WRITECOPY));
}

std::string MemoryScanner::bytesToHexString(const std::vector<BYTE>& bytes) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    for (BYTE b : bytes) {
        ss << std::setw(2) << static_cast<int>(b) << " ";
    }
    
    return ss.str();
}

std::vector<BYTE> MemoryScanner::hexStringToBytes(const std::string& hexString) {
    std::vector<BYTE> bytes;
    std::stringstream ss(hexString);
    std::string byteString;
    
    while (ss >> byteString) {
        if (byteString.length() == 2) {
            BYTE byte = static_cast<BYTE>(std::stoi(byteString, nullptr, 16));
            bytes.push_back(byte);
        }
    }
    
    return bytes;
}

std::vector<BYTE> MemoryScanner::patternToBytes(const std::string& pattern, const std::string& mask) {
    std::vector<BYTE> bytes;
    std::stringstream ss(pattern);
    std::string byteString;
    
    while (ss >> byteString) {
        if (byteString.length() == 2) {
            BYTE byte = static_cast<BYTE>(std::stoi(byteString, nullptr, 16));
            bytes.push_back(byte);
        }
    }
    
    return bytes;
}

bool MemoryScanner::scanMemoryRegion(LPVOID startAddress, SIZE_T size, const std::vector<BYTE>& pattern, 
                                    const std::vector<bool>& mask, std::vector<LPVOID>& results) {
    
    if (!m_processManager->isProcessAttached()) {
        ErrorHandler::logError("scanMemoryRegion: No process attached");
        return false;
    }
    
    if (pattern.empty() || mask.size() != pattern.size()) {
        ErrorHandler::logError("scanMemoryRegion: Invalid pattern or mask");
        return false;
    }
    
    const SIZE_T CHUNK_SIZE = 4096;
    const SIZE_T MAX_SCAN_SIZE = 100 * 1024 * 1024; // 100MB limit for safety
    
    if (size > MAX_SCAN_SIZE) {
        ErrorHandler::logWarning("scanMemoryRegion: Scan size exceeds safety limit, truncating to " + 
                                 std::to_string(MAX_SCAN_SIZE) + " bytes");
        size = MAX_SCAN_SIZE;
    }
    
    std::vector<BYTE> buffer(CHUNK_SIZE);
    SIZE_T successfulReads = 0;
    SIZE_T failedReads = 0;
    
    try {
        for (SIZE_T offset = 0; offset < size; offset += CHUNK_SIZE) {
            SIZE_T readSize = std::min(CHUNK_SIZE, size - offset);
            LPVOID currentAddress = static_cast<LPBYTE>(startAddress) + offset;
            
            // Fix: Use attached process handle instead of GetCurrentProcess()
            if (!ErrorHandler::isAddressAccessible(m_processManager->getProcessHandle(), currentAddress, readSize)) {
                failedReads++;
                continue;
            }
            
            SIZE_T bytesRead = 0;
            if (m_processManager->readMemory(currentAddress, buffer.data(), readSize)) {
                successfulReads++;
                
                // Ensure we don't exceed buffer bounds
                SIZE_T searchEnd = (readSize >= pattern.size()) ? readSize - pattern.size() + 1 : 0;
                
                for (SIZE_T i = 0; i < searchEnd; ++i) {
                    bool match = true;
                    
                    for (SIZE_T j = 0; j < pattern.size(); ++j) {
                        if (mask[j] && buffer[i + j] != pattern[j]) {
                            match = false;
                            break;
                        }
                    }
                    
                    if (match) {
                        LPVOID matchAddress = static_cast<LPBYTE>(currentAddress) + i;
                        results.push_back(matchAddress);
                        
                        // Limit results to prevent memory exhaustion
                        if (results.size() >= 10000) {
                            ErrorHandler::logWarning("scanMemoryRegion: Reached maximum result limit (10000), stopping scan");
                            return true;
                        }
                    }
                }
            } else {
                failedReads++;
            }
        }
        
        if (failedReads > 0) {
            ErrorHandler::logInfo("scanMemoryRegion: " + std::to_string(successfulReads) + 
                                 " successful reads, " + std::to_string(failedReads) + " failed reads");
        }
        
    } catch (const std::exception& e) {
        ErrorHandler::logError("scanMemoryRegion: Exception during scan - " + std::string(e.what()));
        return false;
    }
    
    return true;
}

MEMORY_BASIC_INFORMATION MemoryScanner::getMemoryInfo(LPVOID address) {
    MEMORY_BASIC_INFORMATION mbi;
    // Fix: Use attached process handle instead of GetCurrentProcess()
    VirtualQueryEx(m_processManager->getProcessHandle(), address, &mbi, sizeof(mbi));
    return mbi;
} 