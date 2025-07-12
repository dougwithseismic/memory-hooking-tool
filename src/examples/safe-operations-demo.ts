import { createMemoryHooking, utils, MemoryHookingError } from "../index";

async function safeOperationsDemo() {
  console.log("=== Safe Memory Operations Demo ===\n");

  const memHook = createMemoryHooking();

  try {
    // Test 1: Safe process attachment
    console.log("1. Testing safe process attachment...");
    const notepadProcess = await memHook.findProcessByName("notepad");

    if (notepadProcess) {
      console.log(`   Found notepad: PID ${notepadProcess.processId}`);

      if (memHook.attachToProcess(notepadProcess.processId)) {
        console.log("   ✓ Successfully attached to notepad");

        // Test 2: Safe memory reading
        console.log("\n2. Testing safe memory operations...");
        const mainModule = memHook.getModuleInfo("notepad.exe");

        if (mainModule) {
          console.log(
            `   Main module base: ${utils.toHex(mainModule.baseAddress)}`
          );

          // Try safe read
          const safeReadResult = memHook.safeReadMemory(
            mainModule.baseAddress,
            64
          );

          if (safeReadResult.success && safeReadResult.data) {
            console.log("   ✓ Safe read successful");
            console.log(`   Read ${safeReadResult.data.length} bytes`);

            // Check for PE header
            const peSignature = safeReadResult.data.readUInt16LE(0);
            if (peSignature === 0x5a4d) {
              console.log("   ✓ Valid PE (MZ) header detected");
            }
          } else {
            console.log("   ✗ Safe read failed:");
            console.log(`     Error: ${safeReadResult.error?.message}`);
          }

          // Test 3: Safe memory writing (to a safe location if possible)
          console.log("\n3. Testing safe memory write validation...");

          // Try to write to an invalid address (should fail safely)
          const invalidWriteResult = memHook.safeWriteMemory(
            0x00000001, // Invalid address
            Buffer.from("test")
          );

          if (!invalidWriteResult.success) {
            console.log("   ✓ Safe write correctly rejected invalid address");
            console.log(`     Reason: ${invalidWriteResult.error?.message}`);
          }
        }

        // Test 4: Safe pattern scanning
        console.log("\n4. Testing safe pattern scanning...");

        const patternResult = memHook.safeScanForPattern("4D 5A", "xx"); // MZ header

        if (patternResult.success && patternResult.data) {
          console.log(
            `   ✓ Safe pattern scan found ${patternResult.data.addresses.length} matches`
          );
          console.log(
            `   Successful reads: ${patternResult.data.successfulReads}`
          );
          console.log(`   Failed reads: ${patternResult.data.failedReads}`);

          if (patternResult.data.truncated) {
            console.log("   (Results were truncated for safety)");
          }

          // Show first few results
          if (patternResult.data.addresses.length > 0) {
            console.log("   First few addresses:");
            patternResult.data.addresses.slice(0, 3).forEach((addr, index) => {
              console.log(`     ${index + 1}. ${utils.toHex(addr)}`);
            });
          }
        } else {
          console.log("   Pattern scan failed:");
          console.log(`     Error: ${patternResult.error?.message}`);
        }

        // Test 5: Safe string scanning
        console.log("\n5. Testing safe string scanning...");

        const stringResult = memHook.safeScanForString("Notepad", false);

        if (stringResult.success && stringResult.data) {
          console.log(
            `   ✓ Safe string scan found ${stringResult.data.addresses.length} matches`
          );

          if (stringResult.data.addresses.length > 0) {
            console.log("   First few string locations:");
            stringResult.data.addresses.slice(0, 2).forEach((addr, index) => {
              console.log(`     ${index + 1}. ${utils.toHex(addr)}`);
            });
          }
        } else {
          console.log("   String scan failed:");
          console.log(`     Error: ${stringResult.error?.message}`);
        }

        // Test 6: Error handling demonstration
        console.log("\n6. Testing error handling...");

        // Try to read from an obviously invalid address
        const invalidReadResult = memHook.safeReadMemory(0x00000001, 4);

        if (!invalidReadResult.success && invalidReadResult.error) {
          console.log("   ✓ Invalid memory read properly handled");
          console.log(`     Error code: ${invalidReadResult.error.errorCode}`);
          console.log(`     Error message: ${invalidReadResult.error.message}`);
          console.log(
            `     Detailed: ${invalidReadResult.error.getDetailedMessage()}`
          );
        }

        // Get last error
        const lastError = memHook.getLastError();
        if (lastError) {
          console.log(
            `   Last error recorded: ${lastError.operation} - ${lastError.message}`
          );
        }

        memHook.detachFromProcess();
        console.log("\n   ✓ Safely detached from process");
      } else {
        console.log(
          "   ✗ Failed to attach to notepad (insufficient privileges?)"
        );
      }
    } else {
      console.log(
        "   Notepad not found. Starting alternative demo with current process...\n"
      );

      // Alternative demo with current process
      await demoWithCurrentProcess(memHook);
    }
  } catch (error) {
    console.error("Demo failed with error:", error);

    if (MemoryHookingError.isMemoryHookingError(error)) {
      console.error("Memory hooking error details:");
      console.error(`  Code: ${error.errorCode}`);
      console.error(`  Operation: ${error.operation}`);
      console.error(`  Windows Error: ${error.windowsErrorCode}`);
      console.error(`  Detailed: ${error.getDetailedMessage()}`);
    } else if (error instanceof Error) {
      console.error(`  Standard error: ${error.message}`);
    }
  }

  console.log("\n=== Safe Operations Demo Completed ===");
}

