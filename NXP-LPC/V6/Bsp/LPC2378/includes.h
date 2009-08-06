/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                             NXP LPC2468
*                                                on the
*                                  Embedded Artists LPC2468 OEM Board
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/

#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <stdarg.h>



#include <OS_CPU.h>	
#include <OS_CFG.h>
#include <UCOS_II.h>

#include    <cpu.h> 

#include    <lib_def.h>
#include    <lib_mem.h>
#include    <lib_str.h>

#include    <app_cfg.h>
#include    <bsp.h>

#if OS_VIEW_MODULE > 0
#include    <os_viewc.h>
#include    <os_view.h>
#endif

#if uC_TCPIP_MODULE > 0
#include    <net.h>
#endif




#endif
