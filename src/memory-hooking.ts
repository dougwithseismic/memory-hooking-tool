import {
  ProcessInfo,
  ModuleInfo,
  MemoryRegion,
  ProcessManagerInfo,
  Address,
  Size,
  DataBuffer,
  IMemoryHooking,
  MemoryProtection,
  MemoryState,
  MemoryType,
  MemoryHookingError,
  MemoryHookingErrorCode,
  SafeOperationResult,
  ScanResult,
  PointerResult,
  PatternMatch,
  NumericMatch,
  MemoryRegionEx,
  PatternSearchOptions,
  StringSearchOptions,
  NumericSearchOptions,
  PointerSearchOptions,
  RegionFilterOptions,
  MousePosition,
  ScreenshotData,
  WindowInfo,
  MouseButton,
  KeyCode,
} from "./types";

// Import the native addon
const {
  MemoryHooking: NativeMemoryHooking,
} = require("../build/Release/memory_hooking.node");

export class MemoryHooking implements IMemoryHooking {
  private nativeInstance: any;
  private lastError: MemoryHookingError | null = null;

  constructor() {
    this.nativeInstance = new NativeMemoryHooking();
  }

  private executeWithErrorHandling<T>(
    operation: () => SafeOperationResult<T>
  ): SafeOperationResult<T> {
    try {
      this.lastError = null;
      return operation();
    } catch (error) {
      const memError = new MemoryHookingError(
        error instanceof Error
          ? error.message
          : "Unknown error during operation",
        MemoryHookingErrorCode.UnknownError,
        undefined,
        "executeWithErrorHandling"
      );
      this.lastError = memError;
      return { success: false, error: memError };
    }
  }
  // Safe Memory Operations
  safeReadMemory(
    address: Address,
    size: Size
  ): SafeOperationResult<DataBuffer> {
    try {
      this.lastError = null;
      const data = this.readMemory(address, size);

      if (data === null) {
        const error = new MemoryHookingError(
          `Failed to read ${size} bytes from address ${address.toString(16)}`,
          MemoryHookingErrorCode.ReadMemoryFailed,
          undefined,
          "safeReadMemory"
        );
        this.lastError = error;
        return { success: false, error };
      }

      return { success: true, data };
    } catch (error) {
      const memError = new MemoryHookingError(
        error instanceof Error
          ? error.message
          : "Unknown error during memory read",
        MemoryHookingErrorCode.ReadMemoryFailed,
        undefined,
        "safeReadMemory"
      );
      this.lastError = memError;
      return { success: false, error: memError };
    }
  }

  safeWriteMemory(
    address: Address,
    data: DataBuffer
  ): SafeOperationResult<boolean> {
    try {
      this.lastError = null;
      const success = this.writeMemory(address, data);

      if (!success) {
        const error = new MemoryHookingError(
          `Failed to write ${data.length} bytes to address ${address.toString(16)}`,
          MemoryHookingErrorCode.WriteMemoryFailed,
          undefined,
          "safeWriteMemory"
        );
        this.lastError = error;
        return { success: false, error };
      }

      return { success: true, data: success };
    } catch (error) {
      const memError = new MemoryHookingError(
        error instanceof Error
          ? error.message
          : "Unknown error during memory write",
        MemoryHookingErrorCode.WriteMemoryFailed,
        undefined,
        "safeWriteMemory"
      );
      this.lastError = memError;
      return { success: false, error: memError };
    }
  }

  safeScanForPattern(
    pattern: string,
    mask: string
  ): SafeOperationResult<ScanResult> {
    try {
      this.lastError = null;
      const addresses = this.scanForPattern(pattern, mask);

      const result: ScanResult = {
        addresses,
        totalScanned: addresses.length,
        successfulReads: addresses.length,
        failedReads: 0,
        truncated: addresses.length >= 10000,
      };

      return { success: true, data: result };
    } catch (error) {
      const memError = new MemoryHookingError(
        error instanceof Error
          ? error.message
          : "Unknown error during pattern scan",
        MemoryHookingErrorCode.ScanningFailed,
        undefined,
        "safeScanForPattern"
      );
      this.lastError = memError;
      return { success: false, error: memError };
    }
  }

