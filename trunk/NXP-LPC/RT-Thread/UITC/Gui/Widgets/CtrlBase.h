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
 
typedef struct CTRL_BASE CCtrlBase;

//构造
typedef void (*ctrl_ctor)(CCtrlBase *object);
//析构
typedef void (*ctrl_dtor)(CCtrlBase *object);

typedef  void    (*fnInitCtrl)(CCtrlBase *pCtrl,INT32U wParam, INT32U lParam);

typedef  void    (*fnCreateCtrl)(CCtrlBase *pCtrl);

//按键处理
typedef  void  (*fnOnKeyDownCtrl)(CCtrlBase *pCtrl, INT16U keyCode);
//消息处理虚函数
typedef  void   (*fnOnMessageCtrl)(CCtrlBase *pCtrl,INT32U msg, INT32U param);
//退出时处理
typedef  void	(*fnDestoryCtrl)(CCtrlBase *pCtrl); 

struct CTRL_BASE 
{
	/* constructor and destructor */
	ctrl_ctor Ctor;
	ctrl_dtor Dtor;

	GUI_HANDLE   Handle;

 	CGuiRect	    Block;	    	//定义控件位置

	fnCreateCtrl   pfnCreate;
	fnDestoryCtrl  pfnDestory;
	//回调函数
	fnOnKeyDownCtrl pfnKeyDown;
	fnOnMessageCtrl pfnMessage;	
};

__inline GUI_HANDLE GetCtrlHandle( CCtrlBase *pCtrl )
{
	return pCtrl->Handle;
}

void EraseBackground(CCtrlBase *pCtrlbase);

#ifndef CTRL_BASE_DEF
#define CTRL_BASE_DEF   \
	ctrl_ctor Ctor; \
	ctrl_dtor Dtor;\
	GUI_HANDLE   Handle; \
	CGuiRect	    Block;     \
	fnInitCtrl      pfnInit;\
	fnCreateCtrl    pfnCreate;\
	fnDestoryCtrl   pfnDestory;\
	fnOnKeyDownCtrl pfnKeyDown;\
	fnOnMessageCtrl pfnMessage;
#endif
 
