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
#include <string.h>
#include "applib.h" 
#include "GDI.h" 
#include "GuiBlockHeap.h"

/*  1 if two RECTs overlap.
 *  0 if two RECTs do not overlap.
 */
#define EXTENTCHECK(r1, r2) \
        ((r1)->right > (r2)->left && \
         (r1)->left < (r2)->right && \
         (r1)->bottom > (r2)->top && \
         (r1)->top < (r2)->bottom)


#define REGION_NOT_EMPTY(region) region->pHead

#define INRECT(r, x, y) \
	( ( ((r).right >  x)) && \
	( ((r).left <= x)) && \
	( ((r).bottom >  y)) && \
	( ((r).top <= y)) )
 
/*********************************************************************************************************
** 函数名称: PtInRegion
** 函数名称: PtInRegion
**
** 功能描述：  判断点是否在区域内
**
** 输　入:  CGuiClipRgn * pRegion
** 输　入:  INT32U x
** 输　入:  INT32U y
**          
** 输　出:   INT8U TRUE 点在区域内
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月16日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U  PtInRegion (CGuiClipRgn *pRegion, INT32U x, INT32U y)
{
	int top;
	CGuiClipRect *pClipRect = NULL;

	ASSERT(pRegion);
	if (pRegion)
		return FALSE;

	pClipRect = pRegion->pHead;
 
	/* check with bounding rect of clipping region */
	if (y >= pRegion->pTail->Rect.bottom || y < pRegion->pHead->Rect.top)
		return FALSE;

	/* find the ban in which this point lies */
	pClipRect = pRegion->pHead;
	while (pClipRect && y >= pClipRect->Rect.bottom) 
	{
		pClipRect = pClipRect->pNext;
	}

	if (!pClipRect) 
		return FALSE;

	/* check in this ban */
	top = pClipRect->Rect.top;
	while (pClipRect && pClipRect->Rect.top == top) 
	{
		if (INRECT (pClipRect->Rect, x, y))
			return TRUE;

		pClipRect = pClipRect->pNext;
	}
 
	return FALSE;
}


/* Returns TRUE if rect is at least partly inside region */
INT8U RectInRegion (CGuiClipRgn *pRegion, const CGuiRect* pRect)
{
	INT8U ret = FALSE;

	CGuiClipRect *pClipRect = pRegion->pHead;
	
	if (pClipRect && EXTENTCHECK (&pRegion->RectBound, pRect)) 
	{
		while (pClipRect) 
		{
			if (pClipRect->Rect.bottom <= pRect->top) 
			{
				pClipRect = pClipRect->pNext;
				continue;             /* not far enough down yet */
			}

			if (pClipRect->Rect.top >= pRect->bottom) 
			{
				ret = FALSE;          /* too far down */
				break;
			}

			if (pClipRect->Rect.right <= pRect->left) 
			{
				pClipRect = pClipRect->pNext;
				continue;              /* not far enough over yet */
			}

			if (pClipRect->Rect.left >= pRect->right) 
			{
				pClipRect = pClipRect->pNext;
				continue;
			}

			ret = TRUE;
			break;
		}
	}

	return ret;
}

/* Init a region */
void  InitClipRgn (CGuiClipRgn* pRgn, CGuiBlockHeap* pHeap)
{
//	pRgn->type = NULLREGION;
	SetGuiRectEmpty (&pRgn->RectBound);
	pRgn->pHead = NULL;
	pRgn->pTail = NULL;

	pRgn->pHeap = pHeap;   // read-only field.
}

/* return bound rect of a region */
void GetClipRgnBoundRect (CGuiClipRgn *pRgn, CGuiRect *pRect)
{
	*pRect = pRgn->RectBound;
}

INT8U IsEmptyClipRgn (const CGuiClipRgn* pRgn)
{
	if (pRgn->pHead == NULL)
		return TRUE;

	return FALSE;
}

void EmptyClipRgn (CGuiClipRgn *pRgn)
{
	CGuiClipRect *pCRect, *pTemp;

	pCRect = pRgn->pHead;
	while (pCRect) 
	{
		pTemp = pCRect->pNext ;
//		FreeClipRect (pRgn->heap, pCRect);
		pCRect = pTemp;
	}

//	pRgn->type = NULLREGION;
	SetGuiRectEmpty (&pRgn->RectBound);
	pRgn->pHead = NULL;
	pRgn->pTail = NULL;
}

