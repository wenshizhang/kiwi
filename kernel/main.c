#include	"print.h"
#include	"init.h"

void main(void)
{
	put_str("I am kernel\n");
	init_all();
	put_str("init all done\n");
	asm volatile("sti");				//为演示中断处理，临时开中断
	while(1);
}
