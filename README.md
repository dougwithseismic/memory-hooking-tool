# Memory Hooking Tool - C++ with TypeScript Scripting

A powerful C++ memory hooking and manipulation tool with a TypeScript scripting interface for reverse engineering, game hacking, and process analysis.

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

## API Reference

### Core Classes

#### `MemoryHooking`

Main class providing all memory manipulation functionality.

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
