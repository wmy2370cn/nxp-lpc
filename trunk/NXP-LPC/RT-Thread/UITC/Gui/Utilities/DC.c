/****************************************Copyright (c)**************************************************
**                                      
**               绘图函数                       
**                    感觉有些地方溢出了,待细查                  
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
#include "DC.h"
#include "Func.h"
#include "Icon.h"
#include "asciilib.h"
#include "CnFontslib.h"
#include "StringTable.h"

static const INT8U mask1[]={0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
static const INT8U   DCB_HEX_TAB[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

//LCD 显存刷新标记
typedef INT8U LCD_BUFF_FLAG[LCD_WIDTH][LCD_WIDTH>>3];

static LCD_BUFF g_LcdBuff;

static LCD_BUFF_FLAG g_LcdBuffFlag;


//计算显示缓冲区长宽
#define GetLCDBufferWidth(ignore) LCD_WIDTH
#define GetLCDBufferHeight(ignore) (LCD_HEIGHT>>3)  // (128/8)=16

//得到液晶的长宽
#define GetLCDWidth(ignore) LCD_WIDTH
#define GetLCDHeight(ignore) LCD_HEIGHT

//根据 X 坐标位置，计算出缓冲区相应的索引
#define GetXIndex(x) (x)
//Y的索引
#define GetYIndex(y)  (((LCD_POS)(y))>>3)
// Y点在八位中的位
#define GetYBitPos(y) (((INT8U)(y)) & 0x07)
// Y点在八位中的Mask 0x10形式
#define GetYBitMask(y) (1<<GetYBitPos(y))



static 	void _LcdWriteData(LCD_POS col, LCD_POS row)
{
	LCD_WriteData(col, row, g_LcdBuff[col][row]);
	g_LcdBuffFlag[col][row] = FALSE;
}
/*********************************************************************************************************
** 函数名称: WriteLcdBufData
** 函数名称: WriteLcdBufData
**
** 功能描述：  写一个字节到液晶缓存
**
** 输　入:  LCD_POS col
** 输　入:  LCD_POS row
** 输　入:  INT8U data
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月30日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void WriteLcdBufData( LCD_POS col, LCD_POS row ,INT8U data )
{
	if (col>=GetLCDWidth() || row >= GetLCDHeight())
		return;
	if( g_LcdBuff[col][row] != data)
		g_LcdBuffFlag[col][row] = TRUE;

	g_LcdBuff[col][row] = data;	
}
/*********************************************************************************************************
** 函数名称: SetModify
** 函数名称: SetModify
**
** 功能描述：
**
** 输　入:  LCD_POS col 从0开始到239
** 输　入:  LCD_POS row
**
** 输　出:   void
**
** 全局变量:
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月30日
** 备  注:
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void SetModify(LCD_POS col, LCD_POS row )
{
	if (col>=GetLCDWidth() || row >= GetLCDHeight())
		return;
	g_LcdBuffFlag[col][row] = TRUE;
}
/*********************************************************************************************************
** 函数名称: GetPixel
** 函数名称: GetPixel
**
** 功能描述：  得到某一点的颜色
**
** 输　入:  LCD_POS x 从0开始到240
** 输　入:  LCD_POS y 从0开始到128
**          
** 输　出:   GUI_COLOR
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月23日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static GUI_COLOR GetPixel(LCD_POS x, LCD_POS y)
{
	if(x>=GetLCDWidth() || y>=GetLCDHeight() )
	{
		return GUI_COLOR_BK;
	}

 	if(g_LcdBuff[GetXIndex(x)][GetYIndex(y)] & GetYBitMask(y))
	{
		return GUI_COLOR_FR;
	}
	else
	{
		return GUI_COLOR_BK;
	}
}
/*********************************************************************************************************
** 函数名称: LCD_GetPixel
** 函数名称: LCD_GetPixel
**
** 功能描述： 从显存中获取某个点的值， 
**
** 输　入:  INT32U x 0-240
** 输　入:  INT32U y 0-128
**          
** 输　出:   GUI_COLOR
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月24日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static GUI_COLOR LCD_GetPixel(INT32U x, INT32U y)
{
	GUI_COLOR  bak;
	/* 参数过滤 */
	if(x>=LCD_WIDTH || y>=LCD_HEIGHT) 
		return(0);
	/* 取得该点颜色(用户自行更改) */ 
	bak = g_LcdBuff[x][y>>3];
	if( (bak&DCB_HEX_TAB[y&0x07])!=0 )
		return 1;
	else
		return 0;
}

//画点到液晶缓存区
static void SetPixel2Buf(LCD_POS x, LCD_POS y, GUI_COLOR cr)
{
	if(x>=GetLCDWidth() || y>=GetLCDHeight() )
 		return;
	 
	/* 设置缓冲区相应的点 */
	if( (cr&0x01) != 0 ) 
		g_LcdBuff[x][y>>3] |= DCB_HEX_TAB[y&0x07];
	else  
		g_LcdBuff[x][y>>3] &= (~DCB_HEX_TAB[y&0x07]);
	SetModify(x,y);
}
/*********************************************************************************************************
** 函数名称: _SetPixel2BufPen
** 函数名称: _SetPixel2BufPen
**
** 功能描述：  画点到液晶缓存区
**
** 输　入:  INT32U x
** 输　入:  INT32U y
** 输　入:  GUI_COLOR color
** 输　入:  GUI_PEN pen
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月24日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void SetPixel2BufPen(LCD_POS x, LCD_POS y, GUI_COLOR color, GUI_PEN pen)
{	
	switch(pen) 
	{
	case PEN_COPY:
		{
			SetPixel2Buf(x,y,color);
		}
		break;
	case PEN_NOT:
		{
			//取液晶屏对应点当前颜色的反色 单色方式处理
			SetPixel2Buf(x,y,!color);	
		}
		break;
	case PEN_OR:
		{
			//如果是1就画，如果是零，就和当前状态一致，不画
			if(color==GUI_COLOR_FR)
				SetPixel2Buf(x,y,color);
		}
		break;
	case PEN_XOR:
		{
			color ^= LCD_GetPixel(x,y);
			SetPixel2Buf(x,y,color);
		}
		break;
	case PEN_AND:
		{
			color &= LCD_GetPixel(x,y);
			SetPixel2Buf(x,y,color);
		}
		break;
	}
}
//，先全部写缓存，然后需要更新时再调用更新函数刷新屏幕
//画点
static void DrawPixel(LCD_POS x, LCD_POS y, GUI_COLOR cr/*=GUI_COLOR_FR*/)
{
	INT8U b = 0 ; 
 
   	if(x>=GetLCDWidth() || y>=GetLCDHeight() )
		return;
	 
	b=g_LcdBuff[GetXIndex(x)][GetYIndex(y)];
	if(cr==GUI_COLOR_FR)
	{
		b |= GetYBitMask(y);
	}
	else
	{
		b &= ~ GetYBitMask(y);
	}

	//写到液晶显存
	WriteLcdBufData(GetXIndex(x), GetYIndex(y), b);	
	//hd_LCD_WriteData(GetXIndex(x), GetYIndex(y), b);
}