  safeScanForString(
    text: string,
    caseSensitive: boolean = true
  ): SafeOperationResult<ScanResult> {
    try {
      this.lastError = null;
      const addresses = this.scanForString(text, caseSensitive);

      const result: ScanResult = {
        addresses,
        totalScanned: addresses.length,
        successfulReads: addresses.length,
        failedReads: 0,
        truncated: addresses.length >= 10000,
      };

      return { success: true, data: result };
    } catch (error) {
      const memError = new MemoryHookingError(
        error instanceof Error
          ? error.message
          : "Unknown error during string scan",
        MemoryHookingErrorCode.ScanningFailed,
        undefined,
        "safeScanForString"
      );
      this.lastError = memError;
      return { success: false, error: memError };
    }
  }

  enableLogging(enabled: boolean): void {
    // This would be implemented in the native layer
    console.log(`Logging ${enabled ? "enabled" : "disabled"}`);
  }

  getLastError(): MemoryHookingError | null {
    return this.lastError;
  }

  // Process Management
  getRunningProcesses(): ProcessInfo[] {
    return this.nativeInstance.getRunningProcesses();
  }

  attachToProcess(processIdOrName: number | string): boolean {
    return this.nativeInstance.attachToProcess(processIdOrName);
  }

  detachFromProcess(): void {
    this.nativeInstance.detachFromProcess();
  }

  isProcessAttached(): boolean {
    return this.nativeInstance.isProcessAttached();
  }

  getCurrentProcessInfo(): ProcessManagerInfo {
    return this.nativeInstance.getCurrentProcessInfo();
  }

  // Memory Operations
  readMemory(address: Address, size: Size): DataBuffer | null {
    return this.nativeInstance.readMemory(address, size);
  }

  writeMemory(address: Address, data: DataBuffer): boolean {
    return this.nativeInstance.writeMemory(address, data);
  }

  getModuleInfo(moduleName: string): ModuleInfo | null {
    return this.nativeInstance.getModuleInfo(moduleName);
  }

  // Memory Scanning
  scanForPattern(pattern: string, mask: string): Address[] {
    return this.nativeInstance.scanForPattern(pattern, mask);
  }

  scanForBytes(bytes: number[]): Address[] {
    return this.nativeInstance.scanForBytes(bytes);
  }

  scanForString(text: string, caseSensitive: boolean = true): Address[] {
    return this.nativeInstance.scanForString(text, caseSensitive);
  }

  scanForValue(value: number | string): Address[] {
    return this.nativeInstance.scanForValue(value);
  }

  // Memory Region Analysis
  getMemoryRegions(): MemoryRegion[] {
    return this.nativeInstance.getMemoryRegions();
  }

  isValidAddress(address: Address): boolean {
    return this.nativeInstance.isValidAddress(address);
  }

  // Utility Methods
  bytesToHexString(bytes: number[]): string {
    return this.nativeInstance.bytesToHexString(bytes);
  }

  hexStringToBytes(hexString: string): number[] {
    return this.nativeInstance.hexStringToBytes(hexString);
  }

  // High-level utility methods
  async findProcessByName(processName: string): Promise<ProcessInfo | null> {
    const processes = this.getRunningProcesses();
    return (
      processes.find((p) =>
        p.processName.toLowerCase().includes(processName.toLowerCase())
      ) || null
    );
  }

  async findProcessesByPattern(pattern: string): Promise<ProcessInfo[]> {
    const processes = this.getRunningProcesses();
    const regex = new RegExp(pattern, "i");
    return processes.filter((p) => regex.test(p.processName));
  }

