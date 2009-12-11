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
#include "ScreenBase.h"
#include "DeviceCfgScreen.h" 
#include "StringTable.h"


CDevCfgScreen DeviceCfgScreen;

static void DevCfgScrCtor( CScreenBase *pScr )
{

}

static void DevCfgScrDtor(CScreenBase *pScr)
{
	ASSERT(pScr);

}

static INT32U DevCfgScrProc (GUI_HWND hWnd, INT32U message, GUI_WPARAM wParam, GUI_LPARAM lParam)
{
	switch (message)
	{
// 		case MSG_PAINT:
// 			{
// 				HDC hdc;
// 				char* text;
// 				RECT rc;
// 				hdc = BeginPaint (hWnd);
// 
// 				text = (char*) GetWindowAdditionalData (hWnd);
// 				if (text)
// 				{
// 					GetClientRect (hWnd, &rc);
// 					SetBkMode (hdc, BM_TRANSPARENT);
// 					TextOut (hdc, 5, (rc.bottom - GetSysCharHeight())>>1, text);
// 				}
// 				EndPaint (hWnd, hdc);
// 				return 0;
// 			}
	}

	return DefaultScreenProc(hWnd, message, wParam, lParam);
}

CScreenBase * InitDevCfgScreen(void)
{
//	INT32U nSize = sizeof(CDevCfgScreen);
	CScreenBase *pScr = (CScreenBase *)&DeviceCfgScreen;

	InitScreen(pScr,DevCfgScrCtor,DevCfgScrDtor);


	return pScr;
}



#if 0
//按键处理函数 
static void OnKeyDownDevCfgScreen(INT16U keyCode)   
{	
	 switch (keyCode)
	 { 
	 case KEY_2:
		 {			 	 // 退出该窗口，进入父窗口
			 SwitchScreen(&g_ScreeLib,DeviceCfgScreen.Base.ParentID);

		 
		 }
		 break;
	 default:
		 break;
	 }
}
#endif

