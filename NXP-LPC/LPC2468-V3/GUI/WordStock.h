///////////////////////////////////////////////////////////////
//	文 件 名 : ZikuL.h
//	文件功能 : 字库头文件
//	作    者 : 仲伟
//	创建时间 : 2008-03-06 14:50:04
//	项目名称 : 
//	操作系统 : 
//	备    注 :
//	历史记录 : 
///////////////////////////////////////////////////////////////
#ifndef ZIKUL_H
#define ZIKUL_H

//如果需要自己处理汉字库

typedef struct
{
   INT16U code;			//内码
   INT8U m[32];			//字摸
}WORD_STOCK;

#define CN_WORD_NUM 253		//提供的汉字个数

extern const WORD_STOCK g_WordStock[];

#define	NOFIND 0XFFFF       // 没找到汉字
extern INT16U FindCnCharIndex(INT16U code);

#endif	//ZIKUL_H
