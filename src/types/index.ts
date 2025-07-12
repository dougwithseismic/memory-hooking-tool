export interface ProcessInfo {
  processId: number;
  processName: string;
  windowTitle: string;
}

export interface ModuleInfo {
  baseAddress: number;
  size: number;
  name: string;
}

export interface MemoryRegion {
  baseAddress: number;
  size: number;
  protection: number;
  state: number;
  type: number;
}

export interface ProcessManagerInfo {
  processId: number;
  processName: string;
  isAttached: boolean;
}

export interface ScanOptions {
  caseSensitive?: boolean;
  maxResults?: number;
  startAddress?: number;
  endAddress?: number;
}

export interface PatternScanOptions extends ScanOptions {
  pattern: string;
  mask: string;
}

export interface ValueScanOptions extends ScanOptions {
  value: number | string;
}

export interface BytesScanOptions extends ScanOptions {
  bytes: number[];
}

export interface StringScanOptions extends ScanOptions {
  text: string;
  caseSensitive?: boolean;
}

// Error handling types
export enum MemoryHookingErrorCode {
  None = 0,
  ProcessNotFound = 1,
  AccessDenied = 2,
  InvalidAddress = 3,
  InvalidSize = 4,
  ReadMemoryFailed = 5,
  WriteMemoryFailed = 6,
  ScanningFailed = 7,
  ModuleNotFound = 8,
  InvalidPattern = 9,
  InsufficientPrivileges = 10,
  ProcessNotAttached = 11,
  UnknownError = 12,
}

export class MemoryHookingError extends Error {
  public readonly errorCode: MemoryHookingErrorCode;
  public readonly windowsErrorCode?: number;
  public readonly operation?: string;

  constructor(
    message: string,
    errorCode: MemoryHookingErrorCode = MemoryHookingErrorCode.UnknownError,
    windowsErrorCode?: number,
    operation?: string
  ) {
    super(message);
    this.name = "MemoryHookingError";
    this.errorCode = errorCode;
    this.windowsErrorCode = windowsErrorCode;
    this.operation = operation;
  }

  getDetailedMessage(): string {
    let msg = `${this.operation ? `[${this.operation}] ` : ""}${this.message}`;

    if (this.windowsErrorCode) {
      msg += ` (Windows Error: ${this.windowsErrorCode})`;
    }

    return msg;
  }

  static isMemoryHookingError(error: any): error is MemoryHookingError {
    return error instanceof MemoryHookingError;
  }
}

export interface ScanResult {
  addresses: Address[];
  totalScanned: number;
  successfulReads: number;
  failedReads: number;
  truncated: boolean;
}

export interface SafeOperationResult<T = any> {
  success: boolean;
  data?: T;
  error?: MemoryHookingError;
}

export enum MemoryProtection {
  PAGE_NOACCESS = 0x01,
  PAGE_READONLY = 0x02,
  PAGE_READWRITE = 0x04,
  PAGE_WRITECOPY = 0x08,
  PAGE_EXECUTE = 0x10,
  PAGE_EXECUTE_READ = 0x20,
  PAGE_EXECUTE_READWRITE = 0x40,
  PAGE_EXECUTE_WRITECOPY = 0x80,
  PAGE_GUARD = 0x100,
  PAGE_NOCACHE = 0x200,
  PAGE_WRITECOMBINE = 0x400,
}

export enum MemoryState {
  MEM_COMMIT = 0x1000,
  MEM_FREE = 0x10000,
  MEM_RESERVE = 0x2000,
}

export enum MemoryType {
  MEM_IMAGE = 0x1000000,
  MEM_MAPPED = 0x40000,
  MEM_PRIVATE = 0x20000,
}

export type Address = number;
export type Size = number;
export type DataBuffer = Buffer;

export interface IMemoryHooking {
  // Process Management
  getRunningProcesses(): ProcessInfo[];
  attachToProcess(processId: number): boolean;
  attachToProcess(processName: string): boolean;
  detachFromProcess(): void;
  isProcessAttached(): boolean;
  getCurrentProcessInfo(): ProcessManagerInfo;

