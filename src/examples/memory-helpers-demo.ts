import { createMemoryHooking, utils, MemoryHookingError } from "../index";

/**
 * Comprehensive demo showcasing all memory helper functions
 *
 * This demo demonstrates:
 * - Pointer operations (finding pointers, pointer chains, null pointers)
 * - Enhanced pattern searching (bytes, strings, regex)
 * - Numeric value searching (int32, int64, float, double)
 * - Memory region analysis (extended regions, module regions)
 * - Advanced scanning (code scanning, function prologs, call instructions)
 * - Memory comparison and diffing
 * - Utility functions for pattern parsing and formatting
 */

async function memoryHelpersDemo() {
  console.log("=== Memory Helpers Comprehensive Demo ===\n");

  const memHook = createMemoryHooking();

  try {
    // Find notepad or use current process as fallback
    const notepadProcess = await memHook.findProcessByName("notepad.exe");
    const targetProcess = notepadProcess || {
      processId: process.pid,
      processName: "node.exe",
    };

    console.log(
      `🎯 Target process: ${targetProcess.processName} (PID: ${targetProcess.processId})`
    );

    if (memHook.attachToProcess(targetProcess.processId)) {
      console.log("✓ Successfully attached to process\n");

      // Get some basic module info for testing
      const moduleInfo = memHook.getModuleInfo(targetProcess.processName);
      if (!moduleInfo) {
        console.log(
          "⚠️  Could not get module info, some tests may be limited\n"
        );
      } else {
        console.log(
          `📦 Module info: Base=${utils.toHex(moduleInfo.baseAddress)}, Size=${moduleInfo.size}\n`
        );
      }

      // === POINTER OPERATIONS ===
      await demonstratePointerOperations(memHook, moduleInfo);

      // === PATTERN SEARCHING ===
      await demonstratePatternSearching(memHook, moduleInfo);

      // === NUMERIC SEARCHING ===
      await demonstrateNumericSearching(memHook);

      // === REGION ANALYSIS ===
      await demonstrateRegionAnalysis(memHook);

      // === ADVANCED SCANNING ===
      await demonstrateAdvancedScanning(memHook, moduleInfo);

      // === MEMORY COMPARISON & DIFFING ===
      await demonstrateMemoryComparison(memHook, moduleInfo);

      // === UTILITY FUNCTIONS ===
      await demonstrateUtilityFunctions(memHook);

      memHook.detachFromProcess();
      console.log("\n✓ Successfully detached from process");
    } else {
      console.log("❌ Failed to attach to process");
    }
  } catch (error) {
    console.error("Demo failed:", error);
    if (MemoryHookingError.isMemoryHookingError(error)) {
      console.error(`  Error details: ${error.getDetailedMessage()}`);
    } else if (error instanceof Error) {
      console.error(`  Standard error: ${error.message}`);
    }
  }

  console.log("\n=== Memory Helpers Demo Completed ===");
}

// === POINTER OPERATIONS DEMO ===
async function demonstratePointerOperations(memHook: any, moduleInfo: any) {
  console.log("🔍 POINTER OPERATIONS");
  console.log("=====================");

  if (!moduleInfo) {
    console.log("⚠️  Skipping pointer operations - no module info available\n");
    return;
  }

  try {
    // 1. Find pointers to module base address
    console.log("1. Finding pointers to module base address...");
    const pointersResult = memHook.findPointersTo(moduleInfo.baseAddress);

    if (pointersResult.success && pointersResult.data) {
      console.log(`   ✓ Found ${pointersResult.data.length} pointers`);
      if (pointersResult.data.length > 0) {
        pointersResult.data.slice(0, 3).forEach((ptr: any, i: number) => {
          console.log(
            `     ${i + 1}. ${utils.toHex(ptr.address)} → ${utils.toHex(ptr.pointsTo)}`
          );
        });
      }
    } else {
      console.log(`   ❌ Failed: ${pointersResult.error?.message}`);
    }

    // 2. Find pointer chains
    console.log("\n2. Finding pointer chains...");
    const chainsResult = memHook.findPointerChain(moduleInfo.baseAddress, {
      maxDepth: 2,
    });

    if (chainsResult.success && chainsResult.data) {
      console.log(`   ✓ Found ${chainsResult.data.length} pointer chains`);
      if (chainsResult.data.length > 0) {
        chainsResult.data.slice(0, 2).forEach((chain: any, i: number) => {
          console.log(
            `     Chain ${i + 1}: ${utils.toHex(chain.address)} → ${utils.toHex(chain.pointsTo)} (offset: ${chain.offset})`
          );
        });
      }
    } else {
      console.log(`   ❌ Failed: ${chainsResult.error?.message}`);
    }

    // 3. Find null pointers in a small range
    console.log("\n3. Finding null pointers...");
    const startAddr = moduleInfo.baseAddress;
    const endAddr = startAddr + 0x1000; // Search in first 4KB
    const nullPointersResult = memHook.findNullPointers(startAddr, endAddr);

    if (nullPointersResult.success && nullPointersResult.data) {
      console.log(
        `   ✓ Found ${nullPointersResult.data.length} null pointers in range`
      );
      if (nullPointersResult.data.length > 0) {
        nullPointersResult.data
          .slice(0, 3)
          .forEach((addr: number, i: number) => {
            console.log(`     ${i + 1}. ${utils.toHex(addr)}`);
          });
      }
    } else {
      console.log(`   ❌ Failed: ${nullPointersResult.error?.message}`);
    }
  } catch (error) {
    console.log(
      `   ❌ Pointer operations error: ${error instanceof Error ? error.message : String(error)}`
    );
  }

  console.log();
}