static void DrawPixelEx(LCD_POS x, LCD_POS y,GUI_COLOR cr, GUI_PEN pen/*=PEN_COPY*/)
{
 	if(x>=GetLCDWidth() || y>=GetLCDHeight() )	 
		return; 

	switch(pen)
	{
	case PEN_COPY:
		{
			DrawPixel(x,y,cr);
		}
		break;
	case PEN_NOT:
		{
			//取液晶屏对应点当前颜色的反色
			DrawPixel(x,y,!GetPixel(x,y));
		}
		break;
	case PEN_OR:
		{
			//如果是1就画，如果是零，就和当前状态一致，不画
			if(cr==GUI_COLOR_FR)
				DrawPixel(x,y,cr);
		}
		break;
	case PEN_XOR:
		{
			cr ^= GetPixel(x,y);
			DrawPixel(x,y,cr);
		}
		break;
	case PEN_AND:
		{
			cr &= GetPixel(x,y);
			DrawPixel(x,y,cr);
		}
		break;
	}
}

//画一个字节(高－－低)
static void DrawByte(LCD_POS x, LCD_POS y, INT8U byteValue, GUI_PEN pen/*=PEN_COPY*/)
{
	INT8U i; 
	GUI_COLOR cr;	

	for(i=0; i<8; i++)
	{
		cr = (byteValue & (0x01<<(7-i)))>>(7-i);
		DrawPixelEx(x+i, y, cr, pen);
	}
}

//画线
void DrawLine(LCD_POS x0, LCD_POS y0, LCD_POS x1, LCD_POS y1, GUI_COLOR cr)
{
	INT16U i;
	LCD_POS  dx,dy;
	LCD_POS x,y;
	INT16S e;

	//垂直线
	if(x0==x1)
	{
		if(y0>y1)
		{
			i=y0;
			y0=y1;
			y1=i;
		}
		//		for (i=y0;i<y1;i++) {
		//			DrawPixel(x0, i, cr);
		//		}
		//可以按字节进行优化
		for (i=GetYIndex(y0); i<=GetYIndex(y1); i++) 
		{
			LCD_POS j;
			//检查是否完整的一个字节
			for (j=max(i*8, y0);j<=min((i+1)*8-1, y1);j++) 
			{
				if(GetXIndex(x0)<GetLCDBufferWidth() && i<GetLCDBufferHeight() )
				{
					if(cr==GUI_COLOR_FR)
					{
					//	g_LcdBuff[GetXIndex(x0)][i] |= GetYBitMask(j);
						WriteLcdBufData(x0,i, g_LcdBuff[GetXIndex(x0)][i]|GetYBitMask(j) );
					}
					else
					{
					//	g_LcdBuff[GetXIndex(x0)][i] &= ~ GetYBitMask(j);
						WriteLcdBufData(x0,i, g_LcdBuff[GetXIndex(x0)][i] & (~ GetYBitMask(j)));
					}
				}
			}
//			_LcdWriteData(x0, i);
		}
	}
	else if (y0==y1) // 水平线
	{
		if(x0>x1)
		{
			i=x0;
			x0=x1;
			x1=i;
		}

		for (i=x0;i<=x1;i++)
		{
			DrawPixel(i, y0, cr);
		}
	}
	else	//任意斜线
	{
		//交换直线的起始点
		if(x0>x1)
		{
			i=x0;
			x0=x1;
			x1=i;
			i=y0;
			y0=y1;
			y1=i;
		}

		dx=x1-x0;
		dy=y1-y0;
		e=-dx; x=x0; y=y0;    

		if(aabs(dy)<dx)                  //0<斜率的绝对值 <1
		{
			if(dy>0)                    //0<斜率<=1
			{
				for(i=0; i<=dx; i++)    
				{
					DrawPixel(x, y, cr);
					x+=1;
					e=e+dy+dy;
					if(e>=0)
					{
						y+=1;
						e=e-dx-dx;
					}
				}//end for

				return;
			}//end if

			if(dy<0)
			{			
				for(i=0; i<=dx; i++)    
				{
					DrawPixel(x, y, cr);
					x+=1;
					e=e-dy-dy;
					if(e>=0)
					{
						y-=1;
						e=e-dx-dx;
					}
				}//end for

				return;
			}//end if 

		}//end if
		else
		{
			if(dy>=0)                               // 无穷>斜率>1
			{
				e=-dy;
				for(i=0; i<=dy; i++)
				{
					DrawPixel(x, y, cr);
					y+=1;
					e=e+dx+dx;
					if(e>=0)
					{
						x+=1;
						e=e-dy-dy;
					}
				}//end for

				return;
			}//end if 

			if(dy<=0)
			{
				e=dy;
				for(i=0; i<=-dy; i++)
				{
					DrawPixel(x, y, cr);
					y-=1;
					e=e+dx+dx;
					if(e>=0)
					{
						x+=1;
						e=e+dy+dy;
					}
				}//end for
				return;
			}//endif		
		}//end else	
	}
}
void DrawLineEx(LCD_POS x0, LCD_POS y0, LCD_POS x1, LCD_POS y1, GUI_COLOR cr/*=GUI_COLOR_FR*/, GUI_PEN pen/*=PEN_COPY*/)
{
	switch(pen)
	{
	case PEN_COPY:
		{
			DrawLine(x0,y0,x1,y1,cr);
		}
		break;
	case PEN_OR:
		{
			if(cr==GUI_COLOR_FR)
				DrawLine(x0,y0,x1,y1,cr);
		}
		break;
	case PEN_XOR:
	case PEN_NOT:
	case PEN_AND:
		{
			//与液晶屏当前点颜色有关，需要进一部处理
			INT16U i;
			LCD_POS  dx,dy,e;
			LCD_POS x,y;

			//垂直线
			if(x0==x1)
			{
				if(y0>y1)
				{
					i=y0;
					y0=y1;
					y1=i;
				}

				for (i=y0;i<=y1;i++) 
				{
					DrawPixelEx(x0, i, cr, pen);
				}
			}
			else if (y0==y1) // 水平线
			{
				if(x0>x1)
				{
					i=x0;
					x0=x1;
					x1=i;
				}

				for (i=x0;i<=x1;i++) 
				{
					DrawPixelEx(i, y0, cr, pen);
				}
			}
			else	//任意斜线
			{
				//交换直线的起始点
				if(x0>x1)
				{
					i=x0;
					x0=x1;
					x1=i;
					i=y0;
					y0=y1;
					y1=i;
				}

				dx=x1-x0;
				dy=y1-y0;
				e=-dx; x=x0; y=y0;    

				if(aabs(dy)<dx)                  //0<斜率的绝对值 <1
				{
					if(dy>0)                    //0<斜率<=1
					{
						for(i=0; i<=dx; i++)    
						{
							DrawPixelEx(x, y, cr,pen);
							x+=1;
							e=e+dy+dy;
							if(e>=0)
							{
								y+=1;
								e=e-dx-dx;
							}
						}//end for

						return;
					}//end if

					if(dy<0)
					{			
						for(i=0; i<=dx; i++)    
						{
							DrawPixelEx(x, y, cr,pen);
							x+=1;
							e=e-dy-dy;
							if(e>=0)
							{
								y-=1;
								e=e-dx-dx;
							}
						}//end for

						return;
					}//end if 

				}//end if
				else
				{
					if(dy>=0)                               // 无穷>斜率>1
					{
						e=-dy;
						for(i=0; i<=dy; i++)
						{
							DrawPixelEx(x, y, cr,pen);
							y+=1;
							e=e+dx+dx;
							if(e>=0)
							{
								x+=1;
								e=e-dy-dy;
							}
						}//end for

						return;
					}//end if 

					if(dy<=0)
					{
						e=dy;
						for(i=0; i<=-dy; i++)
						{
							DrawPixelEx(x, y, cr,pen);
							y-=1;
							e=e+dx+dx;
							if(e>=0)
							{
								x+=1;
								e=e+dy+dy;
							}
						}//end for
						return;
					}//endif		
				}//end else	
			}
		}
		break;
	}
}

