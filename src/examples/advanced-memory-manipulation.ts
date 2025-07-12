import { createMemoryHooking, utils, Address, MemoryRegion } from "../index";
import { exec } from "child_process";
import { promisify } from "util";

const execAsync = promisify(exec);

async function openCalculatorIfNotRunning(memHook: any): Promise<boolean> {
  console.log("🔍 Checking if calculator is running...");

  // Check if calculator is already running
  const calculatorNames = ["calc.exe", "calculator.exe", "CalculatorApp.exe"];

  for (const name of calculatorNames) {
    const found = await memHook.findProcessByName(name);
    if (found) {
      console.log(
        `✓ Calculator already running: ${name} (PID: ${found.processId})`
      );
      return true;
    }
  }

  console.log("❌ Calculator not found. Opening calculator...");

  try {
    // Open calculator
    await execAsync("calc");
    console.log("⏳ Waiting for calculator to start...");

    // Wait for calculator to start (check every 500ms for up to 5 seconds)
    for (let i = 0; i < 10; i++) {
      await new Promise((resolve) => setTimeout(resolve, 500));

      for (const name of calculatorNames) {
        const found = await memHook.findProcessByName(name);
        if (found) {
          console.log(
            `✅ Calculator started: ${name} (PID: ${found.processId})`
          );
          return true;
        }
      }
    }

    console.log("⚠️ Calculator didn't start within timeout period");
    return false;
  } catch (error) {
    console.error("❌ Failed to open calculator:", error);
    return false;
  }
}

async function advancedMemoryManipulationExample() {
  console.log("=== Advanced Memory Manipulation Example ===\n");

  const memHook = createMemoryHooking();

  try {
    // Ensure calculator is running
    console.log("1. Ensuring calculator is available...");
    const calcAvailable = await openCalculatorIfNotRunning(memHook);

    if (!calcAvailable) {
      console.log(
        "⚠️ Calculator not available, falling back to other processes..."
      );
    }

    // Look for a target process (calculator as example)
    console.log("\n2. Looking for processes to target...");

    // First, let's see what processes are available
    console.log("Available processes to target:");
    const processNames = [
      "CalculatorApp.exe",
      "calc.exe",
      "calculator.exe",
      "notepad.exe",
      "explorer.exe",
    ];
    let targetProcess = null;

    for (const processName of processNames) {
      const found = await memHook.findProcessByName(processName);
      if (found) {
        console.log(`✓ Found ${processName} (PID: ${found.processId})`);
        if (!targetProcess) {
          targetProcess = found; // Use the first one we find
        }
      } else {
        console.log(`✗ ${processName} not found`);
      }
    }

    if (!targetProcess) {
      console.log(
        "\nNo target processes found. This example will demonstrate techniques on the current Node.js process...\n"
      );

      // Fallback to current process for demonstration
      const currentPid = process.pid;
      const attached = memHook.attachToProcess(currentPid);

      if (attached) {
        await demonstrateAdvancedTechniques(memHook, "current Node.js process");
      }
      return;
    }

    console.log(
      `Found target process: ${targetProcess.processName} (PID: ${targetProcess.processId})`
    );

    // Attach to the target process
    const attached = memHook.attachToProcess(targetProcess.processId);
    if (!attached) {
      console.log(
        "Failed to attach to target process (insufficient privileges?)"
      );
      return;
    }

    console.log("✓ Successfully attached to target process\n");

    await demonstrateAdvancedTechniques(memHook, targetProcess.processName);

    // Clean up
    memHook.detachFromProcess();
    console.log("✓ Detached from target process");
  } catch (error) {
    console.error("Error during execution:", error);
  }

  console.log("\n=== Advanced example completed ===");
}

