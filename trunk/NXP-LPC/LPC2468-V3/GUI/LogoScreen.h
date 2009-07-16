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
#ifndef SCREEN_EXT_H
#define	SCREEN_EXT_H

typedef enum BRAND_STATE
{
	STATE_NORMAL=0,                       //一般显示状态
	STATE_COUNT                           //状态总数 
}BRAND_STATE;

//	描    述 : 启动画面中版本信息画面
//	功    能 : 显示操作模块启动时显示公司版本信息和LOGO
typedef struct CLogoScreen 
{
	CScreenBase  Base;

 

	INT8U   CurrentState;                       //当前显示状态
	CLcdCursor Cursor[STATE_COUNT];   

	 
}CLogoScreen;

//
extern void InitLogoScreen(CLogoScreen *pScreenExt ); 

//	描    述 : 默认显示界面
//	功    能 : 循环显示测量量及报告状况
// 显示闸门开度/流量
typedef struct CScreenDefault
{
	CScreenBase  Base;


	//本窗口要使用的各种资源
	INT8U   Toggle;                 //切换循环显示
	INT16U	CurrentTypeIndex;        //当前显示测值类型在MeasDispType中的索引
	INT16U	CurrentMeasIndex;		//当前显示测值在MeasDisp中的索引

}CScreenDefault;
 
extern void InitDefaultScreen(CScreenDefault *pScreenExt ); 

 
extern CLogoScreen      LogoScreen;       // 版本声明画面
extern CScreenDefault       defscreen;
 

#endif
