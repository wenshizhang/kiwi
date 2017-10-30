/*******************机器模式******************/
#ifndef __LIB_IO_H
#define __LIB_IO_H
#include "stdint.h"


/*************************内联汇编格式***************
 * asm volatile(
		汇编语言模板
		[：输出部分]
		[：输人部分]
		[：破坏描述])
 */


//向端口port中写入一个字节的data
static inline void outb(uint16_t port,uint8_t data)
{
	/*
	 * 端口N指定0~255，d表示dx存储端口号 
	 * %b0对应al，%w1对应dx
	 */
	asm volatile("outb %b0, %w1"::"a"(data),"Nd"(port));
}

//讲addr起始的word_cnt个字写入port端口
static inline void outsw(uint16_t port, const void* addr,uint32_t word_cnt)
{
	/*
	 * +表示此限制既作输入又做输出
	 * outsw是把ds:esi处的16位内容写入了port端口，因此在设置段描述符时
	 * 已经把ds，es，ss段的选择子都设置为相同的值了此时不用担心数据混乱
	 */

	asm volatile ("cld;rep outsw" : "+S"(addr),"+c"(word_cnt) : "d"(port));
}

//从端口port中读一字节数据并发挥
static inline uint8_t inb(uint16_t port)
{
	uint8_t data;
	asm volatile("inb %w1,%b0" : "=a"(data) : "Nd"(port));
	return data;
}

//从port端口中读出word_cnt数据，存储起始地址为addr的内存处
static inline void insw(uint16_t port,void* addr,uint32_t word_cnt)
{
	asm volatile ("cld;rep insw" : "+D"(addr), "+c"(word_cnt) : "d"(port) : "memory");
}

#endif
