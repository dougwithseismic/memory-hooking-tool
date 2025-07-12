#ifndef AUTOMATION_HELPERS_H
#define AUTOMATION_HELPERS_H

#include <windows.h>
#include <vector>
#include <string>
#include <cstdint>

struct MousePosition {
    int x;
    int y;
};

struct ScreenshotData {
    std::vector<uint8_t> data;
    int width;
    int height;
    int bitsPerPixel;
    std::string format; // "BMP", "PNG", etc.
};

struct WindowInfo {
    HWND hwnd;
    std::string title;
    std::string className;
    int x, y, width, height;
    bool isVisible;
};

enum class MouseButton {
    Left = 1,
    Right = 2,
    Middle = 4
};

enum class KeyCode {
    // Common keys
    Backspace = VK_BACK,
    Tab = VK_TAB,
    Enter = VK_RETURN,
    Escape = VK_ESCAPE,
    Space = VK_SPACE,
    Delete = VK_DELETE,
    
    // Arrow keys
    Left = VK_LEFT,
    Up = VK_UP,
    Right = VK_RIGHT,
    Down = VK_DOWN,
    
    // Function keys
    F1 = VK_F1,
    F2 = VK_F2,
    F3 = VK_F3,
    F4 = VK_F4,
    F5 = VK_F5,
    F6 = VK_F6,
    F7 = VK_F7,
    F8 = VK_F8,
    F9 = VK_F9,
    F10 = VK_F10,
    F11 = VK_F11,
    F12 = VK_F12,
    
    // Modifier keys
    Shift = VK_SHIFT,
    Control = VK_CONTROL,
    Alt = VK_MENU,
    Win = VK_LWIN,
    
    // Number keys
    Key0 = 0x30,
    Key1 = 0x31,
    Key2 = 0x32,
    Key3 = 0x33,
    Key4 = 0x34,
    Key5 = 0x35,
    Key6 = 0x36,
    Key7 = 0x37,
    Key8 = 0x38,
    Key9 = 0x39,
    
    // Letter keys
    A = 0x41,
    B = 0x42,
    C = 0x43,
    D = 0x44,
    E = 0x45,
    F = 0x46,
    G = 0x47,
    H = 0x48,
    I = 0x49,
    J = 0x4A,
    K = 0x4B,
    L = 0x4C,
    M = 0x4D,
    N = 0x4E,
    O = 0x4F,
    P = 0x50,
    Q = 0x51,
    R = 0x52,
    S = 0x53,
    T = 0x54,
    U = 0x55,
    V = 0x56,
    W = 0x57,
    X = 0x58,
    Y = 0x59,
    Z = 0x5A
};

class AutomationHelpers {
public:
    // Mouse Control
    static bool MoveMouse(int x, int y);
    static bool ClickMouse(MouseButton button, int x = -1, int y = -1);
    static bool DoubleClickMouse(MouseButton button, int x = -1, int y = -1);
    static bool DragMouse(int fromX, int fromY, int toX, int toY, MouseButton button = MouseButton::Left);
    static bool ScrollMouse(int x, int y, int scrollAmount);
    static MousePosition GetMousePosition();
    static bool IsMouseButtonPressed(MouseButton button);
    
    // Keyboard Control
    static bool PressKey(KeyCode key, int duration = 0);
    static bool ReleaseKey(KeyCode key);
    static bool SendKeyCombo(const std::vector<KeyCode>& keys);
    static bool SendText(const std::string& text);
    static bool SendTextToWindow(HWND hwnd, const std::string& text);
    static bool IsKeyPressed(KeyCode key);
    static std::vector<KeyCode> GetPressedKeys();
    
    // Screenshot and Screen Capture
    static ScreenshotData CaptureScreen();
    static ScreenshotData CaptureWindow(HWND hwnd);
    static ScreenshotData CaptureRegion(int x, int y, int width, int height);
    static ScreenshotData CaptureActiveWindow();
    static bool SaveScreenshotToFile(const ScreenshotData& screenshot, const std::string& filename);
    
    // Window Management
    static std::vector<WindowInfo> GetAllWindows();
    static std::vector<WindowInfo> GetWindowsByTitle(const std::string& titlePattern);
    static std::vector<WindowInfo> GetWindowsByClassName(const std::string& className);
    static WindowInfo GetActiveWindow();
    static bool SetActiveWindow(HWND hwnd);
    static bool ShowWindow(HWND hwnd, int showCommand = SW_SHOW);
    static bool MoveWindow(HWND hwnd, int x, int y, int width, int height);
    static bool CloseWindow(HWND hwnd);
    
    // Screen Information
    static int GetScreenWidth();
    static int GetScreenHeight();
    static int GetScreenDPI();
    static std::vector<RECT> GetMonitorRects();
    
    // Color and Pixel Operations
    static COLORREF GetPixelColor(int x, int y);
    static std::vector<COLORREF> GetPixelColors(int x, int y, int width, int height);
    static std::vector<std::pair<int, int>> FindColorOnScreen(COLORREF color, int tolerance = 0);
    static std::vector<std::pair<int, int>> FindColorInRegion(COLORREF color, int x, int y, int width, int height, int tolerance = 0);
    
    // Wait and Timing
    static void Sleep(int milliseconds);
    static bool WaitForWindow(const std::string& titlePattern, int timeoutMs = 5000);
    static bool WaitForPixelColor(int x, int y, COLORREF color, int timeoutMs = 5000, int tolerance = 0);
    static bool WaitForKeyPress(KeyCode key, int timeoutMs = 5000);
    
    // System Information
    static std::string GetSystemInfo();
    static std::vector<std::string> GetRunningWindowTitles();
    static bool IsScreenLocked();
    static bool IsUserIdle(int idleTimeMs = 5000);

private:
    static const int MAX_WINDOWS = 1000;
    static const int SCREENSHOT_QUALITY = 90;
    static const int DEFAULT_KEY_DELAY = 10;
    static const int DEFAULT_CLICK_DELAY = 100;
    
    // Helper functions
    static HBITMAP CaptureScreenToBitmap(int x, int y, int width, int height);
    static std::vector<uint8_t> BitmapToBytes(HBITMAP hBitmap, int& width, int& height);
    static std::vector<uint8_t> ConvertBitmapToPNG(HBITMAP hBitmap, int width, int height);
    static bool EnumWindowsCallback(HWND hwnd, LPARAM lParam);
    static void SendInputEvent(const INPUT& input);
    static INPUT CreateMouseInput(DWORD flags, int x = 0, int y = 0, int data = 0);
    static INPUT CreateKeyboardInput(WORD vk, DWORD flags);
    static bool ColorMatches(COLORREF color1, COLORREF color2, int tolerance);
};

#endif // AUTOMATION_HELPERS_H 