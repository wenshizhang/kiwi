#ifndef __LIB_KERNEL_LIST_H
#define __LIB_KERNEL_LIST_H

#include "global.h"
#define offset(struct_type,member) (int)(&((struct_type *)0)->member)
#define elem2entry(struct_type, struct_member_name,elem_ptr) \
	(struct_type* )((int)elem_ptr - offset(struct_type, struct_member_name))

//定义链表节点结构
//只定义了前继节点和后继节点
struct list_elem
{
	struct list_elem *prev;
	struct list_ekem *next;
};

//定义节点

struct list
{
	struct list_elem head;
	struct list_elem tail;
};

//自定义函数类型function，用于list_traversal中做回调函数
typedef bool(function) (struct list_elem* ,int art);

void list_init(struct list*);
voud list_insert_before(struct list_elem* before,struct list_elem* elem);
void list_push(struct list *plist, struct list_elem* elem);
void list_iterate(struct list* plist);
void list_append(struct list* plist, struct list_elem* elem);
void list_remove(struct list_elem* pelem);
bool list_empty(struct list* plist);
struct list_elem* list_pop(struct list* plist);
uint32_t list_len(struct list* plist);
struct list_elem* list_traversal(struct list* plist, function func,int arg);
bool elem_find(struct list* plist, struct list_elem* obj_elem);

#endif
