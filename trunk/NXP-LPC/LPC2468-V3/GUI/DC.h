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
#ifndef GUI_DC_H
#define	GUI_DC_H 

//画线
extern void DrawLine(LcdPos x0, LcdPos y0, LcdPos x1, LcdPos y1, COLOR cr);
extern void DrawLineEx(LcdPos x0, LcdPos y0, LcdPos x1, LcdPos y1, COLOR cr/*=COLOR_FR*/, PEN pen/*=PEN_COPY*/);

 
extern void DrawBlock(LcdPos x0, LcdPos y0, LcdPos x1, LcdPos y1, COLOR cr/*=COLOR_FR*/);

extern void DrawBlockRect( LCDRECT *pRect, COLOR cr);

extern void DrawRect(LcdPos x0, LcdPos y0, LcdPos x1, LcdPos y1, COLOR cr/*=COLOR_FR*/);

extern void DrawStringByStr(LcdPos x, LcdPos y, const char * pString, PEN pen/*=PEN_COPY*/, ALIGN nAlign/*=ALIGN_LEFT*/,INT8U nEnFontType);
extern void DrawStringById(LcdPos x, LcdPos y, INT16U uStringResID, PEN pen/*=PEN_COPY*/, ALIGN nAlign/*=ALIGN_LEFT*/,INT8U nEnFontType);

extern void DrawIcon(LcdPos x, LcdPos y, INT16U iIconID, PEN pen/*=PEN_COPY*/);

//液晶重绘
extern void ClearScreen(void);

extern void TurnOnLed(void);
extern void TurnOffLed(void);

#endif

