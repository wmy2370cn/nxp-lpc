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
#pragma  once

#include "guilist.h"
#include "GuiDef.h"

#define MAX_SCR_SIZE 1024

/** Casts the function pointer to an rtgui_constructor */
#define SCR_CTOR(constructor)                ((scr_ctor)(constructor))
/** Casts the function pointer to an rtgui_constructor */
#define SCR_DTOR(destructor)                 ((scr_dtor)(destructor))

struct SCREEN_BASE;
typedef struct SCREEN_BASE CScreenBase;

//构造
typedef void (*scr_ctor)(CScreenBase *object);
//析构
typedef void (*scr_dtor)(CScreenBase *object);

//消息函数
//按键处理
typedef  void  (*fnOnKeyDown)(CScreenBase *pScreen, INT32U keyCode);
//消息处理虚函数
typedef  void   (*fnOnMessage)(CScreenBase *pScreen,INT32U msg, INT32U param);
//初始化窗口
typedef  void    (*fnInit)(CScreenBase *pScreen,INT32U wParam, INT32U lParam);
//创建
typedef  void    (*fnCreate)(CScreenBase *pScreen);
//退出时处理
typedef  void	(*fnDestory)(CScreenBase *pScreen);
//消息通知-通知控件
typedef  void	(*fnOnNotify)(CScreenBase *pScreen,INT32U nf);

 
struct SCREEN_BASE
{
	GUI_HANDLE   Handle;
	INT8U        ParentID;    //父窗口ID号
	INT8U        CurrentID;   //本窗口ID号
	INT16U       ScreenName; //本窗口名称  字符串的ID
	gui_list_t    List;

	/* constructor and destructor */
	scr_ctor Ctor;
	scr_dtor Dtor;

	//回调函数
	fnInit      pfnInit;
	fnCreate    pfnCreate;
	fnDestory   pfnDestory;

	fnOnKeyDown pfnKeyDown;
	fnOnMessage pfnMessage;
	fnOnNotify  pfnOnNotify;

	/* size of type */
	INT32U  Size;
};

//创建一个屏幕
CScreenBase * CreateScreen( INT32U nSize ,scr_ctor ctor,scr_dtor dtor);
//静态初始化一个屏幕
void InitScreen(CScreenBase *pScr, scr_ctor ctor,scr_dtor dtor );
void SetScreenInfo(CScreenBase *pScreen,INT8U ParentID,INT8U CurrentID,INT16U nScreenName );

__inline GUI_HANDLE GetScreenHandle( CScreenBase *pScreen )
{
	return pScreen->Handle;
}


#ifndef SCR_BASE_DEF
#define SCR_BASE_DEF   \
		GUI_HANDLE   Handle;       \
		INT8U        ParentID;     \
		INT8U        CurrentID;    \
		INT16U       ScreenName;   \
		gui_list_t    List;        \
	 	scr_ctor Ctor; \
		scr_dtor Dtor;\
		fnInit      pfnInit;\
		fnCreate    pfnCreate;\
		fnDestory   pfnDestory;\
		fnOnKeyDown pfnKeyDown;\
		fnOnMessage pfnMessage;\
		fnOnNotify  pfnOnNotify;\
	 	INT32U  Size;
#endif


