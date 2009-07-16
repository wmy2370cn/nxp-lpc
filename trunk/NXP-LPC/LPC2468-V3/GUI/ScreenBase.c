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
#include "includes.h" 
#include "GuiConst.h"
#include "ScreenBase.h"
#include "global.h"
#include "DC.h"


static INT8U        g_ActiveID;    //激活的窗口ID



extern __inline INT8U  GetActiveWinID(void)
{
	return g_ActiveID;
}  

extern __inline void   SetActiveWinID(const INT8U id)
{
	g_ActiveID = id;
}  


//得到/设置本窗口父窗口ID
extern __inline INT8U GetFatherID(CScreenBase *pScreen)
{
	if (pScreen)
	{
		return pScreen->ParentID;
	}

	return 0;
}

extern __inline void  SetFatherID(CScreenBase *pScreen, INT8U nID)
{
	if (pScreen)
	{
		pScreen->ParentID = nID;
	}
}

extern void SetScreenInfo(CScreenBase *pScreen,INT8U ParentID,INT8U CurrentID,INT16U nScreenName )
{
	if (pScreen)
	{
		pScreen->ParentID = ParentID;
		pScreen->CurrentID = CurrentID;
		pScreen->ScreenName = nScreenName;
	}
}


 



extern void InitScreenMgr( CScreenMgr *pMgr )
{
	INT16U i = 0;

	if (pMgr)
	{
		for( i=0; i<MAXSCREEN; i++)
			pMgr->m_ScreenArray[i] = NULL;                        // 界面指针数组

		pMgr->ScreensCnt	         = 0;                       // 界面总数
	}
}

// 判断序号ScreenID屏幕是否处于激活状态
extern INT8U IsScreenActived(CScreenMgr *pMgr ,INT8U nScreenID) 
{
	if (pMgr)
	{
		if (nScreenID == GetActiveWinID())
			return TRUE;		
	}
	return FALSE;
}

// 得到指向序号ScreenID屏幕的指针
extern  CScreenBase* GetScreenPtr(CScreenMgr *pMgr ,INT8U nScreenID)
{
	if(pMgr == NULL || (nScreenID >= MAXSCREEN) || (nScreenID == ID_WINDOW_NULL))
		return NULL;
	else
		return (pMgr->m_ScreenArray[nScreenID]);
}
// 得到当前屏幕的指针
extern CScreenBase* GetCurrentScreenPtr(CScreenMgr *pMgr)
{
	if(pMgr)
		return (pMgr->m_ScreenArray[GetActiveWinID()]);
	return NULL;
}
// 得到界面总数
extern __inline INT16U GetScreensCnt(CScreenMgr *pMgr)  
{
	if (pMgr)
	{
		return pMgr->ScreensCnt;
	}
	return 0;	
}	

// 加入一个新的屏幕到界面库中
extern INT8U  AppendScreen(CScreenMgr *pMgr,CScreenBase *pScreen)
{
	if (pScreen== (CScreenBase*)0 || pMgr == (CScreenMgr *)0)
		return FALSE;	

	if( pMgr->ScreensCnt >= MAXSCREEN)
		return FALSE;
	else
	{
		pMgr->ScreensCnt++;
		pMgr->m_ScreenArray[pScreen->CurrentID] =pScreen;  
		return TRUE;
	}	
} 

// 切换到序号ScreenID屏幕
void SwitchScreen(CScreenMgr *pMgr,INT8U nScreenID)
{
	CScreenBase* pScreen = NULL;
	if (pMgr == NULL)
		return ;
	
	pScreen=GetCurrentScreenPtr(pMgr);

	//首先销毁原窗口
	if(pScreen && pScreen->pfnDestory)
	{
		pScreen->pfnDestory();
	}
	
	//切换到下一窗口
	pScreen=GetScreenPtr(pMgr,nScreenID); 
 
 	//如果切换到密码输入框保存密码输入框返回窗口
 	if(nScreenID == ID_PASSWORD || nScreenID == ID_MODALDIALOG)
 		SetFatherID(pScreen,GetActiveWinID());
 	   
	SetActiveWinID(nScreenID);
 
 	//清除屏幕,模式对话框因为要保存上个界面的图形所有不进行清屏
 	if(nScreenID != ID_MODALDIALOG)
 		ClearScreen();
	if(pScreen != NULL && pScreen->pfnInit)
 		pScreen->pfnInit();// 界面默认初始化代码，比如建立资源
}
