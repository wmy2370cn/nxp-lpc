 /****************************************Copyright (c)**************************************************
**							     LCD 硬件驱动部分，这部分需要根据不同的硬件来实现
**                                      
**                                      
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:   LcdDriver.c
**创   建   人:   Lijin
**最后修改日期:   2009-11-15
**描        述:   
**备        注 :   
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
#include <string.h>
#include "applib.h" 
#include "guidef.h"
#include "GraphicDriver.h"


//定义液晶长宽
#define LCD_WIDTH 240
#define LCD_HEIGHT 128

typedef INT8U FRAME_BUFF[LCD_WIDTH][LCD_WIDTH>>3];
static FRAME_BUFF g_LcdBuff;

static void  LcdSetPixel (GUI_COLOR *c, INT32U x, INT32U y);
static void  LcdGetPixel (GUI_COLOR *c, INT32U x, INT32U y);
static void LcdDrawHLine(GUI_COLOR *c, INT32U x1, INT32U x2, INT32U y);
static void LcdDrawVLine(GUI_COLOR *c, INT32U x1, INT32U x2, INT32U y);
static void LcdUpdate(CGuiRect *rect);
//static LCD_BUFF_FLAG g_LcdBuffFlag;


static CGuiHwDriver LcdDrvier = 
{
	LCD_WIDTH,
	LCD_HEIGHT,
	LcdUpdate,
	&g_LcdBuff[0][0],
	LcdSetPixel,
	LcdGetPixel,
	LcdDrawHLine,
	LcdDrawVLine	
};

//定义空指令
#define NOP   __asm{ NOP }
void GuiDelayNS(INT16U ns)
{
	INT32U count=3*ns;
	INT32U i;
	for (i=0;i<count;i++)
	{
		NOP;
	}
}
void GuiDelayMS(INT16U ms)
{
	//执行11059200次语句，花时3.570S，差不多每ms计算3097条语句
	INT32U count=3339*ms;
	INT32U i;
	for (i=0;i<count;i++)
	{
		NOP;
	}
}

/*LCD部分  */
#define   LCD_RESET (*((volatile unsigned short *) 0x82000010))
#define   LCD_CS (*((volatile unsigned short *) 0x82000014))
#define   LCD_WR (*((volatile unsigned short *) 0x82000018))
#define   LCD_RD (*((volatile unsigned short *) 0x82000020))
#define   LCD_DATA (*((volatile unsigned short *) 0x82000024))
#define   LCD_ADDR (*((volatile unsigned short *) 0x82000028))
#define   LCD_BACK (*((volatile unsigned short *) 0x82000030))


void LCD_Senddata(INT8U dat) //写数据
{
	LCD_CS=0x00;
	LCD_ADDR = 0x80;
	LCD_RD = 0x80;
	LCD_WR = 0x00;
	LCD_DATA = dat;
	//	ASM_NOP; ASM_NOP; ASM_NOP;
	GuiDelayNS(1);
	LCD_WR = 0x80;
}
/*********************************************************************************/
void LCD_SendCmd(INT8U cmd) //写命令
{
	LCD_CS=0x00;
	LCD_ADDR = 0x00;
	LCD_RD = 0x80;
	LCD_WR = 0x00;
	LCD_DATA = cmd;
	//	ASM_NOP; ASM_NOP; ASM_NOP;
	GuiDelayNS(1);
	LCD_WR = 0x80;
}

//对液晶写屏
void LCD_WriteData(INT8U x, INT8U y, INT8U val)
{
	INT8U page_address = y >> 3;

	if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT))
		return;

	page_address = (y & 0x0f) | 0xb0;

	LCD_SendCmd(page_address); //设置页地址

	//列地址
	LCD_SendCmd(x & 0x0f);
	LCD_SendCmd(0x10 + (INT8U) ((x & 0xf0) >> 4));

	LCD_Senddata(val); //数值
}

//LCD清屏
void LCD_ClearScreen(void)
{
	INT16U i,j;
	LCD_SendCmd(0XB0); /*SET PAGE ADDRESS*/
	LCD_SendCmd(0X10); /*SET COLUMN ADDRESS MSB(00000000)*/
	LCD_SendCmd(0X00); /*SET COLUMN ADDRESS LSB(00000000)*/
	LCD_SendCmd(0X89); /*SET RAM ADDRESS CONTROL*/
	for(i=0;i<LCD_HEIGHT>>3;i++)
	{
		for(j=0;j<LCD_WIDTH;j++)
			LCD_Senddata(0X00);
	}
}

