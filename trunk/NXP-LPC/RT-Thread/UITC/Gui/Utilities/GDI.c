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
#include "ScreenBase.h"
#include "CtrlBase.h"
#include "GuiConfig.h"
#include "DCExt.h"

/* General DC */
static CGuiDC DCSlot [GUI_DC_SLOT_CNT];

 

/* This function initializes a DC: set the default parameters. */
static void InitDC (CGuiDC* pdc, GUI_HWND hWnd, INT8U bIsClient)
{
	CCtrlBase * pCtrl;

	pdc->HWnd = hWnd;
#if 0
// 	pdc->bkcolor = GAL_MapRGB (pdc->surface->format, 0xFF, 0xFF, 0xFF);
// 	pdc->bkmode = 0;
// 
// 	pdc->pencolor = GAL_MapRGB (pdc->surface->format, 0x00, 0x00, 0x00);
// 	pdc->brushcolor = GAL_MapRGB (pdc->surface->format, 0xFF, 0xFF, 0xFF);
// 
// 	pdc->textcolor = GAL_MapRGB (pdc->surface->format, 0x00, 0x00, 0x00);
// 	if (!(pdc->pLogFont = GetWindowFont (hWnd)))
// 		pdc->pLogFont = GetSystemFont (SYSLOGFONT_WCHAR_DEF);
// 	pdc->tabstop = 8;
// 	pdc->cExtra = pdc->alExtra = pdc->blExtra = 0;
// 
// 	pdc->mapmode = MM_TEXT;
// 	pdc->ViewOrig.x = pdc->ViewOrig.y = 0;
// 	pdc->ViewExtent.x = pdc->ViewExtent.y = 1;
// 	pdc->WindowOrig.x = pdc->WindowOrig.y = 0;
// 	pdc->WindowExtent.x = pdc->WindowExtent.y = 1;

 
// 	pdc->pen_type = PT_SOLID;
// 	pdc->pen_cap_style = PT_CAP_BUTT;
// 	pdc->pen_join_style = PT_JOIN_MITER;
// 	pdc->pen_width = 0;
// 	pdc->dash_offset = 0;
// 	pdc->dash_list = NULL;
// 	pdc->dash_list_len = 0;
// 
// 	pdc->brush_type = BT_SOLID;
// 	pdc->brush_orig.x = pdc->brush_orig.y = 0;
// 	pdc->brush_tile = NULL;
// 	pdc->brush_stipple = NULL;
 

	/* Assume that the local clip region is empty. */
	/* Get global clip region info and generate effective clip region. */
	if (dc_IsGeneralDC (pdc)) 
	{
		RECT minimal;

		pdc->pGCRInfo = GetGCRgnInfo (hWnd);
	 
		pdc->oldage = pdc->pGCRInfo->age;
		ClipRgnCopy (&pdc->ecrgn, &pdc->pGCRInfo->crgn);

		pdc->bIsClient = bIsClient;
		if (bIsClient)
			WndClientRect (pdc->hwnd, &pdc->DevRC);
		else
			WndRect (pdc->hwnd, &pdc->DevRC);

		minimal = pdc->DevRC;

		pCtrl = Control (pdc->hwnd);
		if (pCtrl && !(pCtrl->dwExStyle & WS_EX_CTRLASMAINWIN))
			RestrictControlECRGN (&minimal, pCtrl);

		IntersectClipRect (&pdc->ecrgn, &minimal);
 	}

	/* context info and raster operations. */
	pdc->CurPenPos.x = pdc->CurTextPos.x = 0;
	pdc->CurPenPos.y = pdc->CurTextPos.y = 0;

	pdc->rop = ROP_SET;
	pdc->step = 1;
	pdc->set_pixel = 
		set_pixel_ops [pdc->rop][pdc->surface->format->BytesPerPixel - 1];
	pdc->draw_hline = 
		draw_hline_ops [pdc->rop][pdc->surface->format->BytesPerPixel - 1];
	pdc->put_hline = 
		put_hline_ops [pdc->rop][pdc->surface->format->BytesPerPixel - 1];

	pdc->cur_dst = (BYTE*)pdc->surface->pixels 
		+ pdc->surface->pitch * pdc->DevRC.top
		+ pdc->surface->format->BytesPerPixel * pdc->DevRC.left;

	pdc->move_to = move_to_ops [pdc->surface->format->BytesPerPixel - 1];
	pdc->step_x = step_x_ops [pdc->surface->format->BytesPerPixel - 1];

	memset (pdc->gray_pixels, 0, sizeof (pdc->gray_pixels));
	memset (pdc->filter_pixels, 0, sizeof (pdc->filter_pixels));
	pdc->alpha_pixel_format = NULL;
#endif
}

/*
 * Function: HDC GUIAPI GetClientDC (HWND hWnd)
 *     This function get the specified window client's DC.
 * Parameter:
 *     HWND hWnd: The window, 0 for screen.
 * Return:
 *     The handle of wanted DC.
 */

GUI_HDC GetClientDC (GUI_HWND hWnd)
{
	int i;

	for (i = 0; i < GUI_DC_SLOT_CNT; i++)
	{
		if (!DCSlot[i].InUsed)
		{
			DCSlot[i].InUsed = TRUE;
		 	DCSlot[i].DCType   = TYPE_GENDC;
	//		DCSlot[i].surface = __gal_screen;
			break;
		}
	}

	if (i >= GUI_DC_SLOT_CNT)
		return NULL;

	InitDC (DCSlot + i, hWnd, TRUE);
	return (GUI_HDC) (DCSlot + i);
}


GUI_HDC GetDC (GUI_HWND hWnd)
{
	int i;

	for (i = 0; i < GUI_DC_SLOT_CNT; i++)
	{
		if (!DCSlot[i].InUsed)
		{
			DCSlot[i].InUsed = TRUE;
			DCSlot[i].DCType   = TYPE_GENDC;
	//		DCSlot[i].surface = __gal_screen;
			break;
		}
	}

	if (i >= GUI_DC_SLOT_CNT)
		return NULL;

	InitDC (DCSlot + i, hWnd, FALSE);
	return (GUI_HDC) (DCSlot + i);
}


/*
 * Function: void GUIAPI ReleaseDC(HDC hDC)
 *     This function release the specified DC.
 * Parameter:
 *     HDC hDC: The DC handle want to release.
 * Return:
 *     None. 
 */
void ReleaseDC (GUI_HDC hDC)
{

}

INT8U InitGDI( )
{


	return TRUE;
}
