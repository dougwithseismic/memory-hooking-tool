import { createMemoryHooking, MouseButton, KeyCode } from "../index";

// Simple sleep function
function sleep(ms: number): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

// Feature text to type while doing mouse movements
const featureText = `*** MEMORY HOOKING TOOL - AUTONOMOUS PC CONTROL ***

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

*** Created by Doug @ withseismic.com ***`;

// Global typing state
let typingIndex = 0;
let isTyping = false;
let notepadWindow: any = null;
let notepadBounds = { x: 0, y: 0, width: 0, height: 0 };
let totalPatterns = 13;
let currentPattern = 0;
let textPerPattern = 0;

// Type a few characters
const typeChunk = async (automation: any, chunkSize: number = 5) => {
  if (!isTyping || typingIndex >= featureText.length) return;

  const chunk = featureText.slice(typingIndex, typingIndex + chunkSize);
  for (const char of chunk) {
    if (char === "\n") {
      automation.pressKey(KeyCode.Enter);
    } else {
      automation.sendText(char);
    }
    await sleep(2); // Ultra-fast typing
  }
  typingIndex += chunkSize;
};

// Type text for current pattern
const typeForPattern = async (
  automation: any,
  patternIndex: number,
  progress: number
) => {
  if (!isTyping || typingIndex >= featureText.length) return;

  const targetIndex = Math.floor((patternIndex + progress) * textPerPattern);
  const chunkSize = Math.min(8, targetIndex - typingIndex);

  if (chunkSize > 0) {
    await typeChunk(automation, chunkSize);
  }
};

