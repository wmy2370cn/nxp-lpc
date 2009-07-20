/****************************************Copyright (c)**************************************************
**                                  液晶底层驱动      
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
#include "hd_Const.h"
#include "LcdDriver.h"

#define ECPRO610

#ifdef ECPRO610

/*************************************************************************************
** 液晶模块驱动
** LCD
**************************************************************************************/
void read_status1(void)
{	
	INT8U status;
	FIO1DIR &=0xfc03ffff;//输入
	FIO1CLR =LCD_DIR;//数据读入，245DIR为低
	FIO0CLR = LCD_E1;  //下降沿为写操作，高电平为读操作，低点平总线为高阻状态 
	FIO4CLR = LCD_A0;  //"L"=write command status "H"=write data status
	FIO2SET = LCD_RW;  //"H"=read status "L"=write status
    FIO0SET = LCD_E1;  
	status = (FIO1PIN&LCD_DB)>>18;	
	FIO0CLR = LCD_E1;
	FIO1DIR = 0x1ffc0000;
	
}

void GXM12232_SED1520_delay1s(INT16U delay_xms)
{
	INT16U i,j,m;
	for(i=0;i<delay_xms;i++)
	{
		for(j=0;j<280;j++)
		{
			for(m=0;m<125;m++)
			{
				ASM_NOP;
				ASM_NOP;
				ASM_NOP;
				ASM_NOP;
			}
		}
	}
}

//***********delay50us************************//
void GXM12232_SED1520_delay50us(INT8U count50us)
{
    INT8U  k ;
	ASM_NOP;
    for(k=0;k<2*count50us;k++)
    {
     ASM_NOP;     ASM_NOP;     ASM_NOP;     ASM_NOP;     ASM_NOP;
    }
}

//*************Write  lift screen data*****************//
void  GXM12232_SED1520_LWriteData(INT8U dat)
{	
	FIO1DIR |=0x03fc0000;//输出
	FIO1SET =LCD_DIR; //245DIR=1
	FIO2CLR=LCD_E1;
	//下降沿为写操作，高电平为读操作，低点平总线为高阻状态 
	FIO4SET = LCD_A0;  //"L"=write command status "H"=write data status
	FIO2CLR = LCD_RW;  //"H"=read status "L"=write status
	FIO1CLR = LCD_DB; FIO1SET =dat<<18;
	ASM_NOP;
	ASM_NOP;
	FIO0SET = LCD_E1;   
	FIO0CLR = LCD_E1;
	ASM_NOP;
	ASM_NOP;
	
}

//*************Write  Right screen data*****************//
void  GXM12232_SED1520_RWriteData(INT8U dat)
{	
	FIO1DIR |=0x03fc0000;//输出
	FIO1SET =LCD_DIR; //245DIR=1
	FIO0CLR=LCD_E2;
	//下降沿为写操作，高电平为读操作，低点平总线为高阻状态 
	FIO4SET = LCD_A0;  //"L"=write command status "H"=write data status
	FIO2CLR = LCD_RW;  //"H"=read status "L"=write status
	FIO1CLR = LCD_DB; FIO1SET =dat<<18;
	ASM_NOP;
	ASM_NOP;
	FIO0SET = LCD_E2;   
	FIO0CLR = LCD_E2;
	ASM_NOP;
	ASM_NOP;
	
}

//****************write Lift screen command*******************//
void  GXM12232_SED1520_LWritecommand(INT8U command)
{	
	FIO1DIR |=0x03fc0000;//输出
	FIO1SET =LCD_DIR; //245DIR=1
	FIO0CLR=LCD_E1;
	//下降沿为写操作，高电平为读操作，低点平总线为高阻状态 
	FIO4CLR = LCD_A0;  //"L"=write command status "H"=write data status
	FIO2CLR = LCD_RW;  //"H"=read status "L"=write status
	FIO1CLR = LCD_DB; FIO1SET =command<<18;
	ASM_NOP;
	ASM_NOP;
	FIO0SET = LCD_E1;   
	FIO0CLR = LCD_E1;
	ASM_NOP;
	ASM_NOP;
	
}
//****************write Right screen command*******************//
void  GXM12232_SED1520_RWritecommand(INT8U command)
{	
	FIO1DIR |=0x03fc0000;//输出
	FIO1SET =LCD_DIR; //245DIR=1
	FIO0CLR=LCD_E2;
	//下降沿为写操作，高电平为读操作，低点平总线为高阻状态 
	FIO4CLR = LCD_A0;  //"L"=write command status "H"=write data status
	FIO2CLR = LCD_RW;  //"H"=read status "L"=write status
	FIO1CLR = LCD_DB; FIO1SET =command<<18;
	ASM_NOP;
	ASM_NOP;
	FIO0SET = LCD_E2;   
	FIO0CLR = LCD_E2;
	ASM_NOP;
	ASM_NOP;
	
}

