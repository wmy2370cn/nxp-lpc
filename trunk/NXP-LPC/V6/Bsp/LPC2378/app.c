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
static  OS_STK          AppTaskKbdStk[APP_TASK_KBD_STK_SIZE];

static  void           *AppKbdQStorage[16];
static  OS_EVENT       *AppKbdQ;

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

static  void            AppFormatDec                (CPU_INT08U *s, CPU_INT32U value, CPU_INT08U digits);
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

#if OS_VIEW_MODULE > 0
    OSView_Init(115200);                                        /* OSView Init, baud rate = 115200                          */
    OSView_TerminalRxSetCallback(AppTerminalRx);
    OSView_RxIntEn();                                           /* Enable Rx Interrupts                                     */
#endif

#if (OS_VIEW_MODULE == 0) || (OS_VIEW_COMM_SEL != SER_COMM_SEL)
    Ser_Init();
    AppUserIFState  = 0;
    AppUserIFMbox   = OSMboxCreate((void *)0);                  /* Create MBOX for communication between Kbd and Ser        */
#endif

#if uC_TCPIP_MODULE > 0
    AppInitTCPIP();                                             /* Initialize uC/TCP-IP and associated appliations          */
#endif

    LED_Off(0);                                                 /* Turn OFF all the LEDs                                    */

    AppKbdQ         = OSQCreate(AppKbdQStorage, 16);            /* Create Q for communication between Kbd and Ser           */
    AppTaskCreate();                                            /* Create application tasks                                 */


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.           */

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
            msg     = OSQAccept(AppKbdQ, &err);

            if (err == OS_Q_EMPTY || msg == (void *)0) {
                break;
            }

            led     = (CPU_INT32U)msg;

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

    AppNetIP        = NetASCII_Str_to_IP("10.10.1.129",  &err);
    AppNetMsk       = NetASCII_Str_to_IP("255.255.255.0", &err);
    AppNetGateway   = NetASCII_Str_to_IP("10.10.1.1",   &err);

    err             = NetIP_CfgAddrThisHost(AppNetIP, AppNetMsk);
    err             = NetIP_CfgAddrDfltGateway(AppNetGateway);
}
#endif

/*
*********************************************************************************************************
*                                         RS-232 OUTPUT TASK
*
* Description : This task outputs information through the RS-232 port.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskSer()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

#if (OS_VIEW_MODULE == 0) || (OS_VIEW_COMM_SEL != SER_COMM_SEL)
static  void  AppTaskSer (void *p_arg)
{
    CPU_INT08U     *msg;
    CPU_INT08U      err;
    CPU_INT32U      pstate;
    CPU_INT32U      nstate;


    (void)p_arg;

    Ser_WrStr("\n\n\r");
    AppDispScr_SignOn();
    OSTimeDlyHMSM(0, 0, 1, 0);
    Ser_WrStr("\n\n\r");
    nstate               = 1;


    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.           */
        pstate           = nstate;

        msg = (CPU_INT08U *)(OSMboxPend(AppUserIFMbox, OS_TICKS_PER_SEC / 10, &err));
        if (err == OS_NO_ERR) {
            if (msg != (CPU_INT08U *)0) {
                nstate = *msg;
            }
        }

        if (nstate != pstate) {
            Ser_WrStr("\n\n");
        }

        switch (nstate) {
            case 1:
                 AppDispScr_VersionTickRate();
                 break;

            case 2:
                 AppDispScr_CPU();
                 break;

            case 3:
                 AppDispScr_CtxSw();
                 break;

            case 4:
                 AppDispScr_Tcpip();
                 break;

            case 0:
            default:
                 AppDispScr_SignOn();
                 break;
        }
    }
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
    AppFormatDec(&AppSerStr[26], value, 4);
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

static  void  AppDispScr_CtxSw (void)
{
    CPU_INT32U    value;


    Str_Copy(AppSerStr, "#Ticks = xxxxxxxx; #CtxSw = xxxxxxxx \r");
    value   = (CPU_INT32U)OSTime;
    AppFormatDec(&AppSerStr[9], value, 8);

    value   = (CPU_INT32U)OSCtxSwCtr;
    AppFormatDec(&AppSerStr[28], value, 8);

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

    AppFormatDec(&AppSerStr[33],  NetNIC_StatTxPktCtr, 6);
    AppFormatDec(&AppSerStr[51],  NetNIC_StatTxPktCtr, 6);

    Ser_WrStr(AppSerStr);
}
#endif

/*
*********************************************************************************************************
*                                    KEYBOARD RESPONSE TASK
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskStart()
*
* Arguments   : p_arg   is the argument passed to 'AppStartKbd()' by 'OSTaskCreate()'.
*
* Returns     : none
*********************************************************************************************************
*/

