//*------------------------------------------------------------------------------------------------
//* 文件名				: sys_arch.c
//* 功能描述			: 操作系统模拟层接口函数实现文件
//* 作者    			: 焦海波
//* 版本				: 0.1
//* 建立日期、时间		: 2006/09/04 10:50
//* 修改日期、时间	    : 
//* 修改原因			: 
//*------------------------------------------------------------------------------------------------
//*------------------------------------------ 头文件 -----------------------------------------------
#include <stdlib.h>
#include <string.h>
#include	"../include/lwip/sys.h"
#include	"../include/arch/cc.h"
#include	"../include/arch/sys_arch.h"
//*------------------------------------- 常量、变量、宏 -------------------------------------------
static ST_LWIP_MBOX __staLwIPMBoxs[MBOX_NB];
PST_LWIP_MBOX pstCurFreeMBox;

//* LwIP线程使用的堆栈
OS_STK T_LWIP_THREAD_STK[T_LWIP_THREAD_MAX_NB][T_LWIP_THREAD_STKSIZE];

//* sys_timeouts数组，用于保存timeouts链表的首地址
static struct sys_timeouts __staSysTimeouts[T_LWIP_THREAD_MAX_NB + 1];
//*================================================================================================
//*　　　　　　　　　　　　　　　　　　　　　函　数　区
//*================================================================================================
//*------------------------------------------------------------------------------------------------
//* 函数名称 : sys_sem_new
//* 功能描述 : 建立并返回一个新的信号量
//* 入口参数 : <count>[in] 指定信号量的初始状态
//* 出口参数 : 返回新的信号量
//*------------------------------------------------------------------------------------------------
sys_sem_t sys_sem_new(u8_t count)
{
	return OSSemCreate(count);
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : sys_sem_signal
//* 功能描述 : 发送信号
//* 入口参数 : <sem>[in] sem指定要发送的信号
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
void sys_sem_signal(sys_sem_t sem)
{
	OSSemPost(sem);
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : sys_sem_free
//* 功能描述 : 释放信号量
//* 入口参数 : <sem>[in] 指定要释放的信号量
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
void sys_sem_free(sys_sem_t sem)
{
	INT8U	__u8Err;
	
	while(NULL != OSSemDel(sem, OS_DEL_ALWAYS, &__u8Err))
		OSTimeDlyHMSM(0, 0, 0, 100);
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : __MSToOSTicks
//* 功能描述 : 将毫秒转变成时钟节拍
//* 入口参数 : <u16MS>[in] 节拍数
//* 出口参数 : 毫秒数
//*------------------------------------------------------------------------------------------------
static INT16U __MSToOSTicks(INT16U u16MS)
{
	INT16U	__u16DelayTicks;
	
	if(u16MS != 0)
	{
		__u16DelayTicks = (u16MS * OS_TICKS_PER_SEC)/1000;
		if(__u16DelayTicks < 1)
			__u16DelayTicks = 1;
		else if(__u16DelayTicks > 65535)
			__u16DelayTicks = 65535;
		else;
	}
	else
		__u16DelayTicks = 0;		
		
	return __u16DelayTicks;
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : sys_arch_sem_wait
//* 功能描述 : 等待由参数sem指定的信号并阻塞线程
//* 入口参数 :     <sem>[in] sem指定要发送的信号
//*          : <timeout>[in] 指定等待的最长时间（单位为毫秒）。为0，线程会一直被阻塞直至收到指定的信号；非0，指定线
//*          :               程最长等待时间
//* 出口参数 : -                0: 在指定时间内等到指定信号
//*          : - SYS_ARCH_TIMEOUT: 在指定时间内没有等到指定信号
//*------------------------------------------------------------------------------------------------
u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout)
{	
	INT8U	__u8RtnVal;	
		
	OSSemPend(sem, __MSToOSTicks(timeout), &__u8RtnVal);

	if(__u8RtnVal == OS_NO_ERR)
		return 0;
	else
		return SYS_ARCH_TIMEOUT;
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : sys_init
//* 功能描述 : 根据sys_arch.txt文件建立，功能为初始化sys_arch层
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
void sys_init(void)
{
	INT8U i;
	
	//* 先把数组清0
	memset(__staLwIPMBoxs, 0, sizeof(__staLwIPMBoxs));
	
	//* 建立链表和邮箱
	for(i=0; i<(MBOX_NB-1); i++)
	{
		//* 把数组中的各成员链接在一起
		__staLwIPMBoxs[i].pstNext = &__staLwIPMBoxs[i+1];
				
		//* 建立邮箱，系统必须保证邮箱能够顺利建立，如果出现错误，那是程序BUG，应该在调试阶段排除
		__staLwIPMBoxs[i].hMBox = OSQCreate(__staLwIPMBoxs[i].pvaMsgs, MBOX_SIZE);
	}
	
	//* 别忘了数组中最后一个元素，它还没有建立邮箱呢
	__staLwIPMBoxs[MBOX_NB-1].hMBox = OSQCreate(__staLwIPMBoxs[MBOX_NB-1].pvaMsgs, MBOX_SIZE);
	
	//* 保存链表首地址
	pstCurFreeMBox = __staLwIPMBoxs;
	
	//* 初始化sys_timeouts数组，将每个数组成员保存的链表地址设置为NULL
	for(i=0; i<(T_LWIP_THREAD_MAX_NB + 1); i++)
		__staSysTimeouts[i].next = NULL;
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : sys_mbox_new
//* 功能描述 : 建立一个空的邮箱
//* 入口参数 : 无
//* 出口参数 : - != SYS_MBOX_NULL : 邮箱申请成功，返回一个指向被申请邮箱的指针
//*          : - = SYS_MBOX_NULL  : 邮箱没有申请成功
//*------------------------------------------------------------------------------------------------
sys_mbox_t sys_mbox_new(int size)
{
#if OS_CRITICAL_METHOD == 3                     
    OS_CPU_SR  		cpu_sr = 0;
#endif	

	PST_LWIP_MBOX	__pstMBox = SYS_MBOX_NULL;
   	
	OS_ENTER_CRITICAL()		;

		if(pstCurFreeMBox != NULL)
		{
			__pstMBox = pstCurFreeMBox;
			pstCurFreeMBox = __pstMBox->pstNext;
		}
 
	OS_EXIT_CRITICAL()	 ;
	
	return __pstMBox;
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : sys_mbox_free
//* 功能描述 : 释放邮箱，将邮箱归还给链表
//* 入口参数 : <mbox>[in] 要归还的邮箱
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
void sys_mbox_free(sys_mbox_t mbox)
{	
#if OS_CRITICAL_METHOD == 3                     
    OS_CPU_SR  		cpu_sr = 0;
#endif	
	
	//* 为了防止意外情况发生，再主动清空一次邮箱
	OSQFlush(mbox->hMBox);

	OS_ENTER_CRITICAL()	  ;
	{
		mbox->pstNext = pstCurFreeMBox;
		pstCurFreeMBox = mbox;
	}
	OS_EXIT_CRITICAL() ;
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : sys_mbox_post
//* 功能描述 : 将消息投递到指定的邮箱
//* 入口参数 : <mbox>[in] 指定要投递的邮箱
//*          :  <msg>[in] 指定要投递的消息
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
void sys_mbox_post(sys_mbox_t mbox, void *msg)
{
	while(OSQPost(mbox->hMBox, msg) == OS_Q_FULL)
		OSTimeDlyHMSM(0, 0, 0, 100);
}

err_t sys_mbox_trypost(sys_mbox_t q, void *msg)
{
  u32_t new_head;
  u32_t ret;
  SYS_ARCH_DECL_PROTECT(lev);

  /* parameter check */
//  LWIP_ASSERT("sys_mbox_free ", q != SYS_MBOX_NULL );
//  LWIP_ASSERT("q->sem != NULL", q->sem != NULL);
//  LWIP_ASSERT("q->sem != INVALID_HANDLE_VALUE", q->sem != INVALID_HANDLE_VALUE);

  SYS_ARCH_PROTECT(lev);

 #if 0
  new_head = q->head + 1;
  if (new_head >= MAX_QUEUE_ENTRIES) {
    new_head = 0;
  }
  if (new_head == q->tail) {
    SYS_ARCH_UNPROTECT(lev);
    return ERR_MEM;
  }
 

  q->q_mem[q->head] = msg;
  q->head = new_head;
  LWIP_ASSERT("mbox is full!", q->head != q->tail);
  ret = ReleaseSemaphore(q->sem, 1, 0);
  LWIP_ASSERT("Error releasing sem", ret != 0);
#endif

  SYS_ARCH_UNPROTECT(lev);
  return ERR_OK;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t q, void **msg)
{
  u32_t ret;
  SYS_ARCH_DECL_PROTECT(lev);
#if 0
  /* parameter check */
  LWIP_ASSERT("sys_mbox_free ", q != SYS_MBOX_NULL );
  LWIP_ASSERT("q->sem != NULL", q->sem != NULL);
  LWIP_ASSERT("q->sem != INVALID_HANDLE_VALUE", q->sem != INVALID_HANDLE_VALUE);

  if ((ret = WaitForSingleObject(q->sem, 0)) == WAIT_OBJECT_0) {
    SYS_ARCH_PROTECT(lev);
    if(msg != NULL) {
      *msg  = q->q_mem[q->tail];
    }

    (q->tail)++;
    if (q->tail >= MAX_QUEUE_ENTRIES) {
      q->tail = 0;
    }
    SYS_ARCH_UNPROTECT(lev);
    return 0;
  }
  else
  {
    LWIP_ASSERT("Error waiting for sem", ret == WAIT_TIMEOUT);
    if(msg != NULL) {
      *msg  = NULL;
    }

    return SYS_ARCH_TIMEOUT;
  }
  #endif 
   return SYS_ARCH_TIMEOUT;
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : sys_arch_mbox_fetch
//* 功能描述 : 在指定的邮箱接收消息，该函数会阻塞线程直至超时或者邮箱至少收到一条消息
//* 入口参数 :    <mbox>[in]  指定接收消息的邮箱
//*          :     <msg>[out] 结果参数，保存接收到的消息指针
//*          : <timeout>[in]  指定等待接收的最长时间，为0表明一直等待直至接收到消息，单位为毫秒
//* 出口参数 : -                0: 在指定时间内收到消息
//*          : - SYS_ARCH_TIMEOUT: 在指定时间内没有收到消息
//*------------------------------------------------------------------------------------------------
u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **msg, u32_t timeout)
{
	INT8U	__u8RtnVal;
	
	if(msg != NULL)
		*msg = OSQPend(mbox->hMBox, __MSToOSTicks(timeout), &__u8RtnVal);
	else 
		OSQPend(mbox->hMBox, __MSToOSTicks(timeout), &__u8RtnVal);

	if(__u8RtnVal == OS_NO_ERR)
		return 0;
	else
		return SYS_ARCH_TIMEOUT;
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : sys_thread_new
//* 功能描述 : 建立一个新线程
//* 入口参数 : <thread>[in] 新线程的入口地址
//*          :    <arg>[in] 传递给新线程的参数
//*          :   <prio>[in] 由LwIP指定的新线程优先级，这个优先级从1开始
//* 出口参数 : 返回线程优先级，注意这与prio不同。这个值实际等于T_LWIP_THREAD_START_PRIO + prio，
//*          : 如果建立不成功则返回0
//*------------------------------------------------------------------------------------------------
//sys_thread_t sys_thread_new(void(*thread)(void *arg), void *arg, int prio)
sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio)
{
	INT8U __u8Prio = 0;
	
	//* 如果优先级定义没有超出系统允许的范围
	if(prio > 0 && prio <= T_LWIP_THREAD_MAX_NB)
	{
		__u8Prio = T_LWIP_THREAD_START_PRIO + (prio - 1);
	
		if(OS_NO_ERR == OSTaskCreate(thread, arg, &T_LWIP_THREAD_STK[prio-1][T_LWIP_THREAD_STKSIZE-1], __u8Prio))
			return __u8Prio;
		else
            return 0;
	}
	else	
		return 0;
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : sys_arch_timeouts
//* 功能描述 : 获取当前线程使用的sys_timeouts结构的指针
//* 入口参数 : 无
//* 出口参数 : 返回一个指向当前线程使用的sys_timeouts结构的指针
//*------------------------------------------------------------------------------------------------
struct sys_timeouts *sys_arch_timeouts(void)
{
	INT8S __s8Idx;
	
	//* 减去起始量获得偏移量，也就是LwIP内部的优先级定义
	__s8Idx = OSTCBCur->OSTCBPrio - T_LWIP_THREAD_START_PRIO;
	
	//* 当前线程在指定的LwIP线程优先级号范围之内
	if(__s8Idx >= 0 && __s8Idx < T_LWIP_THREAD_MAX_NB)
		return &__staSysTimeouts[__s8Idx];
	
	//* 如果出现意外情况，当前线程并不在指定的LwIP线程优先级资源之内，则返回__staSysTimeouts数组的最后一个成员
	return &__staSysTimeouts[T_LWIP_THREAD_MAX_NB];
}
