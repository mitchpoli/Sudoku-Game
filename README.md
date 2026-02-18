# Microprogrammed Embedded Systems – Sudoku Game  

## Overview

This project consists in the development of a Sudoku game implemented on a microprogrammed embedded architecture.

The objective was to design and implement:

- Low-level game logic
- Memory management
- Input handling
- Display control
- Microprogram control flow

All functionalities were developed under architectural constraints typical of embedded systems.

## System Architecture

- Custom microprogrammed CPU
- Low-level instruction set
- Memory-mapped I/O
- Manual stack and register handling
- No high-level runtime environment

The game logic interacts directly with hardware-level abstractions.

## Implemented Features

- Sudoku grid generation / loading
- User input validation
- Move legality checks
- Game state updates
- Win-condition detection
- Display refresh logic

## Technical Challenges

- Working without high-level libraries
- Managing memory explicitly
- Designing efficient control flow in microcode
- Handling I/O via memory-mapped registers
- Debugging at architectural level
