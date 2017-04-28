#Introduction
This is an exercise of elephant, I named my OS kiwi because I like kiwi. :)
Anywaym have fun...

#SOURCES
For now, kiwi have three execute file--MBR, Loader and kernel. Complied to get these file,MBR and Loader are implemented in assembly, also part of kernel. Beside kernel, execute file are easy to get.For example:

```
nasm -I PATH_OF_INCLUDE -o mbr.bin mbr.S
nasm -I PATH_OF_INCLUDED -f loader.bin loader.S
```
For kernel, things get more complicate. Kernel is implementd in different language in different code file. Genetate the target file separately, then link all target file to get kernel.bin file, AKA kernel image. For example the kernel main file is main.c, main.c call print file which is implemented in print.S file , print function deckaration in print.h file, the type used in print.h file defined in stdint.S. In general, we have 4 files associated with the kernel--main.c, print.S, print.h and stdint.S. We get the kernel ad follows:

```
gcc -m32 -c -o main.o main.c		#-m32 specify target file is 32bit
nasm -f elf -o print.o print.S		#stdint.h, print.h and print.S has same path
ld -Ttext ADDRESS_KERNEL -e main -o kernel.bin print.o main.o
#-e specify _start 
```

These files need to be written to disk. Because of MBR need 512 byte, We argeed MBR is written into first sector, third sector prepare for Loader, it requires three sector. Kernel is stored tenth sector, need 200 sector. Just like this:

```
dd if=MBR.bin of=HARD_DISK.img bs=512 count=1 conv=notrunct
dd if=LOADER.bin of=HERD_DISK.img bs=512 seek=2 count=3 conv=notrunct
dd if=Kernel.bin of=HARD_DISK.img bs=512 seek=9 count=200 conv=notrunct
```
#NOTE
