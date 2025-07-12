#include "include/error_handler.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

bool ErrorHandler::s_loggingEnabled = true;

// MemoryHookingException implementation
MemoryHookingException::MemoryHookingException(MemoryHookingError errorCode, const std::string& message)
    : m_errorCode(errorCode), m_message(message), m_lastWindowsError(GetLastError()) {
}

const char* MemoryHookingException::what() const noexcept {
    return m_message.c_str();
}

MemoryHookingError MemoryHookingException::getErrorCode() const {
    return m_errorCode;
}

DWORD MemoryHookingException::getLastWindowsError() const {
    return m_lastWindowsError;
}

std::string MemoryHookingException::getDetailedMessage() const {
    std::stringstream ss;
    ss << "MemoryHookingError: " << m_message;
    
    if (m_lastWindowsError != 0) {
        ss << " (Windows Error: " << m_lastWindowsError 
           << " - " << ErrorHandler::getWindowsErrorMessage(m_lastWindowsError) << ")";
    }
    
    return ss.str();
}

// ErrorHandler implementation
std::string ErrorHandler::getWindowsErrorMessage(DWORD errorCode) {
    if (errorCode == 0) return "No error";
    
    LPSTR messageBuffer = nullptr;
    DWORD size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer, 0, nullptr);

    if (size == 0) {
        return "Unknown error (code: " + std::to_string(errorCode) + ")";
    }

    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);
    
    // Remove trailing newlines
    while (!message.empty() && (message.back() == '\n' || message.back() == '\r')) {
        message.pop_back();
    }
    
    return message;
}

std::string ErrorHandler::getMemoryProtectionString(DWORD protection) {
    std::vector<std::string> protections;
    
    if (protection & PAGE_NOACCESS) protections.push_back("NOACCESS");
    if (protection & PAGE_READONLY) protections.push_back("READONLY");
    if (protection & PAGE_READWRITE) protections.push_back("READWRITE");
    if (protection & PAGE_WRITECOPY) protections.push_back("WRITECOPY");
    if (protection & PAGE_EXECUTE) protections.push_back("EXECUTE");
    if (protection & PAGE_EXECUTE_READ) protections.push_back("EXECUTE_READ");
    if (protection & PAGE_EXECUTE_READWRITE) protections.push_back("EXECUTE_READWRITE");
    if (protection & PAGE_EXECUTE_WRITECOPY) protections.push_back("EXECUTE_WRITECOPY");
    if (protection & PAGE_GUARD) protections.push_back("GUARD");
    if (protection & PAGE_NOCACHE) protections.push_back("NOCACHE");
    if (protection & PAGE_WRITECOMBINE) protections.push_back("WRITECOMBINE");
    
    if (protections.empty()) {
        return "UNKNOWN(0x" + std::to_string(protection) + ")";
    }
    
    std::string result;
    for (size_t i = 0; i < protections.size(); ++i) {
        if (i > 0) result += "|";
        result += protections[i];
    }
    return result;
}

bool ErrorHandler::isAddressAccessible(HANDLE processHandle, LPCVOID address, SIZE_T size) {
    if (!isValidProcessHandle(processHandle) || address == nullptr || size == 0) {
        return false;
    }
    
    MEMORY_BASIC_INFORMATION mbi;
    if (!safeVirtualQuery(processHandle, address, &mbi)) {
        return false;
    }
    
    // Check if the memory is committed and not guarded
    if (mbi.State != MEM_COMMIT || (mbi.Protect & PAGE_GUARD) || (mbi.Protect & PAGE_NOACCESS)) {
        return false;
    }
    
    // Check if the entire range is within this memory region
    ULONG_PTR startAddr = reinterpret_cast<ULONG_PTR>(address);
    ULONG_PTR endAddr = startAddr + size - 1;
    ULONG_PTR regionStart = reinterpret_cast<ULONG_PTR>(mbi.BaseAddress);
    ULONG_PTR regionEnd = regionStart + mbi.RegionSize - 1;
    
    return (startAddr >= regionStart && endAddr <= regionEnd);
}