// === PATTERN SEARCHING DEMO ===
async function demonstratePatternSearching(memHook: any, moduleInfo: any) {
  console.log("🔎 ENHANCED PATTERN SEARCHING");
  console.log("=============================");

  try {
    // 1. Byte pattern search - look for MZ header
    console.log("1. Searching for MZ header (DOS signature)...");
    const mzPattern = [0x4d, 0x5a]; // "MZ"
    const mzMask = [true, true];
    const bytePatternResult = memHook.searchBytePattern(mzPattern, mzMask);

    if (bytePatternResult.success && bytePatternResult.data) {
      console.log(`   ✓ Found ${bytePatternResult.data.length} MZ headers`);
      if (bytePatternResult.data.length > 0) {
        bytePatternResult.data.slice(0, 3).forEach((match: any, i: number) => {
          console.log(
            `     ${i + 1}. ${utils.toHex(match.address)} (${match.size} bytes)`
          );
        });
      }
    } else {
      console.log(`   ❌ Failed: ${bytePatternResult.error?.message}`);
    }

    // 2. String pattern search
    console.log("\n2. Searching for common strings...");
    const strings = ["kernel32", "ntdll", "user32"];

    for (const searchString of strings) {
      const stringResult = memHook.searchStringPattern(searchString, {
        caseSensitive: false,
      });

      if (stringResult.success && stringResult.data) {
        console.log(
          `   ✓ "${searchString}": ${stringResult.data.length} matches`
        );
        if (stringResult.data.length > 0) {
          console.log(
            `     First match at: ${utils.toHex(stringResult.data[0].address)}`
          );
        }
      } else {
        console.log(`   ❌ "${searchString}": ${stringResult.error?.message}`);
      }
    }

    // 3. Regex pattern search
    console.log("\n3. Searching with regex patterns...");
    try {
      const regexResult = memHook.searchRegexPattern("kernel\\d+");

      if (regexResult.success && regexResult.data) {
        console.log(
          `   ✓ Regex "kernel\\d+": ${regexResult.data.length} matches`
        );
        if (regexResult.data.length > 0) {
          console.log(
            `     First match at: ${utils.toHex(regexResult.data[0].address)}`
          );
        }
      } else {
        console.log(`   ❌ Regex search failed: ${regexResult.error?.message}`);
      }
    } catch (error) {
      console.log(
        `   ❌ Regex search error: ${error instanceof Error ? error.message : String(error)}`
      );
    }
  } catch (error) {
    console.log(
      `   ❌ Pattern searching error: ${error instanceof Error ? error.message : String(error)}`
    );
  }

  console.log();
}