  // Memory Operations
  readMemory(address: Address, size: Size): DataBuffer | null;
  writeMemory(address: Address, data: DataBuffer): boolean;
  getModuleInfo(moduleName: string): ModuleInfo | null;

  // Safe Memory Operations (with error handling)
  safeReadMemory(address: Address, size: Size): SafeOperationResult<DataBuffer>;
  safeWriteMemory(
    address: Address,
    data: DataBuffer
  ): SafeOperationResult<boolean>;

  // Memory Scanning
  scanForPattern(pattern: string, mask: string): Address[];
  scanForBytes(bytes: number[]): Address[];
  scanForString(text: string, caseSensitive?: boolean): Address[];
  scanForValue(value: number | string): Address[];

  // Safe Memory Scanning (with error handling)
  safeScanForPattern(
    pattern: string,
    mask: string
  ): SafeOperationResult<ScanResult>;
  safeScanForString(
    text: string,
    caseSensitive?: boolean
  ): SafeOperationResult<ScanResult>;

  // Memory Region Analysis
  getMemoryRegions(): MemoryRegion[];
  isValidAddress(address: Address): boolean;

  // Utility Methods
  bytesToHexString(bytes: number[]): string;
  hexStringToBytes(hexString: string): number[];

  // Error handling
  enableLogging(enabled: boolean): void;
  getLastError(): MemoryHookingError | null;
}

// Memory Helpers - New interfaces for helper functions
export interface PointerResult {
  address: number;
  pointsTo: number;
  offset: number;
}

export interface PatternMatch {
  address: number;
  size: number;
  data: number[];
}

export interface NumericMatch {
  address: number;
  dataType: string;
  rawData: number[];
}

export interface MemoryRegionEx {
  startAddress: number;
  endAddress: number;
  size: number;
  protection: number;
  type: number;
  moduleName: string;
}

// Memory Helpers - Search options
export interface PatternSearchOptions {
  searchStart?: number;
  searchEnd?: number;
}

export interface StringSearchOptions extends PatternSearchOptions {
  caseSensitive?: boolean;
  unicode?: boolean;
}

export interface NumericSearchOptions extends PatternSearchOptions {
  aligned?: boolean;
  tolerance?: number;
}

export interface PointerSearchOptions extends PatternSearchOptions {
  maxDepth?: number;
  offsets?: number[];
}

export interface RegionFilterOptions {
  executableOnly?: boolean;
  writableOnly?: boolean;
}

// Automation Helper Types
export interface MousePosition {
  x: number;
  y: number;
}

export interface ScreenshotData {
  data: number[];
  width: number;
  height: number;
  bitsPerPixel: number;
  format: string;
}

export interface WindowInfo {
  hwnd: number;
  title: string;
  className: string;
  x: number;
  y: number;
  width: number;
  height: number;
  isVisible: boolean;
}

export enum MouseButton {
  Left = 1,
  Right = 2,
  Middle = 4,
}

export enum KeyCode {
  // Common keys
  Backspace = 8,
  Tab = 9,
  Enter = 13,
  Escape = 27,
  Space = 32,
  Delete = 46,

  // Arrow keys
  Left = 37,
  Up = 38,
  Right = 39,
  Down = 40,

  // Function keys
  F1 = 112,
  F2 = 113,
  F3 = 114,
  F4 = 115,
  F5 = 116,
  F6 = 117,
  F7 = 118,
  F8 = 119,
  F9 = 120,
  F10 = 121,
  F11 = 122,
  F12 = 123,

  // Modifier keys
  Shift = 16,
  Control = 17,
  Alt = 18,
  Win = 91,

  // Number keys
  Key0 = 48,
  Key1 = 49,
  Key2 = 50,
  Key3 = 51,
  Key4 = 52,
  Key5 = 53,
  Key6 = 54,
  Key7 = 55,
  Key8 = 56,
  Key9 = 57,

