#include <napi.h>
#include <windows.h>
#include "include/process_manager.h"
#include "include/memory_scanner.h"
#include "include/memory_helpers.h"
#include "include/automation_helpers.h"
#include <memory>

class MemoryHookingAddon : public Napi::ObjectWrap<MemoryHookingAddon> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    MemoryHookingAddon(const Napi::CallbackInfo& info);

private:
    static Napi::FunctionReference constructor;
    
    // Process Management Methods
    Napi::Value GetRunningProcesses(const Napi::CallbackInfo& info);
    Napi::Value AttachToProcess(const Napi::CallbackInfo& info);
    Napi::Value DetachFromProcess(const Napi::CallbackInfo& info);
    Napi::Value IsProcessAttached(const Napi::CallbackInfo& info);
    Napi::Value GetCurrentProcessInfo(const Napi::CallbackInfo& info);
    
    // Memory Operations
    Napi::Value ReadMemory(const Napi::CallbackInfo& info);
    Napi::Value WriteMemory(const Napi::CallbackInfo& info);
    Napi::Value GetModuleInfo(const Napi::CallbackInfo& info);
    
    // Memory Scanning
    Napi::Value ScanForPattern(const Napi::CallbackInfo& info);
    Napi::Value ScanForBytes(const Napi::CallbackInfo& info);
    Napi::Value ScanForString(const Napi::CallbackInfo& info);
    Napi::Value ScanForValue(const Napi::CallbackInfo& info);
    
    // Memory Region Analysis
    Napi::Value GetMemoryRegions(const Napi::CallbackInfo& info);
    Napi::Value IsValidAddress(const Napi::CallbackInfo& info);
    
    // Utility Methods
    Napi::Value BytesToHexString(const Napi::CallbackInfo& info);
    Napi::Value HexStringToBytes(const Napi::CallbackInfo& info);
    
    // Memory Helpers - Pointer Operations
    Napi::Value FindPointersTo(const Napi::CallbackInfo& info);
    Napi::Value FindPointerChain(const Napi::CallbackInfo& info);
    Napi::Value FindNullPointers(const Napi::CallbackInfo& info);
    
    // Memory Helpers - Pattern Searching
    Napi::Value SearchBytePattern(const Napi::CallbackInfo& info);
    Napi::Value SearchStringPattern(const Napi::CallbackInfo& info);
    Napi::Value SearchRegexPattern(const Napi::CallbackInfo& info);
    
    // Memory Helpers - Numeric Searching
    Napi::Value SearchInt32(const Napi::CallbackInfo& info);
    Napi::Value SearchInt64(const Napi::CallbackInfo& info);
    Napi::Value SearchFloat(const Napi::CallbackInfo& info);
    Napi::Value SearchDouble(const Napi::CallbackInfo& info);
    
    // Memory Helpers - Region Analysis
    Napi::Value GetMemoryRegionsEx(const Napi::CallbackInfo& info);
    Napi::Value GetModuleRegions(const Napi::CallbackInfo& info);
    Napi::Value GetRegionInfo(const Napi::CallbackInfo& info);
    
    // Memory Helpers - Advanced Scanning
    Napi::Value ScanForCode(const Napi::CallbackInfo& info);
    Napi::Value FindFunctionPrologs(const Napi::CallbackInfo& info);
    Napi::Value FindCallInstructions(const Napi::CallbackInfo& info);
    
    // Memory Helpers - Comparison and Diffing
    Napi::Value CompareMemoryRegions(const Napi::CallbackInfo& info);
    Napi::Value FindChangedBytes(const Napi::CallbackInfo& info);
    
    // Memory Helpers - Utility Functions
    Napi::Value ParseBytePattern(const Napi::CallbackInfo& info);
    Napi::Value ParsePatternMask(const Napi::CallbackInfo& info);
    Napi::Value FormatAddress(const Napi::CallbackInfo& info);
    Napi::Value IsValidCodeAddress(const Napi::CallbackInfo& info);
    Napi::Value GetAlignment(const Napi::CallbackInfo& info);
    
    // Automation Helpers - Mouse Control
    Napi::Value MoveMouse(const Napi::CallbackInfo& info);
    Napi::Value ClickMouse(const Napi::CallbackInfo& info);
    Napi::Value DoubleClickMouse(const Napi::CallbackInfo& info);
    Napi::Value DragMouse(const Napi::CallbackInfo& info);
    Napi::Value ScrollMouse(const Napi::CallbackInfo& info);
    Napi::Value GetMousePosition(const Napi::CallbackInfo& info);
    Napi::Value IsMouseButtonPressed(const Napi::CallbackInfo& info);
    
    // Automation Helpers - Keyboard Control
    Napi::Value PressKey(const Napi::CallbackInfo& info);
    Napi::Value ReleaseKey(const Napi::CallbackInfo& info);
    Napi::Value SendKeyCombo(const Napi::CallbackInfo& info);
    Napi::Value SendText(const Napi::CallbackInfo& info);
    Napi::Value SendTextToWindow(const Napi::CallbackInfo& info);
    Napi::Value IsKeyPressed(const Napi::CallbackInfo& info);
    Napi::Value GetPressedKeys(const Napi::CallbackInfo& info);
    
    // Automation Helpers - Screenshot and Screen Capture
    Napi::Value CaptureScreen(const Napi::CallbackInfo& info);
    Napi::Value CaptureWindow(const Napi::CallbackInfo& info);
    Napi::Value CaptureRegion(const Napi::CallbackInfo& info);
    Napi::Value CaptureActiveWindow(const Napi::CallbackInfo& info);
    Napi::Value SaveScreenshotToFile(const Napi::CallbackInfo& info);
    
    // Automation Helpers - Window Management
    Napi::Value GetAllWindows(const Napi::CallbackInfo& info);
    Napi::Value GetWindowsByTitle(const Napi::CallbackInfo& info);
    Napi::Value GetWindowsByClassName(const Napi::CallbackInfo& info);
    Napi::Value GetActiveWindow(const Napi::CallbackInfo& info);
    Napi::Value SetActiveWindow(const Napi::CallbackInfo& info);
    Napi::Value ShowWindow(const Napi::CallbackInfo& info);
    Napi::Value MoveWindow(const Napi::CallbackInfo& info);
    Napi::Value CloseWindow(const Napi::CallbackInfo& info);
    
    // Automation Helpers - Screen Information
    Napi::Value GetScreenWidth(const Napi::CallbackInfo& info);
    Napi::Value GetScreenHeight(const Napi::CallbackInfo& info);
    Napi::Value GetScreenDPI(const Napi::CallbackInfo& info);
    Napi::Value GetMonitorRects(const Napi::CallbackInfo& info);
    
    // Automation Helpers - Color and Pixel Operations
    Napi::Value GetPixelColor(const Napi::CallbackInfo& info);
    Napi::Value GetPixelColors(const Napi::CallbackInfo& info);
    Napi::Value FindColorOnScreen(const Napi::CallbackInfo& info);
    Napi::Value FindColorInRegion(const Napi::CallbackInfo& info);
    
    // Automation Helpers - Wait and Timing
    Napi::Value Sleep(const Napi::CallbackInfo& info);
    Napi::Value WaitForWindow(const Napi::CallbackInfo& info);
    Napi::Value WaitForPixelColor(const Napi::CallbackInfo& info);
    Napi::Value WaitForKeyPress(const Napi::CallbackInfo& info);
    
    // Automation Helpers - System Information
    Napi::Value GetSystemInfo(const Napi::CallbackInfo& info);
    Napi::Value GetRunningWindowTitles(const Napi::CallbackInfo& info);
    Napi::Value IsScreenLocked(const Napi::CallbackInfo& info);
    Napi::Value IsUserIdle(const Napi::CallbackInfo& info);
    
    std::unique_ptr<ProcessManager> m_processManager;
    std::unique_ptr<MemoryScanner> m_memoryScanner;
};

Napi::FunctionReference MemoryHookingAddon::constructor;

