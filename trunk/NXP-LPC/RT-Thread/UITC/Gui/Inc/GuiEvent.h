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

enum GUI_EVENT_TYPE
{
	GUI_EVENT_INIT=10,                 // 初始化
	GUI_EVENT_CREATE,			/* create a window 		*/
	GUI_EVENT_DESTROY,		   /* destroy a window 	*/

	GUI_EVENT_REDRAW,                // 画面重新刷新
	GUI_EVENT_TIMER,                  // 定时器消息

	GUI_EVENT_KEYDOWN,                // 键盘消息
	GUI_EVENT_MSGBOX,                 //
	GUI_EVENT_COMMAND,              /* user command 		*/

};
typedef enum GUI_EVENT_TYPE GuiEventType;
struct SCREEN_BASE;
struct GUI_EVENT
{
	INT32U Msg;
	INT32U WParam;
	INT32U LParam;
	INT32U Flag;

	struct SCREEN_BASE *pScreen;

	struct GuiListNode NextNode;
};

typedef struct GUI_EVENT GuiEvent;
  
INT8U SendScreenEvnent( struct SCREEN_BASE *pScr,INT32U Msg,INT32U wParam,INT32U lParam );
INT8U PostScreenEvnent( struct SCREEN_BASE *pScr,INT32U Msg,INT32U wParam,INT32U lParam ,INT8U bNoCheck);
INT8U HandleScreenEvent(struct SCREEN_BASE *pScr);
