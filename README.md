# Memory Hooking Tool - C++ with TypeScript Scripting

> **"Hi, this is Doug. This project is more blackhat than you'll be used to seeing me put out, but hear me out. While everyone else is building automated browsers, I'm taking a different approach: giving LLMs complete control over the entire computer — including direct memory access and editing capabilities. I know it sounds crazy, but imagine the possibilities when AI can manipulate not just web pages, but running processes, game states, and system memory itself. This is just the beginning — expect to see an MCP server and agent tooling shortly afterwards!"**
>
> *-Be responsible.*
>
> *— Doug - withseismic.com*

A powerful C++ memory hooking and manipulation tool with a TypeScript scripting interface for reverse engineering, game hacking, process analysis, and complete PC automation. Combines advanced memory manipulation with comprehensive automation capabilities including mouse control, keyboard input, screenshot capture, window management, and visual automation.

## Features

### Core Functionality

- **Process Management**: Enumerate, attach to, and detach from running processes
- **Memory Operations**: Read and write process memory with type-safe wrappers
- **Pattern Scanning**: Advanced byte pattern and signature scanning
- **Memory Analysis**: Comprehensive memory region analysis and validation
- **Module Information**: Extract module base addresses, sizes, and metadata
- **String Search**: Search for ASCII and Unicode strings in process memory
- **Value Scanning**: Search for integers, floats, and custom data types

### Advanced Features

- **PE Header Analysis**: Parse and analyze Windows PE headers
- **Memory Region Filtering**: Filter by protection, state, and type
- **Pattern Creation Helpers**: Create byte patterns with wildcards
- **Type-Safe Memory Access**: Read/write Int32, Int64, Float, Double, Strings
- **Hex Utilities**: Convert between bytes and hex strings
- **Address Validation**: Validate memory addresses and permissions

### Automation Features

- **Complete PC Control**: Full mouse, keyboard, and window automation
- **Mouse Control**: Move, click, drag, scroll with pixel precision
- **Keyboard Control**: Send keys, text, and complex key combinations
- **Screenshot Capture**: Full screen, window, and region screenshots as binary data
- **Window Management**: Find, control, and manipulate windows
- **Screen Analysis**: Color detection, pixel sampling, and visual automation
- **System Integration**: Wait functions, timing, and system state detection
- **Visual Automation**: Color-based automation and pattern detection

> 📖 **Detailed Automation Documentation**: For comprehensive automation features, examples, and advanced use cases, see [AUTOMATION_FEATURES.md](AUTOMATION_FEATURES.md)

## Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   TypeScript    │    │   Node.js       │    │      C++        │
│   Interface     │◄──►│   Native        │◄──►│   Core Engine   │
│                 │    │   Addon         │    │                 │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

- **TypeScript Layer**: High-level API with type safety and utility functions
- **Node.js Addon**: Bridge between JavaScript and C++ using N-API
- **C++ Core**: Low-level Windows API calls for memory manipulation

## Requirements

- **OS**: Windows 10/11 (x64)
- **Node.js**: 18.0 or higher
- **Build Tools**:
  - Visual Studio 2019/2022 with C++ tools
  - Windows SDK
  - Python 3.x
- **Privileges**: Administrator privileges recommended for full functionality

## Installation

1. **Clone and navigate to the project**:

   ```bash
   cd apps/demo
   ```

2. **Install dependencies**:

   ```bash
   pnpm install
   ```

3. **Build the native addon**:

   ```bash
   pnpm run build:native
   ```

4. **Build TypeScript**:

   ```bash
   pnpm run build:ts
   ```

5. **Or build everything**:

   ```bash
   pnpm run build
   ```

## Quick Start

### Basic Usage

```typescript
import { createMemoryHooking, utils } from './src';

const memHook = createMemoryHooking();

// List all running processes
const processes = memHook.getRunningProcesses();
console.log(`Found ${processes.length} processes`);

// Attach to a process
const attached = memHook.attachToProcess('notepad.exe');
if (attached) {
  // Read memory
  const data = memHook.readMemory(0x7FF000000000, 64);
  
  // Scan for patterns
  const addresses = memHook.scanForString('Hello World');
  
  // Get module info
  const module = memHook.getModuleInfo('notepad.exe');
  
  memHook.detachFromProcess();
}
```

### Advanced Usage

