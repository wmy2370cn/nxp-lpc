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

#if (OS_VIEW_MODULE == 0) || (OS_VIEW_COMM_SEL != SER_COMM_SEL)
static  OS_STK          AppTaskSerStk[APP_TASK_SER_STK_SIZE];
static  CPU_CHAR        AppSerStr[80];

static  CPU_INT08U      AppUserIFState;                         /* User I/F state                                           */
static  OS_EVENT       *AppUserIFMbox;
#endif

/*
*********************************************************************************************************
*                                         LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void            AppTaskCreate               (void);
static  void            AppTaskStart                (void *p_arg);
static  void            AppTaskKbd                  (void *p_arg);

#if uC_TCPIP_MODULE > 0
static  void            AppInitTCPIP                (void);
#endif

#if OS_VIEW_MODULE > 0
static  void            AppTerminalRx               (CPU_INT08U rx_data);
#endif

#if (OS_VIEW_MODULE == 0) || (OS_VIEW_COMM_SEL != SER_COMM_SEL)
static  void            AppTaskSer                  (void *p_arg);
static  void            AppTaskKbd                  (void *p_arg);

static  void            AppDispScr_SignOn           (void);
static  void            AppDispScr_VersionTickRate  (void);
static  void            AppDispScr_CPU              (void);
static  void            AppDispScr_CtxSw            (void);
static  void            AppDispScr_Tcpip            (void);
																										   
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
    CPU_INT08U      i;
    CPU_INT08U      j;
    void           *msg;
    CPU_INT08U      err;
    CPU_INT08U      led;
    CPU_INT32U      leds;
    CPU_INT32U      delay0;
    CPU_INT32U      delay1;


    (void)p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                                 */

#if OS_TASK_STAT_EN > 0
    OSStatInit();                                               /* Determine CPU capacity                                   */
#endif
  
#if uC_TCPIP_MODULE > 0
    AppInitTCPIP();                                             /* Initialize uC/TCP-IP and associated appliations          */
#endif

    LED_Off(0);                                                 /* Turn OFF all the LEDs                                    */

    
    while (DEF_TRUE)
	{                                          /* Task body, always written as an infinite loop.           */

        for (i = 1; i <= 4; i++) {                              /* Turn on each LED, one by one, delaying between           */
            LED_On(i);
            delay0  = ADC_GetStatus(0) + 32;
            OSTimeDlyHMSM(0, 0, delay0 / 1000, delay0 % 1000);
        }

        LED_Off(0);                                             /* Turn off all LEDs                                        */
        delay0      = ADC_GetStatus(0) + 32;
        OSTimeDlyHMSM(0, 0, delay0 / 1000, delay0 % 1000);      /* Delay                                                    */

                                                                /* Determine what buttons have been pressed                 */
        leds        = 0;
        while (1) {
       
            if (led > 4) {
                break;
            }

            leds   |= (1 << led);
        }

                                                                /* If no buttons have been pressed, continue at beginning   */
        if (leds == 0) {
            continue;
        }

                                                                /* Otherwise, blink LEDs that were pressed 20 times         */
        for (j = 0; j < 20; j++) {
            for (i = 1; i <= 4; i++) {
                if ((leds & (1 << i)) == (1 << i)) {
                    LED_Toggle(i);
                }
            }
            delay1  = (ADC_GetStatus(1) >> 2) + 32;
            OSTimeDlyHMSM(0, 0, delay1 / 1000, delay1 % 1000);
        }
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

    AppNetIP        = NetASCII_Str_to_IP("192.9.200.128",  &err);
    AppNetMsk       = NetASCII_Str_to_IP("255.255.255.0", &err);
    AppNetGateway   = NetASCII_Str_to_IP("192.9.200.128",   &err);

    err             = NetIP_CfgAddrThisHost(AppNetIP, AppNetMsk);
    err             = NetIP_CfgAddrDfltGateway(AppNetGateway);
}
#endif
/*
*********************************************************************************************************
*                                            DISPLAY SCREENS
*
* Descrition:  These functions each display one of the screens used in the demonstration.
*
* Arguments :  none
*
* Returns   :  none
*********************************************************************************************************
*/

#if (OS_VIEW_MODULE == 0) || (OS_VIEW_COMM_SEL != SER_COMM_SEL)
static  void  AppDispScr_SignOn (void)
{
    Ser_WrStr("Micrium uC/OS-II on the NXP LPC2468\r");
}

static  void  AppDispScr_VersionTickRate (void)
{
    CPU_INT32U    value;


    Str_Copy(AppSerStr, "uC/OS-II Vx.yy running at xxxx ticks/sec \r");
    value           = (CPU_INT32U)OSVersion();
    AppSerStr[10]   =  value / 100       + '0';
    AppSerStr[12]   = (value % 100) / 10 + '0';
    AppSerStr[13]   = (value %  10)      + '0';

    value           = (CPU_INT32U)OS_TICKS_PER_SEC;
//    AppFormatDec(&AppSerStr[26], value, 4);
    Ser_WrStr(AppSerStr);
}

static  void  AppDispScr_CPU (void)
{
    CPU_INT32U    value;


    Str_Copy(AppSerStr, "CPU Usage = xxx% at CPU Speed = xx MHz  \r");
    value           = (CPU_INT32U)OSCPUUsage;
    AppSerStr[12]   = (value / 100)      + '0';
    AppSerStr[13]   = (value /  10) % 10 + '0';
    AppSerStr[14]   = (value      ) % 10 + '0';
    value           = (CPU_INT32U)BSP_CPU_ClkFreq() / 1000000L;
    AppSerStr[32]   = (value / 10) + '0';
    AppSerStr[33]   = (value % 10) + '0';
    Ser_WrStr(AppSerStr);
}

static  void  AppDispScr_Tcpip (void)
{
    CPU_INT32U  link_duplex;
    CPU_INT32U  link_speed;


    if (NetNIC_ConnStatusGet() == DEF_OFF) {
        Str_Copy(AppSerStr, "NO LINK DETECTED     : RX Pkts = xxxxxx; TX Pkts = xxxxxx \r");
    } else {
        Str_Copy(AppSerStr, "100 Mbps, Full Duplex: RX Pkts = xxxxxx; TX Pkts = xxxxxx \r");

        link_duplex = NetNIC_PhyLinkDuplex();
        link_speed  = NetNIC_PhyLinkSpeed();

        if (link_speed == NET_PHY_SPD_10) {
            AppSerStr[2]    = ' ';
        }

        if (link_duplex == NET_PHY_DUPLEX_HALF) {
            AppSerStr[10]   = 'H';
            AppSerStr[11]   = 'a';
            AppSerStr[13]   = 'f';
        }
    }

  //  AppFormatDec(&AppSerStr[33],  NetNIC_StatTxPktCtr, 6);
  //  AppFormatDec(&AppSerStr[51],  NetNIC_StatTxPktCtr, 6);

    Ser_WrStr(AppSerStr);
}
#endif 
  