bool ErrorHandler::isValidProcessHandle(HANDLE processHandle) {
    if (processHandle == nullptr || processHandle == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    DWORD exitCode;
    return GetExitCodeProcess(processHandle, &exitCode) && exitCode == STILL_ACTIVE;
}

void ErrorHandler::logError(const std::string& operation, DWORD errorCode) {
    if (!s_loggingEnabled) return;
    
    if (errorCode == 0) errorCode = GetLastError();
    
    std::cerr << "[ERROR] " << operation;
    if (errorCode != 0) {
        std::cerr << " failed with error " << errorCode << ": " << getWindowsErrorMessage(errorCode);
    }
    std::cerr << std::endl;
}

void ErrorHandler::logWarning(const std::string& message) {
    if (!s_loggingEnabled) return;
    std::cerr << "[WARNING] " << message << std::endl;
}

void ErrorHandler::logInfo(const std::string& message) {
    if (!s_loggingEnabled) return;
    std::cout << "[INFO] " << message << std::endl;
}

bool ErrorHandler::safeReadMemory(HANDLE processHandle, LPCVOID address, LPVOID buffer, SIZE_T size, SIZE_T* bytesRead) {
    if (!isValidProcessHandle(processHandle)) {
        logError("ReadProcessMemory", ERROR_INVALID_HANDLE);
        return false;
    }
    
    if (!isAddressAccessible(processHandle, address, size)) {
        logWarning("Attempting to read from inaccessible memory at 0x" + 
                  std::to_string(reinterpret_cast<ULONG_PTR>(address)));
        return false;
    }
    
    SIZE_T localBytesRead = 0;
    BOOL result = ReadProcessMemory(processHandle, address, buffer, size, &localBytesRead);
    
    if (bytesRead) {
        *bytesRead = localBytesRead;
    }
    
    if (!result) {
        logError("ReadProcessMemory from 0x" + std::to_string(reinterpret_cast<ULONG_PTR>(address)));
        return false;
    }
    
    if (localBytesRead != size) {
        logWarning("Partial read: requested " + std::to_string(size) + 
                  " bytes, got " + std::to_string(localBytesRead) + " bytes");
    }
    
    return true;
}

bool ErrorHandler::safeWriteMemory(HANDLE processHandle, LPVOID address, LPCVOID data, SIZE_T size, SIZE_T* bytesWritten) {
    if (!isValidProcessHandle(processHandle)) {
        logError("WriteProcessMemory", ERROR_INVALID_HANDLE);
        return false;
    }
    
    if (!isAddressAccessible(processHandle, address, size)) {
        logWarning("Attempting to write to inaccessible memory at 0x" + 
                  std::to_string(reinterpret_cast<ULONG_PTR>(address)));
        return false;
    }
    
    // Check if memory is writable
    MEMORY_BASIC_INFORMATION mbi;
    if (safeVirtualQuery(processHandle, address, &mbi)) {
        if (!(mbi.Protect & (PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_WRITECOPY))) {
            logWarning("Attempting to write to non-writable memory (protection: " + 
                      getMemoryProtectionString(mbi.Protect) + ")");
        }
    }
    
    SIZE_T localBytesWritten = 0;
    BOOL result = WriteProcessMemory(processHandle, address, data, size, &localBytesWritten);
    
    if (bytesWritten) {
        *bytesWritten = localBytesWritten;
    }
    
    if (!result) {
        logError("WriteProcessMemory to 0x" + std::to_string(reinterpret_cast<ULONG_PTR>(address)));
        return false;
    }
    
    if (localBytesWritten != size) {
        logWarning("Partial write: requested " + std::to_string(size) + 
                  " bytes, wrote " + std::to_string(localBytesWritten) + " bytes");
    }
    
    return true;
}

bool ErrorHandler::safeVirtualQuery(HANDLE processHandle, LPCVOID address, PMEMORY_BASIC_INFORMATION mbi) {
    if (!isValidProcessHandle(processHandle) || mbi == nullptr) {
        return false;
    }
    
    SIZE_T result = VirtualQueryEx(processHandle, address, mbi, sizeof(MEMORY_BASIC_INFORMATION));
    
    if (result == 0) {
        logError("VirtualQueryEx at 0x" + std::to_string(reinterpret_cast<ULONG_PTR>(address)));
        return false;
    }
    
    return true;
}

// SafeHandle implementation
SafeHandle::SafeHandle(HANDLE handle) : m_handle(handle) {
}

SafeHandle::~SafeHandle() {
    reset();
}

SafeHandle::SafeHandle(SafeHandle&& other) noexcept : m_handle(other.m_handle) {
    other.m_handle = nullptr;
}

SafeHandle& SafeHandle::operator=(SafeHandle&& other) noexcept {
    if (this != &other) {
        reset();
        m_handle = other.m_handle;
        other.m_handle = nullptr;
    }
    return *this;
}

HANDLE SafeHandle::get() const {
    return m_handle;
}

HANDLE SafeHandle::release() {
    HANDLE handle = m_handle;
    m_handle = nullptr;
    return handle;
}

void SafeHandle::reset(HANDLE handle) {
    if (m_handle && m_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(m_handle);
    }
    m_handle = handle;
}

bool SafeHandle::isValid() const {
    return m_handle != nullptr && m_handle != INVALID_HANDLE_VALUE;
}

SafeHandle::operator bool() const {
    return isValid();
}

SafeHandle::operator HANDLE() const {
    return m_handle;
} 