```typescript
import { MemoryHooking, utils, MemoryProtection } from './src';

const memHook = new MemoryHooking();

// Find process by pattern
const chromeProcesses = await memHook.findProcessesByPattern('chrome');

// Attach to specific process
if (memHook.attachToProcess(chromeProcesses[0].processId)) {
  
  // Get memory regions
  const executableRegions = memHook.getExecutableRegions();
  const writableRegions = memHook.getWritableRegions();
  
  // Create byte pattern
  const { pattern, mask } = memHook.createPattern(
    [0x48, 0x89, 0x5C, 0x24], 
    [false, false, false, false]
  );
  
  // Scan in specific module
  const results = memHook.scanInModule('chrome.exe', () => 
    memHook.scanForPattern(pattern, mask)
  );
  
  // Read typed values
  const intValue = memHook.readInt32(results[0]);
  const floatValue = memHook.readFloat(results[0] + 4);
  const stringValue = memHook.readString(results[0] + 8, 256);
  
  // Write typed values
  memHook.writeInt32(results[0], 12345);
  memHook.writeString(results[0] + 4, "Modified!");
  
  memHook.detachFromProcess();
}
```

### Automation Usage

```typescript
import { createMemoryHooking, MouseButton, KeyCode } from './src';

const automation = createMemoryHooking();

// Take a screenshot and save it
const screenshot = automation.captureScreen();
automation.saveScreenshotToFile(screenshot, 'screenshot.bmp');

// Control mouse and keyboard
automation.moveMouse(100, 100);
automation.clickMouse(MouseButton.Left);
automation.sendText('Hello, automated world!');

// Window management
const windows = automation.getWindowsByTitle('Calculator');
if (windows.length > 0) {
  automation.setActiveWindow(windows[0].hwnd);
  automation.moveWindow(windows[0].hwnd, 100, 100, 800, 600);
}

// Color detection and visual automation
const redColor = 0xFF0000; // Pure red
const redPixels = automation.findColorOnScreen(redColor, 10);
if (redPixels.length > 0) {
  automation.clickMouse(MouseButton.Left, redPixels[0].x, redPixels[0].y);
}

// Combined memory manipulation and automation
const gameProcess = await automation.findProcessByName('game.exe');
if (gameProcess && automation.attachToProcess(gameProcess.processId)) {
  const health = automation.readInt32(0x12345678);
  if (health < 50) {
    automation.pressKey(KeyCode.H); // Use health potion
  }
  automation.detachFromProcess();
}
```

## Examples

### Run Basic Example

```bash
pnpm run test
# or
node dist/examples/basic-usage.js
```

### Run Advanced Example

```bash
node dist/examples/advanced-memory-manipulation.js
```

### Run Automation Demo

```bash
node dist/examples/automation-demo.js
```

## API Reference

### Core Classes

#### `MemoryHooking`

Main class providing all memory manipulation and automation functionality.

**Process Management:**

- `getRunningProcesses(): ProcessInfo[]`
- `attachToProcess(processId: number | processName: string): boolean`
- `detachFromProcess(): void`
- `isProcessAttached(): boolean`
- `getCurrentProcessInfo(): ProcessManagerInfo`

**Memory Operations:**

- `readMemory(address: Address, size: Size): Buffer | null`
- `writeMemory(address: Address, data: Buffer): boolean`
- `readInt32(address: Address): number | null`
- `writeInt32(address: Address, value: number): boolean`
- `readString(address: Address, length: number): string | null`

**Memory Scanning:**

- `scanForPattern(pattern: string, mask: string): Address[]`
- `scanForBytes(bytes: number[]): Address[]`
- `scanForString(text: string, caseSensitive?: boolean): Address[]`
- `scanForValue(value: number | string): Address[]`

**Memory Analysis:**

- `getMemoryRegions(): MemoryRegion[]`
- `getExecutableRegions(): MemoryRegion[]`
- `getWritableRegions(): MemoryRegion[]`
- `isValidAddress(address: Address): boolean`

**Automation Operations:**