void DrawBlock(LCD_POS x0, LCD_POS y0, LCD_POS x1, LCD_POS y1, GUI_COLOR cr/*=GUI_COLOR_FR*/)
{
//	L_ASSERT(x0<GetLCDWidth() && x1<GetLCDWidth() && y0<GetLCDHeight() && y1<GetLCDHeight());
	INT32U x = 0;

	for ( x=x0;x<=x1;x++)
	{
		DrawLine(x, y0, x, y1, cr);
	}
}

void DrawBlockRect( CGuiRect *pRect, GUI_COLOR cr)
{
	if (pRect)
	{
		DrawBlock(pRect->left, pRect->top, pRect->right, pRect->bottom, cr);
	}
}


void DrawBlockEx(LCD_POS x0, LCD_POS y0, LCD_POS x1, LCD_POS y1, GUI_COLOR cr/*=GUI_COLOR_FR*/, GUI_PEN pen/*=PEN_COPY*/)
{
//	L_ASSERT(x0<GetLCDWidth() && x1<GetLCDWidth() &&  y0<GetLCDHeight() && y1<GetLCDHeight());

	INT32U x = 0;
	for ( x=x0;x<=x1;x++)
	{
		DrawLineEx(x, y0, x, y1, cr, pen);
	}
}

void DrawRect(LCD_POS x0, LCD_POS y0, LCD_POS x1, LCD_POS y1, GUI_COLOR cr/*=GUI_COLOR_FR*/)
{
//	L_ASSERT(x0<GetLCDWidth() && x1<GetLCDWidth() && y0<GetLCDHeight() && y1<GetLCDHeight());

	if(x0==x1)
	{
		DrawLine(x0,y0,x0,y1,cr);
	}
	else if(y0==y1)
	{
		DrawLine(x0,y1,x1,y0,cr);
	}
	else
	{
		DrawLine(x0, y0, x1,y0,cr);
		DrawLine(x1, y0, x1,y1,cr);
		DrawLine(x1, y1, x0,y1,cr);
		DrawLine(x0, y1, x0,y0,cr);
	}
}

void DrawRectEx(LCD_POS x0, LCD_POS y0, LCD_POS x1, LCD_POS y1, GUI_COLOR cr/*=GUI_COLOR_FR*/, GUI_PEN pen/*=PEN_COPY*/)
{
//	L_ASSERT(x0<GetLCDWidth() && x1<GetLCDWidth() && y0<GetLCDHeight() && y1<GetLCDHeight());

	CGuiRect rect;
	if(x0==x1)
	{
		DrawLineEx(x0,y0,x0,y1, cr, pen);
	}
	else if(y0==y1)
	{
		DrawLineEx(x0,y1,x1,y0, cr, pen);
	}
	else
	{
		rect.left = x0;
		rect.top = y0; 
		rect.right =x1;
		rect.bottom = y1;
		NormalizeRect(&rect);
		DrawLineEx(rect.left, rect.top, rect.right, rect.top, cr, pen);
		DrawLineEx(rect.right, rect.top+1, rect.right, rect.bottom,cr, pen);
		DrawLineEx(rect.right-1, rect.bottom, rect.left, rect.bottom,cr, pen);
		DrawLineEx(rect.left, rect.bottom-1, rect.left, rect.top+1,cr, pen);
	}
}

