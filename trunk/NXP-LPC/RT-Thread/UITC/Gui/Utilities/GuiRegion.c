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

typedef void (*voidProcp1)(CGuiRegion *region, const CGuiRectNode *r1, const CGuiRectNode *r1End,
						   const CGuiRectNode *r2, const CGuiRectNode *r2End, INT32U top, INT32U bottom);
typedef void (*voidProcp2)(CGuiRegion *region, const CGuiRectNode *r, const CGuiRectNode *rEnd,
						   INT32U top, INT32U bottom);

#define AllocRect(heap)     BlockDataAlloc (heap)
#define FreeRect(heap, cr)  BlockDataFree (heap, cr);

/*
 * Allocate a new clipping pRect and add it to the pRegion.
 */

#define NEW_RECT_NODE(pRegion, pRect) \
       {\
            pRect = AllocRect(pRegion->pHeap);\
            pRect->pNext = NULL;\
            pRect->pPrev = pRegion->pTail;\
            if (pRegion->pTail)\
                pRegion->pTail->pNext = pRect;\
            pRegion->pTail = pRect;\
            if (pRegion->pHead == NULL)\
                pRegion->pHead = pRect;\
       }


/*  1 if two RECTs overlap.
 *  0 if two RECTs do not overlap.
 */
#define EXTENTCHECK(r1, r2) \
        ((r1)->right > (r2)->left && \
         (r1)->left < (r2)->right && \
         (r1)->bottom > (r2)->top && \
         (r1)->top < (r2)->bottom)


#define REGION_NOT_EMPTY(pRegion) pRegion->pHead

#define PT_IN_RECT(r, x, y) \
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
** 输　入:  CGuiRegion * pRegion
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
INT8U  PtInRegion (CGuiRegion *pRegion, INT32U x, INT32U y)
{
	int top;
	CGuiRectNode *pClipRect = NULL;

	ASSERT(pRegion);
	if (pRegion)
		return FALSE;

	pClipRect = pRegion->pHead;
 
	/* check with bounding pRect of clipping region */
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
		if (PT_IN_RECT (pClipRect->Rect, x, y))
			return TRUE;

		pClipRect = pClipRect->pNext;
	}
 
	return FALSE;
} 
/*********************************************************************************************************
** 函数名称: RectInRegion
** 函数名称: RectInRegion
**
** 功能描述：  Returns TRUE if rect is at least partly inside region
**
** 输　入:  CGuiRegion * pRegion
** 输　入:  const CGuiRect * pRect
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月17日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U RectInRegion (CGuiRegion *pRegion, const CGuiRect* pRect)
{
	INT8U ret = FALSE;
	CGuiRectNode *pClipRect = pRegion->pHead;
	
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
void  InitRegion (CGuiRegion* pRgn, CGuiBlockHeap* pHeap)
{
	pRgn->Type = GUI_REGION_NULL;
	SetGuiRectEmpty (&pRgn->RectBound);
	pRgn->pHead = NULL;
	pRgn->pTail = NULL;

	pRgn->pHeap = pHeap;   // read-only field.
}

/* return bound pRect of a region */
void GetRegionBoundRect (CGuiRegion *pRgn, CGuiRect *pRect)
{
	*pRect = pRgn->RectBound;
}

INT8U IsEmptyRegion (const CGuiRegion* pRgn)
{
	if (pRgn->pHead == NULL)
		return TRUE;

	return FALSE;
}

void EmptyRegion (CGuiRegion *pRgn)
{
	CGuiRectNode *pCRect, *pTemp;

	pCRect = pRgn->pHead;
	while (pCRect) 
	{
		pTemp = pCRect->pNext ;
 		FreeRect (pRgn->pHeap, pCRect);
		pCRect = pTemp;
	}

 	pRgn->Type = GUI_REGION_NULL;
	SetGuiRectEmpty (&pRgn->RectBound);
	pRgn->pHead = NULL;
	pRgn->pTail = NULL;
}


/* Creates a region */
CGuiRegion*  CreateRegion (void)
{
	CGuiRegion* pRgn = rt_malloc (sizeof(CGuiRegion));
//	InitRegion (pRgn, &__mg_FreeClipRectList);
	return pRgn;
}

/* Destroys a region */
void  DestroyClipRgn (CGuiRegion* pRegion)
{
	EmptyRegion (pRegion);
	rt_free (pRegion);
} 
/*********************************************************************************************************
** 函数名称: ResetRegion
** 函数名称: ResetRegion
**
** 功能描述：  Reset a region
**
** 输　入:  CGuiRegion * pRgn
** 输　入:  const CGuiRect * pRect
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月18日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U  ResetRegion (CGuiRegion*  pRgn, const CGuiRect* pRect)
{
	CGuiRectNode *pClipRect = NULL;

	if (pRgn == NULL || pRect == NULL)
		return FALSE;

	if (IsGuiRectEmpty (pRect))
		return FALSE;

	// empty rgn first
	EmptyRegion(pRgn);

	// get a new clip pRect from free list
 	pClipRect = (CGuiRectNode*) AllocRect (pRgn->pHeap);
	if (pClipRect == NULL)
		return FALSE;

	// set clip pRect
	pClipRect->Rect = *pRect;
	pClipRect->pNext  = NULL;
	pClipRect->pPrev = NULL;

	pRgn->Type = GUI_REGION_SIMPLE;
	pRgn->pHead  = pRgn->pTail = pClipRect;
	pRgn->RectBound = *pRect;

	return TRUE;
} 
/*********************************************************************************************************
** 函数名称: CopyRegion
** 函数名称: CopyRegion
**
** 功能描述：  区域拷贝
**
** 输　入:  CGuiRegion * pDstRgn
** 输　入:  const CGuiRegion *  * pSrcRgn
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月17日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U CopyRegion (CGuiRegion*  pDstRgn, const CGuiRegion*   pSrcRgn)
{
	CGuiRectNode* pRectNode;
	CGuiRectNode* pNewRect, *pPrevNode;

	// return false if the destination region is not an empty one.
	if (pDstRgn == NULL || pSrcRgn == NULL|| pDstRgn == pSrcRgn)
		return FALSE;

	EmptyRegion (pDstRgn);
	if (!(pRectNode = pSrcRgn->pHead))
		return TRUE;

 	pNewRect = (CGuiRectNode*)AllocRect (pDstRgn->pHeap);

	pDstRgn->pHead = pNewRect;
	pNewRect->Rect = pRectNode->Rect;

	pPrevNode = NULL;
	while (pRectNode->pNext) 
	{
	 	pNewRect->pNext =(CGuiRectNode*) AllocRect (pDstRgn->pHeap);
		pNewRect->pPrev = pPrevNode;

		pPrevNode = pNewRect;
		pRectNode = pRectNode->pNext;
		pNewRect = pNewRect->pNext;

		pNewRect->Rect = pRectNode->Rect;
	}

	pNewRect->pPrev = pPrevNode;
	pNewRect->pNext = NULL;
	pDstRgn->pTail = pNewRect;

	pDstRgn->Type = pSrcRgn->Type;
	pDstRgn->RectBound = pSrcRgn->RectBound; 

	return TRUE;
}

/* Re-calculate the rcBound of a region */
static void SetGuiRegionExtents (CGuiRegion *region)
{
    CGuiRectNode *cliprect;
    CGuiRect *pExtents;

    if (region->pHead == NULL) 
	{
        region->RectBound.left = 0; region->RectBound.top = 0;
        region->RectBound.right = 0; region->RectBound.bottom = 0;
        return;
    }

    pExtents = &region->RectBound;

    /*
     * Since head is the first rectangle in the region, it must have the
     * smallest top and since tail is the last rectangle in the region,
     * it must have the largest bottom, because of banding. Initialize left and
     * right from head and tail, resp., as good things to initialize them
     * to...
     */
    pExtents->left = region->pHead->Rect.left;
    pExtents->top = region->pHead->Rect.top;
    pExtents->right = region->pTail->Rect.right;
    pExtents->bottom = region->pTail->Rect.bottom;

    cliprect = region->pHead;
    while (cliprect) 
	{
        if (cliprect->Rect.left < pExtents->left)
            pExtents->left = cliprect->Rect.left;
        if (cliprect->Rect.right > pExtents->right)
            pExtents->right = cliprect->Rect.right;

        cliprect = cliprect->pNext;
    }
}

