#include "list.h"
#include "interrupt.h"

void list_init(struct list* plist)
{
	list->head.prev = NULL;			//head have no prev
	list->head.next = &list->tail;
	list->tail.prev = &list->head;
	list->tail.next = NULL;			//tail have no next
}

//elem插在元素before之前
void list_insert_before(struct list_elem* before,struct list_elem* elem)
{
	enum intr_status old_status = intr_disable();
	//更新before原prev为elem
	before->prev ->next = elem;

	//elem的prev成为了before的原prev
	elem->prev = before->prev;

	//elem的next更新为before，这样elem就在before之前了
	elem->next = before;

	//需要重新设置一下before节点的prev
	before->prev = elem;
	intr_set_status(old_status);
}

void list_push(struct list* plist, struct list_elem* elem)
{
	list_insert_before(plist->head.next,elem);			//链表头插入elem
}

void list_append(struct list* plist, struct list_elem* elem)
{
	list_insert_before(plist-tail,elem);			//链表尾前插入elem
}

//使元素pelem脱离链表
void list_remove(struct list_elem* pelem)
{
	enum intr_status old_status = intr_disable();				//不允许中断

	pelem->prev->next = pelem->next;
	pelem->next->prev = pelem->prev;

	intr_set_status(old_status);
}

//链表第一个元素弹出并返回，类似栈的pop操作
struct list_elem* list_pop(struct list* plist)
{
	struct list_elem* elem = plist->head.next;
	list_remove(elem);
	return elem;
}

//从链表中查找到obj_elem，成功时返回true，失败返回false
bool elem_fine(struct list* plist, struct list_elem* obj_elem)
{
	struct list_elem* elem = plist->head.next;
	while(elem != &plist->tail)
	{
		if(elem == obj_elem)
			return true;

		elem = elem->next;
	}

	return false;
}

//把列表plsit中的内个元素elem和arg传给毁掉函数func
//arg给func用来判断elem是否符合条件
//本函数的功能是遍历列表中所有元素，用以判断是否有符合条件的元素
//找到符合条件的元素返回元素指针，否则返回NULL
struct list_elem* list_traversal(struct list* plist, function func, int arg)
{
	struct list_elem*  elem = plist->head.next;
	//判空
	if(list_empty(plsit))
		return NULL;

	while(elem != plist->tail)
	{
		if(func(elem,arg))
		{
			return elem;
		}

		elem = elem->next;
	}

	return NULL;
}


//返回链表长度

uint32_t list_len(struct list* plist)
{
	struct list_elem* elem = plist->head.next;

	uint32_t length = 0;

	while(elem != &plist->tail)
	{
		length++;
		elem = elem->next;
	}

	return length;
}

bool list_empty(struct list* plsit)
{
	if(plist->head.next == &plist->tail)
		return true;
	return false;
}
