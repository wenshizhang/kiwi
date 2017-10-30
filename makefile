
.PHONY: all

all:build/kernel.bin 

build/kernel.bin:build/main.o build/print.o build/kernel.o build/init.o build/interrupt.o \
		build/timer.o build/debug.o build/string.o build/bitmap.o build/memory.o build/thread.o build/list.o
	ld -m elf_i386 -Ttext 0xc0001500 -e main -o build/kernel.bin build/main.o  build/interrupt.o \
	 build/init.o  build/print.o build/kernel.o build/timer.o build/debug.o build/string.o build/bitmap.o build/memory.o build/thread.o build/list.o
		cp build/kernel.bin /home/shiwen/bochs 
		@echo "create kernel image done"

#build/kernel.bin:build/main.o build/print.o build/kernel.o build/init.o build/interrupt.o
#	ld -m elf_i386 -Ttext 0xc0001500 -e main -o build/kernel.bin build/main.o  build/print.o \
		build/kernel.o
#		cp build/kernel.bin /home/shiwen/bochs 
#		@echo "create kernel image done"

build/interrupt.o:kernel/interrupt.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/interrupt.o \
		kernel/interrupt.c
build/init.o:kernel/init.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/init.o kernel/init.c
build/debug.o:kernel/debug.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/debug.o kernel/debug.c
build/main.o:kernel/main.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -I thread/ -c -fno-builtin -o build/main.o kernel/main.c
build/timer.o:device/timer.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/timer.o device/timer.c
build/string.o:lib/string.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/string.o lib/string.c
build/bitmap.o:lib/bitmap.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/bitmap.o lib/bitmap.c
build/memory.o:kernel/memory.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/memory.o kernel/memory.c
build/thread.o:thread/thread.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/thread.o thread/thread.c
build/list.o:lib/list.c
	gcc -m32 -I lib/ -I lib/kernel/ -I kernel/ -I device/ -c -fno-builtin -o build/list.o lib/list.c


build/kernel.o:kernel/kernel.S
	nasm -f elf32 -o build/kernel.o kernel/kernel.S
build/print.o:lib/print.S
	nasm -f elf32 -o build/print.o lib/print.S


clean:
	rm build/*.o
	rm build/kernel.bin
