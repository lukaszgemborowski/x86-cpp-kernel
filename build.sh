#!/bin/bash

mkdir -p build

CXX_FLAGS="-std=c++17 -fno-builtin -nostdlib -m32 -fno-rtti -fno-exceptions -fno-pie -fno-pic -ffreestanding"
INCLUDES="-Ikernel -I. -Iext/cpptoolbox/include/"

g++ $CXX_FLAGS $INCLUDES -c kernel/main.cxx -o build/main.o
g++ $CXX_FLAGS $INCLUDES -c kernel/init_globals.cxx -o build/init_globals.o
nasm -f elf32 start/start.asm -o build/start.o
nasm -f elf32 kernel/x86/isrs.asm -o build/isrs.o

ld build/main.o build/start.o build/init_globals.o build/isrs.o -o build/kernel -melf_i386 -T link.ld
