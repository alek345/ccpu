#!/bin/sh
gcc cpu.c screen.c main.c -lSDL2 -o ccpu
assembler/ccpu-asm rom.asm rom.bin 0xF000 -m