#ifdef _REGION_DEBUG
void dumpRegion (CLIPRGN* region)
{
	CLIPRECT *cliprect;

	if (!(cliprect = region->head)) 
	{
		fprintf (stderr, "region: %p is a null region.\n", region);
	}
	else 
	{
		fprintf (stderr, "start of region: %p.\n", region);
		fprintf (stderr, "head of region: %p.\n", region->head);
		fprintf (stderr, "tail of region: %p.\n", region->tail);
		fprintf (stderr, "Bound of region: (%d, %d, %d, %d)\n", 
			region->rcBound.left,
			region->rcBound.top,
			region->rcBound.right,
			region->rcBound.bottom);
		while (cliprect) {
			fprintf (stderr, "cliprect %p: (%d, %d, %d, %d)\n", cliprect, 
				cliprect->rc.left, cliprect->rc.top,
				cliprect->rc.right,
				cliprect->rc.bottom);

			cliprect = cliprect->next;
		}
		fprintf (stderr, "end of region: %p.\n", region);
	}

}
#endif

/***********************************************************************
 *           REGION_Coalesce
 *
 *      Attempt to merge the rects in the current band with those in the
 *      previous one. Used only by REGION_RegionOp.
 *
 * Results:
 *      The new index for the previous band.
 *
 * Side Effects:
 *      If coalescing takes place:
 *          - rectangles in the previous band will have their bottom fields
 *            altered.
 *          - some clipping pRect will be deleted.
 *
 */
