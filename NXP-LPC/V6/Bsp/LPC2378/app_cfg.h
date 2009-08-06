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
#define  APP_TASK_KBD_PRIO                     2
#define  OS_VIEW_TASK_PRIO                     3
#define  OS_VIEW_TASK_ID                       3
#define  APP_TASK_SER_PRIO                     4
#define  APP_TASK_PHY_PRIO                     5
#define  NET_OS_CFG_IF_RX_TASK_PRIO            6
#define  NET_OS_CFG_TMR_TASK_PRIO              7
#define  OS_TASK_TMR_PRIO                      8

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE             128
#define  APP_TASK_KBD_STK_SIZE               128 
#define  APP_TASK_SER_STK_SIZE               256
#define  APP_TASK_PHY_STK_SIZE               128
#define  NET_OS_CFG_TMR_TASK_STK_SIZE       1024
#define  NET_OS_CFG_IF_RX_TASK_STK_SIZE      512


/*
*********************************************************************************************************
*                                           uC/OS-View CONSTANTS
*********************************************************************************************************
*/

#define  OS_VIEW_PARSE_TASK                    0                /* Parsing of received packets will be done by a task       */
#define  OS_VIEW_TMR_32_BITS                   1                /* uC/OS-View timer is 32 bits                              */

#define  OS_VIEW_UART_0                        0                /* UART0 selected                                           */
#define  OS_VIEW_UART_1                        1                /* UART1 selected                                           */

#define  OS_VIEW_COMM_SEL         OS_VIEW_UART_0                /* Select UART0 for uC/OS-View to use                       */

/*
*********************************************************************************************************
*                                           RS-232 CONSTANTS
*********************************************************************************************************
*/

#define  SER_UART_0                            0                /* UART0 selected                                           */
#define  SER_UART_1                            1                /* UART1 selected                                           */
#define  SER_COMM_SEL                 SER_UART_1                /* Select UART1 for printing                                */

/*
*********************************************************************************************************
*                                           DCC CONFIGURATION
*********************************************************************************************************
*/

#define  OS_CPU_ARM_DCC_EN                     0




#endif
