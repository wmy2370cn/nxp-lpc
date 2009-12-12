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
#include "applib.h"
#include "guilist.h"
#include "GuiDef.h"

#define MAX_SCR_SIZE 1024

struct SCREEN_BASE;
typedef struct SCREEN_BASE CScreenBase;

//构造
typedef void (*scr_ctor)(CScreenBase *object);
//析构
typedef void (*scr_dtor)(CScreenBase *object);

/** Casts the function pointer to an rtgui_constructor */
#define SCR_CTOR(constructor)                ((scr_ctor)(constructor))
/** Casts the function pointer to an rtgui_constructor */
#define SCR_DTOR(destructor)                 ((scr_dtor)(destructor))


typedef INT32U (* SCREENPROC)(GUI_HWND, INT32U, GUI_WPARAM, GUI_LPARAM);

#if 0
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
#endif

 
struct SCREEN_BASE
{
	/* constructor and destructor */
	scr_ctor Ctor;
	scr_dtor Dtor;

	INT8U   WinType;          // the window type
	INT8U   Style;
	INT16U  ExtStyle;

	GUI_HWND   Handle;
	INT8U      ParentID;    //父窗口ID号
	INT8U      CurrentID;   //本窗口ID号
	INT16U     ScreenName; //本窗口名称  字符串的ID
	gui_list_t    List;
	INT32U (*ScreenProc)(GUI_HWND, INT32U, GUI_WPARAM, GUI_LPARAM); 
	/* size of type */
	INT32U  Size;
};

//动态创建一个屏幕
CScreenBase * CreateScreen( INT32U nSize ,scr_ctor ctor,scr_dtor dtor);
//静态初始化一个屏幕
void InitScreen(CScreenBase *pScr, scr_ctor ctor,scr_dtor dtor );

__inline GUI_HWND GetScreenHandle( CScreenBase *pScreen )
{
	if (pScreen) 
		return pScreen->Handle;	 
	return HWND_NULL;
}

void SetScreenInfo(CScreenBase *pScreen,INT8U ParentID,INT8U CurrentID,INT16U nScreenName );
//设置标题
void SetScreenTitle( void );

INT32U DefaultScreenProc (GUI_HWND hWnd, INT32U message, GUI_WPARAM wParam, GUI_LPARAM lParam);
 

#define GET_SCREEN_PTR(hWnd)   ((CScreenBase *)hWnd) 

INT8U IsScreen( GUI_HWND hWnd );


#ifndef SCR_BASE_DEF
#define SCR_BASE_DEF   \
	scr_ctor Ctor; \
	scr_dtor Dtor; \
	INT8U   WinType;  \
	INT8U   Style;   \
	INT16U  ExtStyle; \
	GUI_HWND   Handle; \
	INT8U        ParentID; \
	INT8U        CurrentID;   \
	INT16U       ScreenName; \
	gui_list_t    List; \
	INT32U (*ScreenProc)(GUI_HWND, INT32U, GUI_WPARAM, GUI_LPARAM); \
	INT32U  Size;
#endif

 