static CGuiRectNode* CoalesceRegion (
             CGuiRegion *region,      /* Region to coalesce */
             CGuiRectNode *prevStart,  /* start of previous band */
             CGuiRectNode *curStart    /* start of current band */
) {
    CGuiRectNode *newStart;         /* Start of new band */
    CGuiRectNode *pPrevRect;        /* Current pRect in previous band */
    CGuiRectNode *pCurRect;         /* Current pRect in current band */
    CGuiRectNode *temp;             /* Temporary clipping pRect */
    INT32U curNumRects;            /* Number of rectangles in current band */
    INT32U prevNumRects;           /* Number of rectangles in previous band */
    INT32U bandtop;                /* top coordinate for current band */

    if (prevStart == NULL) 
		prevStart = region->pHead ;
    if (curStart == NULL) 
		curStart = region->pHead;

    if (prevStart == curStart)
        return prevStart;

    newStart = pCurRect = curStart;

    pPrevRect = prevStart;
    temp = prevStart;
    prevNumRects = 0;
    while (temp != curStart)
	{
        prevNumRects ++;
        temp = temp->pNext;
    }
    /*
     * Figure out how many rectangles are in the current band. Have to do
     * this because multiple bands could have been added in REGION_RegionOp
     * at the end when one region has been exhausted.
     */
    pCurRect = curStart;
    bandtop = pCurRect->Rect.top;
    curNumRects = 0;
    while (pCurRect && (pCurRect->Rect.top == bandtop)) 
	{
        curNumRects ++;
        pCurRect = pCurRect->pNext ;
    }
    
    if (pCurRect) 
	{
        /*
         * If more than one band was added, we have to find the start
         * of the last band added so the next coalescing job can start
         * at the right place... (given when multiple bands are added,
         * this may be pointless -- see above).
         */
        temp = region->pTail;
        while (temp->pPrev->Rect.top == temp->Rect.top)
		{
            temp = temp->pPrev;
        }
        newStart = temp;
    }

    if ((curNumRects == prevNumRects) && (curNumRects != 0)) 
	{
        pCurRect = curStart;
        /*
         * The bands may only be coalesced if the bottom of the previous
         * matches the top scanline of the current.
         */
        if (pPrevRect->Rect.bottom == pCurRect->Rect.top)
		{
            /*
             * Make sure the bands have rects in the same places. This
             * assumes that rects have been added in such a way that they
             * cover the most area possible. I.e. two rects in a band must
             * have some horizontal space between them.
             */
            do {
                if ((pPrevRect->Rect.left != pCurRect->Rect.left) ||
                    (pPrevRect->Rect.right != pCurRect->Rect.right))
                {
                    /*
                     * The bands don't line up so they can't be coalesced.
                     */
                    return newStart;
                }
                pPrevRect = pPrevRect->pNext;
                pCurRect = pCurRect->pNext;
            } while (--prevNumRects);

            /*
             * If only one band was added to the region, we have to backup
             * newStart to the start of the previous band.
             */
            if (pCurRect == NULL) 
			{
                newStart = prevStart;
            }
            /*
             * The bands may be merged, so set the bottom of each pRect
             * in the previous band to that of the corresponding pRect in
             * the current band.
             */
            /*
             * for implementation of MiniGUI, we should free 
             * the clipping rects merged.
             */
            pCurRect = curStart;
            pPrevRect = prevStart;
            do {
                pPrevRect->Rect.bottom = pCurRect->Rect.bottom;
                pPrevRect = pPrevRect->pNext;

                if (pCurRect->pNext)
                    pCurRect->pNext->pPrev = pCurRect->pPrev;
                else
                    region->pTail = pCurRect->pPrev;
                if (pCurRect->pPrev)
                    pCurRect->pPrev->pNext = pCurRect->pNext;
                else
                    region->pHead = pCurRect->pNext;

                temp = pCurRect->pNext;
                FreeRect (region->pHeap , pCurRect);
                pCurRect = temp;
            } while (--curNumRects);

            /*
             *
             * If more than one band was added to the region, copy the
             * other bands down. The assumption here is that the other bands
             * came from the same region as the current one and no further
             * coalescing can be done on them since it's all been done
             * already... newStart is already in the right place.
             */
            /* no need to copy for implementation of MiniGUI -- they are freed.
            if (temp == regionEnd) {
                newStart = prevStart;
            }

            else {
                do {
                    *pPrevRect++ = *pCurRect++;
                } while (pCurRect != regionEnd);
            }
            */
            
        }
    }
    return (newStart);
}
/*********************************************************************************************************
** 函数名称: RegionOperator
** 函数名称: RegionOperator
**
** 功能描述： Apply an operation to two regions. Called by Union, Xor, Subtract, Intersect...
**           The new region is overwritten.
** 输　入:  CGuiRegion * newReg
** 输　入:  const CGuiRegion * reg1
** 输　入:  const CGuiRegion * reg2
** 输　入:  voidProcp1 overlapFunc
** 输　入:  voidProcp2 nonOverlap1Func
** 输　入:  voidProcp2 nonOverlap2Func
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月19日
** 备  注:  
*  Notes:
*      The idea behind this function is to view the two regions as sets.
*      Together they cover a rectangle of area that this function divides
*      into horizontal bands where points are covered only by one region
*      or by both. For the first case, the nonOverlapFunc is called with
*      each the band and the band's upper and lower rcBound. For the
*      second, the overlapFunc is called to process the entire band. It
*      is responsible for clipping the rectangles in the band, though
*      this function provides the boundaries.
*      At the end of each band, the new region is coalesced, if possible,
*      to reduce the number of rectangles in the region.
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void RegionOperator(
            CGuiRegion *pNewRegion, /* Place to store result */
            const CGuiRegion *reg1,   /* First region in operation */
            const CGuiRegion *reg2,   /* 2nd region in operation */
            voidProcp1 overlapFunc,     /* Function to call for over-lapping bands */
            voidProcp2 nonOverlap1Func, /* Function to call for non-overlapping bands in region 1 */
            voidProcp2 nonOverlap2Func  /* Function to call for non-overlapping bands in region 2 */
) {
    CGuiRegion my_dst;
    CGuiRegion* pdst;
    const CGuiRectNode *r1;                 /* Pointer into first region */
    const CGuiRectNode *r2;                 /* Pointer into 2d region */
    const CGuiRectNode *r1BandEnd;          /* End of current band in r1 */
    const CGuiRectNode *r2BandEnd;          /* End of current band in r2 */
    int ybot;                           /* Bottom of intersection */
    int ytop;                           /* Top of intersection */
    CGuiRectNode* prevBand;                 /* start of previous band in pNewRegion */
    CGuiRectNode* curBand;                  /* start of current band in pNewRegion */
    int top;                            /* Top of non-overlapping band */
    int bot;                            /* Bottom of non-overlapping band */
    
    /*
     * Initialization:
     *  set r1, r2, r1End and r2End appropriately, preserve the important
     * parts of the destination region until the end in case it's one of
     * the two source regions, then mark the "new" region empty, allocating
     * another array of rectangles for it to use.
     */
    r1 = reg1->pHead;
    r2 = reg2->pHead;

    /*
     * pNewRegion may be one of the src regions so we can't empty it. We keep a 
     * note of its rects pointer (so that we can free them later), preserve its
     * rcBound and simply set numRects to zero. 
     */
    /*
    oldRects = pNewRegion->rects;
    pNewRegion->numRects = 0;
     */

    /* 
     * for implementation of MiniGUI, we create an empty region.
     */
    if (pNewRegion == reg1 || pNewRegion == reg2)
	{
        InitRegion(&my_dst, pNewRegion->pHeap);
        pdst = &my_dst;
    }
    else 
	{
        EmptyRegion(pNewRegion);
        pdst = pNewRegion;
    }

    /*
     * Allocate a reasonable number of rectangles for the new region. The idea
     * is to allocate enough so the individual functions don't need to
     * reallocate and copy the array, which is time consuming, yet we don't
     * have to worry about using too much memory. I hope to be able to
     * nuke the Xrealloc() at the end of this function eventually.
     */

    /* for implementation of MiniGUI, dst always is an empty region.
    pNewRegion->size = MAX(reg1->numRects,reg2->numRects) * 2;

    if (! (pNewRegion->rects = malloc( sizeof(CLIPRECT) * pNewRegion->size )))
    {
        pNewRegion->size = 0;
        return;
    }
     */    
    /*
     * Initialize ybot and ytop.
     * In the upcoming loop, ybot and ytop serve different functions depending
     * on whether the band being handled is an overlapping or non-overlapping
     * band.
     *  In the case of a non-overlapping band (only one of the regions
     * has points in the band), ybot is the bottom of the most recent
     * intersection and thus clips the top of the rectangles in that band.
     * ytop is the top of the next intersection between the two regions and
     * serves to clip the bottom of the rectangles in the current band.
     *  For an overlapping band (where the two regions intersect), ytop clips
     * the top of the rectangles of both regions and ybot clips the bottoms.
     */
    if (reg1->RectBound.top < reg2->RectBound.top)
        ybot = reg1->RectBound.top;
    else
        ybot = reg2->RectBound.top;    
    /*
     * prevBand serves to mark the start of the previous band so rectangles
     * can be coalesced into larger rectangles. qv. miCoalesce, above.
     * In the beginning, there is no previous band, so prevBand == curBand
     * (curBand is set later on, of course, but the first band will always
     * start at index 0). prevBand and curBand must be indices because of
     * the possible expansion, and resultant moving, of the new region's
     * array of rectangles.
     */
    prevBand = pdst->pHead;
    
	do {
		curBand = pdst->pTail;

		/*
         * This algorithm proceeds one source-band (as opposed to a
         * destination band, which is determined by where the two regions
         * intersect) at a time. r1BandEnd and r2BandEnd serve to mark the
         * rectangle after the last one in the current band for their
         * respective regions.
         */
		r1BandEnd = r1;
		while (r1BandEnd && (r1BandEnd->Rect.top == r1->Rect.top))
			r1BandEnd = r1BandEnd->pNext;

		r2BandEnd = r2;
		while (r2BandEnd && (r2BandEnd->Rect.top == r2->Rect.top))
			r2BandEnd = r2BandEnd->pNext;

        /*
         * First handle the band that doesn't intersect, if any.
         *
         * Note that attention is restricted to one band in the
         * non-intersecting region at once, so if a region has n
         * bands between the current position and the next place it overlaps
         * the other, this entire loop will be passed through n times.
         */
        if (r1->Rect.top < r2->Rect.top)
		{
            top = MAX (r1->Rect.top, ybot);
            bot = MIN (r1->Rect.bottom, r2->Rect.top);

            if ((top != bot) && (nonOverlap1Func != NULL))
                (* nonOverlap1Func) (pdst, r1, r1BandEnd, top, bot);

            ytop = r2->Rect.top;
        }
        else if (r2->Rect.top < r1->Rect.top) 
		{
            top = MAX (r2->Rect.top, ybot);
            bot = MIN (r2->Rect.bottom, r1->Rect.top);

            if ((top != bot) && (nonOverlap2Func != NULL))
                (* nonOverlap2Func) (pdst, r2, r2BandEnd, top, bot);

            ytop = r1->Rect.top;
        }
        else 
		{
            ytop = r1->Rect.top;
        }

        /*
         * If any rectangles got added to the region, try and coalesce them
         * with rectangles from the previous band. Note we could just do
         * this test in miCoalesce, but some machines incur a not
         * inconsiderable cost for function calls, so...
         */
        if (pdst->pTail != curBand) 
		{
            prevBand = CoalesceRegion(pdst, prevBand, curBand);
        }
        /*
         * Now see if we've hit an intersecting band. The two bands only
         * intersect if ybot > ytop
         */
        ybot = MIN (r1->Rect.bottom, r2->Rect.bottom);
        curBand = pdst->pTail;
        if (ybot > ytop)
            (* overlapFunc) (pdst, r1, r1BandEnd, r2, r2BandEnd, ytop, ybot);
        
        if (pdst->pTail != curBand)
            prevBand = CoalesceRegion (pdst, prevBand, curBand);

        /*
         * If we've finished with a band (bottom == ybot) we skip forward
         * in the region to the next band.
         */
        if (r1->Rect.bottom == ybot)
            r1 = r1BandEnd;
        if (r2->Rect.bottom == ybot)
            r2 = r2BandEnd;
    } while (r1 && r2);

    /*
     * Deal with whichever region still has rectangles left.
     */
    curBand = pdst->pTail;
    if (r1) 
	{
        if (nonOverlap1Func != NULL) 
		{
			do {
				r1BandEnd = r1;
				while ((r1BandEnd) && (r1BandEnd->Rect.top == r1->Rect.top)) 
				{
					r1BandEnd = r1BandEnd->pNext;
				}
				(* nonOverlap1Func) (pdst, r1, r1BandEnd,
					MAX (r1->Rect.top, ybot), r1->Rect.bottom);
				r1 = r1BandEnd;
			} while (r1);
		}
    }
    else if ((r2) && (nonOverlap2Func != NULL))
    {
        do {
            r2BandEnd = r2;
            while ((r2BandEnd) && (r2BandEnd->Rect.top == r2->Rect.top))
			{
                 r2BandEnd = r2BandEnd->pNext;
            }
            (* nonOverlap2Func) (pdst, r2, r2BandEnd,MAX (r2->Rect.top, ybot), r2->Rect.bottom);
            r2 = r2BandEnd;
        } while (r2);
    }

    if (pdst->pTail != curBand)
        (void) CoalesceRegion (pdst, prevBand, curBand);

    /*
     * A bit of cleanup. To keep regions from growing without bound,
     * we shrink the array of rectangles to match the new number of
     * rectangles in the region. This never goes to 0, however...
     *
     * Only do this stuff if the number of rectangles allocated is more than
     * twice the number of rectangles in the region (a simple optimization...).
     */

#if 0 // MiniGUI implementation does not need to do this
    if (newReg->numRects < (newReg->size >> 1))
    {
        if (REGION_NOT_EMPTY(newReg))
        {
            CLIPRECT *prev_rects = newReg->rects;
            newReg->size = newReg->numRects;
            newReg->rects = realloc( newReg->rects, sizeof(CLIPRECT) * newReg->size );
            if (! newReg->rects)
                newReg->rects = prev_rects;
        }
        else
        {
            /*
             * No point in doing the extra work involved in an Xrealloc if
             * the region is empty
             */
            newReg->size = 1;
            free( newReg->rects );
            newReg->rects = malloc( sizeof(CLIPRECT) );
        }
    }
    free( oldRects );
#else
    if (pdst != pNewRegion)
	{
        EmptyRegion (pNewRegion);
        *pNewRegion = my_dst;
    }
#endif
}

