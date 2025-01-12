# Custom 32-bit CPU with Assembler and OpenGL Graphics

<div align="center">
  <img src="assets/eagle32.webp" alt="Header" width="350"/>
</div>

This project is a **32-bit CPU** emulator written in C, featuring:

- An extended set of instructions (e.g., `MOV`, `MOVI`, `ADD`, `SUB`, `AND`, `OR`, `XOR`, `NOT`, `SHL`, `SHR`, `LOAD`, `STORE`, `JMP`, `JEQ`, `JNE`, `PUSH`, `POP`, `CALL`, `RET`, `DRAW`)  
- An ALU with **proper flag handling** (Zero, Carry/Borrow, Sign, Overflow)  
- A Control Unit and **general-purpose registers**  
- A **unified bus** for memory and I/O  
- **SDL2 + OpenGL** for **2D rendering** (the `DRAW` instruction allows pixel-level drawing in a window)  
- A **standalone Assembler** that converts assembly code into a `.bin` file executable by the emulator

## Main Features

1. **Emulated CPU**  
   - A broader instruction set with arithmetic, logical, conditional jumps, and subroutines.  
   - A simple pipeline: fetch, decode, execute.

2. **Memory & Bus**  
   - Simulates a linear memory space (up to 1 MB) and a bus for communicating with I/O and other devices.  

3. **I/O & Interrupts (future)**  
   - Currently offers basic I/O functions (`io_read`, `io_write`).  
   - Potential future extensions for interrupts and more advanced peripherals.

4. **Graphics**  
   - Uses SDL2 and OpenGL for 2D drawing in orthographic mode.  
   - The `DRAW R_x, R_y, R_color` instruction draws a single pixel in real time.

5. **Assembler**  
   - A two-pass assembler that supports label resolution, `.db` directives for raw data, etc.  
   - Translates a subset of assembly instructions into a machine code file (`.bin`).

## Example: Drawing a “Hollow Circle” (Ring)

In the repository, you’ll find an assembly example (`circle.asm`) that draws a red ring in the center of the screen, using instructions like `CALL mul`, `JEQ`, and `DRAW`.  
By compiling that assembly with the assembler, you get a `.bin` file that, when loaded into the emulator, shows the ring.

## How to Build

1. **Emulator**:  
   - Use your build system (e.g., `make`) to generate the `emulator` executable.  
2. **Assembler**:  
   - Likewise, compile the `assembler` project to get an `assembler` executable.  
3. **Execution**:  
   - Assemble an assembly file into a `.bin` file:  
     ```bash
     ./assembler myprog.asm myprog.bin
     ```
   - Run it in the emulator:  
     ```bash
     ./emulator myprog.bin
     ```

## Current Status

- **Instruction Set**: Complete with basic ops, conditionals, subroutines, and `DRAW`.  
- **ALU with Flags**: Implemented with Zero, Carry/Borrow, Sign, and Overflow.  
- **Assembler**: Fully functional two-pass assembly, supporting `.db` for raw data.  
- **Graphics**: Pixel drawing with basic examples (drawing lines, circles/rings, etc.).

## Future Extensions

- **Hardware interrupts** and asynchronous I/O/timer handling  
- **Debug mode** with integrated disassembler and breakpoints  
- **More complex pipeline** (stalling, forwarding, etc.)  
- **Caching, paging**, and memory protection for an instructional micro-OS

---
