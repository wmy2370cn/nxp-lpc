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

#if 0
typedef  void    (*fnInitCtrl)(CCtrlBase *pCtrl,INT32U wParam, INT32U lParam);

typedef  void    (*fnCreateCtrl)(CCtrlBase *pCtrl);

//按键处理
typedef  void  (*fnOnKeyDownCtrl)(CCtrlBase *pCtrl, INT16U keyCode);
//消息处理虚函数
typedef  void   (*fnOnMessageCtrl)(CCtrlBase *pCtrl,INT32U msg, INT32U param);
//退出时处理
typedef  void	(*fnDestoryCtrl)(CCtrlBase *pCtrl); 
#endif

struct CTRL_BASE 
{
	/* constructor and destructor */
	ctrl_ctor Ctor;
	ctrl_dtor Dtor;
	INT8U   WinType;       
	INT8U   Style;
 	INT16U  ExStyle;

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
	GUI_HWND   Handle; \
	GUI_HWND   ScreenHandle; \
	GUI_HWND   ParentCtrlHandle; \
	CGuiRect	    Block;     \
	INT32U (*ControlProc)(GUI_HWND, INT32U, GUI_WPARAM, GUI_LPARAM);
#endif
 