/***********************************************************************
 *          Region Intersection
 ***********************************************************************/


/***********************************************************************
 *             REGION_IntersectO
 *
 * Handle an overlapping band for REGION_Intersect.
 *
 * Results:
 *      None.
 *
 * Side Effects:
 *      Rectangles may be added to the region.
 *
 */
static void IntersectO (CGuiRegion *region, const CGuiRectNode *r1, const CGuiRectNode *r1End,
                        const CGuiRectNode *r2, const CGuiRectNode *r2End, INT32U top, INT32U bottom)

{
    INT32U       left, right;
    CGuiRectNode  *pNewClipRect = NULL;

    while ((r1 != r1End) && (r2 != r2End))
    {
        left  = MAX (r1->Rect.left, r2->Rect.left);
        right = MIN (r1->Rect.right, r2->Rect.right);

        /*
         * If there's any overlap between the two rectangles, add that
         * overlap to the new region.
         * There's no need to check for subsumption because the only way
         * such a need could arise is if some region has two rectangles
         * right next to each other. Since that should never happen...
         */
        if (left < right) 
		{
            NEW_RECT_NODE (region, pNewClipRect);

            pNewClipRect->Rect.left = left;
            pNewClipRect->Rect.top = top;
            pNewClipRect->Rect.right = right;
            pNewClipRect->Rect.bottom = bottom;
        }
        /*
         * Need to advance the pointers. Shift the one that extends
         * to the right the least, since the other still has a chance to
         * overlap with that region's next rectangle, if you see what I mean.
         */
        if (r1->Rect.right < r2->Rect.right) 
		{
            r1 = r1->pNext;
        }
        else if (r2->Rect.right < r1->Rect.right)
		{
            r2 = r2->pNext;
        }
        else 
		{
            r1 = r1->pNext;
            r2 = r2->pNext;
        }
    }
}

/***********************************************************************
 *             Region Union
 ***********************************************************************/

/***********************************************************************
 *             REGION_UnionNonO
 *
 *      Handle a non-overlapping band for the union operation. Just
 *      Adds the rectangles into the region. Doesn't have to check for
 *      subsumption or anything.
 *
 * Results:
 *      None.
 *
 * Side Effects:
 *      region->numRects is incremented and the final rectangles overwritten
 *      with the rectangles we're passed.
 *
 */
static void UnionNonO (CGuiRegion *region, const CGuiRectNode *r, const CGuiRectNode *rEnd, INT32U top, INT32U bottom)
{
    CGuiRectNode *newcliprect;

    while (r != rEnd)
	{
        NEW_RECT_NODE (region, newcliprect);
        newcliprect->Rect.left = r->Rect.left;
        newcliprect->Rect.top = top;
        newcliprect->Rect.right = r->Rect.right;
        newcliprect->Rect.bottom = bottom;

        r = r->pNext;
    }
}
/***********************************************************************
 *              UnionO
 *
 *      Handle an overlapping band for the union operation. Picks the
 *      left-most rectangle each time and merges it into the region.
 *
 * Results:
 *      None.
 *
 * Side Effects:
 *      Rectangles are overwritten in region->rects and region->numRects will
 *      be changed.
 *
 */