- `captureScreen(): ScreenshotData`
- `captureWindow(hwnd: number): ScreenshotData`
- `captureRegion(x: number, y: number, width: number, height: number): ScreenshotData`
- `saveScreenshotToFile(screenshot: ScreenshotData, filename: string): void`
- `moveMouse(x: number, y: number): void`
- `clickMouse(button: MouseButton, x?: number, y?: number): void`
- `sendText(text: string): void`
- `pressKey(key: KeyCode, holdTime?: number): void`
- `getWindowsByTitle(title: string): WindowInfo[]`
- `setActiveWindow(hwnd: number): void`
- `findColorOnScreen(color: number, tolerance?: number): Point[]`
- `getPixelColor(x: number, y: number): number`
- `waitForWindow(title: string, timeout: number): boolean`
- `sleep(milliseconds: number): void`

### Helper Modules

#### `ProcessManager`

Core process management and attachment functionality.

**Process Operations:**

- `getRunningProcesses(): ProcessInfo[]` - Enumerate all running processes
- `attachToProcess(processId: number | processName: string): boolean` - Attach to target process
- `detachFromProcess(): void` - Detach from current process
- `isProcessAttached(): boolean` - Check attachment status
- `getCurrentProcessId(): number` - Get attached process ID
- `getCurrentProcessName(): string` - Get attached process name

**Memory Operations:**

- `readMemory(address: Address, buffer: Buffer, size: number): boolean` - Read process memory
- `writeMemory(address: Address, data: Buffer, size: number): boolean` - Write process memory
- `getModuleHandle(moduleName: string): number` - Get module handle
- `getModuleBaseAddress(moduleName: string): Address` - Get module base address
- `getModuleSize(moduleName: string): number` - Get module size

**Features:**

- Automatic debug privilege elevation
- Safe memory access with error checking
- Module enumeration and information retrieval
- Process handle management with automatic cleanup

#### `MemoryScanner`

Basic memory scanning and pattern matching functionality.

**Pattern Scanning:**

- `scanForPattern(pattern: string, mask: string): Address[]` - Scan for byte patterns
- `scanForBytes(bytes: number[]): Address[]` - Scan for byte sequences
- `scanForString(text: string, caseSensitive?: boolean): Address[]` - Scan for text strings

**Value Scanning:**

- `scanForValue(value: number): Address[]` - Scan for integer values
- `scanForValue(value: float): Address[]` - Scan for floating-point values
- `scanForValue(value: string): Address[]` - Scan for string values

**Memory Analysis:**

- `getMemoryRegions(): MemoryRegion[]` - Get all memory regions
- `getExecutableRegions(): MemoryRegion[]` - Get executable regions only
- `getWritableRegions(): MemoryRegion[]` - Get writable regions only
- `isValidAddress(address: Address): boolean` - Validate memory address
- `isExecutableAddress(address: Address): boolean` - Check if address is executable
- `isWritableAddress(address: Address): boolean` - Check if address is writable

**Utility Functions:**

- `bytesToHexString(bytes: number[]): string` - Convert bytes to hex string
- `hexStringToBytes(hexString: string): number[]` - Parse hex string to bytes
- `patternToBytes(pattern: string, mask: string): number[]` - Convert pattern to bytes

#### `MemoryHelpers`

Advanced memory manipulation and analysis utilities.

**Pointer Operations:**

- `findPointersTo(targetAddress: Address, searchStart?: Address, searchEnd?: Address): PointerResult[]` - Find pointers to target address
- `findPointerChain(finalAddress: Address, maxDepth?: number, offsets?: number[]): PointerResult[]` - Find pointer chains
- `findNullPointers(searchStart: Address, searchEnd: Address): Address[]` - Find null pointers in range

**Enhanced Pattern Searching:**

- `searchBytePattern(pattern: number[], mask: boolean[], searchStart?: Address, searchEnd?: Address): PatternMatch[]` - Advanced byte pattern search
- `searchStringPattern(pattern: string, caseSensitive?: boolean, unicode?: boolean, searchStart?: Address, searchEnd?: Address): PatternMatch[]` - Advanced string search
- `searchRegexPattern(regexPattern: string, searchStart?: Address, searchEnd?: Address): PatternMatch[]` - Regular expression search

**Numeric Value Searching:**

- `searchInt32(value: number, searchStart?: Address, searchEnd?: Address, aligned?: boolean): NumericMatch[]` - Search for 32-bit integers
- `searchInt64(value: number, searchStart?: Address, searchEnd?: Address, aligned?: boolean): NumericMatch[]` - Search for 64-bit integers
- `searchFloat(value: number, tolerance?: number, searchStart?: Address, searchEnd?: Address): NumericMatch[]` - Search for floats with tolerance
- `searchDouble(value: number, tolerance?: number, searchStart?: Address, searchEnd?: Address): NumericMatch[]` - Search for doubles with tolerance

