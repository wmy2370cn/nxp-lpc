/****************************************Copyright (c)**************************************************
**					  GUI私有堆的管理，由于是单任务实现，所以，采用私有堆的分配管理效率更高
**                    采用数组循环方式实现，今后可以采用链表来实现                  
**                                      
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:   
**创   建   人:    
**最后修改日期:  
**描        述:   
**备        注 :    
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rtthread.h"
#include "applib.h"  

#include "guiblockheap.h"

/*********************************************************************************************************
** 函数名称: InitBlockDataHeap
** 函数名称: InitBlockDataHeap
**
** 功能描述：  
**
** 输　入:  CGuiBlockHeap * heap
** 输　入:  INT32U bd_size
** 输　入:  INT32U heap_size
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月16日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void InitBlockDataHeap (CGuiBlockHeap * heap, INT32U bd_size, INT32U heap_size)
{
    heap->heap = NULL;
    heap->bd_size = bd_size + sizeof (INT32U);
    heap->heap_size = heap_size;
}
/*********************************************************************************************************
** 函数名称: BlockDataAlloc
** 函数名称: BlockDataAlloc
**
** 功能描述：  
**
** 输　入:  CGuiBlockHeap * heap
**          
** 输　出:   void*
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月16日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void* BlockDataAlloc (CGuiBlockHeap * heap)
{
    int i;
    char* block_data = NULL;

    if (heap->heap == NULL)
	{
        if (!(heap->heap = rt_calloc (heap->heap_size, heap->bd_size)))
            goto ret;
        heap->free = 0;
    }

    block_data = (char*) heap->heap + heap->bd_size*heap->free;
    for (i = heap->free; i < heap->heap_size; i++) 
	{
        if (*((INT32U*)block_data) == BDS_FREE)
		{
            heap->free = i + 1;
            *((INT32U*)block_data) = BDS_USED;

#if 0
            fprintf (stderr, "heap: %p, Allocated: %d, free: %d.\n", heap, i, heap->free);
            fprintf (stderr, "Heap: (bd_size: %d, heap_size: %d, heap: %p).\n", 
                    heap->bd_size, heap->heap_size, heap->heap);
#endif
            goto ret;
        }

        block_data += heap->bd_size;
    }

	//如果在上面没有分配到，则直接从系统申请一块
    if (!(block_data = calloc (1, heap->bd_size)))
        goto ret;

    *((INT32U*)block_data) = BDS_SPECIAL;

ret:
    if (block_data)
        return block_data + sizeof (INT32U);
    return NULL;
}
/*********************************************************************************************************
** 函数名称: BlockDataFree
** 函数名称: BlockDataFree
**
** 功能描述： 回收
**
** 输　入:  CGuiBlockHeap * heap
** 输　入:  void * data
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月16日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void BlockDataFree (CGuiBlockHeap* heap, void* data)
{
    int i;
    char* block_data;
 
    block_data = (char*) data - sizeof (INT32U);
    if (*((INT32U*)block_data) == BDS_SPECIAL)
        rt_free (block_data);
    else if (*((INT32U*)block_data) == BDS_USED) 
	{
        *((INT32U*)block_data) = BDS_FREE;

        i = (block_data - (char*)heap->heap)/heap->bd_size;
        if (heap->free > i)
            heap->free = i;        
#if 0
        fprintf (stderr, "Heap: %p: Freed: %d, free: %d.\n", heap, i, heap->free);
        fprintf (stderr, "Heap: (bd_size: %d, heap_size: %d, heap: %p).\n", 
                    heap->bd_size, heap->heap_size, heap->heap);
#endif
    }
 
}
/*********************************************************************************************************
** 函数名称: DestroyBlockDataHeap
** 函数名称: DestroyBlockDataHeap
**
** 功能描述：  
**
** 输　入:  CGuiBlockHeap * heap
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月16日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void DestroyBlockDataHeap (CGuiBlockHeap* heap)
{
#if 0
    fprintf (stderr, "Heap: (bd_size: %d, heap_size: %d, heap: %p).\n", 
                    heap->bd_size, heap->heap_size, heap->heap);
#endif
 
    rt_free (heap->heap);
}