static void UnionO(CGuiRegion *pRegion, const CGuiRectNode *r1, const CGuiRectNode *r1End,
                           const CGuiRectNode *r2, const CGuiRectNode *r2End, INT32U top, INT32U bottom)
{
    CGuiRectNode *pNewClipRect;

 #define MERGE_RECT(r) \
	if ((pRegion->pHead) &&  \
	(pRegion->pTail->Rect.top == top) &&  \
	(pRegion->pTail->Rect.bottom == bottom) &&  \
	(pRegion->pTail->Rect.right >= r->Rect.left))  \
{  \
	if (pRegion->pTail->Rect.right < r->Rect.right)  \
{  \
	pRegion->pTail->Rect.right = r->Rect.right;  \
}  \
}  \
	else  \
{  \
	NEW_RECT_NODE(pRegion, pNewClipRect);  \
	pNewClipRect->Rect.top = top;  \
	pNewClipRect->Rect.bottom = bottom;  \
	pNewClipRect->Rect.left = r->Rect.left;  \
	pNewClipRect->Rect.right = r->Rect.right;  \
}  \
	r = r->pNext;

    
    while ((r1 != r1End) && (r2 != r2End))
    {
        if (r1->Rect.left < r2->Rect.left)
        {
            MERGE_RECT(r1);
        }
        else
        {
            MERGE_RECT(r2);
        }
    }
    
    if (r1 != r1End)
    {
        do
		{
            MERGE_RECT(r1);
        } while (r1 != r1End);
    }
    else 
    {
		while (r2 != r2End)
        MERGE_RECT(r2);
    }
}

/***********************************************************************
 *             Region Subtraction
 ***********************************************************************/

/***********************************************************************
 *             REGION_SubtractNonO1
 *
 *      Deal with non-overlapping band for subtraction. Any parts from
 *      region 2 we discard. Anything from region 1 we add to the region.
 *
 * Results:
 *      None.
 *
 * Side Effects:
 *      region may be affected.
 *
 */
static void SubtractNonO1 (CGuiRegion *region, const CGuiRectNode *r, const CGuiRectNode *rEnd,
                        INT32U top, INT32U bottom)
{
    CGuiRectNode *newcliprect;
        
    while (r != rEnd) 
	{
        NEW_RECT_NODE(region, newcliprect);
        newcliprect->Rect.left = r->Rect.left;
        newcliprect->Rect.top = top;
        newcliprect->Rect.right = r->Rect.right;
        newcliprect->Rect.bottom = bottom;
        r = r->pNext;
    }
}


/***********************************************************************
 *             REGION_SubtractO
 *
 *      Overlapping band subtraction. x1 is the left-most point not yet
 *      checked.
 *
 * Results:
 *      None.
 *
 * Side Effects:
 *      region may have rectangles added to it.
 *
 */
static void SubtractO (CGuiRegion *region, const CGuiRectNode *r1, const CGuiRectNode *r1End,
                        const CGuiRectNode *r2, const CGuiRectNode *r2End, INT32U top, INT32U bottom)
{
    CGuiRectNode *newcliprect;
    INT32U left;
    
    left = r1->Rect.left;
    while ((r1 != r1End) && (r2 != r2End)) 
	{
        if (r2->Rect.right <= left)
		{
            /*
             * Subtrahend missed the boat: go to next subtrahend.
             */
            r2 = r2->pNext;
        }
        else if (r2->Rect.left <= left)
        {
            /*
             * Subtrahend preceeds minuend: nuke left edge of minuend.
             */
            left = r2->Rect.right;
            if (left >= r1->Rect.right)
            {
                /*
                 * Minuend completely covered: advance to next minuend and
                 * reset left fence to edge of new minuend.
                 */
                r1 = r1->pNext ;
                if (r1 != r1End)
                    left = r1->Rect.left;
            }
            else
            {
                /*
                 * Subtrahend now used up since it doesn't extend beyond
                 * minuend
                 */
                r2 = r2->pNext;
            }
        }
        else if (r2->Rect.left < r1->Rect.right)
        {
            /*
             * Left part of subtrahend covers part of minuend: add uncovered
             * part of minuend to region and skip to next subtrahend.
             */
            NEW_RECT_NODE(region, newcliprect);
            newcliprect->Rect.left = left;
            newcliprect->Rect.top = top;
            newcliprect->Rect.right = r2->Rect.left;
            newcliprect->Rect.bottom = bottom;
            left = r2->Rect.right;
            if (left >= r1->Rect.right)
            {
                /*
                 * Minuend used up: advance to new...
                 */
                r1 = r1->pNext;
                if (r1 != r1End)
                    left = r1->Rect.left;
            }
            else
            {
                /*
                 * Subtrahend used up
                 */
                r2 = r2->pNext;
            }
        }
        else
        {
            /*
             * Minuend used up: add any remaining piece before advancing.
             */
            if (r1->Rect.right > left)
            {
                NEW_RECT_NODE(region, newcliprect);
                newcliprect->Rect.left = left;
                newcliprect->Rect.top = top;
                newcliprect->Rect.right = r1->Rect.right;
                newcliprect->Rect.bottom = bottom;
            }
            r1 = r1->pNext;
            if (r1 != r1End)
                left = r1->Rect.left;
        }
    }
    /*
     * Add remaining minuend rectangles to region.
     */
    while (r1 != r1End)
    {
        NEW_RECT_NODE(region, newcliprect);
        newcliprect->Rect.left = left;
        newcliprect->Rect.top = top;
        newcliprect->Rect.right = r1->Rect.right;
        newcliprect->Rect.bottom = bottom;
        r1 = r1->pNext;
        if (r1 != r1End)
            left = r1->Rect.left;
    }
}

/***********************************************************************
 *             IntersectRegion
 */
INT8U  ClipRgnIntersect (CGuiRegion *dst, const CGuiRegion *src1, const CGuiRegion *src2)
{
    /* check for trivial reject */
    if ( (!(src1->pHead)) || (!(src2->pHead))  ||
        (!EXTENTCHECK(&src1->RectBound, &src2->RectBound)))
    {
        EmptyRegion(dst);
        return FALSE;
    }
    else
        RegionOperator (dst, src1, src2,IntersectO, NULL, NULL);
    
    /*
     * Can't alter dst's rcBound before we call miRegionOp because
     * it might be one of the source regions and miRegionOp depends
     * on the rcBound of those regions being the same. Besides, this
     * way there's no checking against rectangles that will be nuked
     * due to coalescing, so we have to examine fewer rectangles.
     */
    SetGuiRegionExtents(dst);
    dst->Type = (dst->pHead) ? GUI_REGION_COMPLEX : GUI_REGION_NULL ;

    return TRUE;
}

