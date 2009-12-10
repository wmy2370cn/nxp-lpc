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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "ScreenBase.h"


/*********************************************************************************************************
** 函数名称: SetScreenInfo
** 函数名称: SetScreenInfo
**
** 功能描述：  
**
** 输　入:  CScreenBase * pScreen
** 输　入:  INT8U ParentID
** 输　入:  INT8U CurrentID
** 输　入:  INT16U nScreenName
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月12日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void SetScreenInfo(CScreenBase *pScreen,INT8U ParentID,INT8U CurrentID,INT16U nScreenName )
{
	if (pScreen)
	{
		pScreen->ParentID = ParentID;
		pScreen->CurrentID = CurrentID;
		pScreen->ScreenName = nScreenName;
	}
}
/*********************************************************************************************************
** 函数名称: CreateScreen
** 函数名称: CreateScreen
**
** 功能描述：  动态创建一个屏幕 从堆中创建
**
** 输　入:  INT32U nSize
** 输　入:  scr_ctor ctor
** 输　入:  scr_dtor dtor
**          
** 输　出:   CScreenBase *
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月12日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CScreenBase * CreateScreen( INT32U nSize,scr_ctor ctor,scr_dtor dtor )
{
	CScreenBase *pObj;

	if (nSize <= 0 || nSize > MAX_SCR_SIZE)
		return RT_NULL;

	pObj = rt_malloc(nSize);
	if (pObj == RT_NULL)
		return RT_NULL;

	pObj->Ctor = ctor;
	pObj->Dtor = dtor;
	pObj->Size = nSize;

	if(ctor)
	{
		ctor(pObj);
	}
	pObj->Handle = (GUI_HWND) pObj;
	return pObj;
}
/*********************************************************************************************************
** 函数名称: 
**
** 功能描述： 静态创建一个窗口
**
** 输　入:
**
** 输　出:
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月3日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void InitScreen(CScreenBase *pScr, scr_ctor ctor,scr_dtor dtor )
{
	ASSERT(pScr);
	if(pScr)
	{
		pScr->Ctor = ctor;
		pScr->Dtor = dtor;
		pScr->Size = 0; //静态

		if(ctor)
		{
			ctor(pScr);
		}
		pScr->Handle = (GUI_HWND) pScr;
	}
	return;
}


void DrawScreenCaption( CScreenBase *pScreen, INT16U nTotal, INT16U nCur)
{
	if (pScreen == NULL)
		return;
	if (pScreen->ScreenName)
	{
		//清空位置
 //		ClrRectScreenBuf(&g_rcCaption,COLOR_BK);
//		DrawStringById(LCD_WIDTH>>1,0,pScreen->Base.ScreenName,PEN_COPY,ALIGN_CENTER,LCD_EN_FONT_8_16);
	}
	if(nTotal)
	{
		char szTxt[8] = {0};
		snprintf( szTxt,sizeof(szTxt),"%02d/%02d",nCur,nTotal) ;
//		DrawStringByStr( LCD_WIDTH-1,0, szTxt,PEN_COPY,ALIGN_RIGHT,LCD_EN_FONT_8_16);
	}
#if 0
	if(pStr)
	{
		//用标题文字画标题栏
		pOldEnFont = SetEnFontID(&theScreenDC, EN_8_16_BMPFONT);

		DrawText(&theScreenDC, pStr,strlen(pStr),&g_rcCaption,DT_CENTER|DT_VCENTER);
	}
	if(nTotal)
	{
		DrawString(&theScreenDC,GUI_LCM_XMAX-1,0,isprintf("%02d/%02d",nCur,nTotal),PEN_COPY,ALIGN_RIGHT);
	}
	//重置原先的字体
	SetEnFont(&theScreenDC, pOldEnFont);
	LCD_PutRectBuff2Scr(&g_rcCaption);
#endif
}

void DrawScreenStatusBar( void )
{
	char szTxt[32] = "2009.8.8-14:32:34";
	INT8U addr = 32;
	FP32  val = 12.86;
#if 0
	//画线
 	 DrawLine(0, 100,LCD_WIDTH-1, 100, COLOR_FR);
	 //时间及地址
 	 DrawStringByStr(0,104,szTxt,PEN_COPY,ALIGN_LEFT,LCD_EN_FONT_8_8);
	 snprintf( szTxt,sizeof(szTxt),"ADD[ %d ]",addr) ;
	 DrawStringByStr(LCD_WIDTH-1,104,szTxt,PEN_COPY,ALIGN_RIGHT,LCD_EN_FONT_8_8);

	 //闸门开度  ID_STRING_GATAGE
	 DrawStringById(0,112,ID_STRING_GATAGE,PEN_COPY,ALIGN_LEFT,LCD_EN_FONT_8_16);
	 snprintf( szTxt,sizeof(szTxt)," %0.2f ",val) ;
	 DrawStringByStr(LCD_WIDTH-1-40-32-32-16,112,szTxt,PEN_COPY,ALIGN_RIGHT,LCD_EN_FONT_8_16);
	 DrawStringById(LCD_WIDTH-1-40-32-16-16,112,ID_STRING_M,PEN_COPY,ALIGN_RIGHT,LCD_EN_FONT_8_16);

	 //状态
	 DrawStringById(LCD_WIDTH-1-40,112,ID_STRING_STATE,PEN_COPY,ALIGN_RIGHT,LCD_EN_FONT_8_16);
	 DrawStringById(LCD_WIDTH-1,112,ID_STRING_STOP,PEN_COPY,ALIGN_RIGHT,LCD_EN_FONT_8_16);
#endif
}


