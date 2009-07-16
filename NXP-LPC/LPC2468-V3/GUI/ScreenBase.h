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
#ifndef SCREEN_BASE_H
#define	SCREEN_BASE_H
#include "guiconst.h"
#include "global.h"





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
}CScreenBase;

extern void SetScreenInfo(CScreenBase *pScreen,INT8U ParentID,INT8U CurrentID,INT16U nScreenName );


//得到/设置激活的窗口ID
extern INT8U   GetActiveWinID(void);
extern void    SetActiveWinID(const INT8U id);

//得到/设置本窗口父窗口ID
extern INT8U GetFatherID(CScreenBase *pScreen);
extern void  SetFatherID(CScreenBase *pScreen, INT8U nID);



//界面管理
typedef struct CScreenMgr
{
	CScreenBase *m_ScreenArray[MAXSCREEN];   // 界面指针数组
	INT16U 	     ScreensCnt;	          // 界面总数

}CScreenMgr;

extern void InitScreenMgr( CScreenMgr *pMgr );




// 加入一个新的屏幕到界面库中
extern INT8U  AppendScreen(CScreenMgr *pMgr,CScreenBase *pScreen);
// 判断序号ScreenID屏幕是否处于激活状态
extern INT8U IsScreenActived(CScreenMgr *pMgr ,INT8U nScreenID) ;
// 得到指向序号ScreenID屏幕的指针
extern  CScreenBase* GetScreenPtr(CScreenMgr *pMgr ,INT8U nScreenID);
// 得到当前屏幕的指针
extern CScreenBase* GetCurrentScreenPtr(CScreenMgr *pMgr);
// 得到界面总数
extern INT16U GetScreensCnt(CScreenMgr *pMgr) ;

// 切换到序号ScreenID屏幕
extern void  SwitchScreen(CScreenMgr *pMgr,INT8U nScreenID);

#endif