/***********************************************************************
 *             SubtractRegion
 *
 *      Subtract rgnS from rgnM and leave the result in rgnD.
 *      S stands for subtrahend, M for minuend and D for difference.
 *
 * Results:
 *      TRUE.
 *
 * Side Effects:
 *      regD is overwritten.
 *
 */
INT8U  SubtractRegion (CGuiRegion *rgnD, const CGuiRegion *rgnM, const CGuiRegion *rgnS)
{
    /* check for trivial reject */
    if ( (!(rgnM->pHead)) || (!(rgnS->pHead))  ||
            (!EXTENTCHECK (&rgnM->RectBound, &rgnS->RectBound)) ) 
	{
        CopyRegion (rgnD, rgnM);
        return TRUE;
    }
 
    RegionOperator (rgnD, rgnM, rgnS, SubtractO, SubtractNonO1, NULL);

    /*
     * Can't alter pNewRegion's rcBound before we call miRegionOp because
     * it might be one of the source regions and miRegionOp depends
     * on the rcBound of those regions being the unaltered. Besides, this
     * way there's no checking against rectangles that will be nuked
     * due to coalescing, so we have to examine fewer rectangles.
     */
    SetGuiRegionExtents (rgnD);
    rgnD->Type = (rgnD->pHead) ? GUI_REGION_COMPLEX : GUI_REGION_NULL;

    return TRUE;
}
/*********************************************************************************************************
** 函数名称: UnionRegion
** 函数名称: UnionRegion
**
** 功能描述：  
**
** 输　入:  CGuiRegion * pDestRgn
** 输　入:  const CGuiRegion * pSrcRgn1
** 输　入:  const CGuiRegion * pSrcRgn2
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月17日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U  UnionRegion (CGuiRegion *pDestRgn, const CGuiRegion *pSrcRgn1, const CGuiRegion *pSrcRgn2)
{
    /*  checks all the simple cases */
    /*
     * Region 1 and 2 are the same or pRegion 1 is empty
     */
    if ( (pSrcRgn1 == pSrcRgn2) || (!(pSrcRgn1->pHead)) ) 
	{
        if (pDestRgn != pSrcRgn2)
            CopyRegion (pDestRgn, pSrcRgn2);
        return TRUE;
    }

    /*
     * if nothing to union (pRegion 2 empty)
     */
    if (!(pSrcRgn2->pHead))
	{
        if (pDestRgn != pSrcRgn1)
            CopyRegion (pDestRgn, pSrcRgn1);
        return TRUE;
    }

    /*
     * Region 1 completely subsumes pRegion 2
     */
    if ((pSrcRgn1->pHead == pSrcRgn1->pTail) && 
        (pSrcRgn1->RectBound.left <= pSrcRgn2->RectBound.left) &&
        (pSrcRgn1->RectBound.top <= pSrcRgn2->RectBound.top) &&
        (pSrcRgn1->RectBound.right >= pSrcRgn2->RectBound.right) &&
        (pSrcRgn1->RectBound.bottom >= pSrcRgn2->RectBound.bottom))
    {
        if (pDestRgn != pSrcRgn1)
            CopyRegion (pDestRgn, pSrcRgn1);
        return TRUE;
    }

    /*
     * Region 2 completely subsumes pRegion 1
     */
    if ((pSrcRgn2->pHead == pSrcRgn2->pTail) && 
        (pSrcRgn2->RectBound.left <= pSrcRgn1->RectBound.left) &&
        (pSrcRgn2->RectBound.top <= pSrcRgn1->RectBound.top) &&
        (pSrcRgn2->RectBound.right >= pSrcRgn1->RectBound.right) &&
        (pSrcRgn2->RectBound.bottom >= pSrcRgn1->RectBound.bottom))
    {
        if (pDestRgn != pSrcRgn2)
            CopyRegion(pDestRgn, pSrcRgn2);
        return TRUE;
    }

    RegionOperator (pDestRgn, pSrcRgn1, pSrcRgn2, UnionO, UnionNonO, UnionNonO);

    SetGuiRegionExtents (pDestRgn);
    pDestRgn->Type = (pDestRgn->pHead) ? GUI_REGION_COMPLEX : GUI_REGION_NULL ;
#if 0
    dst->rcBound.left = MIN (src1->rcBound.left, src2->rcBound.left);
    dst->rcBound.top = MIN (src1->rcBound.top, src2->rcBound.top);
    dst->rcBound.right = MAX (src1->rcBound.right, src2->rcBound.right);
    dst->rcBound.bottom = MAX (src1->rcBound.bottom, src2->rcBound.bottom);
    dumpRegion (dst);
