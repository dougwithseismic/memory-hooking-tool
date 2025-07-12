#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <windows.h>
#include <string>
#include <vector>

struct ProcessInfo {
    DWORD processId;
    std::string processName;
    std::string windowTitle;
    HANDLE processHandle;
};

class ProcessManager {
public:
    ProcessManager();
    ~ProcessManager();
    
    // Process enumeration and selection
    std::vector<ProcessInfo> getRunningProcesses();
    bool attachToProcess(DWORD processId);
    bool attachToProcess(const std::string& processName);
    void detachFromProcess();
    
    // Process information
    DWORD getCurrentProcessId() const;
    std::string getCurrentProcessName() const;
    bool isProcessAttached() const;
    HANDLE getProcessHandle() const;
    
    // Memory access
    bool readMemory(LPCVOID address, LPVOID buffer, SIZE_T size);
    bool writeMemory(LPVOID address, LPCVOID data, SIZE_T size);
    
    // Module information
    HMODULE getModuleHandle(const std::string& moduleName);
    LPVOID getModuleBaseAddress(const std::string& moduleName);
    SIZE_T getModuleSize(const std::string& moduleName);

private:
    HANDLE m_processHandle;
    DWORD m_processId;
    std::string m_processName;
    
    bool enableDebugPrivileges();
};

#endif // PROCESS_MANAGER_H 