  // Letter keys
  A = 65,
  B = 66,
  C = 67,
  D = 68,
  E = 69,
  F = 70,
  G = 71,
  H = 72,
  I = 73,
  J = 74,
  K = 75,
  L = 76,
  M = 77,
  N = 78,
  O = 79,
  P = 80,
  Q = 81,
  R = 82,
  S = 83,
  T = 84,
  U = 85,
  V = 86,
  W = 87,
  X = 88,
  Y = 89,
  Z = 90,
}

export interface MemoryHookingNative {
  // Process Management
  getRunningProcesses(): ProcessInfo[];
  attachToProcess(processId: number): boolean;
  attachToProcess(processName: string): boolean;
  detachFromProcess(): void;
  isProcessAttached(): boolean;
  getCurrentProcessInfo(): ProcessManagerInfo;

  // Memory Operations
  readMemory(address: Address, size: Size): DataBuffer | null;
  writeMemory(address: Address, data: DataBuffer): boolean;
  getModuleInfo(moduleName: string): ModuleInfo | null;

  // Safe Memory Operations (with error handling)
  safeReadMemory(address: Address, size: Size): SafeOperationResult<DataBuffer>;
  safeWriteMemory(
    address: Address,
    data: DataBuffer
  ): SafeOperationResult<boolean>;

  // Memory Scanning
  scanForPattern(pattern: string, mask: string): Address[];
  scanForBytes(bytes: number[]): Address[];
  scanForString(text: string, caseSensitive?: boolean): Address[];
  scanForValue(value: number | string): Address[];

  // Safe Memory Scanning (with error handling)
  safeScanForPattern(
    pattern: string,
    mask: string
  ): SafeOperationResult<ScanResult>;
  safeScanForString(
    text: string,
    caseSensitive?: boolean
  ): SafeOperationResult<ScanResult>;

  // Memory Region Analysis
  getMemoryRegions(): MemoryRegion[];
  isValidAddress(address: Address): boolean;

  // Utility Methods
  bytesToHexString(bytes: number[]): string;
  hexStringToBytes(hexString: string): number[];

  // Error handling
  enableLogging(enabled: boolean): void;
  getLastError(): MemoryHookingError | null;

  // Memory Helpers - Pointer Operations
  findPointersTo(
    targetAddress: number,
    searchStart?: number,
    searchEnd?: number
  ): PointerResult[];
  findPointerChain(
    finalAddress: number,
    maxDepth?: number,
    offsets?: number[]
  ): PointerResult[];
  findNullPointers(searchStart: number, searchEnd: number): number[];

  // Memory Helpers - Pattern Searching
  searchBytePattern(
    pattern: number[],
    mask: boolean[],
    searchStart?: number,
    searchEnd?: number
  ): PatternMatch[];
  searchStringPattern(
    pattern: string,
    caseSensitive?: boolean,
    unicode?: boolean,
    searchStart?: number,
    searchEnd?: number
  ): PatternMatch[];
  searchRegexPattern(
    regexPattern: string,
    searchStart?: number,
    searchEnd?: number
  ): PatternMatch[];

  // Memory Helpers - Numeric Searching
  searchInt32(
    value: number,
    searchStart?: number,
    searchEnd?: number,
    aligned?: boolean
  ): NumericMatch[];
  searchInt64(
    value: number,
    searchStart?: number,
    searchEnd?: number,
    aligned?: boolean
  ): NumericMatch[];
  searchFloat(
    value: number,
    tolerance?: number,
    searchStart?: number,
    searchEnd?: number
  ): NumericMatch[];
  searchDouble(
    value: number,
    tolerance?: number,
    searchStart?: number,
    searchEnd?: number
  ): NumericMatch[];

  // Memory Helpers - Region Analysis
  getMemoryRegionsEx(
    executableOnly?: boolean,
    writableOnly?: boolean
  ): MemoryRegionEx[];
  getModuleRegions(moduleName?: string): MemoryRegionEx[];
  getRegionInfo(address: number): MemoryRegionEx;

