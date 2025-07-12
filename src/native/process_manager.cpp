#include "include/process_manager.h"
#include "include/error_handler.h"
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <iostream>
#include <algorithm>

ProcessManager::ProcessManager() 
    : m_processHandle(nullptr), m_processId(0), m_processName("") {
    enableDebugPrivileges();
}

ProcessManager::~ProcessManager() {
    detachFromProcess();
}

bool ProcessManager::enableDebugPrivileges() {
    HANDLE tokenHandle;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tokenHandle)) {
        return false;
    }

    TOKEN_PRIVILEGES tokenPrivileges;
    LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &tokenPrivileges.Privileges[0].Luid);
    tokenPrivileges.PrivilegeCount = 1;
    tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    bool result = AdjustTokenPrivileges(tokenHandle, FALSE, &tokenPrivileges, 0, nullptr, nullptr);
    CloseHandle(tokenHandle);
    return result;
}

std::vector<ProcessInfo> ProcessManager::getRunningProcesses() {
    std::vector<ProcessInfo> processes;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (snapshot == INVALID_HANDLE_VALUE) {
        return processes;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &processEntry)) {
        do {
            ProcessInfo info;
            info.processId = processEntry.th32ProcessID;
            info.processName = processEntry.szExeFile;
            info.windowTitle = ""; // Will be populated separately if needed
            info.processHandle = nullptr;
            processes.push_back(info);
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return processes;
}

bool ProcessManager::attachToProcess(DWORD processId) {
    detachFromProcess();
    
    m_processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (m_processHandle == nullptr) {
        return false;
    }
    
    m_processId = processId;
    
    // Get process name
    TCHAR processName[MAX_PATH];
    if (GetModuleBaseName(m_processHandle, nullptr, processName, MAX_PATH)) {
        m_processName = processName;
    }
    
    return true;
}

bool ProcessManager::attachToProcess(const std::string& processName) {
    auto processes = getRunningProcesses();
    
    for (const auto& process : processes) {
        if (process.processName == processName) {
            return attachToProcess(process.processId);
        }
    }
    
    return false;
}

void ProcessManager::detachFromProcess() {
    if (m_processHandle != nullptr) {
        CloseHandle(m_processHandle);
        m_processHandle = nullptr;
        m_processId = 0;
        m_processName = "";
    }
}

DWORD ProcessManager::getCurrentProcessId() const {
    return m_processId;
}

std::string ProcessManager::getCurrentProcessName() const {
    return m_processName;
}

bool ProcessManager::isProcessAttached() const {
    return m_processHandle != nullptr;
}

bool ProcessManager::readMemory(LPCVOID address, LPVOID buffer, SIZE_T size) {
    if (!isProcessAttached()) {
        ErrorHandler::logError("readMemory: No process attached");
        return false;
    }
    
    if (address == nullptr || buffer == nullptr || size == 0) {
        ErrorHandler::logError("readMemory: Invalid parameters");
        return false;
    }
    
    try {
        SIZE_T bytesRead = 0;
        return ErrorHandler::safeReadMemory(m_processHandle, address, buffer, size, &bytesRead) && 
               bytesRead == size;
    } catch (const MemoryHookingException& e) {
        ErrorHandler::logError("readMemory: " + e.getDetailedMessage());
        return false;
    }
}

bool ProcessManager::writeMemory(LPVOID address, LPCVOID data, SIZE_T size) {
    if (!isProcessAttached()) {
        ErrorHandler::logError("writeMemory: No process attached");
        return false;
    }
    
    if (address == nullptr || data == nullptr || size == 0) {
        ErrorHandler::logError("writeMemory: Invalid parameters");
        return false;
    }
    
    try {
        DWORD oldProtection;
        if (!VirtualProtectEx(m_processHandle, address, size, PAGE_EXECUTE_READWRITE, &oldProtection)) {
            ErrorHandler::logError("writeMemory: Failed to change memory protection");
            return false;
        }
        
        SIZE_T bytesWritten = 0;
        bool success = ErrorHandler::safeWriteMemory(m_processHandle, address, data, size, &bytesWritten) && 
                      bytesWritten == size;
        
        // Restore original protection
        DWORD tempProtection;
        VirtualProtectEx(m_processHandle, address, size, oldProtection, &tempProtection);
        
        return success;
    } catch (const MemoryHookingException& e) {
        ErrorHandler::logError("writeMemory: " + e.getDetailedMessage());
        return false;
    }
}

HMODULE ProcessManager::getModuleHandle(const std::string& moduleName) {
    if (!isProcessAttached()) {
        return nullptr;
    }
    
    HMODULE modules[1024];
    DWORD cbNeeded;
    
    if (EnumProcessModules(m_processHandle, modules, sizeof(modules), &cbNeeded)) {
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            TCHAR moduleNameBuffer[MAX_PATH];
            if (GetModuleBaseName(m_processHandle, modules[i], moduleNameBuffer, MAX_PATH)) {
                if (moduleName == moduleNameBuffer) {
                    return modules[i];
                }
            }
        }
    }
    
    return nullptr;
}

LPVOID ProcessManager::getModuleBaseAddress(const std::string& moduleName) {
    return static_cast<LPVOID>(getModuleHandle(moduleName));
}

SIZE_T ProcessManager::getModuleSize(const std::string& moduleName) {
    HMODULE moduleHandle = getModuleHandle(moduleName);
    if (moduleHandle == nullptr) {
        return 0;
    }
    
    MODULEINFO moduleInfo;
    if (GetModuleInformation(m_processHandle, moduleHandle, &moduleInfo, sizeof(moduleInfo))) {
        return moduleInfo.SizeOfImage;
    }
    
    return 0;
}

HANDLE ProcessManager::getProcessHandle() const {
    return m_processHandle;
} 