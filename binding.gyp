{
  "targets": [
    {
      "target_name": "memory_hooking",
      "sources": [
        "src/native/memory_hooking.cpp",
        "src/native/process_manager.cpp",
        "src/native/memory_scanner.cpp",
        "src/native/error_handler.cpp",
        "src/native/memory_helpers.cpp",
        "src/native/automation_helpers.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "src/native/include"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
      "conditions": [
        ["OS=='win'", {
          "libraries": [
            "-lkernel32",
            "-lpsapi",
            "-ladvapi32"
          ],
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1
            }
          }
        }]
      ]
    }
  ]
} 