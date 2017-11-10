.PHONY: all clean

CXX=i386-unknown-elf-g++
AS=nasm
LD=i386-unknown-elf-ld

SRC_KERNEL= \
	kernel/main.cxx \
	kernel/init_globals.cxx

KERNEL_BIN=kernel.elf

SRC_START=start/start.asm
OBJ_START=start/start.o

OBJ_KERNEL=$(SRC_KERNEL:.cxx=.o)

CXX_FLAGS=-std=c++17 -fno-builtin -nostdlib -m32 -fno-rtti -fno-exceptions -fno-pie -fno-pic -ffreestanding
INCLUDES=-Ikernel -Iext/cpptoolbox/include

all: $(KERNEL_BIN)

$(OBJ_KERNEL): %.o: %.cxx
	$(CXX) $(CXX_FLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_START): $(SRC_START)
	$(AS) -felf32 $(SRC_START) -o $(OBJ_START)

$(KERNEL_BIN): $(OBJ_START) $(OBJ_KERNEL)
	$(LD) -melf_i386 -T link.ld $(OBJ_START) $(OBJ_KERNEL) -o $(KERNEL_BIN)

clean:
	rm -f $(OBJ_START) $(OBJ_KERNEL) $(KERNEL_BIN)
