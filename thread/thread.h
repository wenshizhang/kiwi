#ifndef __THREAD_THREAD_H
#define __THREAD_THREAD_H

#include "stdint.h"

//自定义通用该函数类型，他将在很多线程函数中作为形参类型
//函数thread_func的返回值和参数类型可以改变吗？
typedef void thread_func(void *);


//进程状态枚举
enum task_status
{
	TASK_RUNNING,
	TASK_READY,
	TASK_BLOCKED,
	TASK_WAITING,
	TASK_HANGING,
	TASK_DIELD
};

//中断栈
//此结构用于中断发生时保护承诺供需的上下文环境：
//进程或者线程被外部中断或者软中断打断时，会按照此结构压入上下文寄存器，
//intr_exit中的出栈操作是此结构的你操作
//此栈在线程自己的内核栈中位置固定，所在页的最顶端

struct intr_stack
{
	uint32_t vec_no;
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp_dummy;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
	uint32_t gs;
	uint32_t fs;
	uint32_t es;
	uint32_t ds;

	//以下由cpu从低特权级进入搞特权级时压入
	uint32_t err_code;
	void (*eip) (void);
	uint32_t cs;
	uint32_t eflags;
	void* esp;
	uint32_t ss;
};


//线程栈
//线程自己的栈，用于存储线程中待执行的函数，此结构在线程自己的内核栈中位置不固定
//用在switch_to时保存线程环境，实际位置取决于实际运行情况

struct thread_stack
{
	uint32_t ebp;
	uint32_t ebx;
	uint32_t edi;
	uint32_t esi;

	//线程第一次执行时，eip指向待调用的函数kernel_thread其他时候，eip指向switch_to 的返回地址
	void (*eip) (thread_func *func, void* func_arg);

	//以下近第一次被调度上CPU使用
	//参数unused_retaddr置为占位置充数为返回地址
	void (*unused_retaddr);
	thread_func* function;			//function 是几级指针？
	void *func_arg;
};

//进程或者线程的pcb，程序控制块
struct task_struct
{
	uint32_t *self_kstack;
	enum task_status status;
	uint8_t priority;				//优先级
	char name[16];
	uint32_t stack_magic;			//栈的边界标记，检测栈的溢出

};

void thread_create(struct task_struct* pthread, thread_func function, void *func_arg);
void init_thread(struct task_struct* pthread,char* name, int prio);
struct task_struct* thread_start(char* name,int prio,thread_func function, void *func_arg);

#endif
