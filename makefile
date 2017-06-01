
.PHONY: all

all:build/kernel.bin 

build/kernel.bin:build/init.o build/interrupt.o build/kernel.o build/main.o build/print.o build/timer.o build/debug.o
	ld -m elf_i386 -Ttext 0xc0001500 -e main -o build/kernel.bin build/init.o  build/interrupt.o \
		build/kernel.o  build/main.o  build/print.o build/timer.o build/debug.o
		cp build/kernel.bin /home/shiwen/bochs
		@echo "create kernel image done"

build/interrupt.o:kernel/interrupt.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/interrupt.o \
		kernel/interrupt.c
build/init.o:kernel/init.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/init.o kernel/init.c
build/main.o:kernel/main.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/main.o kernel/main.c
build/timer.o:device/timer.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/timer.o device/timer.c

build/debug.o:kernel/debug.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/debug.o kernel/debug.c

build/kernel.o:kernel/kernel.S
	nasm -f elf32 -o build/kernel.o kernel/kernel.S
build/print.o:lib/print.S
	nasm -f elf32 -o build/print.o lib/print.S


clean:
	rm build/*.o
	rm build/kernel.bin