// === NUMERIC SEARCHING DEMO ===
async function demonstrateNumericSearching(memHook: any) {
  console.log("🔢 NUMERIC VALUE SEARCHING");
  console.log("==========================");

  try {
    // 1. Search for common integer values
    console.log("1. Searching for integer values...");
    const intValues = [0, 1, -1, 1000, process.pid];

    for (const value of intValues) {
      const int32Result = memHook.searchInt32(value);

      if (int32Result.success && int32Result.data) {
        console.log(`   ✓ Int32 ${value}: ${int32Result.data.length} matches`);
        if (int32Result.data.length > 0 && int32Result.data.length <= 5) {
          int32Result.data.forEach((match: any, i: number) => {
            console.log(`     ${i + 1}. ${utils.toHex(match.address)}`);
          });
        }
      } else {
        console.log(`   ❌ Int32 ${value}: ${int32Result.error?.message}`);
      }
    }

    // 2. Search for large numbers (int64)
    console.log("\n2. Searching for 64-bit values...");
    const largeValue = Date.now(); // Current timestamp
    const int64Result = memHook.searchInt64(largeValue);

    if (int64Result.success && int64Result.data) {
      console.log(
        `   ✓ Int64 ${largeValue}: ${int64Result.data.length} matches`
      );
    } else {
      console.log(`   ❌ Int64 search failed: ${int64Result.error?.message}`);
    }

    // 3. Search for floating point values
    console.log("\n3. Searching for floating point values...");
    const floatValues = [0.0, 1.0, 3.14159];

    for (const value of floatValues) {
      const floatResult = memHook.searchFloat(value, { tolerance: 0.001 });

      if (floatResult.success && floatResult.data) {
        console.log(`   ✓ Float ${value}: ${floatResult.data.length} matches`);
      } else {
        console.log(`   ❌ Float ${value}: ${floatResult.error?.message}`);
      }
    }

    // 4. Search for double precision values
    console.log("\n4. Searching for double precision values...");
    const doubleValue = Math.PI;
    const doubleResult = memHook.searchDouble(doubleValue, {
      tolerance: 0.0001,
    });

    if (doubleResult.success && doubleResult.data) {
      console.log(
        `   ✓ Double ${doubleValue}: ${doubleResult.data.length} matches`
      );
    } else {
      console.log(`   ❌ Double search failed: ${doubleResult.error?.message}`);
    }
  } catch (error) {
    console.log(
      `   ❌ Numeric searching error: ${error instanceof Error ? error.message : String(error)}`
    );
  }

  console.log();
}

// === REGION ANALYSIS DEMO ===
async function demonstrateRegionAnalysis(memHook: any) {
  console.log("🗺️  MEMORY REGION ANALYSIS");
  console.log("=========================");

  try {
    // 1. Get extended memory regions
    console.log("1. Analyzing memory regions...");
    const regionsResult = memHook.getMemoryRegionsEx();

    if (regionsResult.success && regionsResult.data) {
      const regions = regionsResult.data;
      console.log(`   ✓ Found ${regions.length} memory regions`);

      // Categorize regions
      const executable = regions.filter((r: any) => r.protection & 0x10);
      const writable = regions.filter((r: any) => r.protection & 0x04);
      const readable = regions.filter((r: any) => r.protection & 0x02);

      console.log(`     - Executable: ${executable.length}`);
      console.log(`     - Writable: ${writable.length}`);
      console.log(`     - Readable: ${readable.length}`);

      // Show a few interesting regions
      console.log("     Top 3 largest regions:");
      regions
        .sort((a: any, b: any) => b.size - a.size)
        .slice(0, 3)
        .forEach((region: any, i: number) => {
          console.log(
            `       ${i + 1}. ${utils.toHex(region.startAddress)}-${utils.toHex(region.endAddress)} (${(region.size / 1024 / 1024).toFixed(2)} MB)`
          );
        });
    } else {
      console.log(`   ❌ Failed: ${regionsResult.error?.message}`);
    }

    // 2. Get module-specific regions
    console.log("\n2. Analyzing module regions...");
    const moduleRegionsResult = memHook.getModuleRegions("ntdll.dll");

    if (moduleRegionsResult.success && moduleRegionsResult.data) {
      console.log(
        `   ✓ ntdll.dll has ${moduleRegionsResult.data.length} regions`
      );
      if (moduleRegionsResult.data.length > 0) {
        moduleRegionsResult.data
          .slice(0, 3)
          .forEach((region: any, i: number) => {
            console.log(
              `     ${i + 1}. ${utils.toHex(region.startAddress)} (${region.size} bytes, prot: 0x${region.protection.toString(16)})`
            );
          });
      }
    } else {
      console.log(`   ❌ Failed: ${moduleRegionsResult.error?.message}`);
    }

    // 3. Get specific region info
    console.log("\n3. Getting specific region info...");
    const testAddress = 0x7fff0000; // Common address range
    const regionInfoResult = memHook.getRegionInfo(testAddress);

    if (regionInfoResult.success && regionInfoResult.data) {
      const info = regionInfoResult.data;
      console.log(`   ✓ Region at ${utils.toHex(testAddress)}:`);
      console.log(
        `     Range: ${utils.toHex(info.startAddress)}-${utils.toHex(info.endAddress)}`
      );
      console.log(`     Size: ${info.size} bytes`);
      console.log(`     Protection: 0x${info.protection.toString(16)}`);
      console.log(`     Type: 0x${info.type.toString(16)}`);
    } else {
      console.log(`   ❌ Failed: ${regionInfoResult.error?.message}`);
    }
  } catch (error) {
    console.log(
      `   ❌ Region analysis error: ${error instanceof Error ? error.message : String(error)}`
    );
  }

  console.log();
}

