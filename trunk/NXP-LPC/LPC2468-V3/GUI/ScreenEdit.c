/****************************************Copyright (c)**************************************************
**                             提供编辑功能，消息框显示功能的窗口         
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
#include "includes.h"
#include "GuiConst.h"
#include "ScreenEdit.h"
#include "global.h"


static __inline void SetState(CScreenEdit *pScreenEdit,INT16U nState)
{
	if (pScreenEdit)
	{
		pScreenEdit->m_nState |=nState;
	}
}
static __inline void ClrState(CScreenEdit *pScreenEdit,INT16U nState)
{
	if (pScreenEdit)
	{
		pScreenEdit->m_nState &= ~nState ;
	}
}

static void EnableEdit(CScreenEdit *pScreenEdit,INT8U bEdit)
{
	if (pScreenEdit)
	{
		if(bEdit)
		{
			SetState(pScreenEdit,STATE_EDIT);
		}
		else
		{
			ClrState(pScreenEdit,STATE_EDIT);
		}

//		m_EditBox.SetEditable(bEdit);

	}
}

 