Napi::Object MemoryHookingAddon::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "MemoryHooking", {
        InstanceMethod("getRunningProcesses", &MemoryHookingAddon::GetRunningProcesses),
        InstanceMethod("attachToProcess", &MemoryHookingAddon::AttachToProcess),
        InstanceMethod("detachFromProcess", &MemoryHookingAddon::DetachFromProcess),
        InstanceMethod("isProcessAttached", &MemoryHookingAddon::IsProcessAttached),
        InstanceMethod("getCurrentProcessInfo", &MemoryHookingAddon::GetCurrentProcessInfo),
        InstanceMethod("readMemory", &MemoryHookingAddon::ReadMemory),
        InstanceMethod("writeMemory", &MemoryHookingAddon::WriteMemory),
        InstanceMethod("getModuleInfo", &MemoryHookingAddon::GetModuleInfo),
        InstanceMethod("scanForPattern", &MemoryHookingAddon::ScanForPattern),
        InstanceMethod("scanForBytes", &MemoryHookingAddon::ScanForBytes),
        InstanceMethod("scanForString", &MemoryHookingAddon::ScanForString),
        InstanceMethod("scanForValue", &MemoryHookingAddon::ScanForValue),
        InstanceMethod("getMemoryRegions", &MemoryHookingAddon::GetMemoryRegions),
        InstanceMethod("isValidAddress", &MemoryHookingAddon::IsValidAddress),
        InstanceMethod("bytesToHexString", &MemoryHookingAddon::BytesToHexString),
        InstanceMethod("hexStringToBytes", &MemoryHookingAddon::HexStringToBytes),
        
        // Memory Helpers - Pointer Operations
        InstanceMethod("findPointersTo", &MemoryHookingAddon::FindPointersTo),
        InstanceMethod("findPointerChain", &MemoryHookingAddon::FindPointerChain),
        InstanceMethod("findNullPointers", &MemoryHookingAddon::FindNullPointers),
        
        // Memory Helpers - Pattern Searching
        InstanceMethod("searchBytePattern", &MemoryHookingAddon::SearchBytePattern),
        InstanceMethod("searchStringPattern", &MemoryHookingAddon::SearchStringPattern),
        InstanceMethod("searchRegexPattern", &MemoryHookingAddon::SearchRegexPattern),
        
        // Memory Helpers - Numeric Searching
        InstanceMethod("searchInt32", &MemoryHookingAddon::SearchInt32),
        InstanceMethod("searchInt64", &MemoryHookingAddon::SearchInt64),
        InstanceMethod("searchFloat", &MemoryHookingAddon::SearchFloat),
        InstanceMethod("searchDouble", &MemoryHookingAddon::SearchDouble),
        
        // Memory Helpers - Region Analysis
        InstanceMethod("getMemoryRegionsEx", &MemoryHookingAddon::GetMemoryRegionsEx),
        InstanceMethod("getModuleRegions", &MemoryHookingAddon::GetModuleRegions),
        InstanceMethod("getRegionInfo", &MemoryHookingAddon::GetRegionInfo),
        
        // Memory Helpers - Advanced Scanning
        InstanceMethod("scanForCode", &MemoryHookingAddon::ScanForCode),
        InstanceMethod("findFunctionPrologs", &MemoryHookingAddon::FindFunctionPrologs),
        InstanceMethod("findCallInstructions", &MemoryHookingAddon::FindCallInstructions),
        
        // Memory Helpers - Comparison and Diffing
        InstanceMethod("compareMemoryRegions", &MemoryHookingAddon::CompareMemoryRegions),
        InstanceMethod("findChangedBytes", &MemoryHookingAddon::FindChangedBytes),
        
        // Memory Helpers - Utility Functions
        InstanceMethod("parseBytePattern", &MemoryHookingAddon::ParseBytePattern),
        InstanceMethod("parsePatternMask", &MemoryHookingAddon::ParsePatternMask),
        InstanceMethod("formatAddress", &MemoryHookingAddon::FormatAddress),
        InstanceMethod("isValidCodeAddress", &MemoryHookingAddon::IsValidCodeAddress),
        InstanceMethod("getAlignment", &MemoryHookingAddon::GetAlignment),
        
        // Automation Helpers - Mouse Control
        InstanceMethod("moveMouse", &MemoryHookingAddon::MoveMouse),
        InstanceMethod("clickMouse", &MemoryHookingAddon::ClickMouse),
        InstanceMethod("doubleClickMouse", &MemoryHookingAddon::DoubleClickMouse),
        InstanceMethod("dragMouse", &MemoryHookingAddon::DragMouse),
        InstanceMethod("scrollMouse", &MemoryHookingAddon::ScrollMouse),
        InstanceMethod("getMousePosition", &MemoryHookingAddon::GetMousePosition),
        InstanceMethod("isMouseButtonPressed", &MemoryHookingAddon::IsMouseButtonPressed),
        
        // Automation Helpers - Keyboard Control
        InstanceMethod("pressKey", &MemoryHookingAddon::PressKey),
        InstanceMethod("releaseKey", &MemoryHookingAddon::ReleaseKey),
        InstanceMethod("sendKeyCombo", &MemoryHookingAddon::SendKeyCombo),
        InstanceMethod("sendText", &MemoryHookingAddon::SendText),
        InstanceMethod("sendTextToWindow", &MemoryHookingAddon::SendTextToWindow),
        InstanceMethod("isKeyPressed", &MemoryHookingAddon::IsKeyPressed),
        InstanceMethod("getPressedKeys", &MemoryHookingAddon::GetPressedKeys),
        
        // Automation Helpers - Screenshot and Screen Capture
        InstanceMethod("captureScreen", &MemoryHookingAddon::CaptureScreen),
        InstanceMethod("captureWindow", &MemoryHookingAddon::CaptureWindow),
        InstanceMethod("captureRegion", &MemoryHookingAddon::CaptureRegion),
        InstanceMethod("captureActiveWindow", &MemoryHookingAddon::CaptureActiveWindow),
        InstanceMethod("saveScreenshotToFile", &MemoryHookingAddon::SaveScreenshotToFile),
        
        // Automation Helpers - Window Management
        InstanceMethod("getAllWindows", &MemoryHookingAddon::GetAllWindows),
        InstanceMethod("getWindowsByTitle", &MemoryHookingAddon::GetWindowsByTitle),
        InstanceMethod("getWindowsByClassName", &MemoryHookingAddon::GetWindowsByClassName),
        InstanceMethod("getActiveWindow", &MemoryHookingAddon::GetActiveWindow),
        InstanceMethod("setActiveWindow", &MemoryHookingAddon::SetActiveWindow),
        InstanceMethod("showWindow", &MemoryHookingAddon::ShowWindow),
        InstanceMethod("moveWindow", &MemoryHookingAddon::MoveWindow),
        InstanceMethod("closeWindow", &MemoryHookingAddon::CloseWindow),
        
        // Automation Helpers - Screen Information
        InstanceMethod("getScreenWidth", &MemoryHookingAddon::GetScreenWidth),
        InstanceMethod("getScreenHeight", &MemoryHookingAddon::GetScreenHeight),
        InstanceMethod("getScreenDPI", &MemoryHookingAddon::GetScreenDPI),
        InstanceMethod("getMonitorRects", &MemoryHookingAddon::GetMonitorRects),
        
        // Automation Helpers - Color and Pixel Operations
        InstanceMethod("getPixelColor", &MemoryHookingAddon::GetPixelColor),
        InstanceMethod("getPixelColors", &MemoryHookingAddon::GetPixelColors),
        InstanceMethod("findColorOnScreen", &MemoryHookingAddon::FindColorOnScreen),
        InstanceMethod("findColorInRegion", &MemoryHookingAddon::FindColorInRegion),
        
        // Automation Helpers - Wait and Timing
        InstanceMethod("sleep", &MemoryHookingAddon::Sleep),
        InstanceMethod("waitForWindow", &MemoryHookingAddon::WaitForWindow),
        InstanceMethod("waitForPixelColor", &MemoryHookingAddon::WaitForPixelColor),
        InstanceMethod("waitForKeyPress", &MemoryHookingAddon::WaitForKeyPress),
        
        // Automation Helpers - System Information
        InstanceMethod("getSystemInfo", &MemoryHookingAddon::GetSystemInfo),
        InstanceMethod("getRunningWindowTitles", &MemoryHookingAddon::GetRunningWindowTitles),
        InstanceMethod("isScreenLocked", &MemoryHookingAddon::IsScreenLocked),
        InstanceMethod("isUserIdle", &MemoryHookingAddon::IsUserIdle)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("MemoryHooking", func);
    return exports;
}

