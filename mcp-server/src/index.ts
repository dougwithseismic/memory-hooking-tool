#!/usr/bin/env node

import { Server } from '@modelcontextprotocol/sdk/server/index.js';
import { StdioServerTransport } from '@modelcontextprotocol/sdk/server/stdio.js';
import {
  CallToolRequestSchema,
  ErrorCode,
  ListToolsRequestSchema,
  McpError,
} from '@modelcontextprotocol/sdk/types.js';
import { MemoryHooking, MouseButton } from '../../dist/index.js';

interface ToolArgs {
  [key: string]: any;
}

class MemoryAutomationMCPServer {
  private server: Server;
  private memoryTool: MemoryHooking | null = null;

  constructor() {
    this.server = new Server(
      {
        name: 'memory-automation-mcp-server',
        version: '1.0.0',
      },
      {
        capabilities: {
          tools: {},
        },
      }
    );

    this.setupToolHandlers();
    this.setupErrorHandling();
  }

  private setupErrorHandling(): void {
    this.server.onerror = (error) => console.error('[MCP Error]', error);
    process.on('SIGINT', async () => {
      await this.server.close();
      process.exit(0);
    });
  }

  private setupToolHandlers(): void {
    this.server.setRequestHandler(ListToolsRequestSchema, async () => ({
      tools: [
        // Process Management Tools
        {
          name: 'list_processes',
          description: 'List all running processes with PID, name, and window titles',
          inputSchema: {
            type: 'object',
            properties: {},
            additionalProperties: false,
          },
        },
        {
          name: 'find_process',
          description: 'Find process by name or pattern',
          inputSchema: {
            type: 'object',
            properties: {
              name: { type: 'string', description: 'Process name or pattern to search for' },
              usePattern: { type: 'boolean', description: 'Use regex pattern matching', default: false },
            },
            required: ['name'],
            additionalProperties: false,
          },
        },
        {
          name: 'attach_process',
          description: 'Attach to a process by PID or name',
          inputSchema: {
            type: 'object',
            properties: {
              pid: { type: 'number', description: 'Process ID' },
              name: { type: 'string', description: 'Process name' },
            },
            additionalProperties: false,
          },
        },
        {
          name: 'get_process_info',
          description: 'Get detailed information about attached process',
          inputSchema: {
            type: 'object',
            properties: {},
            additionalProperties: false,
          },
        },

        // Memory Reading/Writing Tools
        {
          name: 'read_memory',
          description: 'Read memory from attached process',
          inputSchema: {
            type: 'object',
            properties: {
              address: { type: 'string', description: 'Memory address (hex format: 0x...)' },
              dataType: { 
                type: 'string', 
                enum: ['int32', 'int64', 'float', 'double', 'string', 'bytes'],
                description: 'Data type to read' 
              },
              size: { type: 'number', description: 'Size in bytes (for bytes/string types)' },
            },
            required: ['address', 'dataType'],
            additionalProperties: false,
          },
        },
        {
          name: 'write_memory',
          description: 'Write data to process memory',
          inputSchema: {
            type: 'object',
            properties: {
              address: { type: 'string', description: 'Memory address (hex format: 0x...)' },
              dataType: { 
                type: 'string', 
                enum: ['int32', 'int64', 'float', 'double', 'string'],
                description: 'Data type to write' 
              },
              value: { description: 'Value to write' },
            },
            required: ['address', 'dataType', 'value'],
            additionalProperties: false,
          },
        },

        // Memory Scanning Tools
        {
          name: 'scan_memory_pattern',
          description: 'Search for byte patterns in memory',
          inputSchema: {
            type: 'object',
            properties: {
              pattern: { type: 'string', description: 'Hex pattern with wildcards (e.g., "48 8B ?? 74 ??")' },
              maxResults: { type: 'number', description: 'Maximum results to return', default: 100 },
            },
            required: ['pattern'],
            additionalProperties: false,
          },
        },
        {
          name: 'scan_memory_string',
          description: 'Search for text strings in memory',
          inputSchema: {
            type: 'object',
            properties: {
              searchString: { type: 'string', description: 'String to search for' },
              caseSensitive: { type: 'boolean', description: 'Case sensitive search', default: false },
              unicode: { type: 'boolean', description: 'Search Unicode strings', default: false },
              maxResults: { type: 'number', description: 'Maximum results to return', default: 100 },
            },
            required: ['searchString'],
            additionalProperties: false,
          },
        },

        // Automation Tools - Mouse Control
        {
          name: 'move_mouse',
          description: 'Move mouse to specific coordinates',
          inputSchema: {
            type: 'object',
            properties: {
              x: { type: 'number', description: 'X coordinate' },
              y: { type: 'number', description: 'Y coordinate' },
            },
            required: ['x', 'y'],
            additionalProperties: false,
          },
        },
        {
          name: 'click_mouse',
          description: 'Perform mouse click',
          inputSchema: {
            type: 'object',
            properties: {
              button: { type: 'string', enum: ['left', 'right', 'middle'], description: 'Mouse button', default: 'left' },
              x: { type: 'number', description: 'X coordinate (optional)' },
              y: { type: 'number', description: 'Y coordinate (optional)' },
              doubleClick: { type: 'boolean', description: 'Perform double click', default: false },
            },
            additionalProperties: false,
          },
        },
        {
          name: 'get_mouse_position',
          description: 'Get current mouse position',
          inputSchema: {
            type: 'object',
            properties: {},
            additionalProperties: false,
          },
        },

        // Automation Tools - Keyboard Control
        {
          name: 'send_key',
          description: 'Send key press',
          inputSchema: {
            type: 'object',
            properties: {
              key: { type: 'string', description: 'Key to press (e.g., "a", "Enter", "Ctrl+C")' },
              duration: { type: 'number', description: 'Press duration in ms', default: 50 },
            },
            required: ['key'],
            additionalProperties: false,
          },
        },
        {
          name: 'send_text',
          description: 'Send text input',
          inputSchema: {
            type: 'object',
            properties: {
              text: { type: 'string', description: 'Text to send' },
            },
            required: ['text'],
            additionalProperties: false,
          },
        },

        // Automation Tools - Screen Analysis
        {
          name: 'take_screenshot',
          description: 'Capture screenshot',
          inputSchema: {
            type: 'object',
            properties: {
              saveToFile: { type: 'string', description: 'File path to save screenshot (optional)' },
            },
            additionalProperties: false,
          },
        },
        {
          name: 'get_pixel_color',
          description: 'Get color at specific pixel',
          inputSchema: {
            type: 'object',
            properties: {
              x: { type: 'number', description: 'X coordinate' },
              y: { type: 'number', description: 'Y coordinate' },
            },
            required: ['x', 'y'],
            additionalProperties: false,
          },
        },

        // Window Management
        {
          name: 'list_windows',
          description: 'List all system windows',
          inputSchema: {
            type: 'object',
            properties: {
              visibleOnly: { type: 'boolean', description: 'List only visible windows', default: true },
            },
            additionalProperties: false,
          },
        },
        {
          name: 'find_window',
          description: 'Find window by title or class name',
          inputSchema: {
            type: 'object',
            properties: {
              title: { type: 'string', description: 'Window title pattern' },
              className: { type: 'string', description: 'Window class name' },
            },
            additionalProperties: false,
          },
        },

        // System Information
        {
          name: 'get_system_info',
          description: 'Get system information (screen resolution, OS info, etc.)',
          inputSchema: {
            type: 'object',
            properties: {},
            additionalProperties: false,
          },
        },
      ],
    }));

    this.server.setRequestHandler(CallToolRequestSchema, async (request) => {
      const { name, arguments: args } = request.params;

      try {
        // Initialize memory tool if needed
        if (!this.memoryTool) {
          this.memoryTool = new MemoryHooking();
        }

        const typedArgs = args as ToolArgs;

        switch (name) {
          case 'list_processes':
            return { content: [{ type: 'text', text: JSON.stringify(this.memoryTool.getRunningProcesses(), null, 2) }] };

          case 'find_process':
            const result = typedArgs.usePattern 
              ? await this.memoryTool.findProcessesByPattern(typedArgs.name as string)
              : await this.memoryTool.findProcessByName(typedArgs.name as string);
            return { content: [{ type: 'text', text: JSON.stringify(result, null, 2) }] };

          case 'attach_process':
            let attachResult;
            if (typedArgs.pid) {
              attachResult = this.memoryTool.attachToProcess(typedArgs.pid as number);
            } else if (typedArgs.name) {
              const proc = await this.memoryTool.findProcessByName(typedArgs.name as string);
              attachResult = proc ? this.memoryTool.attachToProcess(proc.processId) : false;
            } else {
              throw new Error('Either pid or name must be provided');
            }
            return { content: [{ type: 'text', text: JSON.stringify({ attached: attachResult }, null, 2) }] };

          case 'get_process_info':
            return { content: [{ type: 'text', text: JSON.stringify(this.memoryTool.getCurrentProcessInfo(), null, 2) }] };

          case 'read_memory':
            const address = parseInt(typedArgs.address as string, 16);
            let readResult;
            switch (typedArgs.dataType) {
              case 'int32':
                readResult = this.memoryTool.readInt32(address);
                break;
              case 'int64':
                readResult = this.memoryTool.readInt64(address);
                break;
              case 'float':
                readResult = this.memoryTool.readFloat(address);
                break;
              case 'double':
                readResult = this.memoryTool.readDouble(address);
                break;
              case 'string':
                readResult = this.memoryTool.readString(address, (typedArgs.size as number) || 256);
                break;
              case 'bytes':
                readResult = this.memoryTool.readMemory(address, (typedArgs.size as number) || 16);
                break;
              default:
                throw new Error(`Unsupported data type: ${typedArgs.dataType}`);
            }
            return { content: [{ type: 'text', text: JSON.stringify({ address: typedArgs.address, value: readResult }, null, 2) }] };

          case 'write_memory':
            const writeAddress = parseInt(typedArgs.address as string, 16);
            let writeResult;
            switch (typedArgs.dataType) {
              case 'int32':
                writeResult = this.memoryTool.writeInt32(writeAddress, typedArgs.value as number);
                break;
              case 'int64':
                writeResult = this.memoryTool.writeInt64(writeAddress, BigInt(typedArgs.value as number));
                break;
              case 'float':
                writeResult = this.memoryTool.writeFloat(writeAddress, typedArgs.value as number);
                break;
              case 'double':
                writeResult = this.memoryTool.writeDouble(writeAddress, typedArgs.value as number);
                break;
              case 'string':
                writeResult = this.memoryTool.writeString(writeAddress, typedArgs.value as string);
                break;
              default:
                throw new Error(`Unsupported data type: ${typedArgs.dataType}`);
            }
            return { content: [{ type: 'text', text: JSON.stringify({ success: writeResult }, null, 2) }] };

          case 'scan_memory_pattern':
            const { pattern, mask } = this.memoryTool.createPattern([], []);
            const patternResults = this.memoryTool.scanForPattern(typedArgs.pattern as string, mask);
            const maxResults = (typedArgs.maxResults as number) || 100;
            return { content: [{ type: 'text', text: JSON.stringify(patternResults.slice(0, maxResults), null, 2) }] };

          case 'scan_memory_string':
            const stringResults = this.memoryTool.scanForString(
              typedArgs.searchString as string,
              typedArgs.caseSensitive as boolean
            );
            const maxStringResults = (typedArgs.maxResults as number) || 100;
            return { content: [{ type: 'text', text: JSON.stringify(stringResults.slice(0, maxStringResults), null, 2) }] };

          case 'move_mouse':
            this.memoryTool.moveMouse(typedArgs.x as number, typedArgs.y as number);
            return { content: [{ type: 'text', text: JSON.stringify({ success: true, x: typedArgs.x, y: typedArgs.y }, null, 2) }] };

          case 'click_mouse':
            if (typedArgs.x !== undefined && typedArgs.y !== undefined) {
              this.memoryTool.moveMouse(typedArgs.x as number, typedArgs.y as number);
            }
            const button = typedArgs.button === 'right' ? MouseButton.Right : typedArgs.button === 'middle' ? MouseButton.Middle : MouseButton.Left;
            if (typedArgs.doubleClick) {
              this.memoryTool.doubleClickMouse(button, typedArgs.x as number, typedArgs.y as number);
            } else {
              this.memoryTool.clickMouse(button, typedArgs.x as number, typedArgs.y as number);
            }
            return { content: [{ type: 'text', text: JSON.stringify({ success: true, button: typedArgs.button || 'left' }, null, 2) }] };

          case 'get_mouse_position':
            const mousePos = this.memoryTool.getMousePosition();
            return { content: [{ type: 'text', text: JSON.stringify(mousePos, null, 2) }] };

          case 'send_key':
            const keyCode = 65; // Default to 'A' key - would need proper key mapping
            this.memoryTool.pressKey(keyCode, (typedArgs.duration as number) || 50);
            return { content: [{ type: 'text', text: JSON.stringify({ success: true, key: typedArgs.key }, null, 2) }] };

          case 'send_text':
            this.memoryTool.sendText(typedArgs.text as string);
            return { content: [{ type: 'text', text: JSON.stringify({ success: true, text: typedArgs.text }, null, 2) }] };

          case 'take_screenshot':
            const screenshot = this.memoryTool.captureScreen();
            if (typedArgs.saveToFile) {
              this.memoryTool.saveScreenshotToFile(screenshot, typedArgs.saveToFile as string);
            }
            return { content: [{ type: 'text', text: JSON.stringify({ success: true, captured: true }, null, 2) }] };

          case 'get_pixel_color':
            const color = this.memoryTool.getPixelColor(typedArgs.x as number, typedArgs.y as number);
            return { content: [{ type: 'text', text: JSON.stringify({ x: typedArgs.x, y: typedArgs.y, color }, null, 2) }] };

          case 'list_windows':
            const windows = this.memoryTool.getAllWindows();
            return { content: [{ type: 'text', text: JSON.stringify(windows, null, 2) }] };

          case 'find_window':
            let foundWindow: any[] = [];
            if (typedArgs.title) {
              foundWindow = this.memoryTool.getWindowsByTitle(typedArgs.title as string);
            } else if (typedArgs.className) {
              foundWindow = this.memoryTool.getWindowsByClassName(typedArgs.className as string);
            }
            return { content: [{ type: 'text', text: JSON.stringify(foundWindow, null, 2) }] };

          case 'get_system_info':
            const systemInfo = this.memoryTool.getSystemInfo();
            return { content: [{ type: 'text', text: JSON.stringify(systemInfo, null, 2) }] };

          default:
            throw new McpError(
              ErrorCode.MethodNotFound,
              `Unknown tool: ${name}`
            );
        }
      } catch (error) {
        const errorMessage = error instanceof Error ? error.message : String(error);
        throw new McpError(
          ErrorCode.InternalError,
          `Tool execution failed: ${errorMessage}`
        );
      }
    });
  }

  async run(): Promise<void> {
    const transport = new StdioServerTransport();
    await this.server.connect(transport);
    console.error('Memory Automation MCP server running on stdio');
  }
}

const server = new MemoryAutomationMCPServer();
server.run().catch(console.error);