async function demonstrateAdvancedTechniques(
  memHook: any,
  processName: string
) {
  console.log(`2. Advanced memory analysis of ${processName}...\n`);

  // Get comprehensive module information
  console.log("=== Module Analysis ===");
  const mainModuleName = processName.includes(".exe")
    ? processName
    : processName + ".exe";
  const mainModule = memHook.getModuleInfo(mainModuleName);

  if (mainModule) {
    console.log(`Main Module: ${mainModuleName}`);
    console.log(`  Base Address: ${utils.toHex(mainModule.baseAddress)}`);
    console.log(
      `  Size: ${mainModule.size} bytes (${(mainModule.size / 1024 / 1024).toFixed(2)} MB)`
    );
    console.log(
      `  End Address: ${utils.toHex(mainModule.baseAddress + mainModule.size)}\n`
    );

    // Analyze PE header in detail
    await analyzePEHeader(memHook, mainModule.baseAddress);
  }

  // Advanced memory region analysis
  console.log("=== Memory Region Analysis ===");
  const regions = memHook.getMemoryRegions();
  const executableRegions = memHook.getExecutableRegions();
  const writableRegions = memHook.getWritableRegions();
  const committedRegions = memHook.getCommittedRegions();

  console.log(`Total regions: ${regions.length}`);
  console.log(`Executable regions: ${executableRegions.length}`);
  console.log(`Writable regions: ${writableRegions.length}`);
  console.log(`Committed regions: ${committedRegions.length}\n`);

  // Find largest regions
  const sortedRegions = regions.sort(
    (a: MemoryRegion, b: MemoryRegion) => b.size - a.size
  );
  console.log("Largest memory regions:");
  sortedRegions.slice(0, 5).forEach((region: MemoryRegion, index: number) => {
    const protectionStr = getProtectionString(region.protection);
    const sizeStr = (region.size / 1024 / 1024).toFixed(2);
    console.log(
      `  ${index + 1}. ${utils.toHex(region.baseAddress)} | ${sizeStr} MB | ${protectionStr}`
    );
  });
  console.log("");

  // Advanced pattern scanning
  console.log("=== Advanced Pattern Scanning ===");

  // Scan for common Windows API patterns
  await scanForAPIPatterns(memHook);

  // Scan for string patterns
  await scanForStringPatterns(memHook);

  // Scan for numeric patterns
  await scanForNumericPatterns(memHook);

  // Memory region validation
  console.log("=== Memory Validation ===");
  await validateMemoryRegions(memHook, executableRegions.slice(0, 3));

  // Advanced read/write demonstrations (safe examples only)
  console.log("=== Safe Memory Operations ===");
  await demonstrateSafeMemoryOperations(memHook, mainModule);

  // Calculator-specific memory manipulation
  console.log("=== Calculator Memory Manipulation ===");
  await manipulateCalculatorMemory(memHook, processName);
}

async function analyzePEHeader(memHook: any, baseAddress: Address) {
  console.log("=== PE Header Analysis ===");

  // Read DOS header
  const dosHeader = memHook.readMemory(baseAddress, 64);
  if (!dosHeader) {
    console.log("Could not read DOS header");
    return;
  }

  const mzSignature = dosHeader.readUInt16LE(0);
  if (mzSignature !== 0x5a4d) {
    console.log("Invalid MZ signature");
    return;
  }

  console.log("✓ Valid MZ signature found");

  // Get PE header offset
  const peOffset = dosHeader.readUInt32LE(60);
  console.log(`PE header offset: ${utils.toHex(peOffset)}`);

  // Read PE header
  const peHeader = memHook.readMemory(baseAddress + peOffset, 256);
  if (!peHeader) {
    console.log("Could not read PE header");
    return;
  }

  const peSignature = peHeader.readUInt32LE(0);
  if (peSignature !== 0x4550) {
    console.log("Invalid PE signature");
    return;
  }

  console.log("✓ Valid PE signature found");

  // Read COFF header
  const machine = peHeader.readUInt16LE(4);
  const numberOfSections = peHeader.readUInt16LE(6);
  const timeDateStamp = peHeader.readUInt32LE(8);
  const sizeOfOptionalHeader = peHeader.readUInt16LE(20);

  console.log(
    `Machine type: 0x${machine.toString(16)} (${getMachineTypeString(machine)})`
  );
  console.log(`Number of sections: ${numberOfSections}`);
  console.log(`Timestamp: ${new Date(timeDateStamp * 1000).toISOString()}`);
  console.log(`Optional header size: ${sizeOfOptionalHeader} bytes\n`);
}