async function demoWithCurrentProcess(memHook: any) {
  console.log(
    "Alternative demo: Safe operations with current Node.js process..."
  );

  const currentPid = process.pid;

  if (memHook.attachToProcess(currentPid)) {
    console.log(`✓ Attached to current process (PID: ${currentPid})`);

    // Safe module info retrieval
    const nodeModule = memHook.getModuleInfo("node.exe");
    if (nodeModule) {
      console.log(
        `Node.js module: ${utils.toHex(nodeModule.baseAddress)} (${nodeModule.size} bytes)`
      );

      // Safe read from Node.js module
      const safeRead = memHook.safeReadMemory(nodeModule.baseAddress, 16);
      if (safeRead.success && safeRead.data) {
        console.log("✓ Successfully read from Node.js module");
        console.log(`  Hex dump: ${safeRead.data.toString("hex")}`);
      }
    }

    // Safe pattern scan for common x64 instruction
    const patternResult = memHook.safeScanForPattern("48 89", "xx");
    if (patternResult.success && patternResult.data) {
      console.log(
        `✓ Found ${patternResult.data.addresses.length} x64 instruction patterns`
      );
      console.log(
        `  Scan stats: ${patternResult.data.successfulReads} successful, ${patternResult.data.failedReads} failed`
      );
    }

    memHook.detachFromProcess();
    console.log("✓ Safely detached");
  }
}

// Error handling examples
function demonstrateErrorHandling() {
  console.log("\n=== Error Handling Examples ===");

  const memHook = createMemoryHooking();

  // Example 1: Try to read without attaching
  console.log("\n1. Reading without process attachment:");
  const noProcessResult = memHook.safeReadMemory(0x1000, 4);
  if (!noProcessResult.success) {
    console.log(`   ✓ Correctly rejected: ${noProcessResult.error?.message}`);
  }

  // Example 2: Invalid parameters
  console.log("\n2. Invalid parameters:");
  const invalidParamsResult = memHook.safeReadMemory(0, 0); // Invalid size
  if (!invalidParamsResult.success) {
    console.log(
      `   ✓ Correctly rejected: ${invalidParamsResult.error?.message}`
    );
  }

  // Example 3: Enable/disable logging
  console.log("\n3. Logging control:");
  memHook.enableLogging(false);
  console.log("   ✓ Logging disabled");
  memHook.enableLogging(true);
  console.log("   ✓ Logging re-enabled");
}

// Run the demo if this file is executed directly
if (require.main === module) {
  safeOperationsDemo()
    .then(() => demonstrateErrorHandling())
    .catch(console.error);
}

export default safeOperationsDemo;