#endif

    return TRUE;
} 
/*********************************************************************************************************
** 函数名称: XorRegion
** 函数名称: XorRegion
**
** 功能描述： 异或 
**
** 输　入:  CGuiRegion * dst
** 输　入:  const CGuiRegion * src1
** 输　入:  const CGuiRegion * src2
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月17日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U XorRegion (CGuiRegion *pDstRgn, const CGuiRegion *pSrcRgn1, const CGuiRegion *pSrcRgn2)
{
    CGuiRegion tmpa, tmpb;

	ASSERT(pDstRgn && pSrcRgn1 && pSrcRgn2);
	if (pDstRgn == NULL || pSrcRgn1 == NULL || pSrcRgn2 == NULL)
		return FALSE;
    InitRegion (&tmpa, pSrcRgn1->pHeap);
    InitRegion (&tmpb, pSrcRgn2->pHeap);

    SubtractRegion (&tmpa, pSrcRgn1, pSrcRgn2);
    SubtractRegion (&tmpb, pSrcRgn2, pSrcRgn1);
    UnionRegion (pDstRgn, &tmpa, &tmpb);

    EmptyRegion (&tmpa);
    EmptyRegion (&tmpb);

    return TRUE;
} 
/*********************************************************************************************************
** 函数名称: AddRectToRegion
** 函数名称: AddRectToRegion
**
** 功能描述：  Adds a rectangle to a region
**
** 输　入:  CGuiRegion * region
** 输　入:  const CGuiRect * rect
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月17日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U AddRectToRegion (CGuiRegion* pRegion, const CGuiRect *pRect)
{
    CGuiRegion my_region;
    CGuiRectNode my_cliprect;

	if (pRegion == NULL || pRect == NULL)
		return FALSE;	

    if (IsGuiRectEmpty (pRect))
        return FALSE;

#ifdef _REGION_DEBUG
    fprintf (stderr, "\n***************Before Union by pRect (%d, %d, %d, %d):\n",
                    pRect->left, pRect->top, pRect->right, pRect->bottom);
    dumpRegion (pRegion);
#endif

    my_cliprect.Rect = *pRect;
    my_cliprect.pNext = NULL;
    my_cliprect.pPrev = NULL;

    my_region.Type = GUI_REGION_SIMPLE;
    my_region.RectBound = *pRect;
    my_region.pHead = &my_cliprect;
    my_region.pTail = &my_cliprect;
    my_region.pHeap = NULL;

    UnionRegion (pRegion, pRegion, &my_region);

#ifdef _REGION_DEBUG
    dumpRegion (pRegion);
    fprintf (stderr, "***************After Union\n");
#endif

    return TRUE;
}
 
/*********************************************************************************************************
** 函数名称: IntersectRect
** 函数名称: IntersectRect
**
** 功能描述：  Intersect a rect with a region 
**
** 输　入:  CGuiRegion * region
** 输　入:  const CGuiRect * rect
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月19日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U IntersectRect (CGuiRegion* region, const CGuiRect* rect)
{
    CGuiRegion my_region;
    CGuiRectNode  my_cliprect;

    if (IsGuiRectEmpty (rect)) 
	{
        EmptyRegion (region);
        return TRUE;
    }

#ifdef _REGION_DEBUG
    fprintf (stderr, "\n***************before intersect by rect (%d, %d, %d, %d):\n",
                    rect->left, rect->top, rect->right, rect->bottom);
    dumpRegion (region);
#endif

    my_cliprect.Rect = *rect;
    my_cliprect.pNext = NULL;
    my_cliprect.pPrev = NULL;

    my_region.Type = GUI_REGION_SIMPLE;
    my_region.RectBound = *rect;
    my_region.pHead = &my_cliprect;
    my_region.pTail = &my_cliprect;
    my_region.pHeap = NULL;

    ClipRgnIntersect (region, region, &my_region);

#ifdef _REGION_DEBUG
    dumpRegion (region);
    fprintf (stderr, "***************After intersect\n");
#endif

    return TRUE;
}
/*********************************************************************************************************
** 函数名称: SubtractRectFromRegion
** 函数名称: SubtractRectFromRegion
**
** 功能描述：从区域中去除某个矩形  
**
** 输　入:  CGuiRegion * pRegion
** 输　入:  const CGuiRect * pRect
**          
** 输　出:   INT8U
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月19日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U   SubtractRectFromRegion (CGuiRegion* pRegion, const CGuiRect* pRect)
{
    CGuiRegion  my_region;
    CGuiRectNode  my_cliprect;

    if (IsGuiRectEmpty (pRect) || !DoesIntersect (&pRegion->RectBound, pRect))
        return FALSE;

#ifdef _REGION_DEBUG
    fprintf (stderr, "\n***************Before subtract by pRect (%d, %d, %d, %d):\n",
                    pRect->left, pRect->top, pRect->right, pRect->bottom);
    dumpRegion (pRegion);
#endif

	CopyGuiRect(& my_cliprect.Rect , pRect);
	my_cliprect.pNext  = NULL;
    my_cliprect.pPrev = NULL;
    my_region.Type = GUI_REGION_SIMPLE;
     my_region.RectBound = *pRect;
	CopyGuiRect(&  my_region.RectBound  , pRect);
    my_region.pHead = &my_cliprect;
    my_region.pTail = &my_cliprect;
    my_region.pHeap = NULL;
    SubtractRegion (pRegion, pRegion, &my_region);

#ifdef _REGION_DEBUG
    dumpRegion (pRegion);
    fprintf (stderr, "***************After subtraction\n");
#endif

    return TRUE;
}

void OffsetRegionEx (CGuiRegion* region, 
        const CGuiRect *rcClient, const CGuiRect *rcScroll, INT32U x, INT32U y)
{
    CGuiRectNode* cliprect = region->pHead;
    CGuiRect old_cliprc, rc_array[4], rc;
    CGuiRectNode* pTemp;
    INT32U i, nCount = 0;

    if (!rcClient || !rcScroll)
        return;

    if (!IntersectGuiRect (&rc, rcClient, rcScroll))
        return;

#ifdef _REGION_DEBUG
    fprintf (stderr, "***************enter OffsetRegionEx\n");
    dumpRegion (region);
#endif

    while (cliprect) 
	{
        /*not in scroll window region, return*/
        if (!DoesIntersect (&cliprect->Rect, &rc))
		{
            cliprect = cliprect->pNext;
#ifdef _REGION_DEBUG
            fprintf (stderr, "needn't scroll this cliprect.\n");
#endif
            continue;
        }

        /*not covered, recalculate cliprect*/
        if (!IsGuiRectCovered (&cliprect->Rect, &rc)) 
		{
            CopyGuiRect (&old_cliprc, &cliprect->Rect);
            IntersectGuiRect (&cliprect->Rect, &old_cliprc, &rc);
            nCount = SubtractGuiRect (rc_array, &rc, &old_cliprc);
#ifdef _REGION_DEBUG
            fprintf (stderr, "add new %d cliprect to region.\n", nCount);
#endif
            for (i = 0; i < nCount; i++) 
			{
                AddRectToRegion (region, &rc_array[i]);
            }
        }

        OffsetGuiRect (&cliprect->Rect, x, y);
#ifdef _REGION_DEBUG
        fprintf (stderr, "offset current cliprect. \n");
#endif

        /*if not intersect, remove current cliprect from list*/
        if (!DoesIntersect (&cliprect->Rect, &rc)) 
		{
            pTemp = cliprect->pNext;
            FreeRect (region->pHeap, cliprect);
            cliprect = pTemp;
#ifdef _REGION_DEBUG
            fprintf (stderr, "remove current cliprect. \n");
#endif
            continue;
        }

        /*if intersect, tune cliprect*/
        if (!IsGuiRectCovered (&cliprect->Rect, &rc)) 
		{
            CopyGuiRect (&old_cliprc, &cliprect->Rect);
            IntersectGuiRect (&cliprect->Rect, &old_cliprc, &rc);
#ifdef _REGION_DEBUG
            fprintf (stderr, "tune current cliprect. \n");
#endif
        }

        if (region->pHead)
		{
            SetGuiRegionExtents(region);
        }
        cliprect = cliprect->pNext;
    }

#ifdef _REGION_DEBUG
    dumpRegion (region);
    fprintf (stderr, "***************after OffsetRegionEx\n");
