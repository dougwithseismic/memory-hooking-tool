import { createMemoryHooking, utils, examples } from "../index";

async function testBasicFunctionality() {
  console.log("=== Memory Hooking Tool - Basic Test ===\n");

  try {
    // Test 1: Create instance
    console.log("1. Creating MemoryHooking instance...");
    const memHook = createMemoryHooking();
    console.log("   ✓ Instance created successfully\n");

    // Test 2: List processes
    console.log("2. Testing process enumeration...");
    const processes = memHook.getRunningProcesses();
    console.log(`   ✓ Found ${processes.length} running processes\n`);

    // Show some processes
    console.log("   Sample processes:");
    processes.slice(0, 5).forEach((proc) => {
      console.log(
        `     PID: ${proc.processId.toString().padStart(6)} | Name: ${proc.processName}`
      );
    });
    console.log("");

    // Test 3: Utility functions
    console.log("3. Testing utility functions...");
    const testAddress = 0x7ff000000000;
    const hexAddress = utils.toHex(testAddress);
    const backToNumber = utils.fromHex(hexAddress);

    console.log(
      `   Address: ${testAddress} -> ${hexAddress} -> ${backToNumber}`
    );
    console.log(`   ✓ Address conversion works correctly\n`);

    // Test 4: Pattern creation
    console.log("4. Testing pattern creation...");
    const bytes = [0x48, 0x89, 0x5c, 0x24, 0x08];
    const wildcards = [false, false, true, false, false];
    const { pattern, mask } = memHook.createPattern(bytes, wildcards);

    console.log(
      `   Bytes: [${bytes.map((b) => "0x" + b.toString(16)).join(", ")}]`
    );
    console.log(`   Pattern: "${pattern}"`);
    console.log(`   Mask: "${mask}"`);
    console.log("   ✓ Pattern creation works correctly\n");

    // Test 5: Hex utilities
    console.log("5. Testing hex utilities...");
    const testBytes = [0x48, 0x65, 0x6c, 0x6c, 0x6f]; // "Hello"
    const hexString = memHook.bytesToHexString(testBytes);
    const backToBytes = memHook.hexStringToBytes(hexString);

    console.log(`   Bytes: [${testBytes.join(", ")}]`);
    console.log(`   Hex: "${hexString}"`);
    console.log(`   Back to bytes: [${backToBytes.join(", ")}]`);
    console.log("   ✓ Hex conversion works correctly\n");

    // Test 6: String utilities
    console.log("6. Testing string utilities...");
    const testString = "Hello World";
    const stringBytes = utils.stringToBytes(testString);
    const wideStringBytes = utils.wideStringToBytes(testString);

    console.log(`   String: "${testString}"`);
    console.log(`   UTF-8 bytes: [${stringBytes.slice(0, 10).join(", ")}...]`);
    console.log(
      `   UTF-16 bytes: [${wideStringBytes.slice(0, 20).join(", ")}...]`
    );
    console.log("   ✓ String conversion works correctly\n");

    // Test 7: Attempt to attach to current process
    console.log("7. Testing process attachment...");
    const currentPid = process.pid;
    console.log(
      `   Attempting to attach to current process (PID: ${currentPid})...`
    );

    const attached = memHook.attachToProcess(currentPid);
    if (attached) {
      console.log("   ✓ Successfully attached to current process");

      const processInfo = memHook.getCurrentProcessInfo();
      console.log(`   Process ID: ${processInfo.processId}`);
      console.log(`   Process Name: ${processInfo.processName}`);
      console.log(`   Is Attached: ${processInfo.isAttached}`);

      // Try to get module info
      const nodeModule =
        memHook.getModuleInfo("node.exe") || memHook.getModuleInfo("node");
      if (nodeModule) {
        console.log(
          `   Node.js module base: ${utils.toHex(nodeModule.baseAddress)}`
        );
        console.log(`   Node.js module size: ${nodeModule.size} bytes`);
      }

      // Get memory regions count
      const regions = memHook.getMemoryRegions();
      console.log(`   Memory regions: ${regions.length}`);

      memHook.detachFromProcess();
      console.log("   ✓ Detached from process");
    } else {
      console.log(
        "   ⚠ Could not attach to current process (may need admin privileges)"
      );
    }
    console.log("");

    // Test 8: Process search
    console.log("8. Testing process search...");
    const notepadProcess = await memHook.findProcessByName("notepad");
    if (notepadProcess) {
      console.log(`   ✓ Found notepad: PID ${notepadProcess.processId}`);
    } else {
      console.log("   ⚠ Notepad not found (not running)");
    }

    const browserProcesses = await memHook.findProcessesByPattern(
      "(chrome|firefox|edge)"
    );
    console.log(`   Found ${browserProcesses.length} browser processes`);
    console.log("");

    // Test 9: Memory protection helpers
    console.log("9. Testing memory protection helpers...");
    const testProtections = [
      { name: "PAGE_READWRITE", value: 0x04 },
      { name: "PAGE_EXECUTE_READ", value: 0x20 },
      { name: "PAGE_READONLY", value: 0x02 },
      { name: "PAGE_NOACCESS", value: 0x01 },
    ];

    testProtections.forEach((prot) => {
      const readable = utils.isReadable(prot.value);
      const writable = utils.isWritable(prot.value);
      const executable = utils.isExecutable(prot.value);

      console.log(
        `   ${prot.name}: R=${readable} W=${writable} X=${executable}`
      );
    });
    console.log("");

    console.log("=== All basic tests completed successfully! ===");
    console.log("");
    console.log("Next steps:");
    console.log("1. Run: node dist/examples/basic-usage.js");
    console.log("2. Run: node dist/examples/advanced-memory-manipulation.js");
    console.log(
      "3. Start Notepad and run the examples again for full demonstration"
    );
  } catch (error) {
    console.error("❌ Test failed with error:", error);
    console.error("");
    console.error("Common issues:");
    console.error(
      "- Make sure to build the native addon first: pnpm run build:native"
    );
    console.error("- Ensure you have the required build tools installed");
    console.error("- Try running as Administrator for enhanced functionality");
  }
}

// Run tests if this file is executed directly
if (require.main === module) {
  testBasicFunctionality().catch(console.error);
}

export default testBasicFunctionality;
