#!/bin/bash

mkdir -p build

CXX_FLAGS="-std=c++17 -fno-builtin -nostdlib -m32 -fno-rtti -fno-exceptions -fno-pie -fno-pic -ffreestanding"
INCLUDES="-Ikernel"

g++ $CXX_FLAGS $INCLUDES -c kernel/main.cxx -o build/main.o
g++ $CXX_FLAGS $INCLUDES -c kernel/init_globals.cxx -o build/init_globals.o
nasm -f elf32 start/start.asm -o build/start.o

ld build/main.o build/start.o build/init_globals.o -o build/kernel -melf_i386 -T link.ld
