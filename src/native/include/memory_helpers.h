#ifndef MEMORY_HELPERS_H
#define MEMORY_HELPERS_H

#include <windows.h>
#include <vector>
#include <string>
#include <cstdint>

struct PointerResult {
    uintptr_t address;
    uintptr_t pointsTo;
    size_t offset;
};

struct PatternMatch {
    uintptr_t address;
    size_t size;
    std::vector<uint8_t> data;
};

struct MemoryRegionEx {
    uintptr_t startAddress;
    uintptr_t endAddress;
    size_t size;
    DWORD protection;
    DWORD type;
    std::string moduleName;
};

struct NumericMatch {
    uintptr_t address;
    std::string dataType;
    std::vector<uint8_t> rawData;
};

class MemoryHelpers {
public:
    // Pointer finding operations
    static std::vector<PointerResult> FindPointersTo(HANDLE processHandle, uintptr_t targetAddress, 
                                                     uintptr_t searchStart = 0, uintptr_t searchEnd = 0);
    static std::vector<PointerResult> FindPointerChain(HANDLE processHandle, uintptr_t finalAddress, 
                                                       int maxDepth = 5, const std::vector<size_t>& offsets = {});
    static std::vector<uintptr_t> FindNullPointers(HANDLE processHandle, uintptr_t searchStart, 
                                                   uintptr_t searchEnd);

    // Enhanced pattern searching
    static std::vector<PatternMatch> SearchBytePattern(HANDLE processHandle, const std::vector<uint8_t>& pattern, 
                                                      const std::vector<bool>& mask, uintptr_t searchStart = 0, 
                                                      uintptr_t searchEnd = 0);
    static std::vector<PatternMatch> SearchStringPattern(HANDLE processHandle, const std::string& pattern, 
                                                        bool caseSensitive = true, bool unicode = false,
                                                        uintptr_t searchStart = 0, uintptr_t searchEnd = 0);
    static std::vector<PatternMatch> SearchRegexPattern(HANDLE processHandle, const std::string& regexPattern,
                                                       uintptr_t searchStart = 0, uintptr_t searchEnd = 0);

    // Numeric value searching
    static std::vector<NumericMatch> SearchInt32(HANDLE processHandle, int32_t value, uintptr_t searchStart = 0, 
                                                 uintptr_t searchEnd = 0, bool aligned = false);
    static std::vector<NumericMatch> SearchInt64(HANDLE processHandle, int64_t value, uintptr_t searchStart = 0, 
                                                 uintptr_t searchEnd = 0, bool aligned = false);
    static std::vector<NumericMatch> SearchFloat(HANDLE processHandle, float value, float tolerance = 0.001f,
                                                uintptr_t searchStart = 0, uintptr_t searchEnd = 0);
    static std::vector<NumericMatch> SearchDouble(HANDLE processHandle, double value, double tolerance = 0.001,
                                                 uintptr_t searchStart = 0, uintptr_t searchEnd = 0);

    // Memory region analysis
    static std::vector<MemoryRegionEx> GetMemoryRegions(HANDLE processHandle, bool executableOnly = false,
                                                       bool writableOnly = false);
    static std::vector<MemoryRegionEx> GetModuleRegions(HANDLE processHandle, const std::string& moduleName = "");
    static MemoryRegionEx GetRegionInfo(HANDLE processHandle, uintptr_t address);

    // Advanced scanning utilities
    static std::vector<PatternMatch> ScanForCode(HANDLE processHandle, const std::vector<uint8_t>& opcodes,
                                                uintptr_t searchStart = 0, uintptr_t searchEnd = 0);
    static std::vector<uintptr_t> FindFunctionPrologs(HANDLE processHandle, uintptr_t searchStart = 0,
                                                     uintptr_t searchEnd = 0);
    static std::vector<uintptr_t> FindCallInstructions(HANDLE processHandle, uintptr_t targetAddress,
                                                      uintptr_t searchStart = 0, uintptr_t searchEnd = 0);

    // Memory comparison and diffing
    static std::vector<uintptr_t> CompareMemoryRegions(HANDLE processHandle, uintptr_t region1Start, 
                                                      uintptr_t region2Start, size_t size);
    static std::vector<uintptr_t> FindChangedBytes(HANDLE processHandle, uintptr_t address, 
                                                  const std::vector<uint8_t>& originalData);

    // Utility functions
    static std::vector<uint8_t> ParseBytePattern(const std::string& pattern);
    static std::vector<bool> ParsePatternMask(const std::string& pattern);
    static std::string FormatAddress(uintptr_t address, bool withPrefix = true);
    static bool IsValidCodeAddress(HANDLE processHandle, uintptr_t address);
    static size_t GetAlignment(uintptr_t address);

private:
    static const size_t MAX_SCAN_SIZE = 100 * 1024 * 1024; // 100MB
    static const size_t MAX_RESULTS = 10000;
    static const size_t CHUNK_SIZE = 1024 * 1024; // 1MB chunks
    
    static bool IsAddressInRange(uintptr_t address, uintptr_t start, uintptr_t end);
    static std::vector<uint8_t> ReadMemoryChunk(HANDLE processHandle, uintptr_t address, size_t size);
    static bool MatchesPattern(const uint8_t* data, const std::vector<uint8_t>& pattern, 
                              const std::vector<bool>& mask);
};

#endif // MEMORY_HELPERS_H 