**Memory Region Analysis:**

- `getMemoryRegions(executableOnly?: boolean, writableOnly?: boolean): MemoryRegionEx[]` - Get detailed memory regions
- `getModuleRegions(moduleName?: string): MemoryRegionEx[]` - Get regions for specific module
- `getRegionInfo(address: Address): MemoryRegionEx` - Get detailed region information

**Advanced Scanning:**

- `scanForCode(opcodes: number[], searchStart?: Address, searchEnd?: Address): PatternMatch[]` - Scan for machine code
- `findFunctionPrologs(searchStart?: Address, searchEnd?: Address): Address[]` - Find function prologues
- `findCallInstructions(targetAddress: Address, searchStart?: Address, searchEnd?: Address): Address[]` - Find calls to target

**Memory Comparison:**

- `compareMemoryRegions(region1Start: Address, region2Start: Address, size: number): Address[]` - Compare memory regions
- `findChangedBytes(address: Address, originalData: number[]): Address[]` - Find changed bytes since snapshot

**Utility Functions:**

- `parseBytePattern(pattern: string): number[]` - Parse byte pattern from string
- `parsePatternMask(pattern: string): boolean[]` - Parse pattern mask from string
- `formatAddress(address: Address, withPrefix?: boolean): string` - Format address as hex string
- `isValidCodeAddress(address: Address): boolean` - Check if address contains valid code
- `getAlignment(address: Address): number` - Get address alignment

#### `ErrorHandler`

Comprehensive error handling and logging system.

**Exception Handling:**

- `MemoryHookingException` - Custom exception class with detailed error information
- `getWindowsErrorMessage(errorCode: number): string` - Get Windows error description
- `getMemoryProtectionString(protection: number): string` - Get memory protection description

**Safe Operations:**

- `safeReadMemory(processHandle: Handle, address: Address, buffer: Buffer, size: number): boolean` - Safe memory read with validation
- `safeWriteMemory(processHandle: Handle, address: Address, data: Buffer, size: number): boolean` - Safe memory write with validation
- `safeVirtualQuery(processHandle: Handle, address: Address): MemoryInfo` - Safe memory query with validation

**Address Validation:**

- `isAddressAccessible(processHandle: Handle, address: Address, size: number): boolean` - Check if address range is accessible
- `isValidProcessHandle(processHandle: Handle): boolean` - Validate process handle

**Logging System:**

- `logError(operation: string, errorCode?: number): void` - Log error with Windows error details
- `logWarning(message: string): void` - Log warning message
- `logInfo(message: string): void` - Log informational message

**RAII Handle Management:**

- `SafeHandle` - RAII wrapper for Windows handles with automatic cleanup
- Automatic handle closure on destruction
- Move semantics support for efficient handle transfer

**Features:**

- Automatic Windows error code translation
- Memory protection flag interpretation
- Safe memory operations with bounds checking
- Comprehensive logging with configurable levels
- RAII pattern for resource management

#### `AutomationHelpers`

Complete PC automation and control system.

**Mouse Control:**

- `moveMouse(x: number, y: number): boolean` - Move mouse to coordinates
- `clickMouse(button: MouseButton, x?: number, y?: number): boolean` - Click mouse button
- `doubleClickMouse(button: MouseButton, x?: number, y?: number): boolean` - Double-click mouse
- `dragMouse(fromX: number, fromY: number, toX: number, toY: number, button?: MouseButton): boolean` - Drag mouse
- `scrollMouse(x: number, y: number, scrollAmount: number): boolean` - Scroll mouse wheel
- `getMousePosition(): {x: number, y: number}` - Get current mouse position
- `isMouseButtonPressed(button: MouseButton): boolean` - Check if mouse button is pressed

**Keyboard Control:**

- `pressKey(key: KeyCode, duration?: number): boolean` - Press and release key
- `releaseKey(key: KeyCode): boolean` - Release key
- `sendKeyCombo(keys: KeyCode[]): boolean` - Send key combination
- `sendText(text: string): boolean` - Send text input
- `sendTextToWindow(hwnd: number, text: string): boolean` - Send text to specific window
- `isKeyPressed(key: KeyCode): boolean` - Check if key is pressed
- `getPressedKeys(): KeyCode[]` - Get all currently pressed keys

