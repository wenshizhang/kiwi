#ifndef __KERNEL_DEBUG_H
#define __KERNEL_DEBUG_H

void panic_spin(char *fiename,int line, const char *func,const char *condition);

/******************_VA_ARGS******************
* _VA_ARGS是与处理器支持的专用标识符
* 代表所有u省略号对应的参数
* “...”标示定义的宏及其参数可变
  */
#define PANIC(...) panic_spin (__FILE__,__LINE__,__func__,__VA_ARGS__)

#ifdef NDEBUG
	#define ASSERT(CONDITION) ((void)0)
#else
#define ASSERT(CONDITION)\
	if(CONDITION){} else{		\
		/*符号#让编译器将宏的参数转化为字符串字面量*/\
		PANIC(#CONDITION);		\
	}
#endif			//NDEBUG

#endif			//__KERNEL_DEBUG_H
