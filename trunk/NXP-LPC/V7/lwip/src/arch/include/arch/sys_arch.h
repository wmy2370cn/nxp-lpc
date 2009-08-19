/*
 * Copyright (c) 2001, Swedish Institute of Computer Science.
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 3. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: sys_arch.h,v 1.3 2005/03/13 16:03:23 bear Exp $
 */
#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__

#include "arch/cpu.h"
#include "arch/cc.h"

#include "ucos_ii.h" 

#define RT_NULL 				((void *)0)
#define RT_ASSERT(EX)


#define LWIP_TASK_MAX    		1

#define	SYS_MBOX_NULL				((void *)0)
#define	SYS_SEM_NULL				(void *)0

typedef u32_t sys_prot_t;

#define SYS_MBOX_SIZE 16
#define MAX_QUEUE_ENTRIES 20
#define MAX_QUEUES        20
//#define SYS_LWIP_TIMER_NAME "timer"
//#define SYS_LWIP_MBOX_NAME "mbox"
//#define SYS_LWIP_SEM_NAME "sem"

typedef struct {
    OS_EVENT*   pQ;
    void*       pQEntries[MAX_QUEUE_ENTRIES];
} TQ_DESCR, *PQ_DESCR;


typedef OS_EVENT* sys_sem_t; 
typedef PQ_DESCR  sys_mbox_t;
typedef INT8U     sys_thread_t;

#define RT_ASSERT(EX)
#define RT_WAITING_FOREVER	-1		/* Block forever until get resource. */
#define RT_WAITING_NO		0		/* Non-block. */

#endif /* __ARCH_SYS_ARCH_H__ */
