[bits 32]
%define ERROR_CODE nop		 ; 若在相关的异常中cpu已经自动压入了错误码,为保持栈中格式统一,这里不做操作.
%define ZERO push 0		 ; 若在相关的异常中cpu没有压入错误码,为了统一栈中格式,就手工压入一个0

extern put_str			 ;声明外部函数
extern idt_table		 ;声明interupt中定义的idt_table

section .data
global intr_entry_table
intr_entry_table:

%macro VECTOR	2				;%macro  宏名字		参数个数
section .text
intr%lengtry:

	%2
	push ds
	push es
	push fs
	push gs
	pushad			;pushad指令压入所有的32位寄存器
	
	;如果从片进入中断，除了给主片发送eoi，还要给从片发送eoi
	mov al,0x20
	out 0xa0,al			;从片发送
	out	0x20,al			;主片发送

	push %1
	call [idt_table + %1*4]			;idt_table+%1*4 表示该中断在idt_table地址，其中存储了intr_handler函数
	jmp intr_exit					;中断处理结束，调用exit函数
section .data
	dd intr%lentry

%endmacro

section .text
global intr_exit
intr_exit:
	add esp,4		;跳过中断号
	popad			;32位寄存器出栈
	pop gs			;通用寄存器一次出栈，注意顺序和入栈相反
	pop fs
	pop es
	pop ds
	add esp,4			;跳过error_code
	iretd

VECTOR 0x00,ZERO
VECTOR 0x01,ZERO
VECTOR 0x02,ZERO
VECTOR 0x03,ZERO 
VECTOR 0x04,ZERO
VECTOR 0x05,ZERO
VECTOR 0x06,ZERO
VECTOR 0x07,ZERO 
VECTOR 0x08,ERROR_CODE
VECTOR 0x09,ZERO
VECTOR 0x0a,ERROR_CODE
VECTOR 0x0b,ERROR_CODE 
VECTOR 0x0c,ZERO
VECTOR 0x0d,ERROR_CODE
VECTOR 0x0e,ERROR_CODE
VECTOR 0x0f,ZERO 
VECTOR 0x10,ZERO
VECTOR 0x11,ERROR_CODE
VECTOR 0x12,ZERO
VECTOR 0x13,ZERO 
VECTOR 0x14,ZERO
VECTOR 0x15,ZERO
VECTOR 0x16,ZERO
VECTOR 0x17,ZERO 
VECTOR 0x18,ERROR_CODE
VECTOR 0x19,ZERO
VECTOR 0x1a,ERROR_CODE
VECTOR 0x1b,ERROR_CODE 
VECTOR 0x1c,ZERO
VECTOR 0x1d,ERROR_CODE
VECTOR 0x1e,ERROR_CODE
VECTOR 0x1f,ZERO 
VECTOR 0x20,ZERO