/*********************************************************************************************************
** 函数名称: _DrawByteArray
** 函数名称: _DrawByteArray
**
** 功能描述： 一列一列的刷新数据 
**
** 输　入:  LCD_POS x
** 输　入:  LCD_POS y
** 输　入:  INT8U nH
** 输　入:  INT8U nV
** 输　入:  const INT8U * pBuff
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月29日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void _DrawByteArray(LCD_POS x, LCD_POS y, INT8U nH, INT8U nV, const INT8U * pBuff)
{
	INT8U col = 0, row = 0;
	INT32U X = 0,Y = 0;
	INT8U b = 0;
	INT32U index = 0;
	INT32U Y1 = 0 , Y2 = 0;

	if (pBuff == NULL)
		return;

	if( (y&0x07) ==0 )
	{
		//如果纵向8对其的可以对其进行优化 
		for ( col=0;col<nH;col++)
		{
			for ( row=0;row<nV;row++)
			{
				X=GetXIndex(x)+col;
				Y=GetYIndex(y)+row;
				if(X<GetLCDBufferWidth() && Y<GetLCDBufferHeight()  )
				{
//					g_LcdBuff[X][Y]=pBuff[col*nV+row];
//					_LcdWriteData(X,Y);
					WriteLcdBufData(X,Y,pBuff[col*nV+row]);
				}
			}
		}
	}
	else
	{
		//不是对其的，进行优化处理比价复杂
		for (col=0;col<nH;col++)
		{
			for (row=0;row<nV;row++) 
			{
				index=col*nV+row;
				//对应到某两个字节的液晶缓冲中去
				X=GetXIndex(x)+col;

				Y1=GetYIndex(y)+row;
				Y2=GetYIndex(y)+row+1;

				b=pBuff[index];

				//前一个字节的处理
				if(X<GetLCDBufferWidth() && Y1<GetLCDBufferHeight() )
				{
//					g_LcdBuff[X][Y1] = (g_LcdBuff[X][Y1] & (mask1[GetYBitPos(y)]) ) | ((b & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y));
//					_LcdWriteData(X,Y1);
					WriteLcdBufData(X,Y1,(g_LcdBuff[X][Y1] & (mask1[GetYBitPos(y)]) ) | ((b & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y)));
				}
				//后一个字节的处理
				if(X<GetLCDBufferWidth() && Y2<GetLCDBufferHeight() )
				{
//					g_LcdBuff[X][Y2] = (g_LcdBuff[X][Y2] & (~mask1[GetYBitPos(y)]) ) | ((b & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y)));
//					_LcdWriteData(X,Y2);
					WriteLcdBufData(X,Y2,(g_LcdBuff[X][Y2] & (~mask1[GetYBitPos(y)]) ) | ((b & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y))));
				}
			}
		}		
	}
}

static void _DrawByteArrayEx(LCD_POS x, LCD_POS y, INT8U nH, INT8U nV, const INT8U * pBuff, GUI_PEN pen)
{
	INT32U col = 0, row = 0;
	INT32U X = 0,Y = 0;
	INT32U index = 0;
	INT32U Y1 = 0 , Y2 = 0;
	INT8U data = 0;
	INT8U b = 0;

	if( (y&0x07) ==0 )
	{
		//如果纵向8对其的可以对其进行优化
		for (col=0;col<nH;col++) 
		{
			for (row=0;row<nV;row++)
			{
				X=GetXIndex(x)+col;
				Y=GetYIndex(y)+row;
				if(X<GetLCDBufferWidth() && Y<GetLCDBufferHeight())
				{
					data = g_LcdBuff[X][Y];
					switch(pen) 
					{
					case PEN_COPY:
					//	g_LcdBuff[X][Y]=pBuff[col*nV+row];
						data = pBuff[col*nV+row];					
						break;
					case PEN_NOT:
					//	g_LcdBuff[X][Y]=~pBuff[col*nV+row];
						data = ~pBuff[col*nV+row];
						break;
					case PEN_OR:
					//	g_LcdBuff[X][Y] |= pBuff[col*nV+row];
						data |=pBuff[col*nV+row];
						break;
					case PEN_XOR:
					//	g_LcdBuff[X][Y] ^= pBuff[col*nV+row];
						data ^= pBuff[col*nV+row];
						break;
					case PEN_AND:
					//	g_LcdBuff[X][Y] &= pBuff[col*nV+row];
						data &= pBuff[col*nV+row];
						break;
					}
				//	_LcdWriteData(X,Y);
					WriteLcdBufData(X,Y,data);
				}
			}
		}
	}
	else
	{
		//不是对其的，进行优化处理比价复杂
		for (col=0;col<nH;col++) 
		{
			for (row=0;row<nV;row++) 
			{
				index=col*nV+row;
				//对应到某两个字节的液晶缓冲中去
				X=GetXIndex(x)+col;

				Y1=GetYIndex(y)+row;
				Y2=GetYIndex(y)+row+1;

				b=pBuff[index];

				//前一个字节的处理
				if(X<GetLCDBufferWidth() && Y1<GetLCDBufferHeight())
				{
					data = g_LcdBuff[X][Y1] ;
					switch(pen)
					{
					case PEN_COPY:
				//		g_LcdBuff[X][Y1] = (g_LcdBuff[X][Y1] & (mask1[GetYBitPos(y)]) ) | ((b & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y));
				        data = (data & (mask1[GetYBitPos(y)]) ) | ((b & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y));
						break;
					case PEN_NOT:
				//		g_LcdBuff[X][Y1] = (g_LcdBuff[X][Y1] & (mask1[GetYBitPos(y)]) ) | (((~b) & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y));
						data = (data & (mask1[GetYBitPos(y)]) ) | (((~b) & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y));
						break;
					case PEN_OR:
				//		g_LcdBuff[X][Y1] |= ((b & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y));
						data  |= ((b & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y));
						break;
					case PEN_XOR:
				//		g_LcdBuff[X][Y1] = (g_LcdBuff[X][Y1] & (mask1[GetYBitPos(y)]) ) | (( (g_LcdBuff[X][Y1]) ^ ((b & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y)) ) & (~mask1[GetYBitPos(y)]));
						data = (data & (mask1[GetYBitPos(y)]) ) | ((  data ^ ((b & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y)) ) & (~mask1[GetYBitPos(y)]));
						break;
					case PEN_AND:
				//		g_LcdBuff[X][Y1] &= ((b & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y)) | mask1[GetYBitPos(y)];
						data &= ((b & (mask1[8-GetYBitPos(y)])) << GetYBitPos(y)) | mask1[GetYBitPos(y)];
						break;
					}
					WriteLcdBufData(X,Y1,data);
				//	_LcdWriteData(X,Y1);
				}
				//后一个字节的处理
				if(X<GetLCDBufferWidth() && Y2<GetLCDBufferHeight())
				{
					data = g_LcdBuff[X][Y2] ;
					switch(pen) 
					{
					case PEN_COPY:
					//	g_LcdBuff[X][Y2] = (g_LcdBuff[X][Y2] & (~mask1[GetYBitPos(y)]) ) | ((b & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y)));
						data =  (data & (~mask1[GetYBitPos(y)]) ) | ((b & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y)));
						break;
					case PEN_NOT:
					//	g_LcdBuff[X][Y2] = (g_LcdBuff[X][Y2] & (~mask1[GetYBitPos(y)]) ) | (((~b) & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y)));
						data = (data & (~mask1[GetYBitPos(y)]) ) | (((~b) & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y)));
						break;
					case PEN_OR:
					//	g_LcdBuff[X][Y2] |=  ((b & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y)));
						data |=  ((b & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y)));
						break;
					case PEN_XOR:
					//	g_LcdBuff[X][Y2] = (g_LcdBuff[X][Y2] & (~mask1[GetYBitPos(y)]) ) | (( (g_LcdBuff[X][Y2]) ^ ((b & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y))) ) & (mask1[GetYBitPos(y)]));
						data = (data & (~mask1[GetYBitPos(y)]) ) | (( data ^ ((b & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y))) ) & (mask1[GetYBitPos(y)]));
						break;
					case PEN_AND:
					//	g_LcdBuff[X][Y2] &=  ((b & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y))) | (~mask1[GetYBitPos(y)]);
						data &=  ((b & (~mask1[8-GetYBitPos(y)])) >> (8-GetYBitPos(y))) | (~mask1[GetYBitPos(y)]);
						break;
					}
				//	_LcdWriteData(X,Y2);
					WriteLcdBufData(X,Y2,data);
				}
			}
		}		
	}
}

void DrawIcon(LCD_POS x, LCD_POS y, INT16U iIconID, GUI_PEN pen/*=PEN_COPY*/)
{
//	L_ASSERT(iIconID< NUM_ICON);
	const ICON_RES * pIcon=&IconRes[iIconID];

	int nH=pIcon->nWidth;
	int nV=pIcon->nHeight / 8+(pIcon->nHeight%8==0?0:1);

	if(pen==PEN_COPY)
		_DrawByteArray(x,y,nH,nV,pIcon->pBmp);
	else
		_DrawByteArrayEx(x,y,nH,nV,pIcon->pBmp, pen);
}
 
