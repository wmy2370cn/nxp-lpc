//*------------------------------------------------------------------------------------------------
//* 文件名				: cc.h
//* 功能描述			: 与硬件平台相关的环境变量及数据类型声明文件
//* 作者    			: 焦海波
//* 版本				: 0.1
//* 建立日期、时间		: 2006/09/04 14:26
//* 修改日期、时间	    : 
//* 修改原因			: 
//*------------------------------------------------------------------------------------------------
#ifndef	__cc_h__
#define	__cc_h__

#include	"ucos_ii.h"
//*------------------------------- 常用数据类型定义 -----------------------------------------------
typedef unsigned char  u8_t;
typedef signed   char  s8_t;
typedef unsigned short u16_t;
typedef signed   short s16_t;
typedef unsigned int   u32_t;
typedef signed   int   s32_t;
typedef u32_t          mem_ptr_t;
typedef	OS_EVENT*	   HANDLER;
//*---------------------------------- 常用宏定义 -------------------------------------------------
//* 临界代码保护宏
#define	SYS_ARCH_DECL_PROTECT(ulIntStatus)		u32_t  	ulIntStatus = 0;
#define	SYS_ARCH_PROTECT(ulIntStatus)			(ulIntStatus = OSCPUSaveSR());
#define	SYS_ARCH_UNPROTECT(ulIntStatus)			(OSCPURestoreSR(ulIntStatus));
					  

//* 网络数据包结构体封装宏
#define	PACK_STRUCT_FIELD(x)	__packed x
#define	PACK_STRUCT_STRUCT
#define	PACK_STRUCT_BEGIN		__packed
#define	PACK_STRUCT_END

#define	LWIP_PROVIDE_ERRNO

#define BYTE_ORDER LITTLE_ENDIAN

#define LWIP_PLATFORM_ASSERT(x) 
                                    


#endif
