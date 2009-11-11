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
#include <stdio.h>
#include <string.h>


#include "ScreenBase.h"


void SetScreenInfo(CScreenBase *pScreen,INT8U ParentID,INT8U CurrentID,INT16U nScreenName )
{
	if (pScreen)
	{
		pScreen->ParentID = ParentID;
		pScreen->CurrentID = CurrentID;
		pScreen->ScreenName = nScreenName;
	}
}


//动态创建一个屏幕
CScreenBase * CreateScreen( INT32U nSize,scr_ctor ctor,scr_dtor dtor )
{
	CScreenBase *pObj;

	if (nSize <= 0 || nSize > MAX_SCR_SIZE)
		return RT_NULL;

	pObj = rt_malloc(nSize);
	if (pObj == RT_NULL)
		return RT_NULL;

	pObj->Ctor = ctor;
	pObj->Dtor = dtor;
	pObj->Size = nSize;

	if(ctor)
	{
		ctor(pObj);
	}
	return pObj;
}
