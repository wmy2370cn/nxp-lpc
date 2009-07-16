//*------------------------------------------------------------------------------------------------
//* 文件名		   : sys_arch.h
//* 功能描述       : 操作系统模拟层接口函数实现文件的头文件
//* 作者    	   : 焦海波
//* 版本		   : 0.1
//* 建立日期、时间 : 2006/09/04 11:33
//* 修改日期、时间 : 
//* 修改原因       : 
//*------------------------------------------------------------------------------------------------
#ifndef	__sys_arch_h__
#define	__sys_arch_h__

//*--------------------------------------  宏定义 ------------------------------------------------
#define	MBOX_SIZE					16		//* 指定邮箱能够接收的消息数量
#define	MBOX_NB						8		//* 指定邮箱个数，也就是链表长度

#define T_LWIP_THREAD_START_PRIO	7		//* LwIP线程起始优先级号
#define	T_LWIP_THREAD_MAX_NB		1		//* 最多允许建立一个线程
#define	T_LWIP_THREAD_STKSIZE		512 	//* LwIP线程的堆栈大小

#define	SYS_MBOX_NULL				((void *)0)
#define	SYS_SEM_NULL				(void *)0
//*-------------------------------------- 结构定义 ------------------------------------------------
/* LwIP邮箱结构 */
typedef struct stLwIPMBox{
	struct stLwIPMBox 	*pstNext;
	HANDLER 			hMBox;
	void 				*pvaMsgs[MBOX_SIZE];
} ST_LWIP_MBOX, *PST_LWIP_MBOX;
//*------------------------------- 一些自定义的数据类型 -------------------------------------------
typedef HANDLER 		sys_sem_t;
typedef PST_LWIP_MBOX 	sys_mbox_t;		//* LwIP邮箱
typedef u8_t			sys_thread_t;	//* LwIP线程ID
#endif
