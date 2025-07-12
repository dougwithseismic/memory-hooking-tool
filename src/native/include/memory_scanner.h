#ifndef MEMORY_SCANNER_H
#define MEMORY_SCANNER_H

#include <windows.h>
#include <string>
#include <vector>
#include "process_manager.h"

struct ScanResult {
    LPVOID address;
    SIZE_T size;
    std::vector<BYTE> data;
};

struct MemoryRegion {
    LPVOID baseAddress;
    SIZE_T size;
    DWORD protection;
    DWORD state;
    DWORD type;
};

class MemoryScanner {
public:
    MemoryScanner(ProcessManager* processManager);
    ~MemoryScanner();
    
    // Pattern scanning
    std::vector<LPVOID> scanForPattern(const std::string& pattern, const std::string& mask);
    std::vector<LPVOID> scanForBytes(const std::vector<BYTE>& bytes);
    std::vector<LPVOID> scanForString(const std::string& text, bool caseSensitive = true);
    
    // Value scanning
    std::vector<LPVOID> scanForValue(int value);
    std::vector<LPVOID> scanForValue(float value);
    std::vector<LPVOID> scanForValue(double value);
    std::vector<LPVOID> scanForValue(const std::string& value);
    
    // Memory region analysis
    std::vector<MemoryRegion> getMemoryRegions();
    std::vector<MemoryRegion> getExecutableRegions();
    std::vector<MemoryRegion> getWritableRegions();
    
    // Address validation
    bool isValidAddress(LPVOID address);
    bool isExecutableAddress(LPVOID address);
    bool isWritableAddress(LPVOID address);
    
    // Utility functions
    std::string bytesToHexString(const std::vector<BYTE>& bytes);
    std::vector<BYTE> hexStringToBytes(const std::string& hexString);
    std::vector<BYTE> patternToBytes(const std::string& pattern, const std::string& mask);

private:
    ProcessManager* m_processManager;
    
    bool scanMemoryRegion(LPVOID startAddress, SIZE_T size, const std::vector<BYTE>& pattern, 
                         const std::vector<bool>& mask, std::vector<LPVOID>& results);
    MEMORY_BASIC_INFORMATION getMemoryInfo(LPVOID address);
};

#endif // MEMORY_SCANNER_H 