//画一个英文文字函数
void DrawEnChar(LCD_POS x, LCD_POS y, char ch, GUI_PEN pen/*=PEN_COPY*/,INT8U nEnFontType /*= LCD_EN_FONT_8_8*/)
{	 
	if(nEnFontType == LCD_EN_FONT_8_8)
	{
		if(pen==PEN_COPY)
			_DrawByteArray(x,y,8,1,asclib8_8[ch-0x20]);
		else
			_DrawByteArrayEx(x,y,8,1,asclib8_8[ch-0x20], pen);
	}
	else if(nEnFontType == LCD_EN_FONT_8_16)
	{
 	  	_DrawByteArray(x,y,8,2,asclib8_16[ch-0x20] );
//		for( i=0;i<16;i++)
// 			DrawByte(x, y++, asclib8_16[ch-0x20][i], pen);
// 		for (j = 0 ; j < 2; j++)
// 		{
// 			DrawByte(x, y+j, asclib8_16[ch-0x20][k++], pen);
// 		}		
	}
}
void DrawCnChar(LCD_POS x, LCD_POS y, INT16U iID, GUI_PEN pen/*=PEN_COPY*/)
{
//	L_ASSERT(iID <ZIKUNUM);
	if(pen==PEN_COPY)
		_DrawByteArray(x,y,16,2,g_WordStock[iID].m);
	else
		_DrawByteArrayEx(x,y,16,2,g_WordStock[iID].m, pen);
}

void DrawChar(LCD_POS x, LCD_POS y, INT16U index, GUI_PEN pen/*=PEN_COPY*/,INT8U nEnFontType /*= LCD_EN_FONT_8_8*/)
{
	if( index< CNZI_FIRST_INDEX)
	{
		DrawEnChar(x,y,index,pen,nEnFontType);
	}
	else
	{
		DrawCnChar(x,y,index-CNZI_FIRST_INDEX,pen);
	}
}

void  DrawCnCharCode(LCD_POS x, LCD_POS y, INT16U code, GUI_PEN pen/*=PEN_COPY*/)
{
	int nID=FindCnCharIndex(code);
	if(nID != NOFIND)
		DrawCnChar(x,y,nID,pen);
}

