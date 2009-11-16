/****************************************Copyright (c)**************************************************
**
**
**
**
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:
**创   建   人:
**最后修改日期:
**描        述:
**
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人:
** 版  本:
** 日　期:
** 描　述:
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 描　述:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#pragma once
#ifndef GUI_LIST_H
#define GUI_LIST_H

struct GuiListNode
{
	struct GuiListNode* pNext;
};
typedef struct GuiListNode gui_list_t;

__inline void GuiListInit(gui_list_t *l)
{
	l->pNext = (struct GuiListNode *)0;
}

/*********************************************************************************************************
** 函数名称: GuiListAppend
** 函数名称: GuiListAppend
**
** 功能描述：  把节点n添加到链表的最后
**
** 输　入:  gui_list_t * l
** 输　入:  gui_list_t * n
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月16日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
__inline void GuiListAppend(gui_list_t *l, gui_list_t *n)
{
	struct GuiListNode* node = l;

	while (node->pNext) node = node->pNext;

	/* append the node to the tail */
	node->pNext = n;
	n->pNext = (gui_list_t*) 0;
}
/*********************************************************************************************************
** 函数名称: GuiListInsert
** 函数名称: GuiListInsert
**
** 功能描述： 把节点n插入到节点l的后面 
**
** 输　入:  gui_list_t * l
** 输　入:  gui_list_t * n
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月16日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
__inline void GuiListInsert(gui_list_t *l, gui_list_t *n)
{
	n->pNext = l->pNext;
	l->pNext = n;
}
/*********************************************************************************************************
** 函数名称: GuiListRemove
** 函数名称: GuiListRemove
**
** 功能描述：  删除节点n
**
** 输　入:  gui_list_t * l
** 输　入:  gui_list_t * n
**          
** 输　出:   gui_list_t*
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月16日
** 备  注:  遍历整个链表，找到节点n
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
__inline gui_list_t* GuiListRemove(gui_list_t *l, gui_list_t *n)
{
	/* remove slist head */
	struct GuiListNode* node = l;
//	ASSERT(l && n);
	while (node->pNext && node->pNext != n) node = node->pNext;

	if (node == (gui_list_t *)0)
		return node;
	/* remove node */
	if (node->pNext != (gui_list_t *)0)
		node->pNext = node->pNext->pNext;

	return l;
}
/*********************************************************************************************************
** 函数名称: GuiListRemoveHead
** 函数名称: GuiListRemoveHead
**
** 功能描述：删除链表的头部，并把头部返回出来  
**
** 输　入:  gui_list_t * l
**          
** 输　出:   gui_list_t *  被删除掉的头部
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月16日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
__inline  gui_list_t * GuiListRemoveHead(gui_list_t *l)
{
	struct GuiListNode* node = l;
	if (l == (gui_list_t *)0)
		return (gui_list_t *)0;

	node = l->pNext; 
//	l->pNext = 	(gui_list_t *)0;
	if (node != (gui_list_t *)0)
	{
		l->pNext = node->pNext;
	}
	return (gui_list_t *)node;
}

__inline  gui_list_t * GuiListTail(gui_list_t *l)
{
	struct GuiListNode* node = l;

	for (;;)
	{
		if (l->pNext == (gui_list_t *)0)
		{
			node =l;
			break;
		}
		else
		{
			l = l->pNext;
			continue;
		}
	}
	return node;
}

__inline  gui_list_t * GuiListRemoveTail(gui_list_t *l)
{
	struct GuiListNode* node = l;

	for (;;)
	{
		if (l->pNext == (gui_list_t *)0)
		{
			break;
		}
		else
		{
			node =l;
			l = l->pNext;
			continue;
		}
	}
	node->pNext = (gui_list_t *)0;
	return node;
}

__inline int GuiListIsEmpty(const gui_list_t *l)
{
	return l->pNext == (gui_list_t *)0;
}

#define CONTAINING_RECORD(node, type, member)	\
	((type *)((char*)(node)-(unsigned long)(&((type *)0)->member)))

#define GuiListForEach(node, list)	\
	for ((node) = (list)->pNext; (node) != RT_NULL; (node) = (node)->pNext)
#endif
