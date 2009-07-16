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
#include "CtrlBase.h"
#include "global.h"
#include "DC.h"


//擦除背景
static void EraseBackground(CCtrlBase *pCtrlbase)
{
	DrawBlockRect(& pCtrlbase->Block,COLOR_BK);
}














//EDITBOX 内部私有接口


static INT32U GetValue(CEditBox *pEditBox)
{
	INT32U nValue=0;
	INT32U index=0;
	INT32U i = 0;
	if (pEditBox == NULL)
		return -1;

	if(pEditBox->Type==EDIT_TYPE_NUMBER)
	{		
		for ( i=0;i<MAX_CHARNUMBER;i++) 
		{
			if(pEditBox->NumberChar[i].type==CHAR_NUMBER)
			{
				nValue+=(pEditBox->NumberChar[i].ch-'0')*TenMultiple[index];
				index++;
			}
			else if(pEditBox->NumberChar[i].type==CHAR_SIGN)
			{
				if(pEditBox->NumberChar[i].ch=='-')
				{
					nValue=0-nValue;	
				}

				break;
			}
			else if(pEditBox->NumberChar[i].type==CHAR_NONE)
			{
				break;
			}
		}

		return nValue;
	}
	else if(pEditBox->Type==EDIT_TYPE_ENUM)
	{
		return pEditBox->m_pSEnum->pSValue[pEditBox->m_nEnumIndex].n;
	}
	return 0;
}
// 有效性检查
static INT8U DDX(CEditBox *pEditBox)
{
	if (pEditBox == NULL)
		return FALSE;

	pEditBox->m_nValue = GetValue(pEditBox);

	if(pEditBox->Type==EDIT_TYPE_NUMBER)
	{
		if((pEditBox->m_nValue>pEditBox->m_nMax) || (pEditBox->m_nValue<pEditBox->m_nMin))
			return FALSE;
		else
			return TRUE;
	}
	else if (pEditBox->Type==EDIT_TYPE_ENUM)
	{
		if((pEditBox->m_nEnumIndex>pEditBox->m_pSEnum->count) || (pEditBox->m_nEnumIndex<0))
			return FALSE;
		else
			return TRUE;
	}
	return FALSE;
}

extern INT8U  GetValueInt(CEditBox *pEditBox,INT32U *pValue)
{
	INT8U bValid= FALSE;
	
	if (pEditBox == NULL || pValue == NULL )
	{
		return FALSE;
	}

	bValid= DDX(pEditBox);
	*pValue = pEditBox->m_nValue;
	return bValid;
}


extern void SetEditable(CEditBox *pEditBox,INT8U bEditable)
{
	if (pEditBox)
	{
		pEditBox->m_bEditable = bEditable;
		if (pEditBox->Base.pfnMessage)
		{
			pEditBox->Base.pfnMessage(LCD_WM_INI, NULL);
		}
	}
}


 


 
static __inline CMenuData * GetItem(CLcdMenu *pMenu,INT16U nItemIndex) 
{
	if (pMenu)
	{
		return & pMenu->MenuLink[nItemIndex];
	}
	else
		return NULL;
}

static __inline void SetItemData(CLcdMenu *pMenu,INT16U nItemIndex, INT32U nItemData)
{
	if (pMenu == NULL)
		return;

	if(nItemIndex < pMenu->MenuCount)
	{
		pMenu->MenuLink[nItemIndex].nItemData=nItemData;
	}
}

static __inline INT32U  GetItemData(CLcdMenu *pMenu,INT16U nItemIndex) 
{
	if (pMenu)
	{
		return pMenu->MenuLink[nItemIndex].nItemData;
	}
	else 
		return 0;
}

extern INT16U GetCurrentMenuID(CLcdMenu *pMenu) 
{
	CMenuData menu ;
	if (pMenu)
	{
		menu =* GetItem(pMenu,pMenu->CurrentItem );
		return menu.nMenuID;
	}
	return 0;
}

extern void SetCurrentItem(CLcdMenu *pMenu,INT16U nItem)
{
	if (pMenu && nItem<pMenu->MenuCount)
	{
		pMenu->CurrentItem = nItem;
	}
}


extern void AddItemPtr(CLcdMenu *pMenu, const CMenuData * pMenuItem)
{
	if (pMenu == NULL || pMenuItem == NULL )
		return;
	if (pMenu->MenuCount>=MAX_MENU_ITEMS)
		return;
 
	pMenu->MenuLink[pMenu->MenuCount++] = *pMenuItem;
	return;
}

extern void AddItemID(CLcdMenu *pMenu,INT16U nMenuID, INT16U nResourceID)
{
	CMenuData Item;

	if (pMenu == NULL )
		return;
	if (pMenu->MenuCount>=MAX_MENU_ITEMS)
		return;

	Item.nMenuID=nMenuID;
	Item.nResourceID=nResourceID;
	Item.nItemData=0;
	AddItemPtr(pMenu,&Item);
}

extern void AddItems(CLcdMenu *pMenu, const CMenuData Items[], INT16U nItemCount)
{
	INT16U i = 0;

	for ( i=0;i<nItemCount;i++)
	{
		AddItemPtr(pMenu, &Items[i]);
	}
}

/*********************************************************************************************************
** 函数名称: DispMenu
** 函数名称: DispMenu
**
** 功能描述： 显示菜单 内部私有接口
**
** 输　入:  CLcdMenu * pMenu
**          
** 输　出:  void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月22日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void DispMenu(CLcdMenu *pMenu)
{
	INT8U i,start,end;

	if (pMenu == NULL)
		return;

	ClearScreen();

	if(pMenu->MenuCount == 0)
	{
		DrawStringByStr( pMenu->Base.Block.left,pMenu->Base.Block.top,"NO MENU ITEM",PEN_COPY,ALIGN_LEFT,LCD_EN_FONT_8_8);
		return;
	}

}