// === ADVANCED SCANNING DEMO ===
async function demonstrateAdvancedScanning(memHook: any, moduleInfo: any) {
  console.log("🔬 ADVANCED SCANNING");
  console.log("===================");

  try {
    // 1. Scan for common x64 instructions
    console.log("1. Scanning for x64 code patterns...");
    const commonOpcodes = [
      [0x48, 0x89], // mov instruction (x64)
      [0x48, 0x8b], // mov instruction (x64)
      [0xff, 0x25], // jmp qword ptr [rip+offset]
    ];

    for (const opcodes of commonOpcodes) {
      const codeResult = memHook.scanForCode(opcodes);

      if (codeResult.success && codeResult.data) {
        console.log(
          `   ✓ Opcode [${opcodes.map((b) => b.toString(16).padStart(2, "0")).join(" ")}]: ${codeResult.data.length} matches`
        );
        if (codeResult.data.length > 0 && codeResult.data.length <= 3) {
          codeResult.data.forEach((match: any, i: number) => {
            console.log(`     ${i + 1}. ${utils.toHex(match.address)}`);
          });
        }
      } else {
        console.log(`   ❌ Opcode scan failed: ${codeResult.error?.message}`);
      }
    }

    // 2. Find function prologs
    console.log("\n2. Finding function prologs...");
    const prologsResult = memHook.findFunctionPrologs();

    if (prologsResult.success && prologsResult.data) {
      console.log(
        `   ✓ Found ${prologsResult.data.length} potential function prologs`
      );
      if (prologsResult.data.length > 0) {
        prologsResult.data.slice(0, 5).forEach((addr: number, i: number) => {
          console.log(`     ${i + 1}. ${utils.toHex(addr)}`);
        });
      }
    } else {
      console.log(`   ❌ Failed: ${prologsResult.error?.message}`);
    }

    // 3. Find call instructions (if we have a module)
    if (moduleInfo) {
      console.log("\n3. Finding call instructions to module base...");
      const callsResult = memHook.findCallInstructions(moduleInfo.baseAddress);

      if (callsResult.success && callsResult.data) {
        console.log(
          `   ✓ Found ${callsResult.data.length} calls to module base`
        );
        if (callsResult.data.length > 0) {
          callsResult.data.slice(0, 3).forEach((addr: number, i: number) => {
            console.log(`     ${i + 1}. Call at ${utils.toHex(addr)}`);
          });
        }
      } else {
        console.log(`   ❌ Failed: ${callsResult.error?.message}`);
      }
    }
  } catch (error) {
    console.log(
      `   ❌ Advanced scanning error: ${error instanceof Error ? error.message : String(error)}`
    );
  }

  console.log();
}

