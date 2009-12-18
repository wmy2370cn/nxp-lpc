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


typedef struct GUI_RECT_NODE CGuiRectNode;

struct GUI_RECT_NODE
{
	CGuiRect Rect;

	CGuiRectNode *pPrev;
	CGuiRectNode *pNext; 
};

struct GUI_BLOCK_HEAP;
struct GUI_REGION
{
	/**
    * Type of the region, can be one of the following:
    *   - NULLREGION\n
    *     A null region.
    *   - SIMPLEREGION\n
    *     A simple region.
    *   - COMPLEXREGION
    *     A complex region.
    *
    * \note Only defined for _USE_NEWGAL.
    */
    INT8U      Type;
   /**
    * Reserved for alignment.
    *
    * \note Only defined for _USE_NEWGAL.
    */
    INT8U      Reserved[3];
	CGuiRect RectBound; // The bounding rect of the region.
	CGuiRectNode *pHead;
	CGuiRectNode *pTail;

	struct GUI_BLOCK_HEAP *pHeap;
};
typedef struct GUI_REGION CGuiRegion;
typedef struct GUI_REGION CGuiClipRgn;


#define GUI_REGION_NULL      0x00
#define GUI_REGION_SIMPLE    0x01
#define GUI_REGION_COMPLEX   0x02