static  void  AppTaskKbd (void *p_arg)
{
    CPU_BOOLEAN  b1;                                            /* State of Push Button #1                                  */
    CPU_BOOLEAN  b1_prev;
    CPU_BOOLEAN  b2;                                            /* State of Push Button #2                                  */
    CPU_BOOLEAN  b2_prev;
    CPU_BOOLEAN  b3;                                            /* State of Push Button #3                                  */
    CPU_BOOLEAN  b3_prev;
    CPU_BOOLEAN  b4;                                            /* State of Push Button #4                                  */
    CPU_BOOLEAN  b4_prev;
#if (OS_VIEW_MODULE == 0) || (OS_VIEW_COMM_SEL != SER_COMM_SEL)
    CPU_BOOLEAN  b5;                                            /* State of Push Button #5                                  */
    CPU_BOOLEAN  b5_prev;
    CPU_INT08U   key;
#endif

    (void)p_arg;

    b1_prev     = DEF_FALSE;
    b2_prev     = DEF_FALSE;
    b3_prev     = DEF_FALSE;
    b4_prev     = DEF_FALSE;
#if (OS_VIEW_MODULE == 0) || (OS_VIEW_COMM_SEL != SER_COMM_SEL)
    b5_prev     = DEF_FALSE;
    key         = 1;
#endif

    while (DEF_TRUE) {

        b1      = PB_GetStatus(1);
        b2      = PB_GetStatus(2);
        b3      = PB_GetStatus(3);
        b4      = PB_GetStatus(4);
#if (OS_VIEW_MODULE == 0) || (OS_VIEW_COMM_SEL != SER_COMM_SEL)
        b5      = PB_GetStatus(5);
#endif

        if (b1 == DEF_TRUE && b1_prev == DEF_FALSE) {

            OSQPost(AppKbdQ, (CPU_INT32U *)1);
        }

        if (b2 == DEF_TRUE && b2_prev == DEF_FALSE) {

            OSQPost(AppKbdQ, (CPU_INT32U *)2);
        }

        if (b3 == DEF_TRUE && b3_prev == DEF_FALSE) {

            OSQPost(AppKbdQ, (CPU_INT32U *)3);
        }

        if (b4 == DEF_TRUE && b4_prev == DEF_FALSE) {

            OSQPost(AppKbdQ, (CPU_INT32U *)4);
        }

#if (OS_VIEW_MODULE == 0) || (OS_VIEW_COMM_SEL != SER_COMM_SEL)
        if (b5 == DEF_TRUE && b5_prev == DEF_FALSE) {
            if (key == 4) {
                key = 0;
            } else {
                key++;
            }
            AppUserIFState  = key;

            OSMboxPost(AppUserIFMbox, &AppUserIFState);
        }
#endif

        b1_prev = b1;
        b2_prev = b2;
        b3_prev = b3;
        b4_prev = b4;
#if (OS_VIEW_MODULE == 0) || (OS_VIEW_COMM_SEL != SER_COMM_SEL)
        b5_prev = b5;
#endif

        OSTimeDly(OS_TICKS_PER_SEC / 20);

    }
}

/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*
* Description:  This function creates the application tasks.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
    CPU_INT08U      err;


    OSTaskCreateExt(AppTaskKbd,
                    (void *)0,
                    (OS_STK *)&AppTaskKbdStk[APP_TASK_KBD_STK_SIZE - 1],
                    APP_TASK_KBD_PRIO,
                    APP_TASK_KBD_PRIO,
                    (OS_STK *)&AppTaskKbdStk[0],
                    APP_TASK_KBD_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

#if (OS_TASK_NAME_SIZE > 9)
    OSTaskNameSet(APP_TASK_KBD_PRIO, "Keyboard", &err);
#endif

}

/*
*********************************************************************************************************
*                                          uC/OS-VIew Terminal Window Callback
*
* Description : This is the callback function for uC/OS-View
*
* Arguments   : rx_data   is the received data.
*
* Returns     : none
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      FORMAT A DECIMAL VALUE
*
* Description: This function converts a decimal value to ASCII (with leading zeros)
*
* Arguments  : s       is a pointer to the destination ASCII string
*              value   is the value to convert (assumes an unsigned value)
*              digits  is the desired number of digits
*
* Returns    : none
*********************************************************************************************************
*/

#if (OS_VIEW_MODULE == 0) || (OS_VIEW_COMM_SEL != SER_COMM_SEL)
static  void  AppFormatDec (CPU_INT08U *s, CPU_INT32U value, CPU_INT08U digits)
{
    CPU_INT08U      i;
    CPU_INT32U      mult;


    mult        = 1;
    for (i = 0; i < (digits - 1); i++) {
        mult   *= 10;
    }
    while (mult > 0) {
        *s++    = value / mult + '0';
        value  %= mult;
        mult   /= 10;
    }
}
#endif

