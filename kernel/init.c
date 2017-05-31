#include "init.h"
#include "print.h"
#include "interrupt.h"
#include "timer.h"	

void init_all()
{
	put_str("init_all start\n");
	idt_init();
	timer_init();
	put_str("init_all done");
}
