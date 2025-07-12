import { createMemoryHooking, utils, MouseButton, KeyCode } from "../index";
import fs from "fs";
import path from "path";

async function automationDemo() {
  console.log("=== Automation & Memory Hooking Demo ===\n");

  const memHook = createMemoryHooking();

  try {
    // === SYSTEM INFORMATION ===
    console.log("1. System Information");
    console.log("====================");
    const screenWidth = memHook.getScreenWidth();
    const screenHeight = memHook.getScreenHeight();
    const dpi = memHook.getScreenDPI();
    const systemInfo = memHook.getSystemInfo();

    console.log(`Screen: ${screenWidth}x${screenHeight} @ ${dpi} DPI`);
    console.log(`System: ${systemInfo}`);
    console.log(
      `Current position: ${JSON.stringify(memHook.getMousePosition())}\n`
    );

    // === WINDOW MANAGEMENT ===
    console.log("2. Window Management");
    console.log("===================");

    // Get all windows
    const windows = memHook.getAllWindows();
    console.log(`Found ${windows.length} windows`);

    // Show some interesting windows
    console.log("Visible windows:");
    windows
      .filter((w) => w.isVisible && w.title.trim() !== "")
      .slice(0, 5)
      .forEach((window, i) => {
        console.log(
          `  ${i + 1}. "${window.title}" (${window.width}x${window.height})`
        );
      });

    // Find Calculator if it exists
    const calcWindows = memHook.getWindowsByTitle("Calculator");
    if (calcWindows.length > 0) {
      console.log(`\nFound Calculator: ${calcWindows[0].title}`);
    } else {
      console.log("\nCalculator not found, let's open it...");

      // Open Calculator using Windows key + R, then typing calc
      console.log("Opening Run dialog (Win+R)...");
      memHook.sendKeyCombo([KeyCode.Win, KeyCode.R]);
      await sleep(1000);

      console.log("Typing 'calc' and pressing Enter...");
      memHook.sendText("calc");
      await sleep(500);
      memHook.pressKey(KeyCode.Enter);
      await sleep(2000);

      // Check if calculator opened
      const newCalcWindows = memHook.getWindowsByTitle("Calculator");
      if (newCalcWindows.length > 0) {
        console.log("✓ Calculator opened successfully!");
      }
    }

    // === SCREENSHOT DEMONSTRATION ===
    console.log("\n3. Screenshot Capabilities");
    console.log("=========================");

    // Take a full screen screenshot
    console.log("Taking full screen screenshot...");
    const fullScreenshot = memHook.captureScreen();
    if (fullScreenshot.data.length > 0) {
      console.log(
        `✓ Screenshot captured: ${fullScreenshot.width}x${fullScreenshot.height}, ${fullScreenshot.data.length} bytes`
      );

      // Save to file
      const screenshotPath = path.join(process.cwd(), "full-screen.bmp");
      if (memHook.saveScreenshotToFile(fullScreenshot, screenshotPath)) {
        console.log(`✓ Screenshot saved to: ${screenshotPath}`);
      }
    }

    // Capture active window
    console.log("Capturing active window...");
    const activeWindow = memHook.captureActiveWindow();
    if (activeWindow.data.length > 0) {
      console.log(
        `✓ Active window captured: ${activeWindow.width}x${activeWindow.height}`
      );

      const activeWindowPath = path.join(process.cwd(), "active-window.bmp");
      memHook.saveScreenshotToFile(activeWindow, activeWindowPath);
    }

    // === MOUSE AUTOMATION ===
    console.log("\n4. Mouse Automation");
    console.log("==================");

    const originalPos = memHook.getMousePosition();
    console.log(`Current mouse position: (${originalPos.x}, ${originalPos.y})`);

    // Move mouse in a pattern
    console.log("Moving mouse in a pattern...");
    const centerX = Math.floor(screenWidth / 2);
    const centerY = Math.floor(screenHeight / 2);

    // Draw a square with the mouse
    const squareSize = 100;
    const corners = [
      { x: centerX - squareSize, y: centerY - squareSize },
      { x: centerX + squareSize, y: centerY - squareSize },
      { x: centerX + squareSize, y: centerY + squareSize },
      { x: centerX - squareSize, y: centerY + squareSize },
      { x: centerX - squareSize, y: centerY - squareSize },
    ];

    for (let i = 0; i < corners.length; i++) {
      console.log(
        `Moving to corner ${i + 1}: (${corners[i].x}, ${corners[i].y})`
      );
      memHook.moveMouse(corners[i].x, corners[i].y);
      await sleep(300);
    }

    // Click in the center
    console.log("Clicking in center of screen...");
    memHook.clickMouse(MouseButton.Left, centerX, centerY);
    await sleep(500);

    // Restore original position
    console.log("Restoring original mouse position...");
    memHook.moveMouse(originalPos.x, originalPos.y);

    // === KEYBOARD AUTOMATION ===
    console.log("\n5. Keyboard Automation");
    console.log("=====================");

    // Open Notepad
    console.log("Opening Notepad...");
    memHook.sendKeyCombo([KeyCode.Win, KeyCode.R]);
    await sleep(1000);
    memHook.sendText("notepad");
    memHook.pressKey(KeyCode.Enter);
    await sleep(2000);

    // Type some text
    console.log("Typing demonstration text...");
    const demoText = `Memory Hooking & Automation Demo
===============================

This text was automatically typed using the automation system!

Current time: ${new Date().toISOString()}
Screen resolution: ${screenWidth}x${screenHeight}
System DPI: ${dpi}

The system can:
- Control mouse and keyboard
- Take screenshots 
- Manipulate memory
- Manage windows
- And much more!

Press Ctrl+A to select all, then Ctrl+C to copy this text.
`;

    memHook.sendText(demoText);
    await sleep(1000);

    // Select all text and copy
    console.log("Selecting all text (Ctrl+A)...");
    memHook.sendKeyCombo([KeyCode.Control, KeyCode.A]);
    await sleep(500);

    console.log("Copying text (Ctrl+C)...");
    memHook.sendKeyCombo([KeyCode.Control, KeyCode.C]);
    await sleep(500);

    // === MEMORY MANIPULATION WITH AUTOMATION ===
    console.log("\n6. Combined Memory & Automation");
    console.log("==============================");

    // Try to attach to Notepad and scan its memory
    console.log("Looking for Notepad process...");
    const notepadProcess = await memHook.findProcessByName("notepad.exe");

    if (notepadProcess && memHook.attachToProcess(notepadProcess.processId)) {
      console.log(`✓ Attached to Notepad (PID: ${notepadProcess.processId})`);

      // Search for our demo text in memory
      console.log("Searching for demo text in Notepad memory...");
      const textMatches = memHook.scanForString("Memory Hooking", false);
      console.log(
        `Found "${textMatches.length}" matches for our text in memory`
      );

      if (textMatches.length > 0) {
        console.log(`First match at: ${utils.toHex(textMatches[0])}`);

        // Read some memory around the text
        const memoryData = memHook.readMemory(textMatches[0], 100);
        if (memoryData) {
          const textContent = memoryData.toString("utf8").replace(/\0/g, "");
          console.log(`Memory content: "${textContent.substring(0, 50)}..."`);
        }
      }

      memHook.detachFromProcess();
      console.log("✓ Detached from Notepad");
    }

    // === COLOR AND PIXEL DETECTION ===
    console.log("\n7. Color and Pixel Detection");
    console.log("===========================");

    // Get pixel color at current mouse position
    const currentPos = memHook.getMousePosition();
    const pixelColor = memHook.getPixelColor(currentPos.x, currentPos.y);
    console.log(
      `Pixel color at (${currentPos.x}, ${currentPos.y}): 0x${pixelColor.toString(16).padStart(6, "0")}`
    );

    // Get colors in a small region around the cursor
    const colors = memHook.getPixelColors(
      currentPos.x - 2,
      currentPos.y - 2,
      5,
      5
    );
    console.log(`Colors in 5x5 region: ${colors.length} pixels sampled`);

    // === ADVANCED AUTOMATION SEQUENCE ===
    console.log("\n8. Advanced Automation Sequence");
    console.log("===============================");

    // Demonstrate calculator automation
    console.log("Attempting calculator automation...");
    const calcWindows2 = memHook.getWindowsByTitle("Calculator");

    if (calcWindows2.length > 0) {
      console.log("Setting Calculator as active window...");
      memHook.setActiveWindow(calcWindows2[0].hwnd);
      await sleep(500);

      // Perform a calculation
      console.log("Performing calculation: 123 + 456 = ");
      memHook.sendText("123+456=");
      await sleep(1000);

      // Take a screenshot of the calculator
      console.log("Taking calculator screenshot...");
      const calcScreenshot = memHook.captureWindow(calcWindows2[0].hwnd);
      if (calcScreenshot.data.length > 0) {
        const calcPath = path.join(process.cwd(), "calculator.bmp");
        memHook.saveScreenshotToFile(calcScreenshot, calcPath);
        console.log(`✓ Calculator screenshot saved to: ${calcPath}`);
      }

      // Clear the calculation
      console.log("Clearing calculation (Escape)...");
      memHook.pressKey(KeyCode.Escape);
    }

    // === WAIT AND TIMING DEMO ===
    console.log("\n9. Wait and Timing Features");
    console.log("==========================");

    console.log("Demonstrating wait functions...");
    console.log(
      "- Press any key in the next 3 seconds to test key detection..."
    );

    const keyPressed = memHook.waitForKeyPress(KeyCode.Space, 3000);
    if (keyPressed) {
      console.log("✓ Space key detected!");
    } else {
      console.log("No key press detected in timeout period");
    }

    // Check if user is idle
    const isIdle = memHook.isUserIdle(2000);
    console.log(`User idle for 2+ seconds: ${isIdle}`);

    // === CLEANUP ===
    console.log("\n10. Cleanup");
    console.log("===========");

    console.log("Demo completed. Cleaning up...");

    // List all running window titles for reference
    const windowTitles = memHook.getRunningWindowTitles();
    console.log(`Currently ${windowTitles.length} visible windows running`);

    console.log("\n=== Demo Summary ===");
    console.log("✓ System information gathered");
    console.log("✓ Window management demonstrated");
    console.log("✓ Screenshots captured and saved");
    console.log("✓ Mouse automation performed");
    console.log("✓ Keyboard automation executed");
    console.log("✓ Memory manipulation combined with UI");
    console.log("✓ Color detection demonstrated");
    console.log("✓ Advanced automation sequences");
    console.log("✓ Wait and timing features tested");
  } catch (error) {
    console.error("Demo failed with error:", error);

    if (error instanceof Error) {
      console.error(`Error details: ${error.message}`);
      console.error(`Stack trace: ${error.stack}`);
    }
  }

  console.log("\n=== Automation & Memory Hooking Demo Complete ===");
}

