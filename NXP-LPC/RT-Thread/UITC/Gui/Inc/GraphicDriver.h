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

struct GUI_HW_DRIVER
{
	/* screen Width and Height */
	INT32U Width;
	INT32U Height;

	/* screen update */
	void (*ScreenUpdate)(CGuiRect* rect);
	INT8U *pFrameBuffer;

	/* set and get pixel in (x, y) */
	void (* SetPixel) (GUI_COLOR *c, INT32U x, INT32U y);
	void (* GetPixel) (GUI_COLOR *c, INT32U x, INT32U y);

	void (*DrawHLine)(GUI_COLOR *c, INT32U x1, INT32U x2, INT32U y);
	void (*DrawVLine)(GUI_COLOR *c, INT32U x , INT32U y1, INT32U y2);
};

typedef struct GUI_HW_DRIVER CGuiHwDriver;

 