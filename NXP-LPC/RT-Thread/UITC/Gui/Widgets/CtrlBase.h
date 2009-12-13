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

#pragma once
#include "GuiDef.h"
#include "ScreenBase.h"
 

#define MAX_CTRL_SIZE 1024

typedef struct CTRL_BASE CCtrlBase;
struct SCREEN_BASE;
//构造
typedef void (*ctrl_ctor)(CCtrlBase *object);
//析构
typedef void (*ctrl_dtor)(CCtrlBase *object);

struct CTRL_BASE 
{
	/* constructor and destructor */
	ctrl_ctor Ctor;
	ctrl_dtor Dtor;
	INT8U   WinType;       
	INT8U   Style;
 	INT16U  ExStyle;

	INT32U  ID; //资源ID

	GUI_HWND   Handle;
	GUI_HWND   ScreenHandle;
	GUI_HWND   ParentCtrlHandle;

 	CGuiRect   Block;	    	//定义控件位置
	INT32U (*ControlProc)(GUI_HWND, INT32U, GUI_WPARAM, GUI_LPARAM);

	INT32U  Size;
};

__inline GUI_HWND GetCtrlHandle( CCtrlBase *pCtrl )
{
	if(pCtrl)
		return pCtrl->Handle;
	return HWND_NULL;
}

//动态创建一个控件
CCtrlBase * CreateControl( INT32U nSize ,ctrl_ctor ctor,ctrl_dtor dtor);
//静态初始化一个控件
void InitControl(CCtrlBase * pCtrl, ctrl_ctor ctor,ctrl_dtor dtor );

#define GET_CTRL_PTR(hWnd)   ((CCtrlBase *)hWnd) 

INT8U IsControl( GUI_HWND hWnd );

void EraseBackground(CCtrlBase *pCtrlbase);



#ifndef CTRL_BASE_DEF
#define CTRL_BASE_DEF   \
	ctrl_ctor Ctor; \
	ctrl_dtor Dtor;\
	INT8U  WinType;  \
	INT8U   Style; \
	INT16U  ExStyle; \
	INT32U  ID;\
	GUI_HWND   Handle; \
	GUI_HWND   ScreenHandle; \
	GUI_HWND   ParentCtrlHandle; \
	CGuiRect	    Block;     \
	INT32U (*ControlProc)(GUI_HWND, INT32U, GUI_WPARAM, GUI_LPARAM);\
	INT32U  Size;
#endif
 