// Helper function for delays
function sleep(ms: number): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

// Additional demonstration functions

async function demonstrateAdvancedMouseControl(memHook: any) {
  console.log("\n=== Advanced Mouse Control Demo ===");

  const start = memHook.getMousePosition();
  console.log(`Starting position: (${start.x}, ${start.y})`);

  // Demonstrate drag operation
  console.log("Demonstrating drag operation...");
  const dragStart = { x: start.x, y: start.y };
  const dragEnd = { x: start.x + 100, y: start.y + 100 };

  memHook.dragMouse(
    dragStart.x,
    dragStart.y,
    dragEnd.x,
    dragEnd.y,
    MouseButton.Left
  );
  await sleep(1000);

  // Scroll wheel demonstration
  console.log("Demonstrating scroll wheel...");
  memHook.scrollMouse(start.x, start.y, 3); // Scroll up
  await sleep(500);
  memHook.scrollMouse(start.x, start.y, -3); // Scroll down

  // Return to start
  memHook.moveMouse(start.x, start.y);
}

async function demonstrateComplexKeyboardSequences(memHook: any) {
  console.log("\n=== Complex Keyboard Sequences Demo ===");

  // Open Windows Terminal (if available)
  console.log("Attempting to open Windows Terminal...");
  memHook.sendKeyCombo([KeyCode.Win, KeyCode.X]);
  await sleep(500);
  memHook.pressKey(KeyCode.I); // Windows Terminal option
  await sleep(2000);

  // Type some commands
  console.log("Typing command sequence...");
  memHook.sendText("echo 'Hello from automation system!'");
  memHook.pressKey(KeyCode.Enter);
  await sleep(1000);

  memHook.sendText("dir");
  memHook.pressKey(KeyCode.Enter);
  await sleep(1000);

  // Close terminal
  memHook.sendKeyCombo([KeyCode.Alt, KeyCode.F4]);
}

async function demonstratePrecisionPixelWork(memHook: any) {
  console.log("\n=== Precision Pixel Work Demo ===");

  const centerX = Math.floor(memHook.getScreenWidth() / 2);
  const centerY = Math.floor(memHook.getScreenHeight() / 2);

  // Sample colors in a cross pattern
  const samplePoints = [
    { x: centerX, y: centerY }, // Center
    { x: centerX - 50, y: centerY }, // Left
    { x: centerX + 50, y: centerY }, // Right
    { x: centerX, y: centerY - 50 }, // Up
    { x: centerX, y: centerY + 50 }, // Down
  ];

  console.log("Sampling colors in cross pattern:");
  samplePoints.forEach((point, i) => {
    const color = memHook.getPixelColor(point.x, point.y);
    const r = (color >> 16) & 0xff;
    const g = (color >> 8) & 0xff;
    const b = color & 0xff;
    console.log(
      `  Point ${i + 1} (${point.x}, ${point.y}): RGB(${r}, ${g}, ${b})`
    );
  });
}

// Run the demo if this file is executed directly
if (require.main === module) {
  automationDemo().catch(console.error);
}

export default automationDemo;
