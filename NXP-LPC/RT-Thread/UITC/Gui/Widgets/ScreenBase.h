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

#ifndef SCREEN_BASE_H
#define	SCREEN_BASE_H
#include "guiconst.h"
#include "global.h"
//#include "Message.h"


typedef struct SCR_EVENT
{
	INT32U Msg;
	INT32U WParam;
	INT32U LParam;
	INT32U Flag;
}SCR_EVENT ,*pSCR_EVENT;

//按键处理
typedef  void  (*fnOnKeyDown)( INT16U keyCode);
//消息处理虚函数
typedef  void   (*fnOnMessage)(INT32U msg, INT32U param);
//初始化窗口
typedef  void    (*fnInit)();
//退出时处理
typedef  void	(*fnDestory)();
//消息通知
typedef  void	(*fnOnNotify)(INT32U nf);

#define  MAX_ENENT_CNT  4

typedef struct CScreenBase 
{
	INT8U        ParentID;    //父窗口ID号
	INT8U        CurrentID;   //本窗口ID号
	INT16U       ScreenName; //本窗口名称

	//回调函数
	fnOnKeyDown pfnKeyDown;
	fnOnMessage pfnMessage;
	fnInit      pfnInit;
	fnDestory   pfnDestory;
	fnOnNotify  pfnOnNotify;

	//用链表保存事件

//	UEVENT  Event[MAX_ENENT_CNT];
}CScreenBase;

//伪结构
typedef struct CScreen
{
	CScreenBase Base;
}CScreen;


extern void InitScreenBase(CScreenBase *pBase);

extern void SetScreenInfo(CScreenBase *pScreen,INT8U ParentID,INT8U CurrentID,INT16U nScreenName );



//事件
extern INT8U HandleEvent(CScreenBase *pScreen);
extern INT8U PostEvent(CScreenBase *pScreen,INT32U msg,INT32U param);
extern INT8U SendEvent(CScreenBase *pScreen,INT32U msg,INT32U param);
extern void  ClearEvent(CScreenBase *pScreen);




//界面管理
typedef struct CScreenMgr
{
	CScreen *m_ScreenArray[MAXSCREEN];   // 界面指针数组
	INT16U 	     ScreensCnt;	          // 界面总数

}CScreenMgr;

extern void InitScreenMgr( CScreenMgr *pMgr );




// 加入一个新的屏幕到界面库中
extern INT8U  AppendScreen(CScreenMgr *pMgr,CScreen *pScreen);
// 判断序号ScreenID屏幕是否处于激活状态
extern INT8U IsScreenActived(CScreenMgr *pMgr ,INT8U nScreenID) ;
// 得到指向序号ScreenID屏幕的指针
extern  CScreen* GetScreenPtr(CScreenMgr *pMgr ,INT8U nScreenID);
// 得到当前屏幕的指针
extern CScreen* GetCurrentScreenPtr(CScreenMgr *pMgr);
// 得到界面总数
extern INT16U GetScreensCnt(CScreenMgr *pMgr) ;

// 切换到序号ScreenID屏幕
extern void  SwitchScreen(CScreenMgr *pMgr,INT8U nScreenID);


extern void DrawScreenCaption( CScreen  *pScreen, INT16U nTotal, INT16U nCur);
extern void DrawScreenStatusBar( );

#endif
