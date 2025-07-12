# Automation Features - Complete PC Control

The Memory Hooking Tool now includes comprehensive automation capabilities, allowing you to control the entire PC through mouse, keyboard, screenshots, and window management - all while maintaining full memory manipulation capabilities.

## Overview

This enhanced toolkit provides:

- **Memory Manipulation**: Read, write, scan, and analyze process memory
- **Mouse Control**: Move, click, drag, scroll with pixel precision
- **Keyboard Control**: Send keys, text, and complex key combinations
- **Screenshot Capture**: Full screen, window, and region screenshots as binary data
- **Window Management**: Find, control, and manipulate windows
- **Screen Analysis**: Color detection, pixel sampling, and visual automation
- **System Integration**: Wait functions, timing, and system state detection

## Quick Start

```typescript
import { createMemoryHooking, MouseButton, KeyCode } from '@experiments/demo';

const automation = createMemoryHooking();

// Take a screenshot and save it
const screenshot = automation.captureScreen();
automation.saveScreenshotToFile(screenshot, 'screenshot.bmp');

// Control mouse and keyboard
automation.moveMouse(100, 100);
automation.clickMouse(MouseButton.Left);
automation.sendText('Hello, automated world!');

// Memory manipulation
const processes = automation.getRunningProcesses();
console.log(`Found ${processes.length} running processes`);
```

## Mouse Control

### Basic Mouse Operations

```typescript
// Move mouse to specific coordinates
automation.moveMouse(500, 300);

// Get current mouse position
const pos = automation.getMousePosition();
console.log(`Mouse at: ${pos.x}, ${pos.y}`);

// Click at current position or specific coordinates
automation.clickMouse(MouseButton.Left);
automation.clickMouse(MouseButton.Right, 100, 200);

// Double-click
automation.doubleClickMouse(MouseButton.Left, 300, 400);

// Drag operations
automation.dragMouse(100, 100, 200, 200, MouseButton.Left);

// Scroll wheel
automation.scrollMouse(400, 300, 3); // Scroll up
automation.scrollMouse(400, 300, -3); // Scroll down
```

### Advanced Mouse Control

```typescript
// Check if mouse button is pressed
if (automation.isMouseButtonPressed(MouseButton.Left)) {
  console.log('Left mouse button is currently pressed');
}

// Draw patterns with the mouse
const centerX = automation.getScreenWidth() / 2;
const centerY = automation.getScreenHeight() / 2;

// Draw a circle
for (let angle = 0; angle < 360; angle += 10) {
  const x = centerX + Math.cos(angle * Math.PI / 180) * 100;
  const y = centerY + Math.sin(angle * Math.PI / 180) * 100;
  automation.moveMouse(x, y);
  automation.sleep(50);
}
```

## Keyboard Control

### Basic Keyboard Operations

```typescript
// Press individual keys
automation.pressKey(KeyCode.Enter);
automation.pressKey(KeyCode.A, 500); // Hold for 500ms

// Release keys (for manual control)
automation.releaseKey(KeyCode.Shift);

// Send text (automatically handles special characters)
automation.sendText('Hello World!\nThis is a new line.');

// Key combinations
automation.sendKeyCombo([KeyCode.Control, KeyCode.C]); // Ctrl+C
automation.sendKeyCombo([KeyCode.Alt, KeyCode.Tab]); // Alt+Tab
automation.sendKeyCombo([KeyCode.Win, KeyCode.R]); // Win+R
```

### Advanced Keyboard Operations

```typescript
// Check if key is pressed
if (automation.isKeyPressed(KeyCode.Shift)) {
  console.log('Shift key is currently pressed');
}

// Get all currently pressed keys
const pressedKeys = automation.getPressedKeys();
console.log('Currently pressed keys:', pressedKeys);

// Send text to specific window
const windows = automation.getWindowsByTitle('Notepad');
if (windows.length > 0) {
  automation.sendTextToWindow(windows[0].hwnd, 'Text for specific window');
}

// Complex automation sequence
automation.sendKeyCombo([KeyCode.Win, KeyCode.R]); // Open Run dialog
automation.sleep(500);
automation.sendText('notepad');
automation.pressKey(KeyCode.Enter);
automation.sleep(2000);
automation.sendText('Automated notepad content!');
```

