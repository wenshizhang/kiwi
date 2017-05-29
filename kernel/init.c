#include	"init.h"
#include	"print.h"
#include	"interrupt.h"

void init_all()
{
	put_str("init_all start\n");
	idt_init();
	put_str("init_all done");
}