**Screenshot Capture:**

- `captureScreen(): ScreenshotData` - Capture entire screen
- `captureWindow(hwnd: number): ScreenshotData` - Capture specific window
- `captureRegion(x: number, y: number, width: number, height: number): ScreenshotData` - Capture screen region
- `captureActiveWindow(): ScreenshotData` - Capture active window
- `saveScreenshotToFile(screenshot: ScreenshotData, filename: string): boolean` - Save screenshot to file

**Window Management:**

- `getAllWindows(): WindowInfo[]` - Get all windows
- `getWindowsByTitle(titlePattern: string): WindowInfo[]` - Find windows by title (regex supported)
- `getWindowsByClassName(className: string): WindowInfo[]` - Find windows by class name
- `getActiveWindow(): WindowInfo` - Get active window
- `setActiveWindow(hwnd: number): boolean` - Set active window
- `showWindow(hwnd: number, showCommand?: number): boolean` - Show/hide window
- `moveWindow(hwnd: number, x: number, y: number, width: number, height: number): boolean` - Move/resize window
- `closeWindow(hwnd: number): boolean` - Close window

**Screen Information:**

- `getScreenWidth(): number` - Get screen width
- `getScreenHeight(): number` - Get screen height
- `getScreenDPI(): number` - Get screen DPI
- `getMonitorRects(): Rect[]` - Get monitor rectangles

**Color and Pixel Operations:**

- `getPixelColor(x: number, y: number): number` - Get pixel color at coordinates
- `getPixelColors(x: number, y: number, width: number, height: number): number[]` - Get colors in region
- `findColorOnScreen(color: number, tolerance?: number): Point[]` - Find color on screen
- `findColorInRegion(color: number, x: number, y: number, width: number, height: number, tolerance?: number): Point[]` - Find color in region

**Wait Functions:**

- `sleep(milliseconds: number): void` - Sleep for specified time
- `waitForWindow(titlePattern: string, timeout: number): boolean` - Wait for window to appear
- `waitForPixelColor(x: number, y: number, color: number, timeout: number, tolerance?: number): boolean` - Wait for pixel color
- `waitForKeyPress(key: KeyCode, timeout: number): boolean` - Wait for key press

**System Information:**

- `getSystemInfo(): string` - Get system information
- `getRunningWindowTitles(): string[]` - Get titles of visible windows
- `isScreenLocked(): boolean` - Check if screen is locked
- `isUserIdle(idleTime: number): boolean` - Check if user is idle

### Utility Functions

#### `utils`

Collection of utility functions for common operations.

- `toHex(address: Address): string` - Convert address to hex string
- `fromHex(hexString: string): Address` - Parse hex string to address
- `isReadable(protection: number): boolean` - Check if memory is readable
- `isWritable(protection: number): boolean` - Check if memory is writable
- `isExecutable(protection: number): boolean` - Check if memory is executable

### Types

```typescript
interface ProcessInfo {
  processId: number;
  processName: string;
  windowTitle: string;
}

interface ModuleInfo {
  baseAddress: number;
  size: number;
  name: string;
}

interface MemoryRegion {
  baseAddress: number;
  size: number;
  protection: number;
  state: number;
  type: number;
}

interface ScreenshotData {
  width: number;
  height: number;
  bitsPerPixel: number;
  format: string;
  data: number[];
}

interface WindowInfo {
  hwnd: number;
  title: string;
  className: string;
  x: number;
  y: number;
  width: number;
  height: number;
  isVisible: boolean;
}

interface Point {
  x: number;
  y: number;
}

enum MouseButton {
  Left = 1,
  Right = 2,
  Middle = 4
}

enum KeyCode {
  A = 0x41,
  B = 0x42,
  Enter = 0x0D,
  Space = 0x20,
  Control = 0x11,
  Shift = 0x10,
  Alt = 0x12,
  Win = 0x5B
  // ... more key codes
}

// Advanced helper types
interface PointerResult {
  address: number;
  pointsTo: number;
  offset: number;
}

interface PatternMatch {
  address: number;
  size: number;
  data: number[];
}

interface MemoryRegionEx {
  startAddress: number;
  endAddress: number;
  size: number;
  protection: number;
  type: number;
  moduleName: string;
}

interface NumericMatch {
  address: number;
  dataType: string;
  rawData: number[];
}

// Error handling types
enum MemoryHookingError {
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
}

interface Handle {
  value: number;
  isValid: boolean;
}
```