  readInt32(address: Address): number | null {
    const buffer = this.readMemory(address, 4);
    if (!buffer) return null;
    return buffer.readInt32LE(0);
  }

  readInt64(address: Address): bigint | null {
    const buffer = this.readMemory(address, 8);
    if (!buffer) return null;
    return buffer.readBigInt64LE(0);
  }

  readFloat(address: Address): number | null {
    const buffer = this.readMemory(address, 4);
    if (!buffer) return null;
    return buffer.readFloatLE(0);
  }

  readDouble(address: Address): number | null {
    const buffer = this.readMemory(address, 8);
    if (!buffer) return null;
    return buffer.readDoubleLE(0);
  }

  readString(address: Address, length: number): string | null {
    const buffer = this.readMemory(address, length);
    if (!buffer) return null;
    return buffer.toString("utf8").replace(/\0.*$/g, ""); // Remove null terminators
  }

  readWideString(address: Address, length: number): string | null {
    const buffer = this.readMemory(address, length * 2);
    if (!buffer) return null;
    return buffer.toString("utf16le").replace(/\0.*$/g, ""); // Remove null terminators
  }

  writeInt32(address: Address, value: number): boolean {
    const buffer = Buffer.allocUnsafe(4);
    buffer.writeInt32LE(value, 0);
    return this.writeMemory(address, buffer);
  }

  writeInt64(address: Address, value: bigint): boolean {
    const buffer = Buffer.allocUnsafe(8);
    buffer.writeBigInt64LE(value, 0);
    return this.writeMemory(address, buffer);
  }

  writeFloat(address: Address, value: number): boolean {
    const buffer = Buffer.allocUnsafe(4);
    buffer.writeFloatLE(value, 0);
    return this.writeMemory(address, buffer);
  }

  writeDouble(address: Address, value: number): boolean {
    const buffer = Buffer.allocUnsafe(8);
    buffer.writeDoubleLE(value, 0);
    return this.writeMemory(address, buffer);
  }

  writeString(address: Address, value: string): boolean {
    const buffer = Buffer.from(value + "\0", "utf8");
    return this.writeMemory(address, buffer);
  }

  writeWideString(address: Address, value: string): boolean {
    const buffer = Buffer.from(value + "\0", "utf16le");
    return this.writeMemory(address, buffer);
  }

  // Pattern scanning helpers
  createPattern(
    bytes: number[],
    wildcards: boolean[] = []
  ): { pattern: string; mask: string } {
    let pattern = "";
    let mask = "";

    for (let i = 0; i < bytes.length; i++) {
      if (wildcards[i]) {
        pattern += "?? ";
        mask += "?";
      } else {
        pattern += bytes[i].toString(16).padStart(2, "0").toUpperCase() + " ";
        mask += "x";
      }
    }

    return {
      pattern: pattern.trim(),
      mask: mask,
    };
  }

  // Memory region filtering
  getExecutableRegions(): MemoryRegion[] {
    return this.getMemoryRegions().filter(
      (region) =>
        (region.protection &
          (MemoryProtection.PAGE_EXECUTE |
            MemoryProtection.PAGE_EXECUTE_READ |
            MemoryProtection.PAGE_EXECUTE_READWRITE |
            MemoryProtection.PAGE_EXECUTE_WRITECOPY)) !==
        0
    );
  }

  getWritableRegions(): MemoryRegion[] {
    return this.getMemoryRegions().filter(
      (region) =>
        (region.protection &
          (MemoryProtection.PAGE_READWRITE |
            MemoryProtection.PAGE_EXECUTE_READWRITE |
            MemoryProtection.PAGE_WRITECOPY |
            MemoryProtection.PAGE_EXECUTE_WRITECOPY)) !==
        0
    );
  }

  getCommittedRegions(): MemoryRegion[] {
    return this.getMemoryRegions().filter(
      (region) => region.state === MemoryState.MEM_COMMIT
    );
  }