## Screenshot and Image Capture

### Screenshot Types

```typescript
// Full screen screenshot
const fullScreen = automation.captureScreen();
console.log(`Screenshot: ${fullScreen.width}x${fullScreen.height}, ${fullScreen.data.length} bytes`);

// Active window screenshot
const activeWindow = automation.captureActiveWindow();

// Specific window screenshot
const windows = automation.getWindowsByTitle('Calculator');
if (windows.length > 0) {
  const calcScreenshot = automation.captureWindow(windows[0].hwnd);
  automation.saveScreenshotToFile(calcScreenshot, 'calculator.bmp');
}

// Region screenshot
const region = automation.captureRegion(100, 100, 400, 300);
```

### Working with Screenshot Data

```typescript
const screenshot = automation.captureScreen();

// Screenshot data structure
console.log('Width:', screenshot.width);
console.log('Height:', screenshot.height);
console.log('Bits per pixel:', screenshot.bitsPerPixel);
console.log('Format:', screenshot.format);
console.log('Data size:', screenshot.data.length, 'bytes');

// Save to file
automation.saveScreenshotToFile(screenshot, 'my-screenshot.bmp');

// Process the raw image data
const pixelData = screenshot.data; // Raw pixel data as number array
// You can process this data or convert to other formats
```

## Window Management

### Finding and Managing Windows

```typescript
// Get all windows
const allWindows = automation.getAllWindows();
console.log(`Found ${allWindows.length} windows`);

// Find windows by title (supports regex patterns)
const calcWindows = automation.getWindowsByTitle('Calculator');
const chromeWindows = automation.getWindowsByTitle('.*Chrome.*');

// Find by class name
const notepadWindows = automation.getWindowsByClassName('Notepad');

// Get active window
const activeWindow = automation.getActiveWindow();
console.log(`Active window: "${activeWindow.title}"`);
```

### Window Operations

```typescript
// Window information
const window = automation.getActiveWindow();
console.log({
  title: window.title,
  className: window.className,
  position: { x: window.x, y: window.y },
  size: { width: window.width, height: window.height },
  isVisible: window.isVisible,
  hwnd: window.hwnd
});

// Window control
automation.setActiveWindow(window.hwnd); // Bring to front
automation.showWindow(window.hwnd); // Show window
automation.moveWindow(window.hwnd, 100, 100, 800, 600); // Move and resize
automation.closeWindow(window.hwnd); // Close window
```

## Screen Analysis and Color Detection

### Pixel Color Operations

```typescript
// Get pixel color at specific coordinates
const color = automation.getPixelColor(400, 300);
console.log(`Color at (400, 300): 0x${color.toString(16)}`);

// Extract RGB components
const r = (color >> 16) & 0xFF;
const g = (color >> 8) & 0xFF;
const b = color & 0xFF;
console.log(`RGB: (${r}, ${g}, ${b})`);

// Get colors in a region
const colors = automation.getPixelColors(100, 100, 50, 50); // 50x50 region
console.log(`Sampled ${colors.length} pixels`);
```

### Color Search and Detection

```typescript
// Find specific color on screen
const redColor = 0xFF0000; // Pure red
const redPixels = automation.findColorOnScreen(redColor, 10); // 10 pixel tolerance
console.log(`Found red pixels at ${redPixels.length} locations`);

// Find color in specific region
const bluePixels = automation.findColorInRegion(
  0x0000FF, // Blue color
  100, 100, 400, 300, // Region
  5 // Tolerance
);

// Use for automation based on visual cues
if (redPixels.length > 0) {
  automation.clickMouse(MouseButton.Left, redPixels[0].x, redPixels[0].y);
}
```

## System Information and Monitoring