  // Memory Helpers - Advanced Scanning
  scanForCode(
    opcodes: number[],
    searchStart?: number,
    searchEnd?: number
  ): PatternMatch[];
  findFunctionPrologs(searchStart?: number, searchEnd?: number): number[];
  findCallInstructions(
    targetAddress: number,
    searchStart?: number,
    searchEnd?: number
  ): number[];

  // Memory Helpers - Comparison and Diffing
  compareMemoryRegions(
    region1Start: number,
    region2Start: number,
    size: number
  ): number[];
  findChangedBytes(address: number, originalData: number[]): number[];

  // Memory Helpers - Utility Functions
  parseBytePattern(pattern: string): number[];
  parsePatternMask(pattern: string): boolean[];
  formatAddress(address: number, withPrefix?: boolean): string;
  isValidCodeAddress(address: number): boolean;
  getAlignment(address: number): number;

  // Automation Helpers - Mouse Control
  moveMouse(x: number, y: number): boolean;
  clickMouse(button: MouseButton, x?: number, y?: number): boolean;
  doubleClickMouse(button: MouseButton, x?: number, y?: number): boolean;
  dragMouse(
    fromX: number,
    fromY: number,
    toX: number,
    toY: number,
    button?: MouseButton
  ): boolean;
  scrollMouse(x: number, y: number, scrollAmount: number): boolean;
  getMousePosition(): MousePosition;
  isMouseButtonPressed(button: MouseButton): boolean;

  // Automation Helpers - Keyboard Control
  pressKey(key: KeyCode, duration?: number): boolean;
  releaseKey(key: KeyCode): boolean;
  sendKeyCombo(keys: KeyCode[]): boolean;
  sendText(text: string): boolean;
  sendTextToWindow(hwnd: number, text: string): boolean;
  isKeyPressed(key: KeyCode): boolean;
  getPressedKeys(): KeyCode[];

  // Automation Helpers - Screenshot and Screen Capture
  captureScreen(): ScreenshotData;
  captureWindow(hwnd: number): ScreenshotData;
  captureRegion(
    x: number,
    y: number,
    width: number,
    height: number
  ): ScreenshotData;
  captureActiveWindow(): ScreenshotData;
  saveScreenshotToFile(screenshot: ScreenshotData, filename: string): boolean;

  // Automation Helpers - Window Management
  getAllWindows(): WindowInfo[];
  getWindowsByTitle(titlePattern: string): WindowInfo[];
  getWindowsByClassName(className: string): WindowInfo[];
  getActiveWindow(): WindowInfo;
  setActiveWindow(hwnd: number): boolean;
  showWindow(hwnd: number, showCommand?: number): boolean;
  moveWindow(
    hwnd: number,
    x: number,
    y: number,
    width: number,
    height: number
  ): boolean;
  closeWindow(hwnd: number): boolean;

  // Automation Helpers - Screen Information
  getScreenWidth(): number;
  getScreenHeight(): number;
  getScreenDPI(): number;
  getMonitorRects(): Array<{
    left: number;
    top: number;
    right: number;
    bottom: number;
  }>;

  // Automation Helpers - Color and Pixel Operations
  getPixelColor(x: number, y: number): number;
  getPixelColors(x: number, y: number, width: number, height: number): number[];
  findColorOnScreen(
    color: number,
    tolerance?: number
  ): Array<{ x: number; y: number }>;
  findColorInRegion(
    color: number,
    x: number,
    y: number,
    width: number,
    height: number,
    tolerance?: number
  ): Array<{ x: number; y: number }>;

  // Automation Helpers - Wait and Timing
  sleep(milliseconds: number): void;
  waitForWindow(titlePattern: string, timeoutMs?: number): boolean;
  waitForPixelColor(
    x: number,
    y: number,
    color: number,
    timeoutMs?: number,
    tolerance?: number
  ): boolean;
  waitForKeyPress(key: KeyCode, timeoutMs?: number): boolean;

  // Automation Helpers - System Information
  getSystemInfo(): string;
  getRunningWindowTitles(): string[];
  isScreenLocked(): boolean;
  isUserIdle(idleTimeMs?: number): boolean;
}
