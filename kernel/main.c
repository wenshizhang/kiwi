#include "print.h"
#include "init.h"
#include "debug.h"

void main(void)
{
	put_str("I am kernel\n");
	init_all();

	put_str("before assert\n");
	ASSERT(1==2);
	put_str("after assert\n");

//	put_str("init all done\n");
	put_str("nnnnnnnnnnnnnnnnnnnnnnnnnnnnn");
	asm volatile("sti");				//为演示中断处理，临时开中断
	while(1);
}