async function scanForAPIPatterns(memHook: any) {
  console.log("Scanning for Windows API call patterns...");

  // Look for common API call patterns (limit to safer patterns)
  const apiPatterns = [
    { name: "Push EBP/MOV EBP,ESP", pattern: "55 8B EC", mask: "xxx" },
    { name: "Function prologue x64", pattern: "48 89 5C 24", mask: "xxxx" },
  ];

  for (const apiPattern of apiPatterns) {
    try {
      console.log(`  Scanning for ${apiPattern.name}...`);

      // Use safe scanning if available
      if (typeof memHook.safeScanForPattern === "function") {
        const result = memHook.safeScanForPattern(
          apiPattern.pattern,
          apiPattern.mask
        );

        if (result.success && result.data) {
          console.log(
            `  ${apiPattern.name}: ${result.data.addresses.length} matches`
          );

          if (
            result.data.addresses.length > 0 &&
            result.data.addresses.length <= 3
          ) {
            result.data.addresses
              .slice(0, 3)
              .forEach((addr: Address, index: number) => {
                console.log(`    ${index + 1}. ${utils.toHex(addr)}`);
              });
          }

          if (result.data.truncated) {
            console.log(`    (Results truncated for safety)`);
          }
        } else {
          console.log(
            `  ${apiPattern.name}: Scan failed - ${result.error?.message || "Unknown error"}`
          );
        }
      } else {
        // Fallback to regular scanning with limited results
        const results = memHook.scanForPattern(
          apiPattern.pattern,
          apiPattern.mask
        );
        const limitedResults = results.slice(0, 3); // Limit results for safety

        console.log(
          `  ${apiPattern.name}: ${limitedResults.length} matches (limited for safety)`
        );

        limitedResults.forEach((addr: Address, index: number) => {
          console.log(`    ${index + 1}. ${utils.toHex(addr)}`);
        });
      }
    } catch (error) {
      console.log(
        `  ${apiPattern.name}: Error during scan - ${error instanceof Error ? error.message : "Unknown error"}`
      );
    }
  }
  console.log("");
}

async function scanForStringPatterns(memHook: any) {
  console.log("Scanning for interesting strings...");

  // Limit to safer, shorter strings
  const interestingStrings = ["kernel32", "ntdll", "user32", "Error"];

  for (const str of interestingStrings) {
    try {
      console.log(`  Scanning for "${str}"...`);

      // Use safe scanning if available
      if (typeof memHook.safeScanForString === "function") {
        const result = memHook.safeScanForString(str, false);

        if (result.success && result.data) {
          if (result.data.addresses.length > 0) {
            console.log(`  "${str}": ${result.data.addresses.length} matches`);

            if (result.data.addresses.length <= 2) {
              result.data.addresses
                .slice(0, 2)
                .forEach((addr: Address, index: number) => {
                  console.log(`    ${index + 1}. ${utils.toHex(addr)}`);
                });
            }

            if (result.data.truncated) {
              console.log(`    (Results truncated for safety)`);
            }
          }
        } else {
          console.log(
            `  "${str}": Scan failed - ${result.error?.message || "Unknown error"}`
          );
        }
      } else {
        // Fallback to regular scanning with limited results
        const results = memHook.scanForString(str, false);
        const limitedResults = results.slice(0, 2); // Limit results for safety

        if (limitedResults.length > 0) {
          console.log(
            `  "${str}": ${limitedResults.length} matches (limited for safety)`
          );
          limitedResults.forEach((addr: Address, index: number) => {
            console.log(`    ${index + 1}. ${utils.toHex(addr)}`);
          });
        }
      }
    } catch (error) {
      console.log(
        `  "${str}": Error during scan - ${error instanceof Error ? error.message : "Unknown error"}`
      );
    }
  }
  console.log("");
}