MemoryHookingAddon::MemoryHookingAddon(const Napi::CallbackInfo& info) 
    : Napi::ObjectWrap<MemoryHookingAddon>(info) {
    
    m_processManager = std::make_unique<ProcessManager>();
    m_memoryScanner = std::make_unique<MemoryScanner>(m_processManager.get());
}

Napi::Value MemoryHookingAddon::GetRunningProcesses(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    auto processes = m_processManager->getRunningProcesses();
    Napi::Array result = Napi::Array::New(env, processes.size());
    
    for (size_t i = 0; i < processes.size(); ++i) {
        Napi::Object processObj = Napi::Object::New(env);
        processObj.Set("processId", Napi::Number::New(env, processes[i].processId));
        processObj.Set("processName", Napi::String::New(env, processes[i].processName));
        processObj.Set("windowTitle", Napi::String::New(env, processes[i].windowTitle));
        result[i] = processObj;
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::AttachToProcess(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected process ID or name").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    bool success = false;
    
    if (info[0].IsNumber()) {
        DWORD processId = info[0].As<Napi::Number>().Uint32Value();
        success = m_processManager->attachToProcess(processId);
    } else if (info[0].IsString()) {
        std::string processName = info[0].As<Napi::String>().Utf8Value();
        success = m_processManager->attachToProcess(processName);
    }
    
    return Napi::Boolean::New(env, success);
}

Napi::Value MemoryHookingAddon::DetachFromProcess(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    m_processManager->detachFromProcess();
    return env.Undefined();
}

Napi::Value MemoryHookingAddon::IsProcessAttached(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, m_processManager->isProcessAttached());
}

Napi::Value MemoryHookingAddon::GetCurrentProcessInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("processId", Napi::Number::New(env, m_processManager->getCurrentProcessId()));
    result.Set("processName", Napi::String::New(env, m_processManager->getCurrentProcessName()));
    result.Set("isAttached", Napi::Boolean::New(env, m_processManager->isProcessAttached()));
    
    return result;
}

Napi::Value MemoryHookingAddon::ReadMemory(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected address and size").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uint64_t address = info[0].As<Napi::Number>().Int64Value();
    size_t size = info[1].As<Napi::Number>().Uint32Value();
    
    std::vector<BYTE> buffer(size);
    bool success = m_processManager->readMemory(reinterpret_cast<LPCVOID>(address), buffer.data(), size);
    
    if (!success) {
        return env.Null();
    }
    
    Napi::Buffer<BYTE> result = Napi::Buffer<BYTE>::Copy(env, buffer.data(), size);
    return result;
}

Napi::Value MemoryHookingAddon::WriteMemory(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected address and data").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uint64_t address = info[0].As<Napi::Number>().Int64Value();
    Napi::Buffer<BYTE> data = info[1].As<Napi::Buffer<BYTE>>();
    
    bool success = m_processManager->writeMemory(
        reinterpret_cast<LPVOID>(address), 
        data.Data(), 
        data.Length()
    );
    
    return Napi::Boolean::New(env, success);
}

Napi::Value MemoryHookingAddon::GetModuleInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected module name").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string moduleName = info[0].As<Napi::String>().Utf8Value();
    
    LPVOID baseAddress = m_processManager->getModuleBaseAddress(moduleName);
    SIZE_T moduleSize = m_processManager->getModuleSize(moduleName);
    
    if (baseAddress == nullptr) {
        return env.Null();
    }
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("baseAddress", Napi::Number::New(env, reinterpret_cast<uint64_t>(baseAddress)));
    result.Set("size", Napi::Number::New(env, moduleSize));
    result.Set("name", Napi::String::New(env, moduleName));
    
    return result;
}

