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


#include "applib.h"
#include "lcddriver.h"


/*LCD部分  */
#define   LCD_RESET (*((volatile unsigned short *) 0x82000010))
#define   LCD_CS (*((volatile unsigned short *) 0x82000014))
#define   LCD_WR (*((volatile unsigned short *) 0x82000018))
#define   LCD_RD (*((volatile unsigned short *) 0x82000020))
#define   LCD_DATA (*((volatile unsigned short *) 0x82000024))
#define   LCD_ADDR (*((volatile unsigned short *) 0x82000028))
#define   LCD_BACK (*((volatile unsigned short *) 0x82000030))


//液晶灯状态
static INT8U g_nLcdLed;

//关液晶背光
#define  LCD_BACK_OFF(ignore)  LCD_BACK =0x80;
//开液晶背光
#define  LCD_BACK_ON(ignore)  LCD_BACK =0x00;
//点亮液晶灯
void TurnOnLcd(void)
{
 	LCD_BACK_ON( );
	g_nLcdLed=TRUE;
}
//熄灭液晶灯
void TurnOffLcd(void)
{
 	LCD_BACK_OFF( );
	g_nLcdLed=FALSE;
}
//定义空指令
#define NOP   __asm{ NOP }
///////////////////////////////////////////////////////////////
//	函 数 名 : hd_Delay
//	函数功能 : 延时多少ms
//	处理过程 :
//	备    注 :
//	作    者 : 仲伟
//	时    间 : 2005年9月29日
//	返 回 值 : void
//	参数说明 : INT16U ms
///////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////
//	函 数 名 : hd_DelayNS
//	函数功能 : 延时纳秒
//	处理过程 :
//	备    注 :
//	作    者 :
//	时    间 : 2005年10月7日
//	返 回 值 : void
//	参数说明 : INT16U ns
///////////////////////////////////////////////////////////////
void GuiDelayNS(INT16U ns)
{
	INT32U count=3*ns;
	INT32U i;
	for (i=0;i<count;i++)
	{
		NOP;
	}
}

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
/*********************************************************************************************************
** 函数名称: InitLcd
** 函数名称: InitLcd
**
** 功能描述： 启动时调用 
**
** 输　入:  void
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月15日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void InitLcd(void)
{
	//LCD初始化
	LCD_Init();
	LCD_ClearScreen();
}