async function scanForNumericPatterns(memHook: any) {
  console.log("Scanning for numeric patterns...");

  // Scan for common magic numbers
  const magicNumbers = [
    { name: "PE signature", value: 0x4550 },
    { name: "MZ signature", value: 0x5a4d },
    { name: "Common alignment", value: 0x1000 },
    { name: "NULL dword", value: 0x00000000 },
  ];

  for (const magic of magicNumbers) {
    const results = memHook.scanForValue(magic.value);
    if (results.length > 0) {
      console.log(
        `  ${magic.name} (0x${magic.value.toString(16)}): ${results.length} matches`
      );
    }
  }
  console.log("");
}

async function validateMemoryRegions(memHook: any, regions: any[]) {
  console.log("Validating memory regions...");

  for (let i = 0; i < regions.length; i++) {
    const region = regions[i];
    const isValid = memHook.isValidAddress(region.baseAddress);
    const protectionStr = getProtectionString(region.protection);

    console.log(
      `  Region ${i + 1}: ${utils.toHex(region.baseAddress)} | ${protectionStr} | Valid: ${isValid ? "✓" : "✗"}`
    );

    if (isValid) {
      // Try to read a small amount of data
      const sample = memHook.readMemory(
        region.baseAddress,
        Math.min(16, region.size)
      );
      if (sample) {
        const hex = sample.toString("hex").substring(0, 32);
        console.log(`    Sample data: ${hex}...`);
      }
    }
  }
  console.log("");
}

async function demonstrateSafeMemoryOperations(memHook: any, mainModule: any) {
  console.log("Demonstrating safe memory operations...");

  if (!mainModule) {
    console.log("No main module available for demonstration");
    return;
  }

  // Read various data types from the beginning of the module
  const baseAddr = mainModule.baseAddress;

  console.log("Reading different data types from module base:");

  // Read as different data types
  const int32Value = memHook.readInt32(baseAddr);
  const floatValue = memHook.readFloat(baseAddr);
  const stringValue = memHook.readString(baseAddr, 8);

  console.log(`  Int32: ${int32Value} (0x${int32Value?.toString(16)})`);
  console.log(`  Float: ${floatValue}`);
  console.log(`  String (8 bytes): "${stringValue}"`);

  // Demonstrate hex conversion utilities
  const bytes = [0x48, 0x65, 0x6c, 0x6c, 0x6f]; // "Hello"
  const hexString = memHook.bytesToHexString(bytes);
  const backToBytes = memHook.hexStringToBytes(hexString);

  console.log(`  Bytes to hex: [${bytes.join(", ")}] -> "${hexString}"`);
  console.log(`  Hex to bytes: "${hexString}" -> [${backToBytes.join(", ")}]`);

  console.log("");
}

