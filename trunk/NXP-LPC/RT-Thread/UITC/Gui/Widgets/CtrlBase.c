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
#include "CtrlBase.h" 
#include "dc.h"


//擦除背景
void EraseBackground(CCtrlBase *pCtrlbase)
{
	SetScreenRect(& pCtrlbase->Block,GUI_COLOR_BK);
 //	DrawBlockRect(& pCtrlbase->Block,GUI_COLOR_BK);
}

void InitCtrlBase( CCtrlBase *pBase )
{
	if (pBase)
	{
 		memset( &pBase->Block,0,sizeof(CGuiRect));	    	//定义控件位置

		pBase->pfnCreate = NULL;
		pBase->pfnDestory= NULL;
		//回调函数
		pBase->pfnKeyDown= NULL;
		pBase->pfnMessage= NULL;
	}
}
/*********************************************************************************************************
** 函数名称: OnCtrlKeyDown
** 函数名称: OnCtrlKeyDown
**
** 功能描述： 响应按键消息
**
** 输　入:  CCtrlBase * pBase
** 输　入:  INT16U KeyCode
**          
** 输　出:   extern void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年4月24日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void OnCtrlKeyDown( CCtrlBase  *pCtrl,INT16U KeyCode )
{
	if (pCtrl == NULL)
		return;
	if (pCtrl->pfnKeyDown)
	{
		pCtrl->pfnKeyDown(pCtrl, KeyCode );
	}
}
void OnCtrlMsg( CCtrlBase  *pCtrl,INT32U msg, INT32U param  )
{
	if (pCtrl == NULL)
		return;

	if (pCtrl->pfnMessage)
	{
		pCtrl->pfnMessage( pCtrl,msg,param );
	}
} 


INT8U DefaultControlProc (GUI_HWND hWnd, INT32U message, INT32U wParam, INT32U lParam)
{

	return TRUE;
}