void  DrawCharCode(LCD_POS x, LCD_POS y, INT16U code, GUI_PEN pen/*=PEN_COPY*/,INT8U nEnFontType /*= LCD_EN_FONT_8_8*/)
{
	if(code>=256)
	{
		//中文字符
		DrawCnCharCode(x,y,code,pen);
	}
	else
	{
		//英文字符
		DrawEnChar(x,y,code,pen,nEnFontType);
	}
}


//判断字符串格式
typedef enum TEXT_FORMAT
{
	TEXT_ALL_EN,		// 全英文
	TEXT_ALL_CN,		// 全汉字
	TEXT_EN_CN,			// 中应文混合
	TEXT_UNKNOW         // 未知
}TEXT_FORMAT;

static INT8U JudgeStringText(const INT16U * pStr, INT16U nLen)
{
	INT8U flagEN=FALSE;
	INT8U flagCN=FALSE;
	INT16U i = 0;
	INT16U zi = 0;

	for ( i=0;i<nLen;i++)
	{
		zi=pStr[i];
		if(zi>=CNZI_FIRST_INDEX)
		{
			flagCN=TRUE;
		}
		else if(zi!='\n' && zi!='\r' && zi!=' ' && zi!='\t')
		{
			flagEN=TRUE;
		}
	}

	// 全是英文字符
	if((flagCN==FALSE) && (flagEN==TRUE))
		return TEXT_ALL_EN;

	// 全是中文
	if( (flagCN==TRUE) && (flagEN==FALSE))
		return TEXT_ALL_CN;

	// 混和型字符串
	if((flagCN==TRUE) && (flagEN==TRUE))
		return TEXT_EN_CN;

	return TEXT_ALL_EN;
}


//最简单的绘制字符串的函数，不需要换行处理，返回本行占用空间范围
static CGuiRect _DrawString(LCD_POS x, LCD_POS y, const INT16U * pCode, INT16U nLen, GUI_PEN pen/*=PEN_COPY*/,INT8U nEnFontType)
{
	INT16U i = 0;
	INT8U nTextStyle= 0;
	INT16U zi = 0;

	CGuiRect rect;

	rect.right=rect.left=x;
	rect.top=rect.bottom=y;

	if(x>=GetLCDWidth() || y>= GetLCDHeight())
	{
		return rect;
	}

	nTextStyle=JudgeStringText(pCode, nLen);

	switch(nTextStyle) 
	{
	case TEXT_ALL_EN:
		{
			if(nEnFontType == LCD_EN_FONT_8_8)
				rect.bottom+=8;
			else if(nEnFontType == LCD_EN_FONT_8_16)
				rect.bottom+=16;			
		}
		break;
	case TEXT_ALL_CN:
	case TEXT_EN_CN:
		rect.bottom+=16;
		break;
	default :
		break;
	}

	for ( i=0;i<nLen;i++) 
	{
		zi=pCode[i];
		if(zi>=CNZI_FIRST_INDEX)
		{
			//中文
			DrawCnChar(rect.right, rect.top, zi-CNZI_FIRST_INDEX, pen);
			rect.right+=16;	//右移16象素
		}
		else //英文
		{
			switch(nTextStyle) 
			{
			case TEXT_ALL_EN:
				DrawEnChar(rect.right, rect.top,zi,pen,nEnFontType);
				rect.right+=8;
				break;
			case TEXT_EN_CN:
				if(nEnFontType == LCD_EN_FONT_8_8)
				{
					//上半部填充空格
					DrawEnChar(rect.right, rect.top,' ',pen, LCD_EN_FONT_8_8);
					//下半部写英文字符
					DrawEnChar(rect.right, rect.top+8,zi,pen,LCD_EN_FONT_8_8);
					rect.right+=8;				
				}
				else if(nEnFontType == LCD_EN_FONT_8_16)
				{
					DrawEnChar(rect.right, rect.top,zi,pen,LCD_EN_FONT_8_16);
					rect.right+=8;									
				}
				break;
			default:
//				L_ASSERT(FALSE);	//不可能出现这种情况
				break;
			}
		}
	}
	return rect;
}
///////////////////////////////////////////////////////////////
//	函 数 名 : FindStringFirstLine
//	函数功能 : 查找字符串数组第一行，将第一行长度返回，并将下以行存放在pNext中
//	处理过程 : 
//	备    注 : 
//	作    者 : 
//	时    间 : 2006年2月8日
//	返 回 值 : INT16U  返回第一行字符个数
//	参数说明 : INT16U * pString,
//				 INT16U nLen
///////////////////////////////////////////////////////////////
INT16U FindStringFirstLine(const INT16U * pString, INT16U nLen, const INT16U ** pNext)
{
	int count=nLen;
	INT16U zi = 0;
	INT16U i;

	*pNext=NULL;

	for (i=0;i<nLen;i++)
	{
		zi=pString[i];
		if(zi=='\r' || zi=='\n')
		{
			count=i;
			break;
		}
	}

	//查找下一行字符
	for (;i<nLen;i++)
	{
		zi=pString[i];
		if(zi!='\r' && zi!='\n')
		{
			*pNext=&pString[i];
			break;
		}
	}

	return count;
}