async function manipulateCalculatorMemory(memHook: any, processName: string) {
  console.log(`Attempting to manipulate memory for process: ${processName}...`);

  const isCalculator = processName.toLowerCase().includes("calc");
  const isNotepad = processName.toLowerCase().includes("notepad");
  const isExplorer = processName.toLowerCase().includes("explorer");

  if (
    !isCalculator &&
    !isNotepad &&
    !isExplorer &&
    !processName.includes("Node.js")
  ) {
    console.log(
      "Process type not specifically supported - proceeding with general memory manipulation"
    );
  }

  try {
    // Strategy 1: Find and modify display values
    console.log("1. Searching for numeric display values...");

    // Look for common calculator display patterns (numbers 0-9)
    const displayValues = [1, 2, 3, 4, 5, 10, 100, 1000];
    let foundDisplayValue = false;

    for (const value of displayValues) {
      const addresses = memHook.scanForValue(value);
      if (addresses.length > 0 && addresses.length < 50) {
        // Avoid too many matches
        console.log(`Found ${addresses.length} instances of value ${value}`);

        // Try to modify the first few instances
        for (let i = 0; i < Math.min(3, addresses.length); i++) {
          const addr = addresses[i];
          try {
            // Read current value to verify
            const currentValue = memHook.readInt32(addr);
            if (currentValue === value) {
              // Modify to a "magic" number
              const newValue = 42069;
              const success = memHook.writeInt32(addr, newValue);

              if (success) {
                console.log(
                  `  ✓ Changed value at ${utils.toHex(addr)} from ${value} to ${newValue}`
                );
                foundDisplayValue = true;

                // Verify the change
                const verifyValue = memHook.readInt32(addr);
                console.log(`  Verification: value is now ${verifyValue}`);
              } else {
                console.log(
                  `  ✗ Failed to write to ${utils.toHex(addr)} (protected memory)`
                );
              }
            }
          } catch (error) {
            console.log(
              `  ✗ Error modifying memory at ${utils.toHex(addr)}: ${error instanceof Error ? error.message : "Unknown error"}`
            );
          }
        }
      }
    }

    // Strategy 2: Look for process-specific strings and modify nearby memory
    console.log("\n2. Searching for process-specific strings...");
    let targetStrings = ["=", "+", "-", "*", "/"];

    if (isCalculator) {
      targetStrings = ["Calculator", "=", "+", "-", "*", "/"];
    } else if (isNotepad) {
      targetStrings = ["Notepad", "File", "Edit", "View"];
    } else if (isExplorer) {
      targetStrings = ["Explorer", "File", "View", "This PC"];
    } else {
      targetStrings = ["node", "require", "module", "process"];
    }

    for (const str of targetStrings) {
      try {
        const addresses = memHook.scanForString(str, false);
        if (addresses.length > 0 && addresses.length < 20) {
          console.log(`Found "${str}" at ${addresses.length} locations`);

          // For each string location, look for nearby numeric values
          for (let i = 0; i < Math.min(2, addresses.length); i++) {
            const stringAddr = addresses[i];
            console.log(`  Analyzing area around ${utils.toHex(stringAddr)}`);

            // Look for integers in the vicinity (±1KB)
            const searchStart = stringAddr - 1024;
            const searchEnd = stringAddr + 1024;

            // Read memory in chunks and look for modifiable values
            try {
              const chunk = memHook.readMemory(searchStart, 2048);
              if (chunk) {
                // Look for 32-bit integers that look like they could be calculator values
                for (let offset = 0; offset < chunk.length - 4; offset += 4) {
                  const value = chunk.readInt32LE(offset);

                  // Target values that look like calculator results (0-999999)
                  if (value >= 0 && value <= 999999) {
                    const targetAddr = searchStart + offset;

                    try {
                      // Try to change it to our signature value
                      const newValue = 13371337;
                      const success = memHook.writeInt32(targetAddr, newValue);

                      if (success) {
                        console.log(
                          `    ✓ Modified value ${value} -> ${newValue} at ${utils.toHex(targetAddr)}`
                        );

                        // Only modify one value per string to avoid breaking too much
                        break;
                      }
                    } catch (writeError) {
                      // Silently continue - many addresses will be read-only
                    }
                  }
                }
              }
            } catch (readError) {
              console.log(
                `    Could not read memory around ${utils.toHex(stringAddr)}`
              );
            }
          }
        }
      } catch (error) {
        // Continue with other strings
      }
    }

    // Strategy 3: Try to inject a simple "easter egg"
    console.log("\n3. Attempting to inject calculator easter egg...");

    // Look for writable memory regions where we can place a small modification
    const writableRegions = memHook.getWritableRegions();

    if (writableRegions.length > 0) {
      console.log(`Found ${writableRegions.length} writable regions`);

      // Find a suitable writable region (not too small, not too large)
      const suitableRegions = writableRegions.filter(
        (region: MemoryRegion) =>
          region.size >= 1024 && region.size <= 1024 * 1024 * 10 // 1KB to 10MB
      );

      if (suitableRegions.length > 0) {
        const targetRegion = suitableRegions[0];
        console.log(
          `Using writable region at ${utils.toHex(targetRegion.baseAddress)} (${targetRegion.size} bytes)`
        );

        // Create a simple signature pattern
        const signature = Buffer.from("HACKED_BY_MEMORY_HOOK", "ascii");
        const timestampBuffer = Buffer.allocUnsafe(4);
        timestampBuffer.writeUInt32LE(Math.floor(Date.now() / 1000), 0);

        const payload = Buffer.concat([signature, timestampBuffer]);

        try {
          const success = memHook.writeMemory(
            targetRegion.baseAddress,
            payload
          );
          if (success) {
            console.log(
              `  ✓ Injected signature at ${utils.toHex(targetRegion.baseAddress)}`
            );

            // Verify injection
            const verification = memHook.readMemory(
              targetRegion.baseAddress,
              payload.length
            );
            if (verification && verification.equals(payload)) {
              console.log("  ✓ Signature injection verified!");
            }
          }
        } catch (error) {
          console.log(
            `  ✗ Failed to inject signature: ${error instanceof Error ? error.message : "Unknown error"}`
          );
        }
      }
    }

    if (!foundDisplayValue) {
      console.log(
        `\nNote: No obvious display values were modified. The ${processName} might use different data structures.`
      );
      if (isCalculator) {
        console.log(
          "Try performing some calculations first, then run this tool again."
        );
      } else if (isNotepad) {
        console.log(
          "Try typing some text in Notepad first, then run this tool again."
        );
      }
    }

    console.log(`\n⚠️  Memory manipulation of ${processName} complete!`);
    if (isCalculator) {
      console.log(
        "Check the calculator window to see if any values have changed."
      );
      console.log(
        "Some changes might only be visible after performing calculations."
      );
    } else {
      console.log(
        `Check the ${processName} window to see if any behavior has changed.`
      );
      console.log(
        "Some changes might only be visible after interacting with the application."
      );
    }
  } catch (error) {
    console.error(`Error during ${processName} memory manipulation:`, error);
  }
}

