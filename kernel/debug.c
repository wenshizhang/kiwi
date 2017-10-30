#include "debug.h"
#include "print.h"
#include "interrupt.h"

//出现
void panic_spin(char *filename, int line, const char* func, const char* condition)
{
	intr_disable();		//错误开始关中断

	put_str("\n\n\n!!!!error!!!!\n");
	put_str("filename:");
	put_str(filename);
	put_str("\n");
	put_str("line 0x");
	put_int(line);
	put_str("\n");
	put_str("function:");
	put_str((char *)func);
	put_str("\n");
	put_str("condition:");
	put_str((char *)condition);
	put_str("\n");
	while(1);

}
