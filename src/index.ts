import MemoryHooking from "./memory-hooking";
import {
  ProcessInfo,
  ModuleInfo,
  MemoryRegion,
  ProcessManagerInfo,
  MemoryProtection,
  MemoryState,
  MemoryType,
  Address,
  Size,
  DataBuffer,
  MemoryHookingError,
  MemoryHookingErrorCode,
  MousePosition,
  ScreenshotData,
  WindowInfo,
  MouseButton,
  KeyCode,
} from "./types";

// Main export
export default MemoryHooking;
export { MemoryHooking };

// Export all types
export {
  ProcessInfo,
  ModuleInfo,
  MemoryRegion,
  ProcessManagerInfo,
  MemoryProtection,
  MemoryState,
  MemoryType,
  Address,
  Size,
  DataBuffer,
  MemoryHookingError,
  MemoryHookingErrorCode,
  MousePosition,
  ScreenshotData,
  WindowInfo,
  MouseButton,
  KeyCode,
};

// Factory function for easy instantiation
export const createMemoryHooking = () => new MemoryHooking();

// Utility functions
export const utils = {
  // Address conversion utilities
  toHex: (address: Address): string =>
    `0x${address.toString(16).toUpperCase()}`,
  fromHex: (hexString: string): Address =>
    parseInt(hexString.replace("0x", ""), 16),

  // Memory protection helpers
  isReadable: (protection: number): boolean =>
    (protection &
      (MemoryProtection.PAGE_READONLY |
        MemoryProtection.PAGE_READWRITE |
        MemoryProtection.PAGE_EXECUTE_READ |
        MemoryProtection.PAGE_EXECUTE_READWRITE)) !==
    0,

  isWritable: (protection: number): boolean =>
    (protection &
      (MemoryProtection.PAGE_READWRITE |
        MemoryProtection.PAGE_EXECUTE_READWRITE |
        MemoryProtection.PAGE_WRITECOPY |
        MemoryProtection.PAGE_EXECUTE_WRITECOPY)) !==
    0,

  isExecutable: (protection: number): boolean =>
    (protection &
      (MemoryProtection.PAGE_EXECUTE |
        MemoryProtection.PAGE_EXECUTE_READ |
        MemoryProtection.PAGE_EXECUTE_READWRITE |
        MemoryProtection.PAGE_EXECUTE_WRITECOPY)) !==
    0,

  // Pattern creation helpers
  createBytePattern: (bytes: number[]): string =>
    bytes.map((b) => b.toString(16).padStart(2, "0").toUpperCase()).join(" "),

  createMask: (pattern: string): string =>
    pattern
      .split(" ")
      .map((byte) => (byte === "??" ? "?" : "x"))
      .join(""),

  // String to bytes conversion
  stringToBytes: (str: string): number[] =>
    Array.from(Buffer.from(str, "utf8")),

  wideStringToBytes: (str: string): number[] =>
    Array.from(Buffer.from(str, "utf16le")),

  // Data type size constants
  sizeof: {
    byte: 1,
    int16: 2,
    int32: 4,
    int64: 8,
    float: 4,
    double: 8,
    pointer: process.arch === "x64" ? 8 : 4,
  },
};

// Pre-configured instances for common use cases
export const processes = {
  // Find and attach to a process by name
  attachByName: async (processName: string): Promise<MemoryHooking | null> => {
    const memHook = new MemoryHooking();
    const process = await memHook.findProcessByName(processName);

    if (process && memHook.attachToProcess(process.processId)) {
      return memHook;
    }

    return null;
  },

  // List all processes
  list: (): ProcessInfo[] => {
    const memHook = new MemoryHooking();
    return memHook.getRunningProcesses();
  },

  // Find processes by pattern
  findByPattern: async (pattern: string): Promise<ProcessInfo[]> => {
    const memHook = new MemoryHooking();
    return memHook.findProcessesByPattern(pattern);
  },
};

// Example usage and quick start functions
export const examples = {
  // Basic process enumeration
  listProcesses: () => {
    const memHook = new MemoryHooking();
    const processes = memHook.getRunningProcesses();

    console.log("\n=== Running Processes ===");
    processes.forEach((proc) => {
      console.log(
        `PID: ${proc.processId.toString().padStart(6)} | Name: ${proc.processName}`
      );
    });

    return processes;
  },

  // Basic memory reading example
  readProcessMemory: async (
    processName: string,
    address: Address,
    size: number
  ) => {
    const memHook = await processes.attachByName(processName);

    if (!memHook) {
      console.log(`Failed to attach to process: ${processName}`);
      return null;
    }

    console.log(`Attached to ${processName}`);
    const data = memHook.readMemory(address, size);

    if (data) {
      console.log(`Read ${size} bytes from ${utils.toHex(address)}:`);
      console.log(data.toString("hex"));
    } else {
      console.log(`Failed to read memory at ${utils.toHex(address)}`);
    }

    memHook.detachFromProcess();
    return data;
  },
};

// Export minesweeper automation
export {
  MinesweeperAutomation,
  minesweeperDemo,
} from "./examples/minesweeper-automation";
export { runMinesweeperTest } from "./examples/test-minesweeper";

// Console helper for interactive use
if (require.main === module) {
  console.log("Memory Hooking Tool - TypeScript Interface");
  console.log("=========================================");
  console.log("");
  console.log("Available exports:");
  console.log("- MemoryHooking: Main class");
  console.log("- createMemoryHooking(): Factory function");
  console.log("- utils: Utility functions");
  console.log("- processes: Process management helpers");
  console.log("- examples: Example usage functions");
  console.log("- MinesweeperAutomation: Minesweeper bot class");
  console.log("- minesweeperDemo(): Run minesweeper automation demo");
  console.log("- runMinesweeperTest(): Quick minesweeper test");
  console.log("");
  console.log("Quick start:");
  console.log("const memHook = createMemoryHooking();");
  console.log("memHook.getRunningProcesses();");
  console.log("");
  console.log("Minesweeper automation:");
  console.log("import { minesweeperDemo } from '@experiments/demo';");
  console.log("await minesweeperDemo();");
  console.log("");

  // Run a basic example
  examples.listProcesses();
}