/***************************************************************/     
void GXM12232_SED1520_initlcd(void)
{   
	/*******************************************************************************************
	**打开LCD Work at 68 MPU mode  纵向送数方式
	*******************************************************************************************/
	FIO1CLR	= LCD_RESET;
	GXM12232_SED1520_delay50us(1);
	FIO1SET	= LCD_RESET;
  
	FIO1SET = LCD_CL;  
	FIO4CLR = LCD_A0;  //"H"=write command status "L"=write data status//
	FIO2SET = LCD_RW;   //"H"=read status "L"=write status//
	FIO0CLR = LCD_E1;  //下降沿为写操作，高电平为读操作，低点平总线为高阻状态//
	FIO0CLR = LCD_E2;
    read_status1();
     
	GXM12232_SED1520_LWritecommand(0xe2);//;软件复位
    GXM12232_SED1520_RWritecommand(0xe2);//;软件复位
      
    GXM12232_SED1520_LWritecommand(0xa4);//;退出休闲状态
    GXM12232_SED1520_RWritecommand(0xa4);//;退出休闲状态
          
    GXM12232_SED1520_LWritecommand(0xa9);//;设置位32位占空比
    GXM12232_SED1520_RWritecommand(0xa9);//;设置位32位占空比

    GXM12232_SED1520_LWritecommand(0xa0);//;ADC SECLECT SEG0~SEG61
    GXM12232_SED1520_RWritecommand(0xa0);//;ADC SECLECT SEG0~SEG61
         
    GXM12232_SED1520_LWritecommand(0xc0);//;设置起始行
    GXM12232_SED1520_RWritecommand(0xc0);//;设置起始行
      
    GXM12232_SED1520_LWritecommand(0xaf);//;显示开
    GXM12232_SED1520_RWritecommand(0xaf);//;显示开
    read_status1();
}

/***************************************************************/
//点亮液晶
void  hd_LightLCD(INT8U bLightOn)
{
	if(bLightOn)
	{  
		FIO1SET = LCD_CL;
	}
	else
	{
		FIO1CLR = LCD_CL;
	}
}

//LCD清屏
void hd_LCD_ClearScreen(void)
{
	INT8U i,j;
	INT8U page_address;
	for(i=0;i<4;i++) 
	{
    	page_address=i;
   		page_address=page_address|0x0b8;
    	GXM12232_SED1520_LWritecommand(page_address);//;设置页地址
    	GXM12232_SED1520_RWritecommand(page_address);//;设置页地址
    
    	GXM12232_SED1520_LWritecommand(0x00);//;列地址
    	GXM12232_SED1520_RWritecommand(0x00);//;列地址

    	for(j=0;j<61;j++)
    	{
     		GXM12232_SED1520_LWriteData(0x00);
     		GXM12232_SED1520_RWriteData(0x00);
    	}
     
   }
}

//LCD写数据
///////////////////////////////////////////////////////////////
//	函 数 名 : hd_LCD_WriteData
//	函数功能 : 对液晶写屏
//	处理过程 : 
//	备    注 : 
//	作    者 : 仲伟
//	时    间 : 2006年1月24日
//	返 回 值 : void
//	参数说明 : INT8U x,			//横坐标
//				 INT8U y,		//纵坐标
//				 INT8U val		//值
///////////////////////////////////////////////////////////////
extern void hd_LCD_WriteData(INT8U x, INT8U y, INT8U val)
{
	INT8U page_address;

	if((x>LCD_WIDTH-1) || (y>LCD_HEIGHT-1))
		return;
	
	page_address=y|0xb8;

	// 左半块
	if(x<61)
	{
		GXM12232_SED1520_LWritecommand(page_address); //设置页地址
		GXM12232_SED1520_LWritecommand(x);            //列地址
		GXM12232_SED1520_LWriteData(val);             //数值
	}
	else // 右半块
	{ 
		GXM12232_SED1520_RWritecommand(page_address); //设置页地址
		GXM12232_SED1520_RWritecommand(x-61);         //列地址
		GXM12232_SED1520_RWriteData(val);             //数值
	}
} 

#endif // ECPRO610