const insaneMouseDemo = async () => {
  console.log("*** INSANE NOTEPAD MOUSE + TYPING DEMO ***");
  console.log("*** SIMULTANEOUS CHAOS! ***");

  const automation = createMemoryHooking();

  try {
    // Get screen dimensions
    const screenWidth = automation.getScreenWidth();
    const screenHeight = automation.getScreenHeight();
    console.log(`Screen: ${screenWidth}x${screenHeight}`);

    // Step 1: Open Notepad
    console.log("*** OPENING NOTEPAD ***");
    automation.sendKeyCombo([KeyCode.Win, KeyCode.R]);
    await sleep(500);
    automation.sendText("notepad");
    automation.pressKey(KeyCode.Enter);
    await sleep(1500);

    // Step 2: Find and setup Notepad window
    console.log("*** FINDING NOTEPAD WINDOW ***");
    const notepadWindows = automation.getWindowsByTitle("Notepad");
    if (notepadWindows.length === 0) {
      throw new Error("No Notepad windows found!");
    }

    notepadWindow = notepadWindows[0];
    automation.setActiveWindow(notepadWindow.hwnd);
    await sleep(500);

    // Step 3: Resize and center Notepad window
    console.log("*** RESIZING AND CENTERING NOTEPAD ***");
    const newWidth = 800;
    const newHeight = 600;
    const newX = (screenWidth - newWidth) / 2;
    const newY = (screenHeight - newHeight) / 2;

    automation.moveWindow(notepadWindow.hwnd, newX, newY, newWidth, newHeight);
    await sleep(500);

    // Update notepad bounds
    notepadBounds = {
      x: newX,
      y: newY + 60, // Account for title bar
      width: newWidth,
      height: newHeight - 60,
    };

    const centerX = notepadBounds.x + notepadBounds.width / 2;
    const centerY = notepadBounds.y + notepadBounds.height / 2;

    // Clear any existing content
    automation.sendKeyCombo([KeyCode.Control, KeyCode.A]);
    await sleep(100);
    automation.pressKey(KeyCode.Delete);
    await sleep(100);

    // Start typing
    console.log("*** STARTING SIMULTANEOUS TYPING + MOUSE CHAOS ***");
    isTyping = true;
    typingIndex = 0;
    textPerPattern = featureText.length / totalPatterns;

    // 1. SPIRAL VORTEX ATTACK + TYPING (3 seconds)
    console.log("*** SPIRAL VORTEX ATTACK + TYPING ***");
    currentPattern = 0;
    for (let i = 0; i < 150; i++) {
      const angle = i * 0.3 * Math.PI;
      const radius = Math.min(i * 2, 150); // Constrain to notepad
      const x = centerX + Math.cos(angle) * radius;
      const y = centerY + Math.sin(angle) * radius;

      // Keep within bounds
      const boundedX = Math.max(
        notepadBounds.x,
        Math.min(x, notepadBounds.x + notepadBounds.width)
      );
      const boundedY = Math.max(
        notepadBounds.y,
        Math.min(y, notepadBounds.y + notepadBounds.height)
      );

      automation.moveMouse(boundedX, boundedY);

      // Type synchronized with animation
      await typeForPattern(automation, currentPattern, i / 150);

      await sleep(20);
    }

    // 2. LIGHTNING CORNERS + TYPING (3 seconds)
    console.log("*** LIGHTNING CORNERS + TYPING ***");
    currentPattern = 1;
    const corners = [
      { x: notepadBounds.x + 50, y: notepadBounds.y + 50 },
      {
        x: notepadBounds.x + notepadBounds.width - 50,
        y: notepadBounds.y + 50,
      },
      {
        x: notepadBounds.x + notepadBounds.width - 50,
        y: notepadBounds.y + notepadBounds.height - 50,
      },
      {
        x: notepadBounds.x + 50,
        y: notepadBounds.y + notepadBounds.height - 50,
      },
    ];

    let totalMoves = 0;
    const maxMoves = 40;
    for (let cycle = 0; cycle < 10; cycle++) {
      for (const corner of corners) {
        automation.moveMouse(corner.x, corner.y);

        // Type synchronized with animation
        await typeForPattern(automation, currentPattern, totalMoves / maxMoves);
        totalMoves++;

        await sleep(75);
      }
    }

    // 3. FIGURE-8 INFINITY LOOP + TYPING (3 seconds)
    console.log("*** INFINITY LOOP + TYPING ***");
    currentPattern = 2;
    for (let i = 0; i < 200; i++) {
      const t = i * 0.1;
      const x = centerX + Math.sin(t) * 200;
      const y = centerY + Math.sin(t * 2) * 100;

      // Keep within bounds
      const boundedX = Math.max(
        notepadBounds.x,
        Math.min(x, notepadBounds.x + notepadBounds.width)
      );
      const boundedY = Math.max(
        notepadBounds.y,
        Math.min(y, notepadBounds.y + notepadBounds.height)
      );

      automation.moveMouse(boundedX, boundedY);

      // Type synchronized with animation
      await typeForPattern(automation, currentPattern, i / 200);

      await sleep(15);
    }

    // 4. RAPID FIRE MOVEMENTS + TYPING (3 seconds)
    console.log("*** RAPID FIRE MOVEMENTS + TYPING ***");
    currentPattern = 3;
    for (let i = 0; i < 50; i++) {
      const x = notepadBounds.x + Math.random() * notepadBounds.width;
      const y = notepadBounds.y + Math.random() * notepadBounds.height;

      automation.moveMouse(x, y);

      // Type synchronized with animation
      await typeForPattern(automation, currentPattern, i / 50);

      await sleep(60);
    }

    // 5. SINE WAVE PATTERNS + TYPING (3 seconds)
    console.log("*** SINE WAVE SYMPHONY + TYPING ***");
    currentPattern = 4;
    for (let i = 0; i < 300; i++) {
      const progress = i / 300;
      const x = notepadBounds.x + progress * notepadBounds.width;
      const y = centerY + Math.sin(i * 0.1) * 100;

      // Keep within bounds
      const boundedY = Math.max(
        notepadBounds.y,
        Math.min(y, notepadBounds.y + notepadBounds.height)
      );

      automation.moveMouse(x, boundedY);

      // Type synchronized with animation
      await typeForPattern(automation, currentPattern, progress);

      await sleep(10);
    }

    // 5.5. DIAMOND EXPLOSION + TYPING (3 seconds)
    console.log("*** DIAMOND EXPLOSION + TYPING ***");
    currentPattern = 5;
    for (let i = 0; i < 60; i++) {
      const progress = i / 60;
      const size = progress * 120;

      // Create diamond pattern
      const diamondPoints = [
        { x: centerX, y: centerY - size }, // Top
        { x: centerX + size, y: centerY }, // Right
        { x: centerX, y: centerY + size }, // Bottom
        { x: centerX - size, y: centerY }, // Left
      ];

      for (const point of diamondPoints) {
        // Keep within bounds
        const boundedX = Math.max(
          notepadBounds.x,
          Math.min(point.x, notepadBounds.x + notepadBounds.width)
        );
        const boundedY = Math.max(
          notepadBounds.y,
          Math.min(point.y, notepadBounds.y + notepadBounds.height)
        );

        automation.moveMouse(boundedX, boundedY);
        await sleep(12);
      }

      // Type synchronized with animation
      await typeForPattern(automation, currentPattern, progress);
    }

    // 5.6. RADIATING LINES + TYPING (3 seconds)
    console.log("*** RADIATING LINES + TYPING ***");
    currentPattern = 6;
    for (let line = 0; line < 16; line++) {
      const angle = (line * 2 * Math.PI) / 16;

      // Draw line from center outward
      for (let radius = 0; radius <= 100; radius += 5) {
        const x = centerX + Math.cos(angle) * radius;
        const y = centerY + Math.sin(angle) * radius;

        // Keep within bounds
        const boundedX = Math.max(
          notepadBounds.x,
          Math.min(x, notepadBounds.x + notepadBounds.width)
        );
        const boundedY = Math.max(
          notepadBounds.y,
          Math.min(y, notepadBounds.y + notepadBounds.height)
        );

        automation.moveMouse(boundedX, boundedY);
        await sleep(6);
      }

      // Type synchronized with animation
      await typeForPattern(automation, currentPattern, line / 16);
    }

    // 6. CONCENTRIC CIRCLES + TYPING (3 seconds)
    console.log("*** CONCENTRIC CIRCLES + TYPING ***");
    currentPattern = 7;
    let circleProgress = 0;
    const totalCircleSteps = 5 * Math.floor((Math.PI * 2) / 0.3);
    for (let radius = 30; radius <= 150; radius += 30) {
      for (let angle = 0; angle < Math.PI * 2; angle += 0.3) {
        const x = centerX + Math.cos(angle) * radius;
        const y = centerY + Math.sin(angle) * radius;

        // Keep within bounds
        const boundedX = Math.max(
          notepadBounds.x,
          Math.min(x, notepadBounds.x + notepadBounds.width)
        );
        const boundedY = Math.max(
          notepadBounds.y,
          Math.min(y, notepadBounds.y + notepadBounds.height)
        );

        automation.moveMouse(boundedX, boundedY);

        // Type synchronized with animation
        await typeForPattern(
          automation,
          currentPattern,
          circleProgress / totalCircleSteps
        );
        circleProgress++;

        await sleep(15);
      }
    }

    // 7. PULSING HEXAGON + TYPING (3 seconds)
    console.log("*** PULSING HEXAGON + TYPING ***");
    currentPattern = 8;
    for (let pulse = 0; pulse < 20; pulse++) {
      const baseRadius = 50 + pulse * 3;
      const pulseRadius = baseRadius + Math.sin(pulse * 0.8) * 30;

      // Create hexagon
      for (let side = 0; side < 6; side++) {
        const angle = (side * Math.PI) / 3;
        const x = centerX + Math.cos(angle) * pulseRadius;
        const y = centerY + Math.sin(angle) * pulseRadius;

        // Keep within bounds
        const boundedX = Math.max(
          notepadBounds.x,
          Math.min(x, notepadBounds.x + notepadBounds.width)
        );
        const boundedY = Math.max(
          notepadBounds.y,
          Math.min(y, notepadBounds.y + notepadBounds.height)
        );

        automation.moveMouse(boundedX, boundedY);
        await sleep(25);
      }

      // Type synchronized with animation
      await typeForPattern(automation, currentPattern, pulse / 20);
    }

    // 8. CHAOS MODE + TYPING (3 seconds)
    console.log("*** CHAOS MODE + TYPING ***");
    currentPattern = 9;
    for (let i = 0; i < 100; i++) {
      const x = notepadBounds.x + Math.random() * notepadBounds.width;
      const y = notepadBounds.y + Math.random() * notepadBounds.height;

      automation.moveMouse(x, y);

      // Type synchronized with animation
      await typeForPattern(automation, currentPattern, i / 100);

      await sleep(30);
    }

    // 9. STAR PATTERN + TYPING (3 seconds)
    console.log("*** STAR PATTERN + TYPING ***");
    currentPattern = 10;
    const starPoints = 8;
    for (let i = 0; i < starPoints * 5; i++) {
      const angle = (i * 2 * Math.PI) / starPoints;
      const radius = i % 2 === 0 ? 150 : 75;
      const x = centerX + Math.cos(angle) * radius;
      const y = centerY + Math.sin(angle) * radius;

      // Keep within bounds
      const boundedX = Math.max(
        notepadBounds.x,
        Math.min(x, notepadBounds.x + notepadBounds.width)
      );
      const boundedY = Math.max(
        notepadBounds.y,
        Math.min(y, notepadBounds.y + notepadBounds.height)
      );

      automation.moveMouse(boundedX, boundedY);

      // Type synchronized with animation
      await typeForPattern(automation, currentPattern, i / (starPoints * 5));

      await sleep(75);
    }

    // 10. LISSAJOUS CURVES + TYPING (3 seconds)
    console.log("*** LISSAJOUS CURVES + TYPING ***");
    currentPattern = 11;
    for (let i = 0; i < 300; i++) {
      const t = i * 0.05;
      const x = centerX + Math.sin(3 * t) * 150;
      const y = centerY + Math.sin(4 * t) * 100;

      // Keep within bounds
      const boundedX = Math.max(
        notepadBounds.x,
        Math.min(x, notepadBounds.x + notepadBounds.width)
      );
      const boundedY = Math.max(
        notepadBounds.y,
        Math.min(y, notepadBounds.y + notepadBounds.height)
      );

      automation.moveMouse(boundedX, boundedY);

      // Type synchronized with animation
      await typeForPattern(automation, currentPattern, i / 300);

      await sleep(10);
    }

    // 11. FINAL EXPLOSION + TYPING (3 seconds)
    console.log("*** FINAL EXPLOSION + TYPING ***");
    currentPattern = 12;
    for (let i = 0; i < 50; i++) {
      const angle = (i * 2 * Math.PI) / 50;
      const radius = i * 4;
      const x = centerX + Math.cos(angle) * radius;
      const y = centerY + Math.sin(angle) * radius;

      // Keep within bounds
      const boundedX = Math.max(
        notepadBounds.x,
        Math.min(x, notepadBounds.x + notepadBounds.width)
      );
      const boundedY = Math.max(
        notepadBounds.y,
        Math.min(y, notepadBounds.y + notepadBounds.height)
      );

      automation.moveMouse(boundedX, boundedY);

      // Type synchronized with animation
      await typeForPattern(automation, currentPattern, i / 50);

      await sleep(60);
    }

    // Complete any remaining text
    console.log("*** COMPLETING FINAL TEXT ***");
    while (typingIndex < featureText.length) {
      await typeChunk(
        automation,
        Math.min(20, featureText.length - typingIndex)
      );
      await sleep(5);
    }

    // Stop typing
    isTyping = false;

    // Final position
    automation.moveMouse(centerX, centerY);

    console.log("*** INSANE NOTEPAD DEMO COMPLETE! ***");
    console.log("*** PERFECT ANIMATION + TYPING SYNCHRONIZATION! ***");
    console.log("*** NOTEPAD TRANSCENDED REALITY! ***");
  } catch (error) {
    console.error("*** Demo failed:", error);
  }
};

// Run the demo
insaneMouseDemo().catch(console.error);

export { insaneMouseDemo };