#endif
}
/*********************************************************************************************************
** 函数名称: OffsetRegion
** 函数名称: OffsetRegion
**
** 功能描述：  偏移区域
**
** 输　入:  CGuiRegion * region
** 输　入:  INT32U x
** 输　入:  INT32U y
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月18日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void   OffsetRegion (CGuiRegion* region, INT32U x, INT32U y)
{
    CGuiRectNode* pRectNode = region->pHead ;

    while (pRectNode)
	{
        OffsetGuiRect (&pRectNode->Rect, x, y);
        pRectNode = pRectNode->pNext;
    }

    if (region->pHead)
	{
        OffsetGuiRect (&region->RectBound, x, y);
    }
}

static void cb_region (void* context, int x1, int x2, int y)
{
#if 0
    CLIPRGN* region = (CLIPRGN*) context;
    CLIPRECT* newcliprect;

    newcliprect = ClipRectAlloc (region->heap);
    if (x2 > x1) {
        newcliprect->rc.left = x1;
        newcliprect->rc.right = x2 + 1;
    }
    else {
        newcliprect->rc.left = x2;
        newcliprect->rc.right = x1 + 1;
    }
    newcliprect->rc.top = y;
    newcliprect->rc.bottom = y + 1;

    if (region->head == NULL 
            || (newcliprect->rc.top >= region->tail->rc.bottom 
                && (newcliprect->rc.left != region->tail->rc.left
                    || newcliprect->rc.right != region->tail->rc.right))) {
        /* simply append to tail */
        goto append;
    }
    else if (newcliprect->rc.top == region->tail->rc.bottom 
                && newcliprect->rc.left == region->tail->rc.left
                && newcliprect->rc.right == region->tail->rc.right) {
        /* merge with the tail */
        region->tail->rc.bottom += 1;
        FreeClipRect (region->heap, newcliprect);
    }
    else if (newcliprect->rc.bottom == region->head->rc.top
                && newcliprect->rc.left == region->head->rc.left
                && newcliprect->rc.right == region->head->rc.right) {
        /* merge with the head */
        region->head->rc.top -= 1;
        FreeClipRect (region->heap, newcliprect);
    }
    else if (newcliprect->rc.top > region->tail->rc.bottom) {
        /* simply append to tail */
        goto append;
    }
    else if (newcliprect->rc.bottom < region->head->rc.top) {
        /* simply insert to head */
        newcliprect->prev = NULL;
        newcliprect->next = region->head;
        region->head->prev = newcliprect;
        region->head = newcliprect;
    }
    else {
        /* find a position to insert */
        CLIPRECT* cliprect = region->head;
        CLIPRECT* prev;

        printf ("get here.\n");
        printf ("new cliprc: (%d, %d, %d, %d)\n", 
                        newcliprect->rc.left, newcliprect->rc.top,
                        newcliprect->rc.right, newcliprect->rc.bottom);
        printf ("head cliprc: (%d, %d, %d, %d)\n", 
                        region->head->rc.left, region->head->rc.top,
                        region->head->rc.right, region->head->rc.bottom);
        printf ("tail cliprc: (%d, %d, %d, %d)\n", 
                        region->tail->rc.left, region->tail->rc.top,
                        region->tail->rc.right, region->tail->rc.bottom);

        while (cliprect && newcliprect->rc.top >= cliprect->rc.top) {
            cliprect = cliprect->next;
        }

        if (cliprect == NULL) /* simply append to the tail */
            goto append;

        printf ("current cliprc: (%d, %d, %d, %d)\n", 
                        cliprect->rc.left, cliprect->rc.top,
                        cliprect->rc.right, cliprect->rc.bottom);

        /* merge with prev or next? */
        if ((prev = cliprect->prev) 
                && prev->rc.bottom == newcliprect->rc.top
                && prev->rc.left == newcliprect->rc.left
                && prev->rc.right == newcliprect->rc.right) {
            prev->rc.bottom += 1;
            FreeClipRect (region->heap, newcliprect);
            printf ("merge with previous.\n");
        }
        else if (cliprect->rc.top == newcliprect->rc.bottom
                && cliprect->rc.left == newcliprect->rc.left
                && cliprect->rc.right == newcliprect->rc.right) {
            cliprect->rc.top -= 1;
            FreeClipRect (region->heap, newcliprect);
            printf ("merge with current.\n");
        }
        else {
            /* insert before of current cliprc */
            cliprect->prev->next = newcliprect;
            newcliprect->prev = cliprect->prev;
            cliprect->prev = newcliprect;
            newcliprect->next = cliprect;
            printf ("insert before of current.\n");
        }
    }

    return;

append:
    newcliprect->next = NULL;
    newcliprect->prev = region->tail;
    if (region->tail)
        region->tail->next = newcliprect;
    region->tail = newcliprect;
    if (region->head == NULL)
        region->head = newcliprect;
#else
    CGuiRegion* region = (CGuiRegion*) context;
    CGuiRegion newregion;
    CGuiRectNode newcliprect;

    if (x2 > x1) 
	{
        newcliprect.Rect.left = x1;
        newcliprect.Rect.right = x2 + 1;
    }
    else {
        newcliprect.Rect.left = x2;
        newcliprect.Rect.right = x1 + 1;
    }
    newcliprect.Rect.top = y;
    newcliprect.Rect.bottom = y + 1;

    newcliprect.pNext = NULL;
    newcliprect.pPrev = NULL;

    newregion.Type = GUI_REGION_SIMPLE;
    newregion.RectBound = newcliprect.Rect;
    newregion.pHead = &newcliprect;
    newregion.pTail = &newcliprect;
    newregion.pHeap = NULL;

    if (IsEmptyRegion (region))
        CopyRegion (region, &newregion);
    else
        UnionRegion (region, region, &newregion);
#endif
}
#if 0
INT8U InitCircleRegion (CGuiRegion* dst, INT32U x, INT32U y, INT32U r)
{
    EmptyRegion (dst);

    if (r < 1) 
	{
        CGuiRectNode* newcliprect;

        NEW_RECT_NODE (dst, newcliprect);
        newcliprect->Rect.left = x;
        newcliprect->Rect.top = y;
        newcliprect->Rect.right = x + 1;
        newcliprect->Rect.bottom = y + 1;
        return TRUE;
    }

    CircleGenerator (dst, x, y, r, cb_region);

    return TRUE;
}

INT8U   InitEllipseRegion (CGuiRegion* dst, INT32U x, INT32U y, INT32U rx, INT32U ry)
{
    EmptyRegion (dst);

    if (rx < 1 || ry < 1) 
	{
        CGuiRectNode* newcliprect;

        NEW_RECT_NODE (dst, newcliprect);
        newcliprect->Rect.left = x;
        newcliprect->Rect.top = y;
        newcliprect->Rect.right = x + 1;
        newcliprect->Rect.bottom = y + 1;
        return TRUE;
    }

    EllipseGenerator (dst, x, y, rx, ry, cb_region);

    return TRUE;
}

INT8U  InitPolygonRegion (CGuiRegion * dst, const CGuiPoint* pts, INT32U vertices)
{
    EmptyRegion (dst);

    if (PolygonIsMonotoneVertical (pts, vertices)) 
	{
        if (MonotoneVerticalPolygonGenerator (dst, pts, vertices, cb_region))
            goto ok;
    }
    else if (PolygonGenerator (dst, pts, vertices, cb_region)) 
	{
        goto ok;
    }

    return FALSE;

ok:
   return TRUE;
}
#endif