//计算文字行宽
INT16U GetStringLineWidth(const INT16U * pString, INT16U nLen)
{
	INT16U nWidth=0;
	INT16U i = 0;
	for ( i=0;i<nLen;i++) 
	{
		if(pString[i]>=CNZI_FIRST_INDEX)
		{
			nWidth+=16;
		}
		else
		{
			nWidth+=8;
		}
	}

	return nWidth;
}
///////////////////////////////////////////////////////////////
//	函 数 名 : String2Resource
//	所属类名 : 
//	函数功能 : 将字符串转换为资源数组的形式
//	处理过程 : 
//	备    注 : 
//	作    者 :  
//	时    间 : 2006年2月8日
//	返 回 值 : INT16U				实际数组长度
//	参数说明 : INT16U * pBuff,		资源数组
//				 char * pString,	字符串
//				 INT16U nBuffLen	资源数组长度
///////////////////////////////////////////////////////////////
INT16U String2Resource(INT16U * pBuff, const char * pString, INT16U nBuffLen)
{
	INT32U count=0;
	INT32U i=0;
	INT8U c1=0,c2 = 0;
	INT16U index = 0;
	INT16U code = 0;

	while (i<strlen(pString)) 
	{
		c1=pString[i];
		c2=pString[i+1];
		index=NOFIND;
		if( (c1 & 0x80) && ( c2&0x80 ))
		{
			//中文字符
			code= ((INT16U)c1) | (((INT16U)c2)<<8);
			index=FindCnCharIndex(code);
			if(index!=NOFIND)
			{
				index+=CNZI_FIRST_INDEX;
			}
			i+=2;
		}
		else	//英文字符
		{
			index=c1;
			i++;
		}

		if(index!=NOFIND)
		{
			if(count < nBuffLen)
			{
				pBuff[count++]=index;
			}
			else
			{
				break;	//退出
			}
		}
	}

	return count;
}
//以资源数组的形式
static void  DrawStringA(LCD_POS x, LCD_POS y, const INT16U * pCode, INT16U nLen, GUI_PEN pen/*=PEN_COPY*/, GUI_ALIGN nAlign/*=ALIGN_LEFT*/,INT8U nEnFontType)
{
	CGuiRect rect;
	const INT16U * pTemp;
	const INT16U * pNext=pCode;
	INT16U n = 0;
	INT16U nWidth = 0;

	while(pNext!=NULL)
	{
		n=FindStringFirstLine(pNext, nLen-(pNext-pCode), &pTemp);
		switch(nAlign)
		{
		case ALIGN_LEFT:
			{
				rect=_DrawString(x,y,pNext, n, pen,nEnFontType);
				y=rect.bottom+1;
			}
			break;
		case ALIGN_RIGHT:
			{
				nWidth=GetStringLineWidth(pNext, n);
				rect=_DrawString(x-nWidth,y,pNext, n, pen,nEnFontType);
				y=rect.bottom+1;				
			}
			break;
		case ALIGN_CENTER:
			{
				nWidth=GetStringLineWidth(pNext, n);
				rect=_DrawString(x-nWidth/2,y,pNext, n, pen,nEnFontType);
				y=rect.bottom+1;
			}
			break;
		}
		pNext=pTemp;
	}
}
//绘制字符串 char *
static INT16U szDrawString[STRING_MAXLEN] = {0};
void DrawStringByStr(LCD_POS x, LCD_POS y, const char * pString, GUI_PEN pen/*=PEN_COPY*/, GUI_ALIGN nAlign/*=ALIGN_LEFT*/,INT8U nEnFontType)
{
	//将字符串转换为资源数组的形式
	INT16U nLen=String2Resource(szDrawString, pString, STRING_MAXLEN);
	DrawStringA(x, y, szDrawString, nLen, pen, nAlign,nEnFontType);
}
/*********************************************************************************************************
** 函数名称: DrawStringById
** 函数名称: DrawStringById
**
** 功能描述： 绘制字符串资源
**
** 输　入:  LCD_POS x
** 输　入:  LCD_POS y
** 输　入:  INT16U uStringResID
** 输　入:  GUI_PEN pen
** 输　入:  GUI_ALIGN nAlign
** 输　入:  INT8U nEnFontType
**          
** 输　出:  extern void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2008年10月19日
** 备  注: 
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern void DrawStringById(LCD_POS x, LCD_POS y, INT16U uStringResID, GUI_PEN pen/*=PEN_COPY*/, GUI_ALIGN nAlign/*=ALIGN_LEFT*/,INT8U nEnFontType)
{
//	L_ASSERT(uStringResID<NUM_STRINGRESOURCE);
	const STR_RES * pStrRes=&StringRes[uStringResID];
// 	if(GetSysCfgValue(ID_SYS_LANGUAGE)==LANGUAGE_CN)
// 	{
// 		DrawStringA(x,y,pStrRes->cn, pStrRes->cnlen, pen, nAlign,nEnFontType);
// 	}
// 	else if(GetSysCfgValue(ID_SYS_LANGUAGE)==LANGUAGE_EN)
// 	{
// 		//输出英文字符
// 		DrawStringByStr(x,y,pStrRes->en, pen, nAlign,nEnFontType);
// 	}
	//输出中文字符
	DrawStringA(x,y,pStrRes->cn, pStrRes->cnlen, pen, nAlign,nEnFontType);
}
//液晶重绘
void ClearScreen(void)
{
	//清缓冲
	memset(g_LcdBuff, 0, sizeof(g_LcdBuff));
	memset(g_LcdBuffFlag,0,sizeof(g_LcdBuffFlag));

	//清液晶
	LCD_ClearScreen();
}
/*********************************************************************************************************
** 函数名称: ClearScreenEx
** 函数名称: ClearScreenEx
**
** 功能描述：  
**
** 输　入:  INT8U bClear  是否马上清屏
**          
** 输　出:   extern void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年4月27日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void ClearScreenEx(INT8U bClear)
{
	//清缓冲
	memset(g_LcdBuff, 0, sizeof(g_LcdBuff));
	if(bClear)
	{
		//清液晶
		LCD_ClearScreen();
	}
}

void SetScreenRect(CGuiRect* rc,GUI_COLOR color)
{
	//优化处理
	INT16U i;
	INT16U yTop;
	INT8U nStartByteY; //整清除y开始字节
	INT8U nEndByteY;   //整清除y结束字节
	INT16U nCrlLen;     //整清除字节数

	if (rc == NULL)
		return;

	nStartByteY = (INT8U)(rc->top>>3) + ((rc->top%8)?1:0);
	nEndByteY   = (rc->bottom+1)/8; 
	nCrlLen     = rc->right-rc->left+1;
	for(i=nStartByteY; i<nEndByteY; i++ )
	{
		//按页清除可以整清的数据
		memset(&g_LcdBuff[rc->left][i],color?0xff:0,nCrlLen);
		//FIXME: 缺少 SetModify
	}

	//然后用按位清除的方式清除上下两边不是整数字节对齐的地方
	for(i=rc->left; i<=rc->right; i++)
	{
		//上
		if(rc->top%8)
		{
			for( yTop=rc->top; yTop<=((rc->top>>3)<<3)+7; yTop++)
				SetPixel2Buf(i,yTop,color);
		}
		//下
		if((rc->bottom+1)%8)
		{
			for(yTop=((rc->bottom>>3)<<3); yTop<=rc->bottom; yTop++)
				SetPixel2Buf(i,yTop,color);
		}
	}
}
/*********************************************************************************************************
** 函数名称: ReverseClrRect
** 函数名称: ReverseClrRect
**
** 功能描述：  使颜色反转
**
** 输　入:  CGuiRect * rc
**          
** 输　出:   extern void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月30日
** 备  注:  可以再优化下
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
extern void ReverseClrRect(CGuiRect * rc)
{
	//优化处理		
	INT16U i;
	INT16U y,x,yTop;
	INT8U nStartByteY; //整清除y开始字节
	INT8U nEndByteY;   //整清除y结束字节
	INT8U nCrlLen;     //整清除字节数

	if (rc == NULL)
		return;

	nStartByteY = rc->top>>3 + ((rc->top%8)?1:0);
	nEndByteY   = (rc->bottom+1)>>3; 
	nCrlLen     = rc->right-rc->left+1;
	for(i=nStartByteY; i<nEndByteY; i++ )
	{
		//按页反显数据
		for( x=rc->left; x<=rc->right; x++)
		{
			g_LcdBuff[x][i] = ~g_LcdBuff[x][i];
			g_LcdBuffFlag[x][i] = TRUE;
		}
	}

	//然后用按位清除的方式清除上下两边不是整数字节对齐的地方
	for(i=rc->left; i<=rc->right; i++)
	{
		//上
		if(rc->top%8)
		{
			for( yTop=rc->top; yTop<=((rc->top>>3)<<3)+7; yTop++)
				SetPixel2Buf(i,yTop,!LCD_GetPixel(i,yTop));
		}
		//下
		if((rc->bottom+1)%8)
		{
			for(yTop=((rc->bottom>>3)<<3); yTop<=rc->bottom; yTop++)
				SetPixel2Buf(i,yTop,!LCD_GetPixel(i,yTop));
		}
	}
}
/*********************************************************************************************************
** 函数名称: LcdUpdateRect
** 函数名称: LcdUpdateRect
**
** 功能描述：  更新矩形区数据
**
** 输　入:  int x0
** 输　入:  int y0
** 输　入:  int x1
** 输　入:  int y1
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月23日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void  LcdUpdateRect(int x0, int y0, int x1, int y1)
{
	//直接绘图时直接更新共享内存中的液晶数据	
	int nStartPage=y0>>3;
	int nEndPage=(y1>>3) + ((y1%8)?1:0);
	int i = 0,j = 0;

	//页缓存数据是否溢出
	if(nEndPage>=16)
		nEndPage=15;

	//按纵向方式更新液晶
	for( i=nStartPage; i<=nEndPage; i++)
	{
		LCD_SetDisplayPos(i,x0);
		for( j=x0; j<=x1; j++)
		{
			LCD_WriteDataApi(g_LcdBuff[j][i]);             //数值
		}
	}	
}
/*********************************************************************************************************
** 函数名称: UpdateLcdRect
** 函数名称: UpdateLcdRect
**
** 功能描述：  刷新矩形区域到LCD
**
** 输　入:  CGuiRect * pRC
**          
** 输　出:   extern void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月23日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void UpdateLcdRect( CGuiRect *rc)
{
	int nStartPage=rc->top>>3;
	int nEndPage=(rc->bottom>>3) + ((rc->bottom%8)?1:0);
	int i = 0 ,j = 0;

	//页缓存数据是否溢出
	if(nEndPage>=16)
		nEndPage=15;

	//按纵向方式更新液晶
	for( i=nStartPage; i<=nEndPage; i++)
	{
		LCD_SetDisplayPos(i,rc->left);
		for( j=rc->left; j<=rc->right; j++)
		{
			LCD_WriteDataApi(g_LcdBuff[i][j]);             //数值
		}
	}
}
/*********************************************************************************************************
** 函数名称: InvalidateScreen
** 函数名称: InvalidateScreen
**
** 功能描述：  通知准备重画屏幕
**
** 输　入:  INT8U bErase
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月30日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void InvalidateScreen( INT8U bErase )
{
 //	PostGuiTaskEvent(LCD_WM_PAINT,bErase,0);
}
/*********************************************************************************************************
** 函数名称: UpdateScreen
** 函数名称: UpdateScreen
**
** 功能描述：  马上刷新显存到LCD
**
** 输　入:  INT8U bErase
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年6月30日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void UpdateScreen(  INT8U bErase )
{
	INT32U col = 0, row = 0;
	if (bErase)
	{//全部重画				
		for( row=0;row<GetLCDBufferHeight();row++)		
		{
			LCD_SetDisplayPos(row,0);
			for ( col=0;col<GetLCDBufferWidth();col++) 
			{
	 			LCD_WriteDataApi(g_LcdBuff[col][row]);
				g_LcdBuffFlag[col][row] = FALSE;
			} 
		}	 
	}
	else
	{
		for( row=0;row<GetLCDBufferHeight();row++)		
		{
	 		for ( col=0;col<GetLCDBufferWidth();col++) 
			{
				if (g_LcdBuffFlag[col][row])
				{
					_LcdWriteData(col,row);
	 			}				
			} 
		}	 
	}
}


#if 0
static void Redraw(void)
{ 
	INT32U col = 0, row = 0;
	for ( col=0;col<GetLCDBufferWidth();col++) 
	{
		for( row=0;row<GetLCDBufferHeight();row++)
		{
			_LcdWriteData(col, row);
		} 
	}
}
#endif