// === MEMORY COMPARISON & DIFFING DEMO ===
async function demonstrateMemoryComparison(memHook: any, moduleInfo: any) {
  console.log("🔄 MEMORY COMPARISON & DIFFING");
  console.log("==============================");

  if (!moduleInfo) {
    console.log("⚠️  Skipping memory comparison - no module info available\n");
    return;
  }

  try {
    // 1. Compare two regions (demo with same region to show no differences)
    console.log("1. Comparing memory regions...");
    const region1Start = moduleInfo.baseAddress;
    const region2Start = moduleInfo.baseAddress; // Same region for demo
    const compareSize = 256; // Compare first 256 bytes

    const compareResult = memHook.compareMemoryRegions(
      region1Start,
      region2Start,
      compareSize
    );

    if (compareResult.success && compareResult.data) {
      console.log(`   ✓ Compared ${compareSize} bytes`);
      console.log(
        `   Found ${compareResult.data.length} differences (should be 0 for same region)`
      );
    } else {
      console.log(`   ❌ Failed: ${compareResult.error?.message}`);
    }

    // 2. Find changed bytes (demo by reading current data and comparing)
    console.log("\n2. Finding changed bytes...");
    const currentData = memHook.readMemory(moduleInfo.baseAddress, 64);

    if (currentData) {
      // Create a slightly modified version for demo
      const originalBytes = Array.from(currentData) as number[];
      originalBytes[10] = (originalBytes[10] + 1) % 256; // Modify one byte

      const changesResult = memHook.findChangedBytes(
        moduleInfo.baseAddress,
        originalBytes
      );

      if (changesResult.success && changesResult.data) {
        console.log(`   ✓ Found ${changesResult.data.length} changed bytes`);
        if (changesResult.data.length > 0) {
          changesResult.data.forEach((addr: number, i: number) => {
            console.log(`     ${i + 1}. Changed at ${utils.toHex(addr)}`);
          });
        }
      } else {
        console.log(`   ❌ Failed: ${changesResult.error?.message}`);
      }
    } else {
      console.log("   ❌ Could not read memory for comparison");
    }
  } catch (error) {
    console.log(
      `   ❌ Memory comparison error: ${error instanceof Error ? error.message : String(error)}`
    );
  }

  console.log();
}

// === UTILITY FUNCTIONS DEMO ===
async function demonstrateUtilityFunctions(memHook: any) {
  console.log("🛠️  UTILITY FUNCTIONS");
  console.log("====================");

  try {
    // 1. Pattern parsing
    console.log("1. Pattern parsing utilities...");
    const patternStrings = [
      "48 89 E5", // Valid hex pattern
      "48 ?? E5", // Pattern with wildcard
      "FF 25 ?? ?? ?? ??", // Multiple wildcards
    ];

    for (const pattern of patternStrings) {
      const bytesResult = memHook.parseBytePattern(pattern);
      const maskResult = memHook.parsePatternMask(pattern);

      if (bytesResult.success && maskResult.success) {
        console.log(`   ✓ Pattern "${pattern}":`);
        console.log(
          `     Bytes: [${bytesResult.data.map((b: number) => b.toString(16).padStart(2, "0")).join(", ")}]`
        );
        console.log(
          `     Mask:  [${maskResult.data.map((m: boolean) => (m ? "x" : "?")).join(", ")}]`
        );
      } else {
        console.log(`   ❌ Pattern "${pattern}": Parse failed`);
      }
    }

    // 2. Address formatting
    console.log("\n2. Address formatting...");
    const addresses = [0x12345678, 0x7fff00000000, 0x140000000];

    addresses.forEach((addr) => {
      const formatted = memHook.formatAddress(addr, true);
      const noPrefix = memHook.formatAddress(addr, false);
      console.log(`   Address ${addr}: "${formatted}" or "${noPrefix}"`);
    });

    // 3. Address validation
    console.log("\n3. Address validation...");
    const testAddresses = [0x400000, 0x7fff0000, 0x1, 0x0];

    for (const addr of testAddresses) {
      const validResult = memHook.isValidCodeAddress(addr);

      if (validResult.success) {
        console.log(
          `   ${utils.toHex(addr)}: ${validResult.data ? "✓ Valid" : "❌ Invalid"} code address`
        );
      } else {
        console.log(
          `   ${utils.toHex(addr)}: Check failed - ${validResult.error?.message}`
        );
      }
    }

    // 4. Address alignment
    console.log("\n4. Address alignment analysis...");
    const alignmentAddresses = [0x1000, 0x1004, 0x1001, 0x2000];

    alignmentAddresses.forEach((addr) => {
      const alignment = memHook.getAlignment(addr);
      console.log(`   ${utils.toHex(addr)}: ${alignment}-byte aligned`);
    });
  } catch (error) {
    console.log(
      `   ❌ Utility functions error: ${error instanceof Error ? error.message : String(error)}`
    );
  }

  console.log();
}

// Run the demo if this file is executed directly
if (require.main === module) {
  memoryHelpersDemo().catch(console.error);
}

export default memoryHelpersDemo;
