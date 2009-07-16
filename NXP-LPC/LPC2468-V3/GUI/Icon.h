///////////////////////////////////////////////////////////////
//	文 件 名 : Icon.h
//	文件功能 : 资源文件
//	作    者 : 仲伟
//	创建时间 : 2008-03-06 14:50:04
//	项目名称 : 
//	操作系统 : 
//	备    注 :
//	历史记录 : 
///////////////////////////////////////////////////////////////
//资源文件
#ifndef _ICON_H
#define _ICON_H

#define ID_ICON_NULL                                    	0   	

#define ID_ICON_NOICON                                  	1   	

#define ID_ICON_UP                                      	2   	

#define ID_ICON_DOWN                                    	3   	

#define ID_ICON_RIGHT                                   	4   	

#define ID_ICON_FALSE                                   	5   	

#define ID_ICON_TRUE                                    	6   	

#define ID_ICON_OPEN                                    	7   	

#define ID_ICON_CLOSE                                   	8   	

#define ID_ICON_LEFT                                    	9   	

#define ID_ICON_CIRCLE_EMPTY                            	10  	

#define ID_ICON_CIRCLE_FILL                             	11  	

#define ID_ICON_A                                       	12  	

#define ID_ICON_B                                       	13  	

#define ID_ICON_C                                       	14  	

#define ID_ICON_D                                       	15  	

#define ID_ICON_E                                       	16  	

#define ID_ICON_F                                       	17  	

#define ID_ICON_DESKTOP_H                               	18  	

#define ID_ICON_DESKTOP_F                               	19  	

#define ID_ICON_DESKTOP_E                               	20  	

#define ID_ICON_DESKTOP_D                               	21  	

#define ID_ICON_DESKTOP_G                               	22  	

#define ID_ICON_DESKTOP_C                               	23  	

#define ID_ICON_DESKTOP_I                               	24  	

#define ID_ICON_DESKTOP_A                               	25  	

#define ID_ICON_DESKTOP_B                               	26  	

#define ID_ICON_WARN                                    	27  	

#define ID_ICON_INFO                                    	28  	

#define ID_ICON_CIRCLE_EMPTY_LARGE                      	29  	

#define ID_ICON_CIRCLE_FILL_LARGE                       	30  	

#define ID_ICON_WISCOM                                  	31  	


typedef struct
{
	INT8U nWidth;
	INT8U nHeight;
	const INT8U * pBmp;
}ICON_RES;

extern const ICON_RES IconRes[];

extern const INT16U NUM_ICON;


#endif
