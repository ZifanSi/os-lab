Operating Systems (SFWRENG 3SH3) Assignment 4
Indexed Allocation File System Simulator

This assignment implements a simple flat file system in C using indexed allocation.

Included Files
- fs_indexed.h
  Contains the structs, constants, and function declarations.

- fs_indexed.c
  Contains the implementation of the file system functions.

- main.c
  Contains test cases used to check file creation, deletion, listing, and free block recovery.

- Makefile
  Used to compile and run the program.

Mark Breakdown
- File system operations: 20 marks
- Utility functions: 10 marks
- Test cases in main.c: 5 marks
- Makefile: 5 marks

How to Run

Using Makefile:
1. Open a terminal in the assignment folder.
2. Type:
   make
3. Run the program with:
   ./fs_sim

You can also type:
   make run

Manual compile:
1. Type:
   gcc -Wall -Wextra -std=c11 fs_indexed.c main.c -o fs_sim
2. Then run:
   ./fs_sim

What the Program Does
- Initializes the file system
- Creates files using indexed allocation
- Deletes files and returns blocks back to the free list
- Lists files in the root directory
- Prints the current free block list

System Settings
- Total blocks: 64
- Block size: 1024 bytes
- Maximum files: 16