## Security Considerations

⚠️ **Important Security Notes:**

1. **Administrator Privileges**: Many operations require administrator privileges
2. **Process Protection**: Some processes (like system processes) cannot be accessed
3. **Memory Safety**: Always validate addresses before reading/writing
4. **Error Handling**: Implement proper error handling for memory operations
5. **Legal Use**: Only use on processes you own or have explicit permission to analyze

## Example Scripts

### Process Scanner

```typescript
// Scan for processes and show memory information
const processes = memHook.getRunningProcesses();
for (const proc of processes) {
  if (memHook.attachToProcess(proc.processId)) {
    const regions = memHook.getMemoryRegions();
    console.log(`${proc.processName}: ${regions.length} memory regions`);
    memHook.detachFromProcess();
  }
}
```

### Pattern Scanner

```typescript
// Search for specific byte patterns
const { pattern, mask } = memHook.createPattern(
  [0x4C, 0x8D, 0x05], // LEA r8, [rip+offset]
  [false, false, false]
);

const addresses = memHook.scanForPattern(pattern, mask);
console.log(`Found pattern at ${addresses.length} locations`);
```

### Memory Dumper

```typescript
// Dump executable sections to file
const execRegions = memHook.getExecutableRegions();
for (const region of execRegions) {
  const data = memHook.readMemory(region.baseAddress, region.size);
  if (data) {
    fs.writeFileSync(
      `dump_${region.baseAddress.toString(16)}.bin`, 
      data
    );
  }
}
```

### Advanced Memory Analysis

```typescript
// Find pointers to a specific address
const targetAddress = 0x7FF000000000;
const pointers = memHook.findPointersTo(targetAddress);
console.log(`Found ${pointers.length} pointers to ${memHook.formatAddress(targetAddress)}`);

// Search for function prologues
const prologues = memHook.findFunctionPrologs();
console.log(`Found ${prologues.length} function prologues`);

// Search for specific numeric values
const healthAddresses = memHook.searchInt32(100); // Find all locations with value 100
console.log(`Found health value at ${healthAddresses.length} locations`);

// Advanced pattern searching with tolerance
const floatValues = memHook.searchFloat(3.14159, 0.001); // Find PI with tolerance
console.log(`Found PI approximations at ${floatValues.length} locations`);
```

### Automation Examples

```typescript
// Automated screenshot monitoring
const automation = createMemoryHooking();

// Take periodic screenshots
setInterval(() => {
  const screenshot = automation.captureScreen();
  if (screenshot) {
    automation.saveScreenshotToFile(screenshot, `screenshot_${Date.now()}.bmp`);
  }
}, 5000);

// Wait for specific color to appear and click it
const redColor = 0xFF0000;
if (automation.waitForPixelColor(400, 300, redColor, 10000)) {
  automation.clickMouse(MouseButton.Left, 400, 300);
  console.log('Found and clicked red pixel!');
}

// Automated window management
const calcWindows = automation.getWindowsByTitle('Calculator');
if (calcWindows.length > 0) {
  automation.setActiveWindow(calcWindows[0].hwnd);
  automation.moveWindow(calcWindows[0].hwnd, 100, 100, 400, 300);
  automation.sendText('2+2=');
  automation.pressKey(KeyCode.Enter);
}
```

### Error Handling Examples

```typescript
// Safe memory operations with error handling
try {
  const data = memHook.readMemory(0x12345678, 64);
  if (data) {
    console.log('Successfully read memory');
  }
} catch (error) {
  console.error('Memory operation failed:', error.message);
  console.error('Error details:', error.getDetailedMessage());
}

// Using safe operations with validation
const processHandle = memHook.getProcessHandle();
if (ErrorHandler.isValidProcessHandle(processHandle)) {
  const isAccessible = ErrorHandler.isAddressAccessible(processHandle, 0x12345678, 64);
  if (isAccessible) {
    // Safe to read memory
    const data = memHook.readMemory(0x12345678, 64);
  }
}
```

