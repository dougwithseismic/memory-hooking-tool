const { MemoryHooking } = require("./dist/index.js");
const { KeyCode } = require("./dist/types/index.js");

console.log("=== Simple Key Test ===");

const memHook = new MemoryHooking();

console.log("KeyCode.Enter value:", KeyCode.Enter);
console.log("Number(KeyCode.Enter) value:", Number(KeyCode.Enter));
console.log("typeof KeyCode.Enter:", typeof KeyCode.Enter);

try {
  console.log(
    "Testing pressKey with numeric value 13 and explicit duration..."
  );
  const result1 = memHook.pressKey(13, 0);
  console.log("Direct numeric result:", result1);
} catch (error) {
  console.log("Direct numeric error:", error.message);
}

try {
  console.log("Testing pressKey with KeyCode.Enter and explicit duration...");
  const result2 = memHook.pressKey(KeyCode.Enter, 0);
  console.log("KeyCode.Enter result:", result2);
} catch (error) {
  console.log("KeyCode.Enter error:", error.message);
}

try {
  console.log("Testing with undefined duration...");
  const result3 = memHook.pressKey(13, undefined);
  console.log("Undefined duration result:", result3);
} catch (error) {
  console.log("Undefined duration error:", error.message);
}

console.log("=== Test Complete ===");
