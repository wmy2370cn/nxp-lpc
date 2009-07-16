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
/****************************************Copyright (c)**************************************************
**     消息都是由当前窗口响应，消息的发送者有键盘任务，液晶GUI任务，...                                 
**                                      
********************************************************************************************************/
#include <stdlib.h>
#include "includes.h" 
#include "Message.h"
#include "ScreenBase.h"

extern CScreenMgr  g_ScreeLib;
static UIMSG  Msg;



/*********************************************************************************************************
** 函数名称: PostMessage
** 函数名称: PostMessage
**
** 功能描述：寄送消息，异步模式 
**
** 输　入:  INT32U msg
** 输　入:  INT32U param
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月21日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void PostMessage(INT32U msg,INT32U param)
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
	OS_ENTER_CRITICAL();
	if (Msg.Message)
	{//如果发现有消息没有处理
		Msg.Flag ++;
	}
	Msg.Message = msg;
	Msg.WParam = param;

	OS_EXIT_CRITICAL();
}

/*********************************************************************************************************
** 函数名称: SendMessage
** 函数名称: SendMessage
**
** 功能描述： 发送消息，阻塞式
**
** 输　入:  INT32U msg
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月21日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern void SendMessage(INT32U msg,INT32U param)
{
	CScreenBase *pScreen = NULL;
	CScreenMgr *pMgr = NULL;

	pMgr = &g_ScreeLib;

	if (pMgr == NULL)
		return;	

	pScreen = GetCurrentScreenPtr(pMgr);

	if (pScreen && pScreen->pfnMessage)
	{
		pScreen->pfnMessage(msg,param);
	}
}

/*********************************************************************************************************
** 函数名称: HandleMessage
** 函数名称: HandleMessage
**
** 功能描述： 处理异步消息
**
** 输　入:  void
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月21日
** 备  注:  在界面循环任务里面调用
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/ 
extern void HandleMessage(void)
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
	CScreenBase *pScreen = NULL;
	CScreenMgr *pMgr = NULL;

	pMgr = &g_ScreeLib;

	if (pMgr == NULL)
		return;	

	pScreen = GetCurrentScreenPtr(pMgr);

	if (pScreen == NULL)
		return;
 
 
	OS_ENTER_CRITICAL();
	if (Msg.Message)
	{//处理
//		pScreen->pfnMessage(Msg.Message,Msg.WParam);
		if (Msg.Message == LCD_WM_KEYDOWN && pScreen->pfnKeyDown)
		{
			pScreen->pfnKeyDown((INT16U) Msg.WParam );
		}
		else if (  pScreen->pfnMessage )
		{
			pScreen->pfnMessage(Msg.Message,Msg.WParam);
		}			 
		//处理完，清标志
		Msg.Message = 0;
		Msg.WParam = 0;
	}
	OS_EXIT_CRITICAL();		
}
  