  // Advanced scanning with constraints
  scanInRegion(
    startAddress: Address,
    endAddress: Address,
    scanFunction: () => Address[]
  ): Address[] {
    const allResults = scanFunction();
    return allResults.filter(
      (addr) => addr >= startAddress && addr <= endAddress
    );
  }

  scanInModule(moduleName: string, scanFunction: () => Address[]): Address[] {
    const moduleInfo = this.getModuleInfo(moduleName);
    if (!moduleInfo) return [];

    const startAddress = moduleInfo.baseAddress;
    const endAddress = moduleInfo.baseAddress + moduleInfo.size;

    return this.scanInRegion(startAddress, endAddress, scanFunction);
  }

  // Chaining/filtering results
  filterResults(
    addresses: Address[],
    filterFunction: (addr: Address) => boolean
  ): Address[] {
    return addresses.filter(filterFunction);
  }

  limitResults(addresses: Address[], maxResults: number): Address[] {
    return addresses.slice(0, maxResults);
  }

  // Memory Helpers - Pointer Operations
  findPointersTo = (
    targetAddress: number,
    options: Partial<PatternSearchOptions> = {}
  ): SafeOperationResult<PointerResult[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.findPointersTo(
        targetAddress,
        options.searchStart,
        options.searchEnd
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  findPointerChain = (
    finalAddress: number,
    options: Partial<PointerSearchOptions> = {}
  ): SafeOperationResult<PointerResult[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.findPointerChain(
        finalAddress,
        options.maxDepth,
        options.offsets
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  findNullPointers = (
    searchStart: number,
    searchEnd: number
  ): SafeOperationResult<number[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.findNullPointers(
        searchStart,
        searchEnd
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: searchEnd - searchStart,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  // Memory Helpers - Pattern Searching
  searchBytePattern = (
    pattern: number[],
    mask: boolean[],
    options: Partial<PatternSearchOptions> = {}
  ): SafeOperationResult<PatternMatch[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.searchBytePattern(
        pattern,
        mask,
        options.searchStart,
        options.searchEnd
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  searchStringPattern = (
    pattern: string,
    options: Partial<StringSearchOptions> = {}
  ): SafeOperationResult<PatternMatch[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.searchStringPattern(
        pattern,
        options.caseSensitive ?? true,
        options.unicode ?? false,
        options.searchStart,
        options.searchEnd
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  searchRegexPattern = (
    regexPattern: string,
    options: Partial<PatternSearchOptions> = {}
  ): SafeOperationResult<PatternMatch[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.searchRegexPattern(
        regexPattern,
        options.searchStart,
        options.searchEnd
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  // Memory Helpers - Numeric Searching
  searchInt32 = (
    value: number,
    options: Partial<NumericSearchOptions> = {}
  ): SafeOperationResult<NumericMatch[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.searchInt32(
        value,
        options.searchStart,
        options.searchEnd,
        options.aligned ?? false
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  searchInt64 = (
    value: number,
    options: Partial<NumericSearchOptions> = {}
  ): SafeOperationResult<NumericMatch[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.searchInt64(
        value,
        options.searchStart,
        options.searchEnd,
        options.aligned ?? false
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  searchFloat = (
    value: number,
    options: Partial<NumericSearchOptions> = {}
  ): SafeOperationResult<NumericMatch[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.searchFloat(
        value,
        options.tolerance ?? 0.001,
        options.searchStart,
        options.searchEnd
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  searchDouble = (
    value: number,
    options: Partial<NumericSearchOptions> = {}
  ): SafeOperationResult<NumericMatch[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.searchDouble(
        value,
        options.tolerance ?? 0.001,
        options.searchStart,
        options.searchEnd
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  // Memory Helpers - Region Analysis
  getMemoryRegionsEx = (
    options: Partial<RegionFilterOptions> = {}
  ): SafeOperationResult<MemoryRegionEx[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.getMemoryRegionsEx(
        options.executableOnly ?? false,
        options.writableOnly ?? false
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  getModuleRegions = (
    moduleName?: string
  ): SafeOperationResult<MemoryRegionEx[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.getModuleRegions(moduleName);
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  getRegionInfo = (address: number): SafeOperationResult<MemoryRegionEx> => {
    return this.executeWithErrorHandling(() => {
      const result = this.nativeInstance.getRegionInfo(address);
      return {
        success: true,
        data: result,
        scanStats: {
          totalScanned: 1,
          matchesFound: 1,
          timeElapsed: 0,
        },
      };
    });
  };

  // Memory Helpers - Advanced Scanning
  scanForCode = (
    opcodes: number[],
    options: Partial<PatternSearchOptions> = {}
  ): SafeOperationResult<PatternMatch[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.scanForCode(
        opcodes,
        options.searchStart,
        options.searchEnd
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  findFunctionPrologs = (
    options: Partial<PatternSearchOptions> = {}
  ): SafeOperationResult<number[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.findFunctionPrologs(
        options.searchStart,
        options.searchEnd
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  findCallInstructions = (
    targetAddress: number,
    options: Partial<PatternSearchOptions> = {}
  ): SafeOperationResult<number[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.findCallInstructions(
        targetAddress,
        options.searchStart,
        options.searchEnd
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: 0,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  // Memory Helpers - Comparison and Diffing
  compareMemoryRegions = (
    region1Start: number,
    region2Start: number,
    size: number
  ): SafeOperationResult<number[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.compareMemoryRegions(
        region1Start,
        region2Start,
        size
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: size,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  findChangedBytes = (
    address: number,
    originalData: number[]
  ): SafeOperationResult<number[]> => {
    return this.executeWithErrorHandling(() => {
      const results = this.nativeInstance.findChangedBytes(
        address,
        originalData
      );
      return {
        success: true,
        data: results,
        scanStats: {
          totalScanned: originalData.length,
          matchesFound: results.length,
          timeElapsed: 0,
        },
      };
    });
  };

  // Memory Helpers - Utility Functions
  parseBytePattern = (pattern: string): SafeOperationResult<number[]> => {
    return this.executeWithErrorHandling(() => {
      const result = this.nativeInstance.parseBytePattern(pattern);
      return {
        success: true,
        data: result,
        scanStats: {
          totalScanned: 0,
          matchesFound: result.length,
          timeElapsed: 0,
        },
      };
    });
  };

  parsePatternMask = (pattern: string): SafeOperationResult<boolean[]> => {
    return this.executeWithErrorHandling(() => {
      const result = this.nativeInstance.parsePatternMask(pattern);
      return {
        success: true,
        data: result,
        scanStats: {
          totalScanned: 0,
          matchesFound: result.length,
          timeElapsed: 0,
        },
      };
    });
  };

  formatAddress = (address: number, withPrefix: boolean = true): string => {
    return this.nativeInstance.formatAddress(address, withPrefix);
  };

  isValidCodeAddress = (address: number): SafeOperationResult<boolean> => {
    return this.executeWithErrorHandling(() => {
      const result = this.nativeInstance.isValidCodeAddress(address);
      return {
        success: true,
        data: result,
        scanStats: {
          totalScanned: 1,
          matchesFound: result ? 1 : 0,
          timeElapsed: 0,
        },
      };
    });
  };

  getAlignment = (address: number): number => {
    return this.nativeInstance.getAlignment(address);
  };

  // Convenience methods that combine multiple helpers
  searchForValue = (
    value: any,
    options: Partial<NumericSearchOptions> = {}
  ): SafeOperationResult<NumericMatch[]> => {
    const valueType = typeof value;

    if (valueType === "number") {
      if (Number.isInteger(value)) {
        // Check if it fits in int32 range
        if (value >= -2147483648 && value <= 2147483647) {
          return this.searchInt32(value, options);
        } else {
          return this.searchInt64(value, options);
        }
      } else {
        // Float/Double - use double for better precision
        return this.searchDouble(value, options);
      }
    }

    // For non-numeric values, return empty result
    return {
      success: false,
      error: new MemoryHookingError(
        `Unsupported value type: ${valueType}`,
        MemoryHookingErrorCode.InvalidSize
      ),
      data: [],
    };
  };

  searchBytePatternFromString = (
    patternString: string,
    options: Partial<PatternSearchOptions> = {}
  ): SafeOperationResult<PatternMatch[]> => {
    const patternResult = this.parseBytePattern(patternString);
    if (!patternResult.success) {
      return {
        success: false,
        error: patternResult.error,
      };
    }

    const maskResult = this.parsePatternMask(patternString);
    if (!maskResult.success) {
      return {
        success: false,
        error: maskResult.error,
      };
    }

    return this.searchBytePattern(
      patternResult.data!,
      maskResult.data!,
      options
    );
  };

  // Automation Helpers - Mouse Control
  moveMouse(x: number, y: number): boolean {
    return this.nativeInstance.moveMouse(x, y);
  }

  clickMouse(button: MouseButton, x?: number, y?: number): boolean {
    return this.nativeInstance.clickMouse(Number(button), x, y);
  }

  doubleClickMouse(button: MouseButton, x?: number, y?: number): boolean {
    return this.nativeInstance.doubleClickMouse(Number(button), x, y);
  }

  dragMouse(
    fromX: number,
    fromY: number,
    toX: number,
    toY: number,
    button?: MouseButton
  ): boolean {
    return this.nativeInstance.dragMouse(
      fromX,
      fromY,
      toX,
      toY,
      button ? Number(button) : 1
    );
  }

  scrollMouse(x: number, y: number, scrollAmount: number): boolean {
    return this.nativeInstance.scrollMouse(x, y, scrollAmount);
  }

  getMousePosition(): MousePosition {
    return this.nativeInstance.getMousePosition();
  }

  isMouseButtonPressed(button: MouseButton): boolean {
    return this.nativeInstance.isMouseButtonPressed(Number(button));
  }

  // Automation Helpers - Keyboard Control
  pressKey(key: KeyCode, duration?: number): boolean {
    if (duration !== undefined) {
      return this.nativeInstance.pressKey(Number(key), duration);
    } else {
      return this.nativeInstance.pressKey(Number(key));
    }
  }

  releaseKey(key: KeyCode): boolean {
    return this.nativeInstance.releaseKey(Number(key));
  }

  sendKeyCombo(keys: KeyCode[]): boolean {
    return this.nativeInstance.sendKeyCombo(keys.map((k) => Number(k)));
  }

  sendText(text: string): boolean {
    return this.nativeInstance.sendText(text);
  }

  sendTextToWindow(hwnd: number, text: string): boolean {
    return this.nativeInstance.sendTextToWindow(hwnd, text);
  }

  isKeyPressed(key: KeyCode): boolean {
    return this.nativeInstance.isKeyPressed(Number(key));
  }

  getPressedKeys(): KeyCode[] {
    return this.nativeInstance.getPressedKeys();
  }

  // Automation Helpers - Screenshot and Screen Capture
  captureScreen(): ScreenshotData {
    return this.nativeInstance.captureScreen();
  }

  captureWindow(hwnd: number): ScreenshotData {
    return this.nativeInstance.captureWindow(hwnd);
  }

  captureRegion(
    x: number,
    y: number,
    width: number,
    height: number
  ): ScreenshotData {
    return this.nativeInstance.captureRegion(x, y, width, height);
  }

  captureActiveWindow(): ScreenshotData {
    return this.nativeInstance.captureActiveWindow();
  }

  saveScreenshotToFile(screenshot: ScreenshotData, filename: string): boolean {
    return this.nativeInstance.saveScreenshotToFile(screenshot, filename);
  }

  // Automation Helpers - Window Management
  getAllWindows(): WindowInfo[] {
    return this.nativeInstance.getAllWindows();
  }

  getWindowsByTitle(titlePattern: string): WindowInfo[] {
    return this.nativeInstance.getWindowsByTitle(titlePattern);
  }

  getWindowsByClassName(className: string): WindowInfo[] {
    return this.nativeInstance.getWindowsByClassName(className);
  }

  getActiveWindow(): WindowInfo {
    return this.nativeInstance.getActiveWindow();
  }

  setActiveWindow(hwnd: number): boolean {
    return this.nativeInstance.setActiveWindow(hwnd);
  }

  showWindow(hwnd: number, showCommand?: number): boolean {
    return this.nativeInstance.showWindow(hwnd, showCommand);
  }

  moveWindow(
    hwnd: number,
    x: number,
    y: number,
    width: number,
    height: number
  ): boolean {
    return this.nativeInstance.moveWindow(hwnd, x, y, width, height);
  }

  closeWindow(hwnd: number): boolean {
    return this.nativeInstance.closeWindow(hwnd);
  }

  // Automation Helpers - Screen Information
  getScreenWidth(): number {
    return this.nativeInstance.getScreenWidth();
  }

  getScreenHeight(): number {
    return this.nativeInstance.getScreenHeight();
  }

  getScreenDPI(): number {
    return this.nativeInstance.getScreenDPI();
  }

  getMonitorRects(): Array<{
    left: number;
    top: number;
    right: number;
    bottom: number;
  }> {
    return this.nativeInstance.getMonitorRects();
  }

  // Automation Helpers - Color and Pixel Operations
  getPixelColor(x: number, y: number): number {
    return this.nativeInstance.getPixelColor(x, y);
  }

  getPixelColors(
    x: number,
    y: number,
    width: number,
    height: number
  ): number[] {
    return this.nativeInstance.getPixelColors(x, y, width, height);
  }

  findColorOnScreen(
    color: number,
    tolerance?: number
  ): Array<{ x: number; y: number }> {
    return this.nativeInstance.findColorOnScreen(color, tolerance);
  }

  findColorInRegion(
    color: number,
    x: number,
    y: number,
    width: number,
    height: number,
    tolerance?: number
  ): Array<{ x: number; y: number }> {
    return this.nativeInstance.findColorInRegion(
      color,
      x,
      y,
      width,
      height,
      tolerance
    );
  }

  // Automation Helpers - Wait and Timing
  sleep(milliseconds: number): void {
    return this.nativeInstance.sleep(milliseconds);
  }

  waitForWindow(titlePattern: string, timeoutMs?: number): boolean {
    return this.nativeInstance.waitForWindow(titlePattern, timeoutMs);
  }

  waitForPixelColor(
    x: number,
    y: number,
    color: number,
    timeoutMs?: number,
    tolerance?: number
  ): boolean {
    return this.nativeInstance.waitForPixelColor(
      x,
      y,
      color,
      timeoutMs,
      tolerance
    );
  }

  waitForKeyPress(key: KeyCode, timeoutMs?: number): boolean {
    return this.nativeInstance.waitForKeyPress(Number(key), timeoutMs);
  }

  // Automation Helpers - System Information
  getSystemInfo(): string {
    return this.nativeInstance.getSystemInfo();
  }

  getRunningWindowTitles(): string[] {
    return this.nativeInstance.getRunningWindowTitles();
  }

  isScreenLocked(): boolean {
    return this.nativeInstance.isScreenLocked();
  }

  isUserIdle(idleTimeMs?: number): boolean {
    return this.nativeInstance.isUserIdle(idleTimeMs);
  }
}

export default MemoryHooking;

// Export types for external use
export * from "./types";