### Comprehensive Game Bot Example

```typescript
// Advanced game automation combining memory manipulation and automation
class GameBot {
  private memHook: MemoryHooking;
  private automation: MemoryHooking;
  
  constructor() {
    this.memHook = createMemoryHooking();
    this.automation = createMemoryHooking();
  }
  
  async initialize() {
    // Find and attach to game process
    const gameProcess = await this.findGameProcess();
    if (!gameProcess) {
      throw new Error('Game process not found');
    }
    
    this.memHook.attachToProcess(gameProcess.processId);
    
    // Set up automation
    const gameWindows = this.automation.getWindowsByTitle('MyGame');
    if (gameWindows.length > 0) {
      this.automation.setActiveWindow(gameWindows[0].hwnd);
    }
  }
  
  async findGameProcess() {
    const processes = this.memHook.getRunningProcesses();
    return processes.find(p => p.processName.includes('game.exe'));
  }
  
  async runBot() {
    while (true) {
      try {
        // Check health from memory
        const healthValue = this.memHook.readInt32(0x12345678);
        if (healthValue && healthValue < 50) {
          this.automation.pressKey(KeyCode.H); // Use health potion
          await this.sleep(1000);
        }
        
        // Visual enemy detection
        const enemyColor = 0xFF0000;
        const enemies = this.automation.findColorOnScreen(enemyColor, 20);
        
        if (enemies.length > 0) {
          // Attack nearest enemy
          const nearest = this.findNearestEnemy(enemies);
          this.automation.clickMouse(MouseButton.Left, nearest.x, nearest.y);
          this.automation.pressKey(KeyCode.Space); // Attack key
          await this.sleep(500);
        }
        
        // Check for loot
        const lootColor = 0x00FF00;
        const loot = this.automation.findColorOnScreen(lootColor, 10);
        if (loot.length > 0) {
          this.automation.clickMouse(MouseButton.Left, loot[0].x, loot[0].y);
          await this.sleep(200);
        }
        
        await this.sleep(100); // Main loop delay
        
      } catch (error) {
        console.error('Bot error:', error);
        await this.sleep(5000); // Error recovery delay
      }
    }
  }
  
  private findNearestEnemy(enemies: Point[]): Point {
    const center = { x: 960, y: 540 }; // Screen center
    return enemies.reduce((nearest, enemy) => {
      const distance = Math.sqrt(
        Math.pow(enemy.x - center.x, 2) + 
        Math.pow(enemy.y - center.y, 2)
      );
      const nearestDistance = Math.sqrt(
        Math.pow(nearest.x - center.x, 2) + 
        Math.pow(nearest.y - center.y, 2)
      );
      return distance < nearestDistance ? enemy : nearest;
    });
  }
  
  private sleep(ms: number): Promise<void> {
    return new Promise(resolve => setTimeout(resolve, ms));
  }
}

// Usage
const bot = new GameBot();
await bot.initialize();
await bot.runBot();
```

## Building from Source

### Prerequisites

1. Install Visual Studio with C++ tools
2. Install Node.js and pnpm
3. Install Windows SDK

### Build Steps

```bash
# Install dependencies
pnpm install

# Configure native build
pnpm run build:native

# Build TypeScript
pnpm run build:ts

# Run tests
pnpm test
```

## Troubleshooting

### Common Issues

**Build Errors:**

- Ensure Visual Studio C++ tools are installed
- Check that Windows SDK is available
- Verify Node.js version compatibility

**Runtime Errors:**

- Run as Administrator for process access
- Check target process architecture (x64 vs x86)
- Verify memory addresses are valid

**Access Denied:**

- Some processes require elevated privileges
- Anti-virus software may block memory access
- Windows Defender may flag the tool

### Debug Mode

```typescript
// Enable verbose logging
process.env.DEBUG = '1';
const memHook = createMemoryHooking();
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure all tests pass
5. Submit a pull request

## License

MIT License - see LICENSE file for details.

## Disclaimer

This tool is for educational and legitimate security research purposes only. Users are responsible for ensuring compliance with applicable laws and regulations. The authors are not responsible for any misuse of this software.

---

**Note**: This tool requires Windows and administrator privileges for full functionality. Always use responsibly and only on systems you own or have explicit permission to analyze.
