#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <windows.h>
#include <string>
#include <exception>

enum class MemoryHookingError {
    None = 0,
    ProcessNotFound,
    AccessDenied,
    InvalidAddress,
    InvalidSize,
    ReadMemoryFailed,
    WriteMemoryFailed,
    ScanningFailed,
    ModuleNotFound,
    InvalidPattern,
    InsufficientPrivileges,
    ProcessNotAttached,
    UnknownError
};

class MemoryHookingException : public std::exception {
public:
    MemoryHookingException(MemoryHookingError errorCode, const std::string& message);
    
    const char* what() const noexcept override;
    MemoryHookingError getErrorCode() const;
    DWORD getLastWindowsError() const;
    std::string getDetailedMessage() const;

private:
    MemoryHookingError m_errorCode;
    std::string m_message;
    DWORD m_lastWindowsError;
};

class ErrorHandler {
public:
    static std::string getWindowsErrorMessage(DWORD errorCode);
    static std::string getMemoryProtectionString(DWORD protection);
    static bool isAddressAccessible(HANDLE processHandle, LPCVOID address, SIZE_T size);
    static bool isValidProcessHandle(HANDLE processHandle);
    static void logError(const std::string& operation, DWORD errorCode = 0);
    static void logWarning(const std::string& message);
    static void logInfo(const std::string& message);
    
    // Safe memory operations with error checking
    static bool safeReadMemory(HANDLE processHandle, LPCVOID address, LPVOID buffer, SIZE_T size, SIZE_T* bytesRead = nullptr);
    static bool safeWriteMemory(HANDLE processHandle, LPVOID address, LPCVOID data, SIZE_T size, SIZE_T* bytesWritten = nullptr);
    static bool safeVirtualQuery(HANDLE processHandle, LPCVOID address, PMEMORY_BASIC_INFORMATION mbi);
    
private:
    static bool s_loggingEnabled;
};

// RAII wrapper for Windows handles
class SafeHandle {
public:
    SafeHandle(HANDLE handle = nullptr);
    ~SafeHandle();
    
    SafeHandle(const SafeHandle&) = delete;
    SafeHandle& operator=(const SafeHandle&) = delete;
    
    SafeHandle(SafeHandle&& other) noexcept;
    SafeHandle& operator=(SafeHandle&& other) noexcept;
    
    HANDLE get() const;
    HANDLE release();
    void reset(HANDLE handle = nullptr);
    bool isValid() const;
    
    operator bool() const;
    operator HANDLE() const;

private:
    HANDLE m_handle;
};

// Macros for error checking
#define THROW_IF_FAILED(condition, errorCode, message) \
    do { \
        if (!(condition)) { \
            throw MemoryHookingException(errorCode, message); \
        } \
    } while(0)

#define LOG_LAST_ERROR(operation) \
    ErrorHandler::logError(operation, GetLastError())

#define SAFE_CALL(operation, errorCode, message) \
    do { \
        if (!(operation)) { \
            LOG_LAST_ERROR(#operation); \
            throw MemoryHookingException(errorCode, message); \
        } \
    } while(0)

#endif // ERROR_HANDLER_H 