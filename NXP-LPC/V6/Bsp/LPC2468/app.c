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
*                                             EXAMPLE CODE
*
*                                             NXP LPC2468
*                                                on the
*                                  Embedded Artists LPC2468 OEM Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/

#include <includes.h>
#include "BsdSocketTest.h"

#if uC_TCPIP_MODULE > 0
#include <net_phy.h>
#include <net_phy_def.h>
#endif

/*
*********************************************************************************************************
*                                           #DEFINE CONSTANTS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            LOCAL VARIABLES
*********************************************************************************************************
*/

static  OS_STK          AppTaskStartStk[APP_TASK_START_STK_SIZE];

#if uC_TCPIP_MODULE > 0
        NET_IP_ADDR     AppNetIP;
        NET_IP_ADDR     AppNetMsk;
        NET_IP_ADDR     AppNetGateway;
#endif

/*
*********************************************************************************************************
*                                         LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void            AppTaskStart                (void *p_arg); 

#if uC_TCPIP_MODULE > 0
static  void            AppInitTCPIP                (void);
#endif


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary 68HC12 and C initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    CPU_INT08U  err;


    BSP_IntDisAll();                                            /* Disable all interrupts until we are ready to accept them */

    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"              */

    OSTaskCreateExt(AppTaskStart,                               /* Create the start task                                    */
                    (void *)0,
                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
                    APP_TASK_START_PRIO,
                    APP_TASK_START_PRIO,
                    (OS_STK *)&AppTaskStartStk[0],
                    APP_TASK_START_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if OS_TASK_NAME_SIZE > 13
    OSTaskNameSet(APP_TASK_START_PRIO, "Start Task", &err);
#endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)       */
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    void           *msg;
    CPU_INT08U      err;
    

    (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                                 */

#if OS_TASK_STAT_EN > 0
    OSStatInit();                                               /* Determine CPU capacity                                   */
#endif
  
#if uC_TCPIP_MODULE > 0
    AppInitTCPIP();                                             /* Initialize uC/TCP-IP and associated appliations          */
#endif

	InitNetSvr(  );

  
    
	while (DEF_TRUE)
	{                         
	//	OSTimeDlyHMSM(0, 0, 1, 0);
		OSTimeDlyHMSM(0,0,1,0);
		SetLed(1,1);
		OSTimeDlyHMSM(0,0,1,0);
		SetLed(1,0);
	}
}

/*
*********************************************************************************************************
*                                      AppInitTCPIP()
*
* Description : This function is called by AppTaskStart() and is responsible for initializing uC/TCP-IP
*               uC/HTTPs, uC/TFTPs and uC/DHCPc if enabled.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

#if uC_TCPIP_MODULE > 0
static  void  AppInitTCPIP (void)
{
    NET_ERR  err;


#if EMAC_CFG_MAC_ADDR_SEL == EMAC_CFG_MAC_ADDR_SEL_CFG
    NetIF_MAC_Addr[0] = 0x00;
    NetIF_MAC_Addr[1] = 0x50;
    NetIF_MAC_Addr[2] = 0xC2;
    NetIF_MAC_Addr[3] = 0x25;
    NetIF_MAC_Addr[4] = 0x61;
    NetIF_MAC_Addr[5] = 0x39;
#endif

    err             = Net_Init();                               /* Initialize uC/TCP-IP                                     */

    AppNetIP        = NetASCII_Str_to_IP("192.9.200.199",  &err);
    AppNetMsk       = NetASCII_Str_to_IP("255.255.255.0", &err);
    AppNetGateway   = NetASCII_Str_to_IP("192.9.200.1",   &err);

    err             = NetIP_CfgAddrThisHost(AppNetIP, AppNetMsk);
    err             = NetIP_CfgAddrDfltGateway(AppNetGateway);
}
#endif 

  
