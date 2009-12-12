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

// 		pBase->pfnCreate = NULL;
// 		pBase->pfnDestory= NULL;
// 		//回调函数
// 		pBase->pfnKeyDown= NULL;
// 		pBase->pfnMessage= NULL;
	}
}
/*********************************************************************************************************
** 函数名称: CreateControl
** 函数名称: CreateControl
**
** 功能描述：  从堆中动态创建一个控件
**
** 输　入:  INT32U nSize
** 输　入:  scr_ctor ctor
** 输　入:  scr_dtor dtor
**          
** 输　出:   CCtrlBase *
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月12日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
CCtrlBase * CreateControl( INT32U nSize,ctrl_ctor ctor,ctrl_dtor dtor )
{
	CCtrlBase *pObj = NULL;

	if (nSize <= 0 || nSize > MAX_CTRL_SIZE)
		return RT_NULL;

	pObj = rt_malloc(nSize);
	if (pObj == RT_NULL)
		return RT_NULL;

	pObj->Ctor = ctor;
	pObj->Dtor = dtor;
	pObj->Size = nSize;
	pObj->Handle = (GUI_HWND) pObj;
	pObj->WinType = TYPE_CONTROL;

	if(ctor)
	{
		ctor(pObj);
	}
	return pObj;
}
/*********************************************************************************************************
** 函数名称: InitControl
** 函数名称: InitControl
**
** 功能描述：静态初始化一个控件  
**
** 输　入:  CCtrlBase * pObj
** 输　入:  ctrl_ctor ctor
** 输　入:  ctrl_dtor dtor
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月12日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void InitControl(CCtrlBase *pObj, ctrl_ctor ctor,ctrl_dtor dtor )
{
	ASSERT(pObj);
	if(pObj)
	{
		pObj->Ctor = ctor;
		pObj->Dtor = dtor;
		pObj->Size = 0; //静态
		pObj->Handle = (GUI_HWND) pObj;
		pObj->WinType = TYPE_CONTROL;

		if(ctor)
		{
			ctor(pObj);
		}
	}
	return;
}

INT8U DefaultControlProc (GUI_HWND hWnd, INT32U message, INT32U wParam, INT32U lParam)
{

	return TRUE;
}
/*********************************************************************************************************
** 函数名称: IsControl
** 函数名称: IsControl
**
** 功能描述： 判断该句柄是不是控件
**
** 输　入:  GUI_HWND hWnd
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月12日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U IsControl( GUI_HWND hWnd )
{
	CCtrlBase *pObj = NULL;
	if (hWnd == HWND_NULL || hWnd == HWND_INVALID )
		return FALSE;
	pObj = GET_CTRL_PTR(hWnd);
	return (pObj->WinType == TYPE_CONTROL);
}