function getProtectionString(protection: number): string {
  const protections = [];

  if (protection & 0x01) protections.push("NOACCESS");
  if (protection & 0x02) protections.push("READONLY");
  if (protection & 0x04) protections.push("READWRITE");
  if (protection & 0x08) protections.push("WRITECOPY");
  if (protection & 0x10) protections.push("EXECUTE");
  if (protection & 0x20) protections.push("EXECUTE_READ");
  if (protection & 0x40) protections.push("EXECUTE_READWRITE");
  if (protection & 0x80) protections.push("EXECUTE_WRITECOPY");
  if (protection & 0x100) protections.push("GUARD");
  if (protection & 0x200) protections.push("NOCACHE");
  if (protection & 0x400) protections.push("WRITECOMBINE");

  return protections.length > 0 ? protections.join("|") : "UNKNOWN";
}

function getMachineTypeString(machine: number): string {
  switch (machine) {
    case 0x014c:
      return "i386";
    case 0x0162:
      return "R3000";
    case 0x0166:
      return "R4000";
    case 0x0168:
      return "R10000";
    case 0x0169:
      return "WCEMIPSV2";
    case 0x0184:
      return "ALPHA";
    case 0x01a2:
      return "SH3";
    case 0x01a3:
      return "SH3DSP";
    case 0x01a4:
      return "SH3E";
    case 0x01a6:
      return "SH4";
    case 0x01a8:
      return "SH5";
    case 0x01c0:
      return "ARM";
    case 0x01c2:
      return "THUMB";
    case 0x01c4:
      return "ARMNT";
    case 0x01d3:
      return "AM33";
    case 0x01f0:
      return "POWERPC";
    case 0x01f1:
      return "POWERPCFP";
    case 0x0200:
      return "IA64";
    case 0x0266:
      return "MIPS16";
    case 0x0284:
      return "ALPHA64";
    case 0x0366:
      return "MIPSFPU";
    case 0x0466:
      return "MIPSFPU16";
    case 0x0520:
      return "TRICORE";
    case 0x0cef:
      return "CEF";
    case 0x0ebc:
      return "EBC";
    case 0x8664:
      return "AMD64";
    case 0x9041:
      return "M32R";
    case 0xaa64:
      return "ARM64";
    case 0xc0ee:
      return "CEE";
    default:
      return "UNKNOWN";
  }
}

// Run the example if this file is executed directly
if (require.main === module) {
  advancedMemoryManipulationExample().catch(console.error);
}

export default advancedMemoryManipulationExample;