### Screen and System Info

```typescript
// Screen information
const width = automation.getScreenWidth();
const height = automation.getScreenHeight();
const dpi = automation.getScreenDPI();
console.log(`Screen: ${width}x${height} @ ${dpi} DPI`);

// Monitor information
const monitors = automation.getMonitorRects();
monitors.forEach((monitor, i) => {
  console.log(`Monitor ${i + 1}: ${monitor.left},${monitor.top} to ${monitor.right},${monitor.bottom}`);
});

// System status
const systemInfo = automation.getSystemInfo();
console.log('System info:', systemInfo);

const isLocked = automation.isScreenLocked();
const isIdle = automation.isUserIdle(5000); // Idle for 5 seconds
console.log(`Screen locked: ${isLocked}, User idle: ${isIdle}`);
```

### Window Monitoring

```typescript
// Get all visible window titles
const windowTitles = automation.getRunningWindowTitles();
console.log('Visible windows:', windowTitles);

// Monitor for specific window
setInterval(() => {
  const activeWindow = automation.getActiveWindow();
  console.log(`Current active window: ${activeWindow.title}`);
}, 1000);
```

## Wait Functions and Timing

### Synchronization and Timing

```typescript
// Basic sleep
automation.sleep(1000); // Wait 1 second

// Wait for window to appear
const windowAppeared = automation.waitForWindow('Calculator', 5000);
if (windowAppeared) {
  console.log('Calculator window appeared');
}

// Wait for specific pixel color
const colorFound = automation.waitForPixelColor(
  400, 300, // Position
  0xFF0000, // Red color
  5000, // 5 second timeout
  10 // Color tolerance
);

// Wait for key press
console.log('Press Space within 10 seconds...');
const keyPressed = automation.waitForKeyPress(KeyCode.Space, 10000);
if (keyPressed) {
  console.log('Space was pressed!');
}
```

## Combining Memory Manipulation with Automation

### Advanced Use Cases

```typescript
// Example: Automated game bot
async function gameBot() {
  // Find game window
  const gameWindows = automation.getWindowsByTitle('MyGame');
  if (gameWindows.length === 0) return;
  
  const gameWindow = gameWindows[0];
  automation.setActiveWindow(gameWindow.hwnd);
  
  // Attach to game process for memory reading
  const gameProcess = await automation.findProcessByName('game.exe');
  if (gameProcess && automation.attachToProcess(gameProcess.processId)) {
    
    // Read game state from memory
    const healthAddress = 0x12345678;
    const health = automation.readInt32(healthAddress);
    
    // Take action based on memory state
    if (health < 50) {
      // Use health potion (press H key)
      automation.pressKey(KeyCode.H);
    }
    
    // Visual detection for enemies
    const enemyColor = 0xFF0000; // Red enemies
    const enemies = automation.findColorOnScreen(enemyColor, 20);
    
    if (enemies.length > 0) {
      // Click on first enemy
      automation.clickMouse(MouseButton.Left, enemies[0].x, enemies[0].y);
      automation.pressKey(KeyCode.Space); // Attack
    }
    
    automation.detachFromProcess();
  }
}

// Example: Automated testing
async function automatedTesting() {
  // Open application
  automation.sendKeyCombo([KeyCode.Win, KeyCode.R]);
  automation.sleep(500);
  automation.sendText('myapp.exe');
  automation.pressKey(KeyCode.Enter);
  
  // Wait for app to load
  automation.waitForWindow('My Application', 10000);
  
  // Take baseline screenshot
  const baseline = automation.captureActiveWindow();
  automation.saveScreenshotToFile(baseline, 'baseline.bmp');
  
  // Perform test actions
  automation.clickMouse(MouseButton.Left, 100, 200);
  automation.sendText('Test input');
  automation.pressKey(KeyCode.Enter);
  
  // Verify result with another screenshot
  automation.sleep(1000);
  const result = automation.captureActiveWindow();
  automation.saveScreenshotToFile(result, 'test-result.bmp');
  
  // Check for specific colors indicating success/failure
  const successColor = 0x00FF00; // Green
  const errorColor = 0xFF0000; // Red
  
  const successPixels = automation.findColorOnScreen(successColor);
  const errorPixels = automation.findColorOnScreen(errorColor);
  
  if (successPixels.length > 0) {
    console.log('Test PASSED');
  } else if (errorPixels.length > 0) {
    console.log('Test FAILED');
  }
}
```

