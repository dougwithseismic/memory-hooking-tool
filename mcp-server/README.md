# Memory Automation MCP Server

A comprehensive Model Context Protocol (MCP) server that exposes powerful memory hooking and system automation capabilities for AI assistants.

## Features

### 🔍 Process Management
- List all running processes
- Find processes by name or pattern
- Attach to processes for memory access
- Get detailed process information

### 🧠 Memory Operations
- Read/write memory with multiple data types (int32, int64, float, double, string, bytes)
- Advanced memory scanning (patterns, strings, values)
- Memory region analysis
- Pointer chain following
- Find pointers to specific addresses

### 🖱️ System Automation
- **Mouse Control**: Move, click, get position
- **Keyboard Control**: Send keys, text input
- **Screen Analysis**: Screenshots, pixel colors, color finding
- **Window Management**: List, find, control windows
- **System Information**: Get screen resolution, OS info

### ⚡ Advanced Capabilities
- Wait for conditions (windows, colors, key presses)
- Real-time system monitoring
- Error handling and safety features

## Installation

```bash
npm install
npm run build
```

## Usage

### With Claude Desktop

Add to your Claude Desktop configuration:

```json
{
  "mcpServers": {
    "memory-automation": {
      "command": "node",
      "args": ["dist/index.js"],
      "cwd": "/path/to/memory-hooking-tool/mcp-server"
    }
  }
}
```

### Standalone

```bash
npm start
```

## Tool Categories

### Process Management Tools
- `list_processes` - List all running processes
- `find_process` - Find process by name/pattern
- `attach_process` - Attach to process for memory access
- `get_process_info` - Get attached process details

### Memory Access Tools
- `read_memory` - Read memory with type safety
- `write_memory` - Write data to memory
- `scan_memory_pattern` - Search byte patterns
- `scan_memory_string` - Search text strings
- `scan_memory_value` - Search numeric values
- `get_memory_regions` - Analyze memory layout
- `find_pointers` - Find pointers to addresses
- `follow_pointer_chain` - Follow pointer chains

### Automation Tools
- `move_mouse` / `click_mouse` - Mouse control
- `send_key` / `send_text` - Keyboard input
- `take_screenshot` - Screen capture
- `get_pixel_color` / `find_color` - Screen analysis
- `list_windows` / `find_window` / `control_window` - Window management
- `get_system_info` - System information
- `wait_for_condition` - Conditional waiting

## Security Considerations

This tool provides powerful system access capabilities including:
- Memory reading/writing of arbitrary processes
- System automation and control
- Screen capture and analysis

**Use responsibly and only for:**
- Legitimate testing and debugging
- Security research and analysis
- Automation of your own applications
- Educational purposes

**Do not use for:**
- Unauthorized access to systems
- Malicious activities
- Violating software licenses
- Any illegal purposes

## Requirements

- Windows operating system
- Node.js 18+
- Administrator privileges (for memory operations)

## License

GPL-3.0 - See LICENSE file for details.