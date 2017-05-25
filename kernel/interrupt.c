/*#########################################################################
# File Name: interrupt.c
# Author: echo zhang
# mail: wenshizhang555@hoxmail.com
# Created Time: Sun 07 May 2017 08:23:59 PM CST
# Description:
#########################################################################*/


#include "interrupt.h"
#include "stdint.h"
#include "global.h"
#include "io.h"
#include "print.h"

#define		IDT_DESC_CNT	0x21			//目前支持的中断总数

#define		PIC_M_CTRL		0x20			//主片控制端口是0x20
#define		PIC_M_DATA		0x21			//主片数据端口是0x21
#define		PIC_S_CTRL		0xa0			//从片控制端口是0xa0
#define		PIC_S_DATA		0xa1			//从片数据端口是0xa1

//中断门描述符结构体
struct gate_desc
{
	uint16_t	func_offset_low_word;
	uint16_t	selector;
	uint8_t		dcount;					//门描述符第四字节，固定值
	uint8_t		attribute;
	uint16_t	func_offset_high_word;
};

static void	make_idt_desc(struct gate_desc *p_gdesc,uint8_t attr, intr_handler function);
static struct gate_desc idt[IDT_DESC_CNT];			//idt是中断描述符表

extern intr_handler intr_entry_table[IDT_DESC_CNT];


//初始化可编程中断控制器8259A
static void pic_init(void)
{
	//init master
	outb (PIC_M_CTRL,0x11);			//边沿触发，级联模式
	outb (PIC_M_DATA,0x20);			//起始中断向量号是0x20，因此IR[0~7]是0x20~0x27	
	outb (PIC_M_DATA,0x04);			//IR2接从片
	outb (PIC_M_DATA,0x01);			//正常的EOI，8086模式

	//init slave
	outb (PIC_S_CTRL, 0x11);		//和slave一样,边沿出发级联模式
	outb (PIC_S_CTRL, 0x28);		//从片起始中断向量号是0x27
	outb (PIC_S_CTRL, 0x02);		//设置从片连接到主片的IR2引脚
	outb (PIC_S_CTRL, 0x01);		//正常的EOI，8086模式

	//打开主片的IR0，意味着屏蔽除了时钟中断之外的所有中断
	outb (PIC_M_DATA, 0xfe);
	outb (PIC_S_DATA,0xff);
}


//创建中断门描述符
static void make_idt_desc(struct gate_desc * p_gdesc, uint8_t attr,intr_handler function)
{
	p_gdesc->func_offset_low_word = (uint32_t)function &0x0000ffff;
	p_gdesc->dcount = 0;
	p_gdesc->attribute = attr;
	p_gdesc->func_offset_high_word = ((uint32_t)function & 0xffff0000) >> 16;
}

//初始化中断描述符
static void idt_desc_init(void)
{
	int i;
	for (i = 0; i < IDT_DESC_CNT;i++)
	{
		make_idt_desc(&idt[i],IDT_DESC_DPL0,intr_entry_table[i]);
	}
	put_str("	idt_desc_init done\n");
}

//完成有关中断的初始化工作
void idt_init()
{
	put_str("idt_init start\n");
	idt_desc_init();
	pic_init();

	//加载idt
	uint64_t idt_operand = ((sizeof(idt)-1) | ((uint64_t)((uint32_t)idt << 16)));
	asm volatile("lidt %0"::"m"(idt_operand));
	put_str("idt_init done\n");
}