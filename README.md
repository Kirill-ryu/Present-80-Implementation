# PRESENT-80 Implementation

## Project Overview
- **Objective:** To implement the PRESENT-80 ultra-lightweight block cipher from scratch to deeply understand its hardware-optimized SPN (Substitution-Permutation Network) structure, which is widely used in resource-constrained IoT environments.
- **Context:** Academic project for Security for IoT Edge Network at Dublin City University, first semester.

## Development Environment
- **Language:** C++ (Utilized `__int128_t` and `uint64_t` for bit-level processing)
- **OS:** Windows 10 (PowerShell)
- **Compiler/Tool:** GCC 15.2.0 (g++)

## Features & Scope
- **Key Size:** 80-bit
- **Block Size:** 64-bit
- **Block Cipher Mode:** Single Block Encryption (Base ECB)
- **Core Functions Implemented:**
  - `generateRoundKey()` (Extracts 64-bit round key from the 80-bit key register)
  - `addRoundKey()`
  - `sBox()` (Non-linear substitution layer using 4-bit S-boxes)
  - `pLayer()` (Linear bit permutation layer)
  - `KeyUpdate()` (Key scheduling for 31 rounds)
    - `Rotation()` (19-bit right rotation)
    - `Substitution()` (S-box application on the leftmost 4 bits)
    - `CounterXOR()` (XOR with the round counter)
  - `Present80()` (Main encryption wrapper function executing 31 rounds + final key whitening)

## How to Run
This project uses the `__int128_t` type, which requires a 64-bit GCC compiler.

```powershell
# Navigate to the project directory
$ cd "path/to/PRESENT-80/"

# Compile all .cpp files into an executable named 'main'
$ g++ *.cpp -o main

# Run the executable
$ .\main
