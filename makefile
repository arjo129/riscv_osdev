#####
## QEMU
#####
QEMU=qemu-system-riscv64
MACH=virt
CPU=rv64
CPUS=4
MEM=128M
DISK=hdd.dsk
DRIVE= -drive if=none,format=raw,file=$(DISK),id=foo -device virtio-blk-device,scsi=off,drive=foo
OPTS=-nographic -serial mon:stdio -bios none -device virtio-rng-device -device virtio-gpu-device
OPTS+=-device virtio-net-device -device virtio-tablet-device -device virtio-keyboard-device

####
## C Compiler
####
CFLAGS= -Wall -Wextra -pedantic -Wextra -O0 -g
CFLAGS+= -static -ffreestanding -nostdlib -fno-rtti -fno-exceptions
CFLAGS+= -march=rv64gc -mabi=lp64

SOURCES_ASM=boot.s main.c mmio.c uart.c
LINKER_SCRIPT= -Tlinker.ld

OUT=kernel.elf
cc=riscv-none-embed-gcc

all:
	$(cc) $(CFLAGS) $(LINKER_SCRIPT) -o $(OUT) $(SOURCES_ASM)

qemu:
	$(QEMU)	-s -S -machine $(MACH) -cpu $(CPU) -smp $(CPUS) -m $(MEM)  -nographic -serial mon:stdio -bios none -kernel $(OUT) -drive if=none,format=raw,file=$(DISK),id=foo -device virtio-blk-device,scsi=off,drive=foo