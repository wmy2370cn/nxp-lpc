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
#ifndef CTRL_BASE_H
#define	CTRL_BASE_H
#include "guiconst.h"
#include "global.h"


typedef struct CCtrlBase 
{
	INT8U           Focus;       //是否聚焦到当前控件
	LCDRECT			Block;	    	//定义控件位置

	fnCreate   pfnCreate;
	fnDestory  pfnDestory;
	//回调函数
	fnOnKeyDown pfnKeyDown;
	fnOnMessage pfnMessage;
	
}CCtrlBase;



//EDIT控件的类型 数字型/控制字型
typedef enum EDIT_TYPE
{
	EDIT_TYPE_NUMBER,
	EDIT_TYPE_ENUM
}EDIT_TYPE;

typedef struct CNumberChar
{
	char  ch;	//该处字符
	INT8U type;	//位置类型
}CNumberChar;

typedef enum EVENT_EDITBOX 
{
	EVENT_REDRAW,	//控件重画
	EVENT_DRAWRECT	//重刷修改
}EVENT_EDITBOX;

/*数字型控件需要用到的变量*/
//各字符位置的数字意义
typedef enum CHAR_IDT
{
	CHAR_NONE,		//无效位置
	CHAR_SIGN,		//符号位
	CHAR_NUMBER,	//数字位
	CHAR_DOT		//小数点位
}CHAR_IDT;


//枚举类型，结构定义如下
typedef struct CSValue
{
	INT16U	NameID;			//枚举名称
	INT16U	n;				//值
}CSValue;//非数据类型的定值与数据值关联

typedef struct CSEnum
{
	INT16U	count;				//枚举类型个数
	const CSValue * pSValue;	//指向枚举类型数组的指针
	INT16U def;					//默认值
}CSEnum;

#define MAX_CHARNUMBER 12

typedef struct CEditBox
{
	CCtrlBase  Base;

	//////////////////////////////////////////////////////////////////////////
	/*EDIT控件用到的变量*/
	CSEnum  *m_pSEnum;
	INT32U   m_nValue;
	INT16U   m_nEnumIndex;
	INT32U   m_nMax;
	INT32U   m_nMin;
	INT8U   m_nPoint;



	INT8U   Type;		//EDIT控件类型

	CNumberChar NumberChar[MAX_CHARNUMBER];
	INT8U       NumberCharCur;		//聚焦点


	INT8U  m_bEditable;             // 是否可编辑
}CEditBox;

// 得到编辑框的数值，返回数值的有效性
//extern INT8U  GetValue(CEditBox *pEditBox , INT32U *pValue);
extern INT8U  GetValueInt(CEditBox *pEditBox,INT32U *pValue);
extern void SetEditable(CEditBox *pEditBox,INT8U bEditable);


typedef struct CEditBoxEx
{
	CEditBox BaseBox;


	INT8U    m_bShowToolTip;
	INT8U    m_bShowCaption;
	INT8U    m_bShowUnit;
	INT16U   m_nCaptionID;
	INT8U  *m_pUint;

}CEditBoxEx;




///////////////////////////////////////////////////////////////
//	类    名 : CLcdMenu
//	基    类 :  
//	描    述 : 菜单管理数据结构
//	功    能 : 对菜单进行管理
//	历史记录 : 
//	使用说明 : 
//	作    者 : 
//	时    间 : 
//	备    注 : 
///////////////////////////////////////////////////////////////
#define MAX_MENU_ITEMS 0x30    // 菜单最大有20个菜单项

typedef struct  CLcdMenu
{
	CCtrlBase  Base;

//////////////////////////////////////////////////////////////////////////
	// 必须填充的变量
	CMenuData  MenuLink[MAX_MENU_ITEMS]; //菜单链接
	INT16U     MenuCount;

	INT16U      MaxShowChars;           //该控件窗口中能显示的字符数
	INT16U      MaxShowLines;           //该控件窗口中能显示的行数


	/*动态滚动条*/
	LCDRECT ScrollBarRect;		//滚动条位置
	INT8U   ShowScrollBar;		//是否显示滚动条
 
	INT16U   CurrentItem;		//当前聚焦行
}CLcdMenu;


extern void AddItemPtr(CLcdMenu *pMenu, const CMenuData * pMenuItem);

extern void AddItemID(CLcdMenu *pMenu,INT16U nMenuID, INT16U nResourceID);

extern void AddItems(CLcdMenu *pMenu,const  CMenuData Items[], INT16U nItemCount);



 

#endif
