#include "timer.h"
#include "io.h"
#include "print.h"

#define IRQ0_FREQUENCY		100
#define INPUT_FREQUENCY		1193180
#define	COUNTER0_VALUE		INPUT_FREQUENCY / IRQ0_FREQUENCY
#define COUNTER0_PORT		0x40
#define COUNTER0_NO			0
#define COUNTER0_MODE		2
#define READ_WRITE_LATCH	3
#define PIT_CONTROL_PORT	0x43

/*把操作计数器counter_no，读写锁属性rwl、计数器模式counter_mode 
  写入模式控制寄存器并赋予初始值counter_value*/

static void frequency_set(uint8_t counter_port,
						 uint8_t counter_no,
						 uint8_t rwl,
						 uint8_t counter_mode,
						 uint16_t counter_value)
{
	//控制寄存器端口0x43写入控制字
	outb(PIT_CONTROL_PORT, (uint8_t)(counter_no << 6 | rwl<<4|counter_mode << 1));
	//写入counter_value低8位
	outb(counter_port,(uint8_t )counter_value);
	//写入counter_value高8位
	outb(counter_port,(uint8_t)counter_value >> 8);
}

void timer_init()
{
//	put_str("timer init start\n");

	frequency_set(COUNTER0_PORT,COUNTER0_NO,READ_WRITE_LATCH,COUNTER0_MODE,COUNTER0_VALUE);

//	put_str("timer init done\n");
}

