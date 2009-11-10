///////////////////////////////////////////////////////////////
//	文 件 名 : ZikuL.h
//	文件功能 : 字库头文件
//	作    者 :  
//	创建时间 : 2008-03-06 14:50:04
//	项目名称 : 
//	操作系统 : 
//	备    注 :
//	历史记录 : 
///////////////////////////////////////////////////////////////
#pragma  once
//如果需要自己处理汉字库

typedef struct CN_FONTS_LIB
{
   INT16U code;			//内码
   INT8U m[32];			//字摸
}CN_FONTS_LIB;

#define CN_WORD_NUM 87		//提供的汉字个数

extern const CN_FONTS_LIB g_WordStock[];

#define	NOFIND 0XFFFF       // 没找到汉字
extern INT16U FindCnCharIndex(INT16U code);

 