//设置内存存取位置
void LCD_SetDisplayPos(INT8U page_address, INT8U col_address)
{
	LCD_SendCmd(0xb0 + (INT8U) (page_address & 0x0f));
	LCD_SendCmd(col_address & 0x0f);
	LCD_SendCmd(0x10 + (INT8U) ((col_address & 0xf0) >> 4));
}

void LCD_WriteDataApi(INT8U val)
{
	LCD_Senddata(val);             //数值
}
static void  LCD_Init(void)  //液晶初始化
{
	LCD_RESET=0;
	GuiDelayMS(10);
	LCD_RESET=0x80;
	GuiDelayMS(10);
	LCD_SendCmd(0xE2);        //system reset
	GuiDelayMS(1);
	LCD_SendCmd(0xe9);        //set lcd bias ratio (10b=12.0)
	LCD_SendCmd(0x27);        //set mux rate and temperature compensation (11b=-0.20%/c)
	LCD_SendCmd(0x81);        //
	LCD_SendCmd(0xc0);        //set gain and potentiometer(double-byte command)

	LCD_SendCmd(0x2f);        //set power control
	LCD_SendCmd(0x40);        //set start line to 0
	LCD_SendCmd(0xa4);        //set ALL_PIXEL_ON to OFF
	LCD_SendCmd(0xa6);        //set Inverse Display to OFF
	LCD_SendCmd(0x89);        //set RAM Address Cntrol
	LCD_SendCmd(0xc8);        //set Map Control:
	LCD_SendCmd(0xaf);		  //Display Enable;
}


// Y点在八位中的位
#define GetYBitPos(y) (((INT8U)(y)) & 0x07)
// Y点在八位中的Mask 0x10形式
#define GetYBitMask(y) (1<<GetYBitPos(y))
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
	if (col>=LCD_WIDTH|| row >= LCD_HEIGHT)
		return;
// 	if( g_LcdBuff[col][row] != data)
// 		g_LcdBuffFlag[col][row] = TRUE;

	g_LcdBuff[col][row] = data;	
}
/*********************************************************************************************************
** 函数名称: LcdSetPixel
** 函数名称: LcdSetPixel
**
** 功能描述： 画一个点 
**
** 输　入:  GUI_COLOR * c
** 输　入:  INT32U x
** 输　入:  INT32U y
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月14日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void  LcdSetPixel (GUI_COLOR *pClr, INT32U x, INT32U y)
{
	INT8U b = 0 ; 

	if(x>=LCD_WIDTH || y>= LCD_HEIGHT || pClr == NULL)
		return;	 
 	b=g_LcdBuff[x][y>>3];

	if (*pClr == GUI_COLOR_BK)
	{
		b &= ~ GetYBitMask(y);
	}
	else
	{
		b |= GetYBitMask(y);
	}


//	LCD_WriteData(x,y,*c); 
}

static void LcdGetPixel(GUI_COLOR *pClr, INT32U x, INT32U y)
{ 
	if(x>=LCD_WIDTH || y>= LCD_HEIGHT || pClr == NULL)
		return;	 

}
/*********************************************************************************************************
** 函数名称: LcdDrawHLine
** 函数名称: LcdDrawHLine
**
** 功能描述：  画水平线
**
** 输　入:  GUI_COLOR * c
** 输　入:  INT32U x1
** 输　入:  INT32U x2
** 输　入:  INT32U y
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月14日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void LcdDrawHLine(GUI_COLOR *c, INT32U x1, INT32U x2, INT32U y)
{
	INT32U i  = 0;
	INT8U page_address = y >> 3;

	if ((x1 >= LCD_WIDTH) ||(x2 >= LCD_WIDTH) || (y >= LCD_HEIGHT))
		return;

	if(x1>x2)
	{
		i=x1;
		x1=x2;
		x2=i;
	}

	page_address = (y & 0x0f) | 0xb0;

	LCD_SendCmd(page_address); //设置页地址
	for (i=x1;i<x2;i++)
	{
		//列地址
		LCD_SendCmd(i & 0x0f);
		LCD_SendCmd(0x10 + (INT8U) ((i & 0xf0) >> 4));

		LCD_Senddata(*c); //数值 
	}  
}
/*********************************************************************************************************
** 函数名称: LcdDrawVLine
** 函数名称: LcdDrawVLine
**
** 功能描述：  垂直线
**
** 输　入:  GUI_COLOR * c
** 输　入:  INT32U x
** 输　入:  INT32U y1
** 输　入:  INT32U y2
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年12月14日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void LcdDrawVLine(GUI_COLOR *c, INT32U x, INT32U y1, INT32U y2)
{

}

static void LcdUpdate(CGuiRect *rect)
{
	/* nothing for none-DMA mode driver */
}