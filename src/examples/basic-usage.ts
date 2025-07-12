import { createMemoryHooking, utils, processes, ProcessInfo } from "../index";

async function basicUsageExample() {
  console.log("=== Memory Hooking Tool - Basic Usage Example ===\n");

  // Create a memory hooking instance
  const memHook = createMemoryHooking();

  try {
    // 1. List all running processes
    console.log("1. Listing running processes...");
    const allProcesses = memHook.getRunningProcesses();
    console.log(`Found ${allProcesses.length} running processes.\n`);

    // Show first 10 processes
    console.log("First 10 processes:");
    allProcesses.slice(0, 10).forEach((proc) => {
      console.log(
        `  PID: ${proc.processId.toString().padStart(6)} | Name: ${proc.processName}`
      );
    });
    console.log("");

    // 2. Find processes by pattern
    console.log('2. Finding processes containing "chrome" or "firefox"...');
    const browserProcesses =
      await memHook.findProcessesByPattern("(chrome|firefox)");
    if (browserProcesses.length > 0) {
      browserProcesses.forEach((proc) => {
        console.log(`  Found: ${proc.processName} (PID: ${proc.processId})`);
      });
    } else {
      console.log("  No browser processes found.");
    }
    console.log("");

    // 3. Find and attach to a specific process (notepad as example)
    console.log("3. Looking for notepad.exe...");
    const notepadProcess = await memHook.findProcessByName("notepad.exe");

    if (notepadProcess) {
      console.log(`Found notepad.exe (PID: ${notepadProcess.processId})`);

      // Try to attach to the process
      console.log("Attempting to attach...");
      const attached = memHook.attachToProcess(notepadProcess.processId);

      if (attached) {
        console.log("✓ Successfully attached to notepad.exe");

        // Get current process info
        const processInfo = memHook.getCurrentProcessInfo();
        console.log(
          `Process Info: PID=${processInfo.processId}, Name=${processInfo.processName}`
        );

        // Get module information
        console.log("\n4. Getting module information...");
        const mainModule = memHook.getModuleInfo("notepad.exe");
        if (mainModule) {
          console.log(
            `Main module base address: ${utils.toHex(mainModule.baseAddress)}`
          );
          console.log(`Module size: ${mainModule.size} bytes`);
        }

        // Get memory regions
        console.log("\n5. Analyzing memory regions...");
        const regions = memHook.getMemoryRegions();
        const executableRegions = memHook.getExecutableRegions();
        const writableRegions = memHook.getWritableRegions();

        console.log(`Total memory regions: ${regions.length}`);
        console.log(`Executable regions: ${executableRegions.length}`);
        console.log(`Writable regions: ${writableRegions.length}`);

        // Show some executable regions
        console.log("\nFirst 5 executable regions:");
        executableRegions.slice(0, 5).forEach((region, index) => {
          console.log(
            `  ${index + 1}. Base: ${utils.toHex(region.baseAddress)}, Size: ${region.size} bytes`
          );
        });

        // 6. Simple memory scanning example
        console.log('\n6. Scanning for string "Notepad"...');
        const stringAddresses = memHook.scanForString("Notepad", false);
        console.log(`Found "Notepad" at ${stringAddresses.length} locations`);

        if (stringAddresses.length > 0) {
          console.log("First 5 addresses:");
          stringAddresses.slice(0, 5).forEach((addr, index) => {
            console.log(`  ${index + 1}. ${utils.toHex(addr)}`);
          });
        }

        // 7. Read memory example
        if (mainModule && mainModule.baseAddress) {
          console.log("\n7. Reading first 64 bytes of main module...");
          const data = memHook.readMemory(mainModule.baseAddress, 64);

          if (data) {
            console.log("Hex dump:");
            console.log(
              data
                .toString("hex")
                .match(/.{1,32}/g)
                ?.join("\n")
            );

            // Check for PE header
            const peSignature = data.readUInt16LE(0);
            if (peSignature === 0x5a4d) {
              console.log("\n✓ Valid PE (MZ) header detected");
            }
          } else {
            console.log("Failed to read memory from main module");
          }
        }

        // 8. Pattern scanning example
        console.log("\n8. Pattern scanning example...");
        // Look for common instruction patterns (this is just an example)
        const { pattern, mask } = memHook.createPattern(
          [0x48, 0x89],
          [false, false]
        ); // mov instruction pattern
        const patternResults = memHook.scanForPattern(pattern, mask);
        console.log(`Found pattern at ${patternResults.length} locations`);

        // Detach from process
        memHook.detachFromProcess();
        console.log("\n✓ Detached from process");
      } else {
        console.log(
          "✗ Failed to attach to notepad.exe (insufficient privileges?)"
        );
      }
    } else {
      console.log(
        "Notepad.exe not found. Please start Notepad and run this example again."
      );

      // Alternative: Show example with the current process
      console.log("\nAlternative: Analyzing current Node.js process...");
      const currentPid = process.pid;
      const attached = memHook.attachToProcess(currentPid);

      if (attached) {
        console.log(`✓ Attached to current process (PID: ${currentPid})`);

        const nodeModule =
          memHook.getModuleInfo("node.exe") || memHook.getModuleInfo("node");
        if (nodeModule) {
          console.log(
            `Node.js module base: ${utils.toHex(nodeModule.baseAddress)}`
          );
          console.log(`Node.js module size: ${nodeModule.size} bytes`);
        }

        memHook.detachFromProcess();
      }
    }
  } catch (error) {
    console.error("Error during execution:", error);
  }

  console.log("\n=== Example completed ===");
}

// Run the example if this file is executed directly
if (require.main === module) {
  basicUsageExample().catch(console.error);
}

export default basicUsageExample;
