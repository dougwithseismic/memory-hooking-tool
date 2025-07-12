import { createMemoryHooking, MouseButton, KeyCode } from "../index";

// Simple sleep function like in automation-demo.ts
function sleep(ms: number): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

const demoText = `*** MEMORY HOOKING TOOL - AUTONOMOUS PC CONTROL ***

Welcome to the future of PC automation! This tool gives AI agents
complete control over your computer through:

* Direct Memory Access & Manipulation
* Process Injection & Hooking  
* Real-time Memory Scanning
* Complete Mouse & Keyboard Control
* Window Management & Screenshots
* Visual Automation & Color Detection

Built with C++ core engine and TypeScript interface for maximum
performance and ease of use. Perfect for game automation,
reverse engineering, and autonomous PC control.

*** LIGHTNING FAST AUTOMATION DEMO ***

Watch as this bot demonstrates superhuman speed and precision:
- Instant text manipulation
- Pixel-perfect mouse control
- Blazing fast window management
- Real-time visual processing

This is just the beginning. Imagine LLMs with complete PC access!

*** Created by Doug @ withseismic.com ***`;

const flashyDemo = async () => {
  console.log("*** Starting Flashy Demo Script...");

  const automation = createMemoryHooking();

  try {
    // Step 1: Open Notepad using Windows key + R
    console.log("*** Opening Notepad...");
    automation.sendKeyCombo([KeyCode.Win, KeyCode.R]);
    await sleep(1000);
    automation.sendText("notepad");
    automation.pressKey(KeyCode.Enter);

    // Wait for notepad to open
    console.log("*** Waiting for Notepad to appear...");
    const notepadFound = automation.waitForWindow("Notepad", 5000);
    if (!notepadFound) {
      throw new Error("Notepad not found!");
    }

    // Give it a moment to fully load
    await sleep(2000);

    // Step 2: Find and activate notepad window
    console.log("*** Finding Notepad window...");
    const notepadWindows = automation.getWindowsByTitle("Notepad");
    if (notepadWindows.length === 0) {
      throw new Error("No Notepad windows found!");
    }

    const notepadWindow = notepadWindows[0];
    automation.setActiveWindow(notepadWindow.hwnd);

    // Step 3: Clear any existing content
    console.log("*** Clearing existing content...");
    automation.sendKeyCombo([KeyCode.Control, KeyCode.A]);
    await sleep(100);
    automation.pressKey(KeyCode.Delete);
    await sleep(100);

    // Step 4: Write the intro text with dramatic effect
    console.log("*** Writing epic intro...");
    const lines = demoText.split("\n");

    for (let i = 0; i < lines.length; i++) {
      const line = lines[i];

      // Add some dramatic pauses for effect
      if (line.includes("***")) {
        await sleep(300);
      }

      // Type the line character by character for better reliability
      for (const char of line) {
        automation.sendText(char);
        await sleep(15); // Slower typing for reliability
      }

      automation.pressKey(KeyCode.Enter);

      // Pause between lines
      await sleep(100);
    }

    // Step 5: CRAZY MOUSE MOVEMENTS AND TEXT SELECTION
    console.log("*** Starting INSANE mouse choreography...");

    // Get notepad window dimensions for mouse movements
    const windowRect = {
      x: notepadWindow.x,
      y: notepadWindow.y,
      width: notepadWindow.width,
      height: notepadWindow.height,
    };

    // Move to center of notepad
    const centerX = windowRect.x + windowRect.width / 2;
    const centerY = windowRect.y + windowRect.height / 2;

    // 1. Lightning-fast spiral movement
    console.log("*** Executing spiral attack...");
    for (let i = 0; i < 50; i++) {
      const angle = i * 0.5 * Math.PI;
      const radius = i * 3;
      const x = centerX + Math.cos(angle) * radius;
      const y = centerY + Math.sin(angle) * radius;
      automation.moveMouse(x, y);
      await sleep(10);
    }

    // 2. Rapid corner-to-corner movements
    console.log("*** Corner-to-corner lightning strikes...");
    const corners = [
      { x: windowRect.x + 50, y: windowRect.y + 100 },
      { x: windowRect.x + windowRect.width - 50, y: windowRect.y + 100 },
      {
        x: windowRect.x + windowRect.width - 50,
        y: windowRect.y + windowRect.height - 100,
      },
      { x: windowRect.x + 50, y: windowRect.y + windowRect.height - 100 },
    ];

    for (let cycle = 0; cycle < 3; cycle++) {
      for (const corner of corners) {
        automation.moveMouse(corner.x, corner.y);
        await sleep(100);
      }
    }

    // 3. Crazy text selection showcase
    console.log("*** Demonstrating superhuman text selection...");

    // Select title line
    automation.moveMouse(windowRect.x + 50, windowRect.y + 120);
    automation.clickMouse(MouseButton.Left);
    await sleep(50);

    // Triple-click to select entire line
    automation.clickMouse(MouseButton.Left);
    await sleep(20);
    automation.clickMouse(MouseButton.Left);
    await sleep(20);
    automation.clickMouse(MouseButton.Left);
    await sleep(200);

    // Now do rapid selections of different paragraphs
    const selectionPoints = [
      { x: windowRect.x + 100, y: windowRect.y + 200 },
      { x: windowRect.x + 200, y: windowRect.y + 300 },
      { x: windowRect.x + 150, y: windowRect.y + 400 },
      { x: windowRect.x + 250, y: windowRect.y + 500 },
    ];

    for (const point of selectionPoints) {
      // Move to point
      automation.moveMouse(point.x, point.y);
      await sleep(50);

      // Select word
      automation.doubleClickMouse(MouseButton.Left);
      await sleep(100);

      // Deselect
      automation.clickMouse(MouseButton.Left, point.x + 50, point.y);
      await sleep(50);
    }

    // 4. Rapid-fire drag selections
    console.log("*** Rapid-fire drag selections...");
    for (let i = 0; i < 5; i++) {
      const startX = windowRect.x + 50 + i * 40;
      const startY = windowRect.y + 200 + i * 30;
      const endX = startX + 200;
      const endY = startY + 50;

      automation.dragMouse(startX, startY, endX, endY, MouseButton.Left);
      await sleep(150);

      // Deselect
      automation.clickMouse(MouseButton.Left, endX + 10, endY);
      await sleep(50);
    }

    // 5. Final flourish - crazy window manipulation
    console.log("*** Final window manipulation flourish...");

    // Resize window rapidly
    for (let i = 0; i < 10; i++) {
      const newWidth = 600 + i * 20;
      const newHeight = 400 + i * 15;
      automation.moveWindow(
        notepadWindow.hwnd,
        windowRect.x,
        windowRect.y,
        newWidth,
        newHeight
      );
      await sleep(100);
    }

    // Move window around
    for (let i = 0; i < 8; i++) {
      const newX = windowRect.x + Math.sin(i * 0.5) * 100;
      const newY = windowRect.y + Math.cos(i * 0.5) * 50;
      automation.moveWindow(notepadWindow.hwnd, newX, newY, 800, 600);
      await sleep(150);
    }

    // Return to center
    automation.moveWindow(notepadWindow.hwnd, 200, 100, 800, 600);

    // 6. Grand finale - add signature with style
    console.log("*** Grand finale signature...");

    // Go to end of document
    automation.sendKeyCombo([KeyCode.Control, KeyCode.Right]);
    await sleep(100);

    // Add some newlines
    automation.pressKey(KeyCode.Enter);
    automation.pressKey(KeyCode.Enter);

    // Type signature with dramatic effect
    const signature = "*** DEMO COMPLETE - AUTONOMOUS PC CONTROL ACHIEVED! ***";
    for (const char of signature) {
      automation.sendText(char);
      await sleep(35); // Slower typewriter effect for reliability
    }

    // Final mouse celebration
    automation.moveMouse(centerX, centerY);
    for (let i = 0; i < 20; i++) {
      const x = centerX + Math.sin(i * 0.3) * 100;
      const y = centerY + Math.cos(i * 0.3) * 50;
      automation.moveMouse(x, y);
      await sleep(50);
    }

    // End with mouse in center
    automation.moveMouse(centerX, centerY);

    console.log("*** FLASHY DEMO COMPLETE! ***");
    console.log("*** The bot has demonstrated superhuman speed and precision!");
    console.log("*** Ready to take over the world... responsibly! ***");
  } catch (error) {
    console.error("*** Demo failed:", error);
  }
};

// Run the demo
flashyDemo().catch(console.error);

export { flashyDemo };
