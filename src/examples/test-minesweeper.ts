import {
  MinesweeperAutomation,
  minesweeperDemo,
} from "./minesweeper-automation";

async function runMinesweeperTest() {
  console.log("🎮 Quick Minesweeper Automation Test");
  console.log("===================================\n");

  console.log("💡 Instructions:");
  console.log("1. Open Windows Minesweeper (or any compatible version)");
  console.log("2. Start a new game");
  console.log("3. Run this script\n");

  console.log("🚀 Starting automation in 3 seconds...");
  await sleep(3000);

  // Run the full demo
  await minesweeperDemo();
}

function sleep(ms: number): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

// Run if called directly
if (require.main === module) {
  runMinesweeperTest().catch(console.error);
}

export { runMinesweeperTest };
