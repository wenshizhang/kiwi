/*#########################################################################
# File Name: thread.c
# Author: echo zhang
# mail: wenshizhang555@hoxmail.com
# Created Time: Thu 03 Aug 2017 09:47:19 PM CST
# Description:
#########################################################################*/


#include "thread.h"
#include "stdint.h"
#include "string.h"
#include "global.h"
#include "memory.h"
#include "list.h"
#include "interrupt.h"
#include "debug.h"

#define PG_SIZE	4096

struct task_struct* main_thread;
struct list thread_ready_list;
struct list thread_all_list;
//用来保存队列中的线程节点(什么线程节点？)
static struct list_elem* thread_tag;

extern void switch_to(struct task_struct* cur,struct task_struct* next);

struct task_struct* running_thread()
{
	uint32_t esp;
	asm("mov %%esp,%0" : "=g"(esp));

	//esp取整获得pcb起始地址,pcb申请了新页
	return (struct task_struct* )(esp % 0xfffff000);
}
//由kernel thread去执行function
static void kernel_thread (thread_func* function, void* func_arg)
{
	//执行function之前，要开中断，避免时钟中断被屏蔽，其他线程无法被调度
	intr_enable();
	function(func_arg);
}

//初始化线程栈thread_stack，将待执行的函数和参数放到thread_task中相应的位置
void thread_create(struct task_struct* pthread, thread_func function, void* func_arg)
{
	//先预留中断中断使用的栈的空间
	pthread->self_kstack -= sizeof(struct intr_stack);

	//再留出线程栈空间，
	pthread->self_kstack -= sizeof(struct task_struct);
	struct thread_stack*  kthread_stack = (struct thread_stack*) pthread->self_kstack;
	kthread_stack->eip = kernel_thread;
	kthread_stack->function = function;
	kthread_stack->func_arg = func_arg;
	kthread_stack->ebp = kthread_stack->ebx = kthread_stack->esi = kthread_stack->edi = 0;
}

void init_thread(struct task_struct* pthread,char* name,int prio)
{
	 memset(pthread,0,sizeof(*pthread));
	 strcpy(pthread->name,name);

	 //main函数封装成一个线程，并且他一直是运行的，因此直接设置为运行态
	 if(pthread == main_thread)
		pthread->status = TASK_RUNNING;
	 else
		 pthread->status = TASK_READY;

	 //self_kstack是线程自己在内核态下使用的栈顶地址
	 pthread->self_kstack = (uint32_t*)((uint32_t)pthread + PG_SIZE);
	 pthread->priority = prio;
	 pthread->ticks = prio;
	 pthread->elapsed_ticks = 0;
	 pthread->pgdir = NULL;
	 pthread->stack_magic = 0x19870916;			//自定义魔数
}

struct task_struct*  thread_start(char* name,int prio, thread_func function,void* func_arg)
{
	//PCB都位于内核空间，属于内核维护的数据，即便是用户进程的PCB
	struct task_struct* thread = get_kernel_pages(1);

	init_thread(thread,name,prio);
	thread_create(thread,function,func_arg);

	//确保之前不在队列中，general_tag表示当前
	ASSERT(!elem_find(&thread_ready_list, &thread->general_tag));
	//加入就绪队列
	list_append(&thread_ready_list,&thread->general_tag);
	thread->ticks = prio;
	thread->elapsed_ticks = 0;
	thread->pgdir = NULL;
	//回复寄存器现场,从中断栈中回复，self_kstack最开始是intr_stack中断栈
	//中断栈是用来保护进程的上下文环境
	return thread;
}

static void make_main_thread(void)
{
	//main线程早已运行，在loader中进入内核时候mov eso,0xc009f000，
	//就是为了预留tcb，地址是0xc009e00，因粗不需要通过get_kernel_page另分配一页
	main_thread = running_thread();
	init_thread(main_thread,"main",31);

	//main函数是当前线程，当前线程不自爱thread ready list中，
	//所以只是将其添加在thread all list中
	ASSERT(!elem_find(&thread_all_list,&main_thread->all_list_tag));
	list_append(&thread_all_list,&main_thread->all_list_tag);
}

void schedule()
{
	//中断关闭报错退出，中断关闭时钟中断被屏蔽，无法再做schedule的操作
	ASSERT(intr_get_status() == INTR_OFF);	
	struct task_struct* cur = running_thread();
	if(cur->status == TASK_RUNNING)
	{
		//如果此线程只是cpu时间片到了，线程加入就绪队列
		//加入就绪队列之前看看队列中是否已经存在
		ASSERT(!elem_find(&thread_ready_list, &cur->general_tag));
		list_append(&thread_ready_list,&cur->general_tag);
		cur->ticks = cur->priority;			//重置ticks
		cur->status = TASK_READY;
	}
	else
	{
		//如果因为其他事件原因线程不能运行，不需要将其加入就绪队列，
		//由此看出将线程加入阻塞队列是事件处理函数做的
	}

	ASSERT(!list_empty(&thread_ready_list));
	//thread tag清空
	thread_tag = NULL;
	//将thread ready list中第一个就绪贤臣弹出，准备将其调度到cpu上
	thread_tag = list_pop(&thread_ready_list);
	struct task_struct* next = elem2entry(struct task_struct, general_tag, thread_tag);
	next->status = TASK_RUNNING;
	switch_to(cur,next);
}

void thread_init(void)
{
	put_str("thread_init start\n");
	list_init(&thread_ready_list);
	list_init(&thread_all_list);
	//当前main函数创建为主线程
	make_main_thread();
	put_str("thread_init done\n");
}