## Error Handling and Best Practices

### Safe Automation

```typescript
// Always check window exists before operations
const windows = automation.getWindowsByTitle('Target App');
if (windows.length > 0) {
  automation.setActiveWindow(windows[0].hwnd);
  // Perform operations
} else {
  console.log('Target window not found');
}

// Use timeouts for wait operations
const windowFound = automation.waitForWindow('Loading...', 30000);
if (!windowFound) {
  console.log('Timeout waiting for window');
  return;
}

// Store original mouse position for restoration
const originalPos = automation.getMousePosition();
try {
  // Perform automation
  automation.moveMouse(100, 100);
  automation.clickMouse(MouseButton.Left);
} finally {
  // Restore position
  automation.moveMouse(originalPos.x, originalPos.y);
}
```

### Performance Considerations

```typescript
// Use appropriate delays between actions
automation.clickMouse(MouseButton.Left, 100, 100);
automation.sleep(100); // Short delay for UI to respond
automation.sendText('Hello');

// Batch operations when possible
const keyCombo = [KeyCode.Control, KeyCode.Shift, KeyCode.N];
automation.sendKeyCombo(keyCombo); // Better than individual key presses

// Limit screenshot frequency for performance
let lastScreenshot = 0;
function captureIfNeeded() {
  const now = Date.now();
  if (now - lastScreenshot > 1000) { // Max once per second
    const screenshot = automation.captureScreen();
    lastScreenshot = now;
    return screenshot;
  }
  return null;
}
```

## Building and Running

### Prerequisites

- Windows 10/11 (x64)
- Node.js 18.0 or higher
- Visual Studio 2019/2022 with C++ tools
- Administrator privileges (recommended)

### Build Steps

```bash
# Install dependencies
pnpm install

# Build native components
pnpm run build:native

# Build TypeScript
pnpm run build:ts

# Run the automation demo
pnpm run test:automation
```

### Example Scripts

```bash
# Basic functionality test
node dist/examples/basic-usage.js

# Advanced memory manipulation
node dist/examples/advanced-memory-manipulation.js

# Full automation demo
node dist/examples/automation-demo.js

# Memory helpers demo
node dist/examples/memory-helpers-demo.js
```

## Security and Ethical Use

⚠️ **Important Considerations:**

1. **Administrator Rights**: Many operations require elevated privileges
2. **Antivirus Detection**: Some security software may flag automation tools
3. **Ethical Use**: Only use on systems you own or have explicit permission to control
4. **Legal Compliance**: Ensure compliance with applicable laws and regulations
5. **Process Protection**: Some processes cannot be accessed due to system protection

## Troubleshooting

### Common Issues

**Build Errors:**

- Ensure Visual Studio C++ tools are installed
- Check Windows SDK availability
- Verify Node.js version compatibility

**Runtime Issues:**

- Run as Administrator for full functionality
- Check target application architecture (x64 vs x86)
- Verify screen coordinates are within bounds

**Performance Issues:**

- Reduce screenshot frequency
- Use appropriate delays between operations
- Limit color detection regions

### Debug Mode

```typescript
// Enable detailed logging
process.env.DEBUG = '1';
const automation = createMemoryHooking();

// Use error checking
try {
  const result = automation.captureScreen();
  if (result.data.length === 0) {
    console.log('Screenshot capture failed');
  }
} catch (error) {
  console.error('Automation error:', error);
}
```

This comprehensive automation system allows you to create sophisticated bots, testing frameworks, monitoring tools, and interactive applications that combine the power of memory manipulation with complete PC control.
