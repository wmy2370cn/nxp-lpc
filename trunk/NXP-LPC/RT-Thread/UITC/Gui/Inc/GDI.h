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
#include "GuiDef.h" 


typedef struct GUI_CLIPRECT CGuiClipRect;

struct GUI_CLIPRECT
{
	CGuiRect Rect;

	CGuiClipRect *pPrev;
	CGuiClipRect *pNext; 
};

struct GUI_BLOCK_HEAP;
struct GUI_CLIPRGN
{
	CGuiRect RectBound; // The bounding rect of the region.

	CGuiClipRect *pHead;
	CGuiClipRect *pTail;

	struct GUI_BLOCK_HEAP *pHeap;
};

typedef struct GUI_CLIPRGN CGuiClipRgn;


#define GUI_REGION_NULL      0x00
#define GUI_REGION_SIMPLE    0x01
#define GUI_REGION_COMPLEX   0x02
