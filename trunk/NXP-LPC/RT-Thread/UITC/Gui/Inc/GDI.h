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


    /**
     * \defgroup map_fns Mapping Operations
     *
     * The mapping mode defines the unit of measure used to transform 
     * page-space units into device-space units, and also defines 
     * the orientation of the device's x and y axes. 
     *
     * So far, MiniGUI support only two mapping mode: 
     *
     *      - MM_TEXT\n
     *        Each logical unit is mapped to on device pixel. 
     *        Positive x is to the right; positive y is down.
     *      - MM_ANISOTROPIC\n
     *        Logical units are mapped to arbitrary units with arbitrarily 
     *        scaled axes; Use \a SetWindowExt and \a SetViewportExt functions 
     *        to specify the units, orientation, and scaling required.
     *
     * The following formula shows the math involved in converting a point 
     * from page space to device space:
     *
     * \code
     *      Dx = ((Lx - WOx) * VEx / WEx) + VOx
     * \endcode
     *
     * The following variables are involved:
     *
     *      - Dx    x value in device units
     *      - Lx    x value in logical units (also known as page space units)
     *      - WO    window x origin
     *      - VO    viewport x origin
     *      - WE    window x-extent
     *      - VE    viewport x-extent
     *
     * The same equation with y replacing x transforms the y component of 
     * a point. The formula first offsets the point from its coordinate origin. 
     * This value, no longer biased by the  origin, is then scaled into 
     * the destination coordinate system by the ratio of the extents. 
     * Finally, the scaled value is offset by the destination origin to 
     * its final mapping.
     *
     * @{
     */

#define MM_TEXT               0
#define MM_ANISOTROPIC        1

