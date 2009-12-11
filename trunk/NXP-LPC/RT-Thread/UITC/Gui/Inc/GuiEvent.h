/****************************************Copyright (c)**************************************************
**                                   消息定义
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

enum GUI_EVENT_TYPE
{
	GUI_EVENT_INIT=10,                 // 初始化
	GUI_EVENT_CREATE,			/* create a window 		*/
	GUI_EVENT_DESTROY,		   /* destroy a window 	*/

	GUI_EVENT_REDRAW,                // 画面重新刷新
	GUI_EVENT_TIMER,                 // 定时器消息
	GUI_EVENT_PAINT,                 // 画客户区
	GUI_EVENT_NCPAINT,               // 非客户区 ,比如标题
	GUI_EVENT_KEYDOWN,                // 键盘消息
 	GUI_EVENT_USER,              /* user command 		*/

};
typedef enum GUI_EVENT_TYPE GuiEventType;
struct SCREEN_BASE;

struct GUI_EVENT
{
	INT32U Msg;
	GUI_WPARAM WParam;
	GUI_LPARAM LParam;
	INT32U Flag;
	GUI_HWND Handle;
	INT32U  Time;
};

typedef struct GUI_EVENT GuiEvent;

#ifndef GUI_EVENT_DEF
#define  GUI_EVENT_DEF \
	INT32U Msg;\
	GUI_WPARAM WParam;\
	GUI_LPARAM LParam;\
	INT32U Flag;\
	GUI_HWND Handle;\
	INT32U  Time;	
#endif


struct GUI_EVENT_NODE
{
	GUI_EVENT_DEF
	struct GuiListNode NextNode;
};

typedef struct GUI_EVENT_NODE GuiEventNode;

//初始化消息队列
INT8U InitGuiEventQueue( void );

INT8U SendScreenEvnent( GUI_HWND *pWnd,INT32U Msg,INT32U wParam,INT32U lParam );
INT8U PostScreenEvnent( GUI_HWND *pWnd,INT32U Msg,INT32U wParam,INT32U lParam ,INT8U bNoCheck);
INT8U HandleScreenEvent(struct SCREEN_BASE *pScr);

//用于控件向父窗口邮寄消息-通知消息
INT8U SendNotifyEvent(GUI_HWND hScr,INT32U Msg,INT32U WParam,INT32U LParam);




INT8U PostEventToGuiTask(  GuiEvent * pEvent );
INT8U GuiTaskEventRecv( GuiEvent * pEvent  );
INT8U HandleTaskEvent(  GuiEvent * pEvent );

