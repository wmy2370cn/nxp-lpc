//#include <rtthread.h>

#include "lwip/debug.h"
#include "lwip/sys.h"
#include "lwip/opt.h"
#include "lwip/stats.h"
#include "arch/sys_arch.h"

#include <string.h>

static char g_QueueMemoryPool[MAX_QUEUES * sizeof(TQ_DESCR) ];
static OS_MEM *g_pQueueMem = NULL;

struct sys_timeouts lwip_timeouts[LWIP_TASK_MAX+1];
struct sys_timeouts null_timeouts;

#define T_LWIP_START_PRIO  		7
#define T_LWIP_STKSIZE      512
static u8_t curr_prio_offset;
#pragma arm section rwdata = "SRAM",zidata = "SRAM"
OS_STK T_LWIP_TASK_STK[LWIP_TASK_MAX+1][T_LWIP_STKSIZE];
#pragma arm section 


void sys_init(void)
{
	INT8U   ucErr = 0;
	INT8U   i = 0;

	g_pQueueMem = OSMemCreate( (void*)g_QueueMemoryPool, MAX_QUEUES, sizeof(TQ_DESCR), &ucErr );
	
	for(i=0;i<LWIP_TASK_MAX;i++)
	{
		lwip_timeouts[i].next = NULL;
	}
	return;
}

sys_sem_t sys_sem_new(u8_t count)
{
	sys_sem_t pSem;
	pSem = OSSemCreate((u16_t)count );
	return pSem; 
}

void sys_sem_free(sys_sem_t sem)
{ 
	u8_t     ucErr;
	(void)OSSemDel((OS_EVENT *)sem, OS_DEL_NO_PEND, &ucErr );
	return;
}

void sys_sem_signal(sys_sem_t sem)
{ 
	u8_t     ucErr;
	ucErr = OSSemPost((OS_EVENT *)sem );
	ucErr=ucErr;
	return;
}

u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout)
{
	u32_t ret;
	s32_t t;
 

	return ret;
}

sys_mbox_t sys_mbox_new(int size)
{
	 

	return 0;
}

void sys_mbox_free(sys_mbox_t mbox)
{
 

	return;
}

void sys_mbox_post(sys_mbox_t mbox, void *msg)
{
 

	return;
}

err_t sys_mbox_trypost(sys_mbox_t mbox, void *msg)
{
 

	return ERR_MEM;
}

u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **msg, u32_t timeout)
{
	 

 
	return 0;
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t mbox, void **msg)
{
	int ret;

 

	return ret;
}

struct sys_timeouts *sys_arch_timeouts(void)
{
	u8_t curr_prio;
	s16_t offset;

	OS_TCB curr_task_pcb;

	null_timeouts.next = NULL;
	//get current task prio
	(void)OSTaskQuery(OS_PRIO_SELF,&curr_task_pcb);  

	curr_prio = curr_task_pcb.OSTCBPrio;

	offset = curr_prio - T_LWIP_START_PRIO;
	//not called by a lwip task ,return timeouts->NULL
	if(offset < 0 || offset >= LWIP_TASK_MAX)
	{
		return &null_timeouts;
	}

	return &lwip_timeouts[offset];
}

sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio)
{
	if(curr_prio_offset < LWIP_TASK_MAX)
	{

		OSTaskCreate(thread, (void *)arg, &T_LWIP_TASK_STK[curr_prio_offset][T_LWIP_STKSIZE-1],T_LWIP_START_PRIO+curr_prio_offset );
		curr_prio_offset++; 
	}
	else
	{
		// PRINT(" lwip task prio out of range ! error! ");
	}
	return T_LWIP_START_PRIO+curr_prio_offset;
}

sys_prot_t sys_arch_protect(void)
{
	/* disable interrupt */
	return CPU_SR_Save();
}

void sys_arch_unprotect(sys_prot_t pval)
{
	/* enable interrupt */
	CPU_SR_Restore(pval);
	return;
}