Napi::Value MemoryHookingAddon::ScanForPattern(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected pattern and mask").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string pattern = info[0].As<Napi::String>().Utf8Value();
    std::string mask = info[1].As<Napi::String>().Utf8Value();
    
    auto addresses = m_memoryScanner->scanForPattern(pattern, mask);
    
    Napi::Array result = Napi::Array::New(env, addresses.size());
    for (size_t i = 0; i < addresses.size(); ++i) {
        result[i] = Napi::Number::New(env, reinterpret_cast<uint64_t>(addresses[i]));
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::ScanForString(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected string to search").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string searchString = info[0].As<Napi::String>().Utf8Value();
    bool caseSensitive = info.Length() > 1 ? info[1].As<Napi::Boolean>().Value() : true;
    
    auto addresses = m_memoryScanner->scanForString(searchString, caseSensitive);
    
    Napi::Array result = Napi::Array::New(env, addresses.size());
    for (size_t i = 0; i < addresses.size(); ++i) {
        result[i] = Napi::Number::New(env, reinterpret_cast<uint64_t>(addresses[i]));
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::ScanForValue(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected value to search").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::vector<LPVOID> addresses;
    
    if (info[0].IsNumber()) {
        double value = info[0].As<Napi::Number>().DoubleValue();
        if (value == static_cast<int>(value)) {
            addresses = m_memoryScanner->scanForValue(static_cast<int>(value));
        } else {
            addresses = m_memoryScanner->scanForValue(value);
        }
    } else if (info[0].IsString()) {
        std::string value = info[0].As<Napi::String>().Utf8Value();
        addresses = m_memoryScanner->scanForValue(value);
    }
    
    Napi::Array result = Napi::Array::New(env, addresses.size());
    for (size_t i = 0; i < addresses.size(); ++i) {
        result[i] = Napi::Number::New(env, reinterpret_cast<uint64_t>(addresses[i]));
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::ScanForBytes(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected bytes array").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    Napi::Array bytesArray = info[0].As<Napi::Array>();
    std::vector<BYTE> bytes;
    
    for (uint32_t i = 0; i < bytesArray.Length(); ++i) {
        bytes.push_back(static_cast<BYTE>(bytesArray.Get(i).As<Napi::Number>().Uint32Value()));
    }
    
    auto addresses = m_memoryScanner->scanForBytes(bytes);
    
    Napi::Array result = Napi::Array::New(env, addresses.size());
    for (size_t i = 0; i < addresses.size(); ++i) {
        result[i] = Napi::Number::New(env, reinterpret_cast<uint64_t>(addresses[i]));
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::GetMemoryRegions(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    auto regions = m_memoryScanner->getMemoryRegions();
    Napi::Array result = Napi::Array::New(env, regions.size());
    
    for (size_t i = 0; i < regions.size(); ++i) {
        Napi::Object regionObj = Napi::Object::New(env);
        regionObj.Set("baseAddress", Napi::Number::New(env, reinterpret_cast<uint64_t>(regions[i].baseAddress)));
        regionObj.Set("size", Napi::Number::New(env, regions[i].size));
        regionObj.Set("protection", Napi::Number::New(env, regions[i].protection));
        regionObj.Set("state", Napi::Number::New(env, regions[i].state));
        regionObj.Set("type", Napi::Number::New(env, regions[i].type));
        result[i] = regionObj;
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::IsValidAddress(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected address").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uint64_t address = info[0].As<Napi::Number>().Int64Value();
    bool isValid = m_memoryScanner->isValidAddress(reinterpret_cast<LPVOID>(address));
    
    return Napi::Boolean::New(env, isValid);
}

Napi::Value MemoryHookingAddon::BytesToHexString(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected bytes array").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    Napi::Array bytesArray = info[0].As<Napi::Array>();
    std::vector<BYTE> bytes;
    
    for (uint32_t i = 0; i < bytesArray.Length(); ++i) {
        bytes.push_back(static_cast<BYTE>(bytesArray.Get(i).As<Napi::Number>().Uint32Value()));
    }
    
    std::string hexString = m_memoryScanner->bytesToHexString(bytes);
    return Napi::String::New(env, hexString);
}

Napi::Value MemoryHookingAddon::HexStringToBytes(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected hex string").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string hexString = info[0].As<Napi::String>().Utf8Value();
    auto bytes = m_memoryScanner->hexStringToBytes(hexString);
    
    Napi::Array result = Napi::Array::New(env, bytes.size());
    for (size_t i = 0; i < bytes.size(); ++i) {
        result[i] = Napi::Number::New(env, bytes[i]);
    }
    
    return result;
}

// Memory Helpers - Pointer Operations
Napi::Value MemoryHookingAddon::FindPointersTo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected target address and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uintptr_t targetAddress = info[0].As<Napi::Number>().Int64Value();
    uintptr_t searchStart = info.Length() > 1 ? info[1].As<Napi::Number>().Int64Value() : 0;
    uintptr_t searchEnd = info.Length() > 2 ? info[2].As<Napi::Number>().Int64Value() : 0;
    
    auto results = MemoryHelpers::FindPointersTo(m_processManager->getProcessHandle(), targetAddress, searchStart, searchEnd);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object pointerObj = Napi::Object::New(env);
        pointerObj.Set("address", Napi::Number::New(env, results[i].address));
        pointerObj.Set("pointsTo", Napi::Number::New(env, results[i].pointsTo));
        pointerObj.Set("offset", Napi::Number::New(env, results[i].offset));
        resultArray[i] = pointerObj;
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::FindPointerChain(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected final address and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uintptr_t finalAddress = info[0].As<Napi::Number>().Int64Value();
    int maxDepth = info.Length() > 1 ? info[1].As<Napi::Number>().Int32Value() : 5;
    
    std::vector<size_t> offsets;
    if (info.Length() > 2 && info[2].IsArray()) {
        Napi::Array offsetsArray = info[2].As<Napi::Array>();
        for (uint32_t i = 0; i < offsetsArray.Length(); ++i) {
            offsets.push_back(offsetsArray.Get(i).As<Napi::Number>().Uint32Value());
        }
    }
    
    auto results = MemoryHelpers::FindPointerChain(m_processManager->getProcessHandle(), finalAddress, maxDepth, offsets);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object pointerObj = Napi::Object::New(env);
        pointerObj.Set("address", Napi::Number::New(env, results[i].address));
        pointerObj.Set("pointsTo", Napi::Number::New(env, results[i].pointsTo));
        pointerObj.Set("offset", Napi::Number::New(env, results[i].offset));
        resultArray[i] = pointerObj;
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::FindNullPointers(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected search start, search end, and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uintptr_t searchStart = info[0].As<Napi::Number>().Int64Value();
    uintptr_t searchEnd = info[1].As<Napi::Number>().Int64Value();
    
    auto results = MemoryHelpers::FindNullPointers(m_processManager->getProcessHandle(), searchStart, searchEnd);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        resultArray[i] = Napi::Number::New(env, results[i]);
    }
    
    return resultArray;
}

// Memory Helpers - Pattern Searching
Napi::Value MemoryHookingAddon::SearchBytePattern(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected pattern array, mask array, and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    // Parse pattern
    Napi::Array patternArray = info[0].As<Napi::Array>();
    std::vector<uint8_t> pattern;
    for (uint32_t i = 0; i < patternArray.Length(); ++i) {
        pattern.push_back(static_cast<uint8_t>(patternArray.Get(i).As<Napi::Number>().Uint32Value()));
    }
    
    // Parse mask
    Napi::Array maskArray = info[1].As<Napi::Array>();
    std::vector<bool> mask;
    for (uint32_t i = 0; i < maskArray.Length(); ++i) {
        mask.push_back(maskArray.Get(i).As<Napi::Boolean>().Value());
    }
    
    uintptr_t searchStart = info.Length() > 2 ? info[2].As<Napi::Number>().Int64Value() : 0;
    uintptr_t searchEnd = info.Length() > 3 ? info[3].As<Napi::Number>().Int64Value() : 0;
    
    auto results = MemoryHelpers::SearchBytePattern(m_processManager->getProcessHandle(), pattern, mask, searchStart, searchEnd);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object matchObj = Napi::Object::New(env);
        matchObj.Set("address", Napi::Number::New(env, results[i].address));
        matchObj.Set("size", Napi::Number::New(env, results[i].size));
        
        Napi::Array dataArray = Napi::Array::New(env, results[i].data.size());
        for (size_t j = 0; j < results[i].data.size(); ++j) {
            dataArray[j] = Napi::Number::New(env, results[i].data[j]);
        }
        matchObj.Set("data", dataArray);
        
        resultArray[i] = matchObj;
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::SearchStringPattern(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected search string and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string pattern = info[0].As<Napi::String>().Utf8Value();
    bool caseSensitive = info.Length() > 1 ? info[1].As<Napi::Boolean>().Value() : true;
    bool unicode = info.Length() > 2 ? info[2].As<Napi::Boolean>().Value() : false;
    uintptr_t searchStart = info.Length() > 3 ? info[3].As<Napi::Number>().Int64Value() : 0;
    uintptr_t searchEnd = info.Length() > 4 ? info[4].As<Napi::Number>().Int64Value() : 0;
    
    auto results = MemoryHelpers::SearchStringPattern(m_processManager->getProcessHandle(), pattern, caseSensitive, unicode, searchStart, searchEnd);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object matchObj = Napi::Object::New(env);
        matchObj.Set("address", Napi::Number::New(env, results[i].address));
        matchObj.Set("size", Napi::Number::New(env, results[i].size));
        
        Napi::Array dataArray = Napi::Array::New(env, results[i].data.size());
        for (size_t j = 0; j < results[i].data.size(); ++j) {
            dataArray[j] = Napi::Number::New(env, results[i].data[j]);
        }
        matchObj.Set("data", dataArray);
        
        resultArray[i] = matchObj;
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::SearchRegexPattern(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected regex pattern and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string regexPattern = info[0].As<Napi::String>().Utf8Value();
    uintptr_t searchStart = info.Length() > 1 ? info[1].As<Napi::Number>().Int64Value() : 0;
    uintptr_t searchEnd = info.Length() > 2 ? info[2].As<Napi::Number>().Int64Value() : 0;
    
    auto results = MemoryHelpers::SearchRegexPattern(m_processManager->getProcessHandle(), regexPattern, searchStart, searchEnd);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object matchObj = Napi::Object::New(env);
        matchObj.Set("address", Napi::Number::New(env, results[i].address));
        matchObj.Set("size", Napi::Number::New(env, results[i].size));
        
        Napi::Array dataArray = Napi::Array::New(env, results[i].data.size());
        for (size_t j = 0; j < results[i].data.size(); ++j) {
            dataArray[j] = Napi::Number::New(env, results[i].data[j]);
        }
        matchObj.Set("data", dataArray);
        
        resultArray[i] = matchObj;
    }
    
    return resultArray;
}

// Memory Helpers - Numeric Searching
Napi::Value MemoryHookingAddon::SearchInt32(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected int32 value and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int32_t value = info[0].As<Napi::Number>().Int32Value();
    uintptr_t searchStart = info.Length() > 1 ? info[1].As<Napi::Number>().Int64Value() : 0;
    uintptr_t searchEnd = info.Length() > 2 ? info[2].As<Napi::Number>().Int64Value() : 0;
    bool aligned = info.Length() > 3 ? info[3].As<Napi::Boolean>().Value() : false;
    
    auto results = MemoryHelpers::SearchInt32(m_processManager->getProcessHandle(), value, searchStart, searchEnd, aligned);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object matchObj = Napi::Object::New(env);
        matchObj.Set("address", Napi::Number::New(env, results[i].address));
        matchObj.Set("dataType", Napi::String::New(env, results[i].dataType));
        
        Napi::Array dataArray = Napi::Array::New(env, results[i].rawData.size());
        for (size_t j = 0; j < results[i].rawData.size(); ++j) {
            dataArray[j] = Napi::Number::New(env, results[i].rawData[j]);
        }
        matchObj.Set("rawData", dataArray);
        
        resultArray[i] = matchObj;
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::SearchInt64(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected int64 value and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int64_t value = info[0].As<Napi::Number>().Int64Value();
    uintptr_t searchStart = info.Length() > 1 ? info[1].As<Napi::Number>().Int64Value() : 0;
    uintptr_t searchEnd = info.Length() > 2 ? info[2].As<Napi::Number>().Int64Value() : 0;
    bool aligned = info.Length() > 3 ? info[3].As<Napi::Boolean>().Value() : false;
    
    auto results = MemoryHelpers::SearchInt64(m_processManager->getProcessHandle(), value, searchStart, searchEnd, aligned);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object matchObj = Napi::Object::New(env);
        matchObj.Set("address", Napi::Number::New(env, results[i].address));
        matchObj.Set("dataType", Napi::String::New(env, results[i].dataType));
        
        Napi::Array dataArray = Napi::Array::New(env, results[i].rawData.size());
        for (size_t j = 0; j < results[i].rawData.size(); ++j) {
            dataArray[j] = Napi::Number::New(env, results[i].rawData[j]);
        }
        matchObj.Set("rawData", dataArray);
        
        resultArray[i] = matchObj;
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::SearchFloat(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected float value and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    float value = info[0].As<Napi::Number>().FloatValue();
    float tolerance = info.Length() > 1 ? info[1].As<Napi::Number>().FloatValue() : 0.001f;
    uintptr_t searchStart = info.Length() > 2 ? info[2].As<Napi::Number>().Int64Value() : 0;
    uintptr_t searchEnd = info.Length() > 3 ? info[3].As<Napi::Number>().Int64Value() : 0;
    
    auto results = MemoryHelpers::SearchFloat(m_processManager->getProcessHandle(), value, tolerance, searchStart, searchEnd);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object matchObj = Napi::Object::New(env);
        matchObj.Set("address", Napi::Number::New(env, results[i].address));
        matchObj.Set("dataType", Napi::String::New(env, results[i].dataType));
        
        Napi::Array dataArray = Napi::Array::New(env, results[i].rawData.size());
        for (size_t j = 0; j < results[i].rawData.size(); ++j) {
            dataArray[j] = Napi::Number::New(env, results[i].rawData[j]);
        }
        matchObj.Set("rawData", dataArray);
        
        resultArray[i] = matchObj;
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::SearchDouble(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected double value and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    double value = info[0].As<Napi::Number>().DoubleValue();
    double tolerance = info.Length() > 1 ? info[1].As<Napi::Number>().DoubleValue() : 0.001;
    uintptr_t searchStart = info.Length() > 2 ? info[2].As<Napi::Number>().Int64Value() : 0;
    uintptr_t searchEnd = info.Length() > 3 ? info[3].As<Napi::Number>().Int64Value() : 0;
    
    auto results = MemoryHelpers::SearchDouble(m_processManager->getProcessHandle(), value, tolerance, searchStart, searchEnd);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object matchObj = Napi::Object::New(env);
        matchObj.Set("address", Napi::Number::New(env, results[i].address));
        matchObj.Set("dataType", Napi::String::New(env, results[i].dataType));
        
        Napi::Array dataArray = Napi::Array::New(env, results[i].rawData.size());
        for (size_t j = 0; j < results[i].rawData.size(); ++j) {
            dataArray[j] = Napi::Number::New(env, results[i].rawData[j]);
        }
        matchObj.Set("rawData", dataArray);
        
        resultArray[i] = matchObj;
    }
    
    return resultArray;
}

// Memory Helpers - Region Analysis
Napi::Value MemoryHookingAddon::GetMemoryRegionsEx(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Process not attached").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    bool executableOnly = info.Length() > 0 ? info[0].As<Napi::Boolean>().Value() : false;
    bool writableOnly = info.Length() > 1 ? info[1].As<Napi::Boolean>().Value() : false;
    
    auto results = MemoryHelpers::GetMemoryRegions(m_processManager->getProcessHandle(), executableOnly, writableOnly);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object regionObj = Napi::Object::New(env);
        regionObj.Set("startAddress", Napi::Number::New(env, results[i].startAddress));
        regionObj.Set("endAddress", Napi::Number::New(env, results[i].endAddress));
        regionObj.Set("size", Napi::Number::New(env, results[i].size));
        regionObj.Set("protection", Napi::Number::New(env, results[i].protection));
        regionObj.Set("type", Napi::Number::New(env, results[i].type));
        regionObj.Set("moduleName", Napi::String::New(env, results[i].moduleName));
        resultArray[i] = regionObj;
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::GetModuleRegions(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Process not attached").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string moduleName = info.Length() > 0 ? info[0].As<Napi::String>().Utf8Value() : "";
    
    auto results = MemoryHelpers::GetModuleRegions(m_processManager->getProcessHandle(), moduleName);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object regionObj = Napi::Object::New(env);
        regionObj.Set("startAddress", Napi::Number::New(env, results[i].startAddress));
        regionObj.Set("endAddress", Napi::Number::New(env, results[i].endAddress));
        regionObj.Set("size", Napi::Number::New(env, results[i].size));
        regionObj.Set("protection", Napi::Number::New(env, results[i].protection));
        regionObj.Set("type", Napi::Number::New(env, results[i].type));
        regionObj.Set("moduleName", Napi::String::New(env, results[i].moduleName));
        resultArray[i] = regionObj;
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::GetRegionInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected address and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uintptr_t address = info[0].As<Napi::Number>().Int64Value();
    
    auto result = MemoryHelpers::GetRegionInfo(m_processManager->getProcessHandle(), address);
    
    Napi::Object regionObj = Napi::Object::New(env);
    regionObj.Set("startAddress", Napi::Number::New(env, result.startAddress));
    regionObj.Set("endAddress", Napi::Number::New(env, result.endAddress));
    regionObj.Set("size", Napi::Number::New(env, result.size));
    regionObj.Set("protection", Napi::Number::New(env, result.protection));
    regionObj.Set("type", Napi::Number::New(env, result.type));
    regionObj.Set("moduleName", Napi::String::New(env, result.moduleName));
    
    return regionObj;
}

// Memory Helpers - Advanced Scanning
Napi::Value MemoryHookingAddon::ScanForCode(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected opcodes array and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    Napi::Array opcodesArray = info[0].As<Napi::Array>();
    std::vector<uint8_t> opcodes;
    for (uint32_t i = 0; i < opcodesArray.Length(); ++i) {
        opcodes.push_back(static_cast<uint8_t>(opcodesArray.Get(i).As<Napi::Number>().Uint32Value()));
    }
    
    uintptr_t searchStart = info.Length() > 1 ? info[1].As<Napi::Number>().Int64Value() : 0;
    uintptr_t searchEnd = info.Length() > 2 ? info[2].As<Napi::Number>().Int64Value() : 0;
    
    auto results = MemoryHelpers::ScanForCode(m_processManager->getProcessHandle(), opcodes, searchStart, searchEnd);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        Napi::Object matchObj = Napi::Object::New(env);
        matchObj.Set("address", Napi::Number::New(env, results[i].address));
        matchObj.Set("size", Napi::Number::New(env, results[i].size));
        
        Napi::Array dataArray = Napi::Array::New(env, results[i].data.size());
        for (size_t j = 0; j < results[i].data.size(); ++j) {
            dataArray[j] = Napi::Number::New(env, results[i].data[j]);
        }
        matchObj.Set("data", dataArray);
        
        resultArray[i] = matchObj;
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::FindFunctionPrologs(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (!m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Process not attached").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uintptr_t searchStart = info.Length() > 0 ? info[0].As<Napi::Number>().Int64Value() : 0;
    uintptr_t searchEnd = info.Length() > 1 ? info[1].As<Napi::Number>().Int64Value() : 0;
    
    auto results = MemoryHelpers::FindFunctionPrologs(m_processManager->getProcessHandle(), searchStart, searchEnd);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        resultArray[i] = Napi::Number::New(env, results[i]);
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::FindCallInstructions(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected target address and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uintptr_t targetAddress = info[0].As<Napi::Number>().Int64Value();
    uintptr_t searchStart = info.Length() > 1 ? info[1].As<Napi::Number>().Int64Value() : 0;
    uintptr_t searchEnd = info.Length() > 2 ? info[2].As<Napi::Number>().Int64Value() : 0;
    
    auto results = MemoryHelpers::FindCallInstructions(m_processManager->getProcessHandle(), targetAddress, searchStart, searchEnd);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        resultArray[i] = Napi::Number::New(env, results[i]);
    }
    
    return resultArray;
}

// Memory Helpers - Comparison and Diffing
Napi::Value MemoryHookingAddon::CompareMemoryRegions(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected region1Start, region2Start, size, and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uintptr_t region1Start = info[0].As<Napi::Number>().Int64Value();
    uintptr_t region2Start = info[1].As<Napi::Number>().Int64Value();
    size_t size = info[2].As<Napi::Number>().Uint32Value();
    
    auto results = MemoryHelpers::CompareMemoryRegions(m_processManager->getProcessHandle(), region1Start, region2Start, size);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        resultArray[i] = Napi::Number::New(env, results[i]);
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::FindChangedBytes(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected address, original data array, and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uintptr_t address = info[0].As<Napi::Number>().Int64Value();
    
    Napi::Array originalArray = info[1].As<Napi::Array>();
    std::vector<uint8_t> originalData;
    for (uint32_t i = 0; i < originalArray.Length(); ++i) {
        originalData.push_back(static_cast<uint8_t>(originalArray.Get(i).As<Napi::Number>().Uint32Value()));
    }
    
    auto results = MemoryHelpers::FindChangedBytes(m_processManager->getProcessHandle(), address, originalData);
    
    Napi::Array resultArray = Napi::Array::New(env, results.size());
    for (size_t i = 0; i < results.size(); ++i) {
        resultArray[i] = Napi::Number::New(env, results[i]);
    }
    
    return resultArray;
}

// Memory Helpers - Utility Functions
Napi::Value MemoryHookingAddon::ParseBytePattern(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected pattern string").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string pattern = info[0].As<Napi::String>().Utf8Value();
    auto result = MemoryHelpers::ParseBytePattern(pattern);
    
    Napi::Array resultArray = Napi::Array::New(env, result.size());
    for (size_t i = 0; i < result.size(); ++i) {
        resultArray[i] = Napi::Number::New(env, result[i]);
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::ParsePatternMask(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected pattern string").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string pattern = info[0].As<Napi::String>().Utf8Value();
    auto result = MemoryHelpers::ParsePatternMask(pattern);
    
    Napi::Array resultArray = Napi::Array::New(env, result.size());
    for (size_t i = 0; i < result.size(); ++i) {
        resultArray[i] = Napi::Boolean::New(env, result[i]);
    }
    
    return resultArray;
}

Napi::Value MemoryHookingAddon::FormatAddress(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected address").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uintptr_t address = info[0].As<Napi::Number>().Int64Value();
    bool withPrefix = info.Length() > 1 ? info[1].As<Napi::Boolean>().Value() : true;
    
    std::string result = MemoryHelpers::FormatAddress(address, withPrefix);
    return Napi::String::New(env, result);
}

Napi::Value MemoryHookingAddon::IsValidCodeAddress(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1 || !m_processManager->isProcessAttached()) {
        Napi::TypeError::New(env, "Expected address and attached process").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uintptr_t address = info[0].As<Napi::Number>().Int64Value();
    bool result = MemoryHelpers::IsValidCodeAddress(m_processManager->getProcessHandle(), address);
    
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::GetAlignment(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected address").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uintptr_t address = info[0].As<Napi::Number>().Int64Value();
    size_t result = MemoryHelpers::GetAlignment(address);
    
    return Napi::Number::New(env, result);
}

// Automation Helpers - Mouse Control
Napi::Value MemoryHookingAddon::MoveMouse(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected x and y coordinates").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    
    bool result = AutomationHelpers::MoveMouse(x, y);
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::ClickMouse(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    int button = info.Length() > 0 ? info[0].As<Napi::Number>().Int32Value() : 1; // Default to left button
    int x = info.Length() > 1 ? info[1].As<Napi::Number>().Int32Value() : -1;
    int y = info.Length() > 2 ? info[2].As<Napi::Number>().Int32Value() : -1;
    
    MouseButton mouseButton = static_cast<MouseButton>(button);
    bool result = AutomationHelpers::ClickMouse(mouseButton, x, y);
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::DoubleClickMouse(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    int button = info.Length() > 0 ? info[0].As<Napi::Number>().Int32Value() : 1;
    int x = info.Length() > 1 ? info[1].As<Napi::Number>().Int32Value() : -1;
    int y = info.Length() > 2 ? info[2].As<Napi::Number>().Int32Value() : -1;
    
    MouseButton mouseButton = static_cast<MouseButton>(button);
    bool result = AutomationHelpers::DoubleClickMouse(mouseButton, x, y);
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::DragMouse(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected startX, startY, endX, endY").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int startX = info[0].As<Napi::Number>().Int32Value();
    int startY = info[1].As<Napi::Number>().Int32Value();
    int endX = info[2].As<Napi::Number>().Int32Value();
    int endY = info[3].As<Napi::Number>().Int32Value();
    int button = info.Length() > 4 ? info[4].As<Napi::Number>().Int32Value() : 1;
    
    MouseButton mouseButton = static_cast<MouseButton>(button);
    bool result = AutomationHelpers::DragMouse(startX, startY, endX, endY, mouseButton);
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::ScrollMouse(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected x, y, scrollAmount").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    int scrollAmount = info[2].As<Napi::Number>().Int32Value();
    
    bool result = AutomationHelpers::ScrollMouse(x, y, scrollAmount);
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::GetMousePosition(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    MousePosition pos = AutomationHelpers::GetMousePosition();
    
    Napi::Object position = Napi::Object::New(env);
    position.Set("x", Napi::Number::New(env, pos.x));
    position.Set("y", Napi::Number::New(env, pos.y));
    
    return position;
}

Napi::Value MemoryHookingAddon::IsMouseButtonPressed(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected button code").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int button = info[0].As<Napi::Number>().Int32Value();
    MouseButton mouseButton = static_cast<MouseButton>(button);
    bool result = AutomationHelpers::IsMouseButtonPressed(mouseButton);
    
    return Napi::Boolean::New(env, result);
}

// Automation Helpers - Keyboard Control
Napi::Value MemoryHookingAddon::PressKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected key code").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int keyCode = info[0].As<Napi::Number>().Int32Value();
    int duration = info.Length() > 1 ? info[1].As<Napi::Number>().Int32Value() : 0;
    KeyCode key = static_cast<KeyCode>(keyCode);
    bool result = AutomationHelpers::PressKey(key, duration);
    
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::ReleaseKey(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected key code").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int keyCode = info[0].As<Napi::Number>().Int32Value();
    KeyCode key = static_cast<KeyCode>(keyCode);
    bool result = AutomationHelpers::ReleaseKey(key);
    
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::SendKeyCombo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected key codes array").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    Napi::Array keysArray = info[0].As<Napi::Array>();
    std::vector<KeyCode> keys;
    
    for (uint32_t i = 0; i < keysArray.Length(); ++i) {
        int keyInt = keysArray.Get(i).As<Napi::Number>().Int32Value();
        keys.push_back(static_cast<KeyCode>(keyInt));
    }
    
    bool result = AutomationHelpers::SendKeyCombo(keys);
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::SendText(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected text string").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string text = info[0].As<Napi::String>().Utf8Value();
    bool result = AutomationHelpers::SendText(text);
    
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::SendTextToWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected window handle and text").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uint64_t hwnd = info[0].As<Napi::Number>().Int64Value();
    std::string text = info[1].As<Napi::String>().Utf8Value();
    
    bool result = AutomationHelpers::SendTextToWindow(reinterpret_cast<HWND>(hwnd), text);
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::IsKeyPressed(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected key code").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int keyCode = info[0].As<Napi::Number>().Int32Value();
    KeyCode key = static_cast<KeyCode>(keyCode);
    bool result = AutomationHelpers::IsKeyPressed(key);
    
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::GetPressedKeys(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    auto pressedKeys = AutomationHelpers::GetPressedKeys();
    
    Napi::Array result = Napi::Array::New(env, pressedKeys.size());
    for (size_t i = 0; i < pressedKeys.size(); ++i) {
        result[i] = Napi::Number::New(env, static_cast<int>(pressedKeys[i]));
    }
    
    return result;
}

// Automation Helpers - Screenshot and Screen Capture
Napi::Value MemoryHookingAddon::CaptureScreen(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    auto screenshot = AutomationHelpers::CaptureScreen();
    
    if (screenshot.data.empty()) {
        return env.Null();
    }
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("width", Napi::Number::New(env, screenshot.width));
    result.Set("height", Napi::Number::New(env, screenshot.height));
    result.Set("bitsPerPixel", Napi::Number::New(env, screenshot.bitsPerPixel));
    
    Napi::Array dataArray = Napi::Array::New(env, screenshot.data.size());
    for (size_t i = 0; i < screenshot.data.size(); ++i) {
        dataArray[i] = Napi::Number::New(env, screenshot.data[i]);
    }
    result.Set("data", dataArray);
    
    return result;
}

Napi::Value MemoryHookingAddon::CaptureWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected window handle").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uint64_t hwnd = info[0].As<Napi::Number>().Int64Value();
    auto screenshot = AutomationHelpers::CaptureWindow(reinterpret_cast<HWND>(hwnd));
    
    if (screenshot.data.empty()) {
        return env.Null();
    }
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("width", Napi::Number::New(env, screenshot.width));
    result.Set("height", Napi::Number::New(env, screenshot.height));
    result.Set("bitsPerPixel", Napi::Number::New(env, screenshot.bitsPerPixel));
    
    Napi::Array dataArray = Napi::Array::New(env, screenshot.data.size());
    for (size_t i = 0; i < screenshot.data.size(); ++i) {
        dataArray[i] = Napi::Number::New(env, screenshot.data[i]);
    }
    result.Set("data", dataArray);
    
    return result;
}

Napi::Value MemoryHookingAddon::CaptureRegion(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected x, y, width, height").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    int width = info[2].As<Napi::Number>().Int32Value();
    int height = info[3].As<Napi::Number>().Int32Value();
    
    auto screenshot = AutomationHelpers::CaptureRegion(x, y, width, height);
    
    if (screenshot.data.empty()) {
        return env.Null();
    }
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("width", Napi::Number::New(env, screenshot.width));
    result.Set("height", Napi::Number::New(env, screenshot.height));
    result.Set("bitsPerPixel", Napi::Number::New(env, screenshot.bitsPerPixel));
    
    Napi::Array dataArray = Napi::Array::New(env, screenshot.data.size());
    for (size_t i = 0; i < screenshot.data.size(); ++i) {
        dataArray[i] = Napi::Number::New(env, screenshot.data[i]);
    }
    result.Set("data", dataArray);
    
    return result;
}

Napi::Value MemoryHookingAddon::CaptureActiveWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    auto screenshot = AutomationHelpers::CaptureActiveWindow();
    
    if (screenshot.data.empty()) {
        return env.Null();
    }
    
    Napi::Object result = Napi::Object::New(env);
    result.Set("width", Napi::Number::New(env, screenshot.width));
    result.Set("height", Napi::Number::New(env, screenshot.height));
    result.Set("bitsPerPixel", Napi::Number::New(env, screenshot.bitsPerPixel));
    
    Napi::Array dataArray = Napi::Array::New(env, screenshot.data.size());
    for (size_t i = 0; i < screenshot.data.size(); ++i) {
        dataArray[i] = Napi::Number::New(env, screenshot.data[i]);
    }
    result.Set("data", dataArray);
    
    return result;
}

Napi::Value MemoryHookingAddon::SaveScreenshotToFile(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected screenshot data and filename").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    // Create screenshot data from JS object
    Napi::Object screenshotObj = info[0].As<Napi::Object>();
    std::string filename = info[1].As<Napi::String>().Utf8Value();
    
    ScreenshotData screenshot;
    screenshot.width = screenshotObj.Get("width").As<Napi::Number>().Int32Value();
    screenshot.height = screenshotObj.Get("height").As<Napi::Number>().Int32Value();
    screenshot.bitsPerPixel = screenshotObj.Get("bitsPerPixel").As<Napi::Number>().Int32Value();
    
    Napi::Array dataArray = screenshotObj.Get("data").As<Napi::Array>();
    screenshot.data.reserve(dataArray.Length());
    for (uint32_t i = 0; i < dataArray.Length(); ++i) {
        screenshot.data.push_back(static_cast<uint8_t>(dataArray.Get(i).As<Napi::Number>().Uint32Value()));
    }
    
    bool result = AutomationHelpers::SaveScreenshotToFile(screenshot, filename);
    return Napi::Boolean::New(env, result);
}

// Automation Helpers - Window Management
Napi::Value MemoryHookingAddon::GetAllWindows(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    auto windows = AutomationHelpers::GetAllWindows();
    
    Napi::Array result = Napi::Array::New(env, windows.size());
    for (size_t i = 0; i < windows.size(); ++i) {
        Napi::Object windowObj = Napi::Object::New(env);
        windowObj.Set("hwnd", Napi::Number::New(env, reinterpret_cast<uint64_t>(windows[i].hwnd)));
        windowObj.Set("title", Napi::String::New(env, windows[i].title));
        windowObj.Set("className", Napi::String::New(env, windows[i].className));
        windowObj.Set("x", Napi::Number::New(env, windows[i].x));
        windowObj.Set("y", Napi::Number::New(env, windows[i].y));
        windowObj.Set("width", Napi::Number::New(env, windows[i].width));
        windowObj.Set("height", Napi::Number::New(env, windows[i].height));
        windowObj.Set("isVisible", Napi::Boolean::New(env, windows[i].isVisible));
        result[i] = windowObj;
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::GetWindowsByTitle(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected title string").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string title = info[0].As<Napi::String>().Utf8Value();
    
    auto windows = AutomationHelpers::GetWindowsByTitle(title);
    
    Napi::Array result = Napi::Array::New(env, windows.size());
    for (size_t i = 0; i < windows.size(); ++i) {
        Napi::Object windowObj = Napi::Object::New(env);
        windowObj.Set("hwnd", Napi::Number::New(env, reinterpret_cast<uint64_t>(windows[i].hwnd)));
        windowObj.Set("title", Napi::String::New(env, windows[i].title));
        windowObj.Set("className", Napi::String::New(env, windows[i].className));
        windowObj.Set("x", Napi::Number::New(env, windows[i].x));
        windowObj.Set("y", Napi::Number::New(env, windows[i].y));
        windowObj.Set("width", Napi::Number::New(env, windows[i].width));
        windowObj.Set("height", Napi::Number::New(env, windows[i].height));
        windowObj.Set("isVisible", Napi::Boolean::New(env, windows[i].isVisible));
        result[i] = windowObj;
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::GetWindowsByClassName(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected class name string").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string className = info[0].As<Napi::String>().Utf8Value();
    
    auto windows = AutomationHelpers::GetWindowsByClassName(className);
    
    Napi::Array result = Napi::Array::New(env, windows.size());
    for (size_t i = 0; i < windows.size(); ++i) {
        Napi::Object windowObj = Napi::Object::New(env);
        windowObj.Set("hwnd", Napi::Number::New(env, reinterpret_cast<uint64_t>(windows[i].hwnd)));
        windowObj.Set("title", Napi::String::New(env, windows[i].title));
        windowObj.Set("className", Napi::String::New(env, windows[i].className));
        windowObj.Set("x", Napi::Number::New(env, windows[i].x));
        windowObj.Set("y", Napi::Number::New(env, windows[i].y));
        windowObj.Set("width", Napi::Number::New(env, windows[i].width));
        windowObj.Set("height", Napi::Number::New(env, windows[i].height));
        windowObj.Set("isVisible", Napi::Boolean::New(env, windows[i].isVisible));
        result[i] = windowObj;
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::GetActiveWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    auto window = AutomationHelpers::GetActiveWindow();
    
    if (window.hwnd == nullptr) {
        return env.Null();
    }
    
    Napi::Object windowObj = Napi::Object::New(env);
    windowObj.Set("hwnd", Napi::Number::New(env, reinterpret_cast<uint64_t>(window.hwnd)));
    windowObj.Set("title", Napi::String::New(env, window.title));
    windowObj.Set("className", Napi::String::New(env, window.className));
    windowObj.Set("x", Napi::Number::New(env, window.x));
    windowObj.Set("y", Napi::Number::New(env, window.y));
    windowObj.Set("width", Napi::Number::New(env, window.width));
    windowObj.Set("height", Napi::Number::New(env, window.height));
    windowObj.Set("isVisible", Napi::Boolean::New(env, window.isVisible));
    
    return windowObj;
}

Napi::Value MemoryHookingAddon::SetActiveWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected window handle").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uint64_t hwnd = info[0].As<Napi::Number>().Int64Value();
    bool result = AutomationHelpers::SetActiveWindow(reinterpret_cast<HWND>(hwnd));
    
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::ShowWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected window handle and show command").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uint64_t hwnd = info[0].As<Napi::Number>().Int64Value();
    int showCmd = info[1].As<Napi::Number>().Int32Value();
    
    bool result = AutomationHelpers::ShowWindow(reinterpret_cast<HWND>(hwnd), showCmd);
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::MoveWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 5) {
        Napi::TypeError::New(env, "Expected window handle, x, y, width, height").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uint64_t hwnd = info[0].As<Napi::Number>().Int64Value();
    int x = info[1].As<Napi::Number>().Int32Value();
    int y = info[2].As<Napi::Number>().Int32Value();
    int width = info[3].As<Napi::Number>().Int32Value();
    int height = info[4].As<Napi::Number>().Int32Value();
    
    bool result = AutomationHelpers::MoveWindow(reinterpret_cast<HWND>(hwnd), x, y, width, height);
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::CloseWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected window handle").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uint64_t hwnd = info[0].As<Napi::Number>().Int64Value();
    bool result = AutomationHelpers::CloseWindow(reinterpret_cast<HWND>(hwnd));
    
    return Napi::Boolean::New(env, result);
}

// Automation Helpers - Screen Information
Napi::Value MemoryHookingAddon::GetScreenWidth(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    int width = AutomationHelpers::GetScreenWidth();
    return Napi::Number::New(env, width);
}

Napi::Value MemoryHookingAddon::GetScreenHeight(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    int height = AutomationHelpers::GetScreenHeight();
    return Napi::Number::New(env, height);
}

Napi::Value MemoryHookingAddon::GetScreenDPI(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    int dpi = AutomationHelpers::GetScreenDPI();
    return Napi::Number::New(env, dpi);
}

Napi::Value MemoryHookingAddon::GetMonitorRects(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    auto monitors = AutomationHelpers::GetMonitorRects();
    
    Napi::Array result = Napi::Array::New(env, monitors.size());
    for (size_t i = 0; i < monitors.size(); ++i) {
        Napi::Object rectObj = Napi::Object::New(env);
        rectObj.Set("left", Napi::Number::New(env, monitors[i].left));
        rectObj.Set("top", Napi::Number::New(env, monitors[i].top));
        rectObj.Set("right", Napi::Number::New(env, monitors[i].right));
        rectObj.Set("bottom", Napi::Number::New(env, monitors[i].bottom));
        rectObj.Set("width", Napi::Number::New(env, monitors[i].right - monitors[i].left));
        rectObj.Set("height", Napi::Number::New(env, monitors[i].bottom - monitors[i].top));
        result[i] = rectObj;
    }
    
    return result;
}

// Automation Helpers - Color and Pixel Operations
Napi::Value MemoryHookingAddon::GetPixelColor(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected x and y coordinates").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    
    COLORREF color = AutomationHelpers::GetPixelColor(x, y);
    return Napi::Number::New(env, static_cast<uint32_t>(color));
}

Napi::Value MemoryHookingAddon::GetPixelColors(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Expected x, y, width, height").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    int width = info[2].As<Napi::Number>().Int32Value();
    int height = info[3].As<Napi::Number>().Int32Value();
    
    auto colors = AutomationHelpers::GetPixelColors(x, y, width, height);
    
    Napi::Array result = Napi::Array::New(env, colors.size());
    for (size_t i = 0; i < colors.size(); ++i) {
        result[i] = Napi::Number::New(env, static_cast<uint32_t>(colors[i]));
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::FindColorOnScreen(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected color value").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    COLORREF color = static_cast<COLORREF>(info[0].As<Napi::Number>().Uint32Value());
    int tolerance = info.Length() > 1 ? info[1].As<Napi::Number>().Int32Value() : 0;
    
    auto positions = AutomationHelpers::FindColorOnScreen(color, tolerance);
    
    Napi::Array result = Napi::Array::New(env, positions.size());
    for (size_t i = 0; i < positions.size(); ++i) {
        Napi::Object posObj = Napi::Object::New(env);
        posObj.Set("x", Napi::Number::New(env, positions[i].first));
        posObj.Set("y", Napi::Number::New(env, positions[i].second));
        result[i] = posObj;
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::FindColorInRegion(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 5) {
        Napi::TypeError::New(env, "Expected color, x, y, width, height").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    COLORREF color = static_cast<COLORREF>(info[0].As<Napi::Number>().Uint32Value());
    int regionX = info[1].As<Napi::Number>().Int32Value();
    int regionY = info[2].As<Napi::Number>().Int32Value();
    int regionWidth = info[3].As<Napi::Number>().Int32Value();
    int regionHeight = info[4].As<Napi::Number>().Int32Value();
    int tolerance = info.Length() > 5 ? info[5].As<Napi::Number>().Int32Value() : 0;
    
    auto positions = AutomationHelpers::FindColorInRegion(color, regionX, regionY, regionWidth, regionHeight, tolerance);
    
    Napi::Array result = Napi::Array::New(env, positions.size());
    for (size_t i = 0; i < positions.size(); ++i) {
        Napi::Object posObj = Napi::Object::New(env);
        posObj.Set("x", Napi::Number::New(env, positions[i].first));
        posObj.Set("y", Napi::Number::New(env, positions[i].second));
        result[i] = posObj;
    }
    
    return result;
}

// Automation Helpers - Wait and Timing
Napi::Value MemoryHookingAddon::Sleep(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected milliseconds").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int milliseconds = info[0].As<Napi::Number>().Int32Value();
    AutomationHelpers::Sleep(milliseconds);
    
    return env.Undefined();
}

Napi::Value MemoryHookingAddon::WaitForWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected window title").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    std::string title = info[0].As<Napi::String>().Utf8Value();
    int timeoutMs = info.Length() > 1 ? info[1].As<Napi::Number>().Int32Value() : 5000;
    
    bool result = AutomationHelpers::WaitForWindow(title, timeoutMs);
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::WaitForPixelColor(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Expected x, y, color").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int x = info[0].As<Napi::Number>().Int32Value();
    int y = info[1].As<Napi::Number>().Int32Value();
    COLORREF color = static_cast<COLORREF>(info[2].As<Napi::Number>().Uint32Value());
    int timeoutMs = info.Length() > 3 ? info[3].As<Napi::Number>().Int32Value() : 5000;
    int tolerance = info.Length() > 4 ? info[4].As<Napi::Number>().Int32Value() : 0;
    
    bool result = AutomationHelpers::WaitForPixelColor(x, y, color, timeoutMs, tolerance);
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::WaitForKeyPress(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Expected key code").ThrowAsJavaScriptException();
        return env.Null();
    }
    
    int keyCode = info[0].As<Napi::Number>().Int32Value();
    int timeoutMs = info.Length() > 1 ? info[1].As<Napi::Number>().Int32Value() : 5000;
    
    KeyCode key = static_cast<KeyCode>(keyCode);
    bool result = AutomationHelpers::WaitForKeyPress(key, timeoutMs);
    return Napi::Boolean::New(env, result);
}

// Automation Helpers - System Information
Napi::Value MemoryHookingAddon::GetSystemInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    std::string sysInfo = AutomationHelpers::GetSystemInfo();
    return Napi::String::New(env, sysInfo);
}

Napi::Value MemoryHookingAddon::GetRunningWindowTitles(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    auto titles = AutomationHelpers::GetRunningWindowTitles();
    
    Napi::Array result = Napi::Array::New(env, titles.size());
    for (size_t i = 0; i < titles.size(); ++i) {
        result[i] = Napi::String::New(env, titles[i]);
    }
    
    return result;
}

Napi::Value MemoryHookingAddon::IsScreenLocked(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    bool result = AutomationHelpers::IsScreenLocked();
    return Napi::Boolean::New(env, result);
}

Napi::Value MemoryHookingAddon::IsUserIdle(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    int thresholdMs = info.Length() > 0 ? info[0].As<Napi::Number>().Int32Value() : 30000; // 30 seconds default
    
    bool result = AutomationHelpers::IsUserIdle(thresholdMs);
    return Napi::Boolean::New(env, result);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    return MemoryHookingAddon::Init(env, exports);
}

NODE_API_MODULE(memory_hooking, InitAll) 