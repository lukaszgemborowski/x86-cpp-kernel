#!/bin/bash

mkdir -p build

g++ -std=c++17 -c main.cxx -o build/main.o -fno-builtin -nostdlib -m32 -fno-rtti -fno-exceptions -fno-pie -fno-pic -ffreestanding
g++ -std=c++17 -c init_globals.cxx -o build/init_globals.o -fno-builtin -nostdlib -m32 -fno-rtti -fno-exceptions -fno-pie -fno-pic -ffreestanding
nasm -f elf32 start.asm -o build/start.o

ld build/main.o build/start.o build/init_globals.o -o build/kernel -melf_i386 -T link.ld
