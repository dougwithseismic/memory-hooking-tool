#include "include/automation_helpers.h"
#include "include/error_handler.h"
#include <iostream>
#include <regex>
#include <chrono>
#include <thread>
#include <algorithm>
#include <wingdi.h>
#include <winuser.h>

// Global variables for EnumWindows callback
static std::vector<WindowInfo>* g_windowList = nullptr;
static std::string g_searchPattern = "";

// Mouse Control Implementation
bool AutomationHelpers::MoveMouse(int x, int y) {
    try {
        return SetCursorPos(x, y) != 0;
    } catch (const std::exception& e) {
        ErrorHandler::logError("MoveMouse failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::ClickMouse(MouseButton button, int x, int y) {
    try {
        if (x >= 0 && y >= 0) {
            if (!MoveMouse(x, y)) return false;
            Sleep(DEFAULT_CLICK_DELAY);
        }
        
        DWORD downFlag = 0, upFlag = 0;
        switch (button) {
            case MouseButton::Left:
                downFlag = MOUSEEVENTF_LEFTDOWN;
                upFlag = MOUSEEVENTF_LEFTUP;
                break;
            case MouseButton::Right:
                downFlag = MOUSEEVENTF_RIGHTDOWN;
                upFlag = MOUSEEVENTF_RIGHTUP;
                break;
            case MouseButton::Middle:
                downFlag = MOUSEEVENTF_MIDDLEDOWN;
                upFlag = MOUSEEVENTF_MIDDLEUP;
                break;
        }
        
        INPUT inputs[2] = {};
        inputs[0] = CreateMouseInput(downFlag);
        inputs[1] = CreateMouseInput(upFlag);
        
        UINT sent = SendInput(2, inputs, sizeof(INPUT));
        return sent == 2;
    } catch (const std::exception& e) {
        ErrorHandler::logError("ClickMouse failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::DoubleClickMouse(MouseButton button, int x, int y) {
    try {
        return ClickMouse(button, x, y) && ClickMouse(button, -1, -1);
    } catch (const std::exception& e) {
        ErrorHandler::logError("DoubleClickMouse failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::DragMouse(int fromX, int fromY, int toX, int toY, MouseButton button) {
    try {
        if (!MoveMouse(fromX, fromY)) return false;
        Sleep(DEFAULT_CLICK_DELAY);
        
        DWORD downFlag = 0, upFlag = 0;
        switch (button) {
            case MouseButton::Left:
                downFlag = MOUSEEVENTF_LEFTDOWN;
                upFlag = MOUSEEVENTF_LEFTUP;
                break;
            case MouseButton::Right:
                downFlag = MOUSEEVENTF_RIGHTDOWN;
                upFlag = MOUSEEVENTF_RIGHTUP;
                break;
            case MouseButton::Middle:
                downFlag = MOUSEEVENTF_MIDDLEDOWN;
                upFlag = MOUSEEVENTF_MIDDLEUP;
                break;
        }
        
        // Press button down
        INPUT downInput = CreateMouseInput(downFlag);
        SendInput(1, &downInput, sizeof(INPUT));
        Sleep(50);
        
        // Drag to destination
        if (!MoveMouse(toX, toY)) return false;
        Sleep(DEFAULT_CLICK_DELAY);
        
        // Release button
        INPUT upInput = CreateMouseInput(upFlag);
        SendInput(1, &upInput, sizeof(INPUT));
        
        return true;
    } catch (const std::exception& e) {
        ErrorHandler::logError("DragMouse failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::ScrollMouse(int x, int y, int scrollAmount) {
    try {
        if (x >= 0 && y >= 0) {
            if (!MoveMouse(x, y)) return false;
            Sleep(DEFAULT_CLICK_DELAY);
        }
        
        INPUT input = CreateMouseInput(MOUSEEVENTF_WHEEL, 0, 0, scrollAmount * WHEEL_DELTA);
        return SendInput(1, &input, sizeof(INPUT)) == 1;
    } catch (const std::exception& e) {
        ErrorHandler::logError("ScrollMouse failed: " + std::string(e.what()));
        return false;
    }
}

MousePosition AutomationHelpers::GetMousePosition() {
    POINT point;
    if (GetCursorPos(&point)) {
        return {point.x, point.y};
    }
    return {-1, -1};
}

bool AutomationHelpers::IsMouseButtonPressed(MouseButton button) {
    try {
        int vk = 0;
        switch (button) {
            case MouseButton::Left: vk = VK_LBUTTON; break;
            case MouseButton::Right: vk = VK_RBUTTON; break;
            case MouseButton::Middle: vk = VK_MBUTTON; break;
        }
        return (GetAsyncKeyState(vk) & 0x8000) != 0;
    } catch (const std::exception& e) {
        ErrorHandler::logError("IsMouseButtonPressed failed: " + std::string(e.what()));
        return false;
    }
}

// Keyboard Control Implementation
bool AutomationHelpers::PressKey(KeyCode key, int duration) {
    try {
        INPUT inputs[2] = {};
        inputs[0] = CreateKeyboardInput(static_cast<WORD>(key), 0);
        inputs[1] = CreateKeyboardInput(static_cast<WORD>(key), KEYEVENTF_KEYUP);
        
        SendInput(1, &inputs[0], sizeof(INPUT));
        if (duration > 0) {
            Sleep(duration);
        } else {
            Sleep(DEFAULT_KEY_DELAY);
        }
        SendInput(1, &inputs[1], sizeof(INPUT));
        
        return true;
    } catch (const std::exception& e) {
        ErrorHandler::logError("PressKey failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::ReleaseKey(KeyCode key) {
    try {
        INPUT input = CreateKeyboardInput(static_cast<WORD>(key), KEYEVENTF_KEYUP);
        return SendInput(1, &input, sizeof(INPUT)) == 1;
    } catch (const std::exception& e) {
        ErrorHandler::logError("ReleaseKey failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::SendKeyCombo(const std::vector<KeyCode>& keys) {
    try {
        std::vector<INPUT> inputs;
        
        // Press all keys down
        for (auto key : keys) {
            inputs.push_back(CreateKeyboardInput(static_cast<WORD>(key), 0));
        }
        
        // Release all keys up (in reverse order)
        for (auto it = keys.rbegin(); it != keys.rend(); ++it) {
            inputs.push_back(CreateKeyboardInput(static_cast<WORD>(*it), KEYEVENTF_KEYUP));
        }
        
        UINT sent = SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));
        return sent == inputs.size();
    } catch (const std::exception& e) {
        ErrorHandler::logError("SendKeyCombo failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::SendText(const std::string& text) {
    try {
        std::vector<INPUT> inputs;
        
        for (char c : text) {
            // Handle special characters
            if (c == '\n') {
                inputs.push_back(CreateKeyboardInput(VK_RETURN, 0));
                inputs.push_back(CreateKeyboardInput(VK_RETURN, KEYEVENTF_KEYUP));
            } else if (c == '\t') {
                inputs.push_back(CreateKeyboardInput(VK_TAB, 0));
                inputs.push_back(CreateKeyboardInput(VK_TAB, KEYEVENTF_KEYUP));
            } else {
                // Use Unicode input for regular characters
                INPUT unicodeInput = {};
                unicodeInput.type = INPUT_KEYBOARD;
                unicodeInput.ki.wVk = 0;
                unicodeInput.ki.wScan = c;
                unicodeInput.ki.dwFlags = KEYEVENTF_UNICODE;
                inputs.push_back(unicodeInput);
                
                unicodeInput.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
                inputs.push_back(unicodeInput);
            }
        }
        
        UINT sent = SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT));
        return sent == inputs.size();
    } catch (const std::exception& e) {
        ErrorHandler::logError("SendText failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::SendTextToWindow(HWND hwnd, const std::string& text) {
    try {
        if (!SetActiveWindow(hwnd)) return false;
        Sleep(100); // Wait for window to become active
        return SendText(text);
    } catch (const std::exception& e) {
        ErrorHandler::logError("SendTextToWindow failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::IsKeyPressed(KeyCode key) {
    try {
        return (GetAsyncKeyState(static_cast<int>(key)) & 0x8000) != 0;
    } catch (const std::exception& e) {
        ErrorHandler::logError("IsKeyPressed failed: " + std::string(e.what()));
        return false;
    }
}

std::vector<KeyCode> AutomationHelpers::GetPressedKeys() {
    std::vector<KeyCode> pressedKeys;
    try {
        for (int vk = 1; vk < 256; ++vk) {
            if (GetAsyncKeyState(vk) & 0x8000) {
                pressedKeys.push_back(static_cast<KeyCode>(vk));
            }
        }
    } catch (const std::exception& e) {
        ErrorHandler::logError("GetPressedKeys failed: " + std::string(e.what()));
    }
    return pressedKeys;
}

// Screenshot and Screen Capture Implementation
ScreenshotData AutomationHelpers::CaptureScreen() {
    try {
        int width = GetSystemMetrics(SM_CXSCREEN);
        int height = GetSystemMetrics(SM_CYSCREEN);
        return CaptureRegion(0, 0, width, height);
    } catch (const std::exception& e) {
        ErrorHandler::logError("CaptureScreen failed: " + std::string(e.what()));
        return {};
    }
}

ScreenshotData AutomationHelpers::CaptureWindow(HWND hwnd) {
    try {
        RECT rect;
        if (!GetWindowRect(hwnd, &rect)) {
            return {};
        }
        
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        return CaptureRegion(rect.left, rect.top, width, height);
    } catch (const std::exception& e) {
        ErrorHandler::logError("CaptureWindow failed: " + std::string(e.what()));
        return {};
    }
}

ScreenshotData AutomationHelpers::CaptureRegion(int x, int y, int width, int height) {
    ScreenshotData screenshot;
    
    try {
        HBITMAP hBitmap = CaptureScreenToBitmap(x, y, width, height);
        if (hBitmap) {
            screenshot.data = BitmapToBytes(hBitmap, screenshot.width, screenshot.height);
            screenshot.bitsPerPixel = 32;
            screenshot.format = "BMP";
            DeleteObject(hBitmap);
        }
    } catch (const std::exception& e) {
        ErrorHandler::logError("CaptureRegion failed: " + std::string(e.what()));
    }
    
    return screenshot;
}

ScreenshotData AutomationHelpers::CaptureActiveWindow() {
    try {
        HWND hwnd = GetForegroundWindow();
        if (hwnd) {
            return CaptureWindow(hwnd);
        }
    } catch (const std::exception& e) {
        ErrorHandler::logError("CaptureActiveWindow failed: " + std::string(e.what()));
    }
    return {};
}

bool AutomationHelpers::SaveScreenshotToFile(const ScreenshotData& screenshot, const std::string& filename) {
    try {
        if (screenshot.data.empty()) return false;
        
        // Calculate BMP file parameters
        int width = screenshot.width;
        int height = screenshot.height;
        int bytesPerPixel = screenshot.bitsPerPixel / 8;
        int rowSize = ((width * bytesPerPixel + 3) / 4) * 4; // Row size must be multiple of 4
        int imageSize = rowSize * height;
        int fileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + imageSize;
        
        // Create BMP file header
        BITMAPFILEHEADER fileHeader = {};
        fileHeader.bfType = 0x4D42; // "BM"
        fileHeader.bfSize = fileSize;
        fileHeader.bfReserved1 = 0;
        fileHeader.bfReserved2 = 0;
        fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        
        // Create BMP info header
        BITMAPINFOHEADER infoHeader = {};
        infoHeader.biSize = sizeof(BITMAPINFOHEADER);
        infoHeader.biWidth = width;
        infoHeader.biHeight = -height; // Negative for top-down bitmap
        infoHeader.biPlanes = 1;
        infoHeader.biBitCount = screenshot.bitsPerPixel;
        infoHeader.biCompression = BI_RGB;
        infoHeader.biSizeImage = imageSize;
        infoHeader.biXPelsPerMeter = 2835; // 72 DPI
        infoHeader.biYPelsPerMeter = 2835; // 72 DPI
        infoHeader.biClrUsed = 0;
        infoHeader.biClrImportant = 0;
        
        // Open file for writing
        HANDLE hFile = CreateFileA(filename.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hFile == INVALID_HANDLE_VALUE) return false;
        
        DWORD written;
        bool success = true;
        
        // Write file header
        success &= WriteFile(hFile, &fileHeader, sizeof(fileHeader), &written, nullptr);
        
        // Write info header
        success &= WriteFile(hFile, &infoHeader, sizeof(infoHeader), &written, nullptr);
        
        // Write pixel data
        if (rowSize == width * bytesPerPixel) {
            // No padding needed, write all data at once
            success &= WriteFile(hFile, screenshot.data.data(), static_cast<DWORD>(screenshot.data.size()), &written, nullptr);
        } else {
            // Row padding needed, write row by row
            std::vector<uint8_t> rowBuffer(rowSize, 0);
            for (int y = 0; y < height; ++y) {
                int srcOffset = y * width * bytesPerPixel;
                int copySize = width * bytesPerPixel;
                
                // Copy row data
                memcpy(rowBuffer.data(), screenshot.data.data() + srcOffset, copySize);
                
                // Write padded row
                success &= WriteFile(hFile, rowBuffer.data(), rowSize, &written, nullptr);
            }
        }
        
        CloseHandle(hFile);
        return success;
        
    } catch (const std::exception& e) {
        ErrorHandler::logError("SaveScreenshotToFile failed: " + std::string(e.what()));
        return false;
    }
}

// Window Management Implementation
std::vector<WindowInfo> AutomationHelpers::GetAllWindows() {
    std::vector<WindowInfo> windows;
    try {
        g_windowList = &windows;
        EnumWindows(reinterpret_cast<WNDENUMPROC>(EnumWindowsCallback), 0);
        g_windowList = nullptr;
    } catch (const std::exception& e) {
        ErrorHandler::logError("GetAllWindows failed: " + std::string(e.what()));
    }
    return windows;
}

std::vector<WindowInfo> AutomationHelpers::GetWindowsByTitle(const std::string& titlePattern) {
    std::vector<WindowInfo> windows;
    try {
        g_windowList = &windows;
        g_searchPattern = titlePattern;
        EnumWindows(reinterpret_cast<WNDENUMPROC>(EnumWindowsCallback), 1);
        g_windowList = nullptr;
        g_searchPattern = "";
    } catch (const std::exception& e) {
        ErrorHandler::logError("GetWindowsByTitle failed: " + std::string(e.what()));
    }
    return windows;
}

std::vector<WindowInfo> AutomationHelpers::GetWindowsByClassName(const std::string& className) {
    std::vector<WindowInfo> windows;
    try {
        g_windowList = &windows;
        g_searchPattern = className;
        EnumWindows(reinterpret_cast<WNDENUMPROC>(EnumWindowsCallback), 2);
        g_windowList = nullptr;
        g_searchPattern = "";
    } catch (const std::exception& e) {
        ErrorHandler::logError("GetWindowsByClassName failed: " + std::string(e.what()));
    }
    return windows;
}

WindowInfo AutomationHelpers::GetActiveWindow() {
    try {
        HWND hwnd = GetForegroundWindow();
        if (hwnd) {
            WindowInfo info;
            info.hwnd = hwnd;
            
            char title[256];
            GetWindowTextA(hwnd, title, sizeof(title));
            info.title = title;
            
            char className[256];
            GetClassNameA(hwnd, className, sizeof(className));
            info.className = className;
            
            RECT rect;
            GetWindowRect(hwnd, &rect);
            info.x = rect.left;
            info.y = rect.top;
            info.width = rect.right - rect.left;
            info.height = rect.bottom - rect.top;
            info.isVisible = IsWindowVisible(hwnd);
            
            return info;
        }
    } catch (const std::exception& e) {
        ErrorHandler::logError("GetActiveWindow failed: " + std::string(e.what()));
    }
    return {};
}

bool AutomationHelpers::SetActiveWindow(HWND hwnd) {
    try {
        return SetForegroundWindow(hwnd) != 0;
    } catch (const std::exception& e) {
        ErrorHandler::logError("SetActiveWindow failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::ShowWindow(HWND hwnd, int showCommand) {
    try {
        return ::ShowWindow(hwnd, showCommand) != 0;
    } catch (const std::exception& e) {
        ErrorHandler::logError("ShowWindow failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::MoveWindow(HWND hwnd, int x, int y, int width, int height) {
    try {
        return ::MoveWindow(hwnd, x, y, width, height, TRUE) != 0;
    } catch (const std::exception& e) {
        ErrorHandler::logError("MoveWindow failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::CloseWindow(HWND hwnd) {
    try {
        return PostMessage(hwnd, WM_CLOSE, 0, 0) != 0;
    } catch (const std::exception& e) {
        ErrorHandler::logError("CloseWindow failed: " + std::string(e.what()));
        return false;
    }
}

// Screen Information Implementation
int AutomationHelpers::GetScreenWidth() {
    return GetSystemMetrics(SM_CXSCREEN);
}

int AutomationHelpers::GetScreenHeight() {
    return GetSystemMetrics(SM_CYSCREEN);
}

int AutomationHelpers::GetScreenDPI() {
    HDC hdc = GetDC(nullptr);
    int dpi = GetDeviceCaps(hdc, LOGPIXELSX);
    ReleaseDC(nullptr, hdc);
    return dpi;
}

std::vector<RECT> AutomationHelpers::GetMonitorRects() {
    std::vector<RECT> monitors;
    // This would require EnumDisplayMonitors callback - simplified for now
    RECT primary = {0, 0, GetScreenWidth(), GetScreenHeight()};
    monitors.push_back(primary);
    return monitors;
}

// Color and Pixel Operations Implementation
COLORREF AutomationHelpers::GetPixelColor(int x, int y) {
    try {
        HDC hdc = GetDC(nullptr);
        COLORREF color = GetPixel(hdc, x, y);
        ReleaseDC(nullptr, hdc);
        return color;
    } catch (const std::exception& e) {
        ErrorHandler::logError("GetPixelColor failed: " + std::string(e.what()));
        return RGB(0, 0, 0);
    }
}

std::vector<COLORREF> AutomationHelpers::GetPixelColors(int x, int y, int width, int height) {
    std::vector<COLORREF> colors;
    try {
        HDC hdc = GetDC(nullptr);
        for (int py = y; py < y + height; ++py) {
            for (int px = x; px < x + width; ++px) {
                colors.push_back(GetPixel(hdc, px, py));
            }
        }
        ReleaseDC(nullptr, hdc);
    } catch (const std::exception& e) {
        ErrorHandler::logError("GetPixelColors failed: " + std::string(e.what()));
    }
    return colors;
}

std::vector<std::pair<int, int>> AutomationHelpers::FindColorOnScreen(COLORREF color, int tolerance) {
    return FindColorInRegion(color, 0, 0, GetScreenWidth(), GetScreenHeight(), tolerance);
}

std::vector<std::pair<int, int>> AutomationHelpers::FindColorInRegion(COLORREF color, int x, int y, int width, int height, int tolerance) {
    std::vector<std::pair<int, int>> positions;
    try {
        HDC hdc = GetDC(nullptr);
        for (int py = y; py < y + height; py += 2) { // Skip pixels for performance
            for (int px = x; px < x + width; px += 2) {
                COLORREF pixelColor = GetPixel(hdc, px, py);
                if (ColorMatches(color, pixelColor, tolerance)) {
                    positions.push_back({px, py});
                }
            }
        }
        ReleaseDC(nullptr, hdc);
    } catch (const std::exception& e) {
        ErrorHandler::logError("FindColorInRegion failed: " + std::string(e.what()));
    }
    return positions;
}

// Wait and Timing Implementation
void AutomationHelpers::Sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

bool AutomationHelpers::WaitForWindow(const std::string& titlePattern, int timeoutMs) {
    try {
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - start).count() < timeoutMs) {
            auto windows = GetWindowsByTitle(titlePattern);
            if (!windows.empty()) {
                return true;
            }
            Sleep(100);
        }
    } catch (const std::exception& e) {
        ErrorHandler::logError("WaitForWindow failed: " + std::string(e.what()));
    }
    return false;
}

bool AutomationHelpers::WaitForPixelColor(int x, int y, COLORREF color, int timeoutMs, int tolerance) {
    try {
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - start).count() < timeoutMs) {
            COLORREF currentColor = GetPixelColor(x, y);
            if (ColorMatches(color, currentColor, tolerance)) {
                return true;
            }
            Sleep(50);
        }
    } catch (const std::exception& e) {
        ErrorHandler::logError("WaitForPixelColor failed: " + std::string(e.what()));
    }
    return false;
}

bool AutomationHelpers::WaitForKeyPress(KeyCode key, int timeoutMs) {
    try {
        auto start = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - start).count() < timeoutMs) {
            if (IsKeyPressed(key)) {
                return true;
            }
            Sleep(50);
        }
    } catch (const std::exception& e) {
        ErrorHandler::logError("WaitForKeyPress failed: " + std::string(e.what()));
    }
    return false;
}

// System Information Implementation
std::string AutomationHelpers::GetSystemInfo() {
    try {
        OSVERSIONINFOA osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFOA));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
        
        std::string info = "Screen: " + std::to_string(GetScreenWidth()) + "x" + std::to_string(GetScreenHeight());
        info += ", DPI: " + std::to_string(GetScreenDPI());
        return info;
    } catch (const std::exception& e) {
        ErrorHandler::logError("GetSystemInfo failed: " + std::string(e.what()));
        return "Unknown";
    }
}

std::vector<std::string> AutomationHelpers::GetRunningWindowTitles() {
    std::vector<std::string> titles;
    try {
        auto windows = GetAllWindows();
        for (const auto& window : windows) {
            if (!window.title.empty() && window.isVisible) {
                titles.push_back(window.title);
            }
        }
    } catch (const std::exception& e) {
        ErrorHandler::logError("GetRunningWindowTitles failed: " + std::string(e.what()));
    }
    return titles;
}

bool AutomationHelpers::IsScreenLocked() {
    try {
        HDESK hDesk = OpenInputDesktop(0, FALSE, DESKTOP_SWITCHDESKTOP);
        if (hDesk) {
            CloseDesktop(hDesk);
            return false;
        }
        return true;
    } catch (const std::exception& e) {
        ErrorHandler::logError("IsScreenLocked failed: " + std::string(e.what()));
        return false;
    }
}

bool AutomationHelpers::IsUserIdle(int idleTimeMs) {
    try {
        LASTINPUTINFO lii;
        lii.cbSize = sizeof(LASTINPUTINFO);
        if (GetLastInputInfo(&lii)) {
            DWORD currentTime = GetTickCount();
            DWORD idleTime = currentTime - lii.dwTime;
            return idleTime >= static_cast<DWORD>(idleTimeMs);
        }
    } catch (const std::exception& e) {
        ErrorHandler::logError("IsUserIdle failed: " + std::string(e.what()));
    }
    return false;
}

// Private Helper Functions Implementation
HBITMAP AutomationHelpers::CaptureScreenToBitmap(int x, int y, int width, int height) {
    try {
        HDC hScreen = GetDC(nullptr);
        HDC hDC = CreateCompatibleDC(hScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);
        
        SelectObject(hDC, hBitmap);
        BitBlt(hDC, 0, 0, width, height, hScreen, x, y, SRCCOPY);
        
        DeleteDC(hDC);
        ReleaseDC(nullptr, hScreen);
        
        return hBitmap;
    } catch (const std::exception& e) {
        ErrorHandler::logError("CaptureScreenToBitmap failed: " + std::string(e.what()));
        return nullptr;
    }
}

std::vector<uint8_t> AutomationHelpers::BitmapToBytes(HBITMAP hBitmap, int& width, int& height) {
    std::vector<uint8_t> bytes;
    
    try {
        BITMAP bmp;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);
        width = bmp.bmWidth;
        height = bmp.bmHeight;
        
        BITMAPINFOHEADER bi;
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = width;
        bi.biHeight = -height; // Top-down
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrUsed = 0;
        bi.biClrImportant = 0;
        
        int dataSize = width * height * 4;
        bytes.resize(dataSize);
        
        HDC hdc = GetDC(nullptr);
        GetDIBits(hdc, hBitmap, 0, height, bytes.data(), (BITMAPINFO*)&bi, DIB_RGB_COLORS);
        ReleaseDC(nullptr, hdc);
    } catch (const std::exception& e) {
        ErrorHandler::logError("BitmapToBytes failed: " + std::string(e.what()));
    }
    
    return bytes;
}

bool AutomationHelpers::EnumWindowsCallback(HWND hwnd, LPARAM lParam) {
    if (!g_windowList) return FALSE;
    
    try {
        char title[256];
        char className[256];
        GetWindowTextA(hwnd, title, sizeof(title));
        GetClassNameA(hwnd, className, sizeof(className));
        
        bool shouldAdd = false;
        
        switch (lParam) {
            case 0: // All windows
                shouldAdd = true;
                break;
            case 1: // Filter by title
                shouldAdd = std::regex_search(title, std::regex(g_searchPattern, std::regex_constants::icase));
                break;
            case 2: // Filter by class name
                shouldAdd = (g_searchPattern == className);
                break;
        }
        
        if (shouldAdd) {
            WindowInfo info;
            info.hwnd = hwnd;
            info.title = title;
            info.className = className;
            
            RECT rect;
            GetWindowRect(hwnd, &rect);
            info.x = rect.left;
            info.y = rect.top;
            info.width = rect.right - rect.left;
            info.height = rect.bottom - rect.top;
            info.isVisible = IsWindowVisible(hwnd);
            
            g_windowList->push_back(info);
        }
    } catch (const std::exception& e) {
        ErrorHandler::logError("EnumWindowsCallback failed: " + std::string(e.what()));
    }
    
    return TRUE;
}

INPUT AutomationHelpers::CreateMouseInput(DWORD flags, int x, int y, int data) {
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = flags;
    input.mi.dx = x;
    input.mi.dy = y;
    input.mi.mouseData = data;
    return input;
}

INPUT AutomationHelpers::CreateKeyboardInput(WORD vk, DWORD flags) {
    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    input.ki.dwFlags = flags;
    return input;
}

bool AutomationHelpers::ColorMatches(COLORREF color1, COLORREF color2, int tolerance) {
    if (tolerance == 0) return color1 == color2;
    
    int r1 = GetRValue(color1), g1 = GetGValue(color1), b1 = GetBValue(color1);
    int r2 = GetRValue(color2), g2 = GetGValue(color2), b2 = GetBValue(color2);
    
    return (abs(r1 - r2) <= tolerance) && 
           (abs(g1 - g2) <= tolerance) && 
           (abs(b1 - b2) <= tolerance);
} 