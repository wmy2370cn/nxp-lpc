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
*                                       APPLICATION CONFIGURATION
*
*                                             NXP LPC2468
*                                                on the
*                                  Embedded Artists LPC2468 OEM Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

/*
*********************************************************************************************************
*                                       ADDITIONAL uC/MODULE ENABLES
*********************************************************************************************************
*/

#define  uC_CFG_OPTIMIZE_ASM_EN              DEF_ENABLED                /* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */
#define  uC_TCPIP_MODULE                     DEF_ENABLED

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_TASK_START_PRIO                   1

#define  APP_TASK_PHY_PRIO                     5
#define  NET_OS_CFG_IF_RX_TASK_PRIO            6
#define  NET_OS_CFG_TMR_TASK_PRIO              7
#define  OS_TASK_TMR_PRIO                      8

#define  NET_SVR_TASK_PRIO_BASE                20

#define  LISTEN_TASK_PRIO                      30

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE             256
#define  APP_TASK_PHY_STK_SIZE               128
#define  NET_OS_CFG_TMR_TASK_STK_SIZE       1024
#define  NET_OS_CFG_IF_RX_TASK_STK_SIZE      512
#define  LISTEN_TASK_STK_SIZE                256
#define  NET_SVR_TASK_STK_SIZE               512



/*
*********************************************************************************************************
*                                           uC/OS-View CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           DCC CONFIGURATION
*********************************************************************************************************
*/

#define  OS_CPU_ARM_DCC_EN                     0




#endif
