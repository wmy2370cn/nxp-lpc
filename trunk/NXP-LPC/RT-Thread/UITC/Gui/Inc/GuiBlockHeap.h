/****************************************Copyright (c)**************************************************
**						显示硬件适配驱动，只支持1个显示接口	     
**                                      
**                                      
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:   LcdDriver.h
**创   建   人:   Author: admin
**最后修改日期: Created on: 2009-11-15
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

#pragma once 

#include "applib.h" 

#define BDS_FREE        0x0000
#define BDS_SPECIAL     0x0001
#define BDS_USED        0x0002

 /**
 * GUI's private block data heap.
 *
 * \sa InitBlockDataHeap, DestroyBlockDataHeap
 */
struct	GUI_BLOCK_HEAP
{
    /**
     * Size of one block element.
     */
    INT32U          bd_size;
    /**
     * Size of the heap in blocks.
     */
    INT32U          heap_size;
    /**
     * The first free element in the heap.
     */
    INT32U             free;
    /**
     * Pointer to the pre-allocated heap.
     */
    void*           heap;
} ;

typedef struct GUI_BLOCK_HEAP CGuiBlockHeap;

void* BlockDataAlloc (CGuiBlockHeap * heap);
void BlockDataFree (CGuiBlockHeap* heap, void* data);
