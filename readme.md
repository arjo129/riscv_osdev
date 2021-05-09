# RISC-V OS
Currently implements a simple round-robin multi-tasking RTOS in just a few files. Not meant for production use, rather meant for my own learning.

## Building
Make sure you have the `riscv-none-embed` toolchain from `xpack`. Run `make`.

## Debugging with VS Code

By default the makefile runs qemu in debug mode and waits for the debugger to attach
`make qemu`

If you use vscode you may use the following json as a reference for setting up GDB. This will allow you to set breakpoints through out the code.

```json
{

    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) Launch",
            "type": "cppdbg",
            "request": "launch",
            "program": "kernel.elf",
            "miDebuggerServerAddress": "localhost:1234",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "client": "riscv-none-embed-gdb",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "linux": {
                "miDebuggerPath": "/opt/riscv/bin/riscv-none-embed-gdb"
            },
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```
