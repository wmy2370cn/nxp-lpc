/*
*********************************************************************************************************
*                                     MICRIUM BOARD SUPPORT SUPPORT
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
*                                        BOARD SUPPORT PACKAGE
*
*                                             NXP LPC2468
*                                                on the
*                                  Embedded Artists LPC2468 OEM Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/

#define  BSP_GLOBALS
#include <includes.h> 
#include <lpc24XX.h>

/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/

typedef  void (*BSP_FNCT_PTR)(void);                            /* Pointer to ISR handler function                          */

/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

        CPU_INT32U  VIC_SpuriousInt;

/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/
static void CPU_PIN_Init(void);
static  void  PLL_Init          (void);
static  void  MAM_Init          (void);
static  void  GPIO_Init         (void);
static  void  VIC_Init          (void);
static  void  ADC_Init          (void);

static  void  DCC_Init          (void);

static  void  Tmr_TickInit      (void);

static  void  VIC_Dummy         (void);                         /* Prototypes for dummy interrupt handlers                  */
static  void  VIC_DummyWDT      (void);
static  void  VIC_DummySW       (void);
static  void  VIC_DummyDEBUGRX  (void);
static  void  VIC_DummyDEBUGTX  (void);
static  void  VIC_DummyTIMER0   (void);
static  void  VIC_DummyTIMER1   (void);
static  void  VIC_DummyUART0    (void);
static  void  VIC_DummyUART1    (void);
static  void  VIC_DummyPWM01    (void);
static  void  VIC_DummyI2C0     (void);
static  void  VIC_DummySPI      (void);
static  void  VIC_DummySSP1     (void);
static  void  VIC_DummyPLL      (void);
static  void  VIC_DummyRTC      (void);
static  void  VIC_DummyEINT0    (void);
static  void  VIC_DummyEINT1    (void);
static  void  VIC_DummyEINT2    (void);
static  void  VIC_DummyEINT3    (void);
static  void  VIC_DummyAD0      (void);
static  void  VIC_DummyI2C1     (void);
static  void  VIC_DummyBOD      (void);
static  void  VIC_DummyETHERNET (void);
static  void  VIC_DummyUSB      (void);
static  void  VIC_DummyCAN01    (void);
static  void  VIC_DummyMMC      (void);
static  void  VIC_DummyGP_DMA   (void);
static  void  VIC_DummyTIMER2   (void);
static  void  VIC_DummyTIMER3   (void);
static  void  VIC_DummyUART2    (void);
static  void  VIC_DummyUART3    (void);
static  void  VIC_DummyI2C2     (void);
static  void  VIC_DummyI2S      (void);

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                         Global Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         BSP INITIALIZATION
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguements  : None
*
* Returns     : None
*********************************************************************************************************
*/

void  BSP_Init (void)
{
	CPU_PIN_Init();
 //   PLL_Init();                                                 /* Initialize the PLL                                       */
 //   MAM_Init();                                                 /* Initialize the Memory Acceleration Module                */
 //   GPIO_Init();                                                /* Initialize the board's I/Os                              */
 //   ADC_Init();                                                 /* Initialize the board's ADCs                              */
      VIC_Init();                                                 /* Initialize the Vectored Interrupt Controller             */
 //   I2C_Init();                                                 /* Initialize the I2C                                       */

    Tmr_TickInit();                                             /* Initialize the uC/OS-II tick interrupt                   */
}


/*
*********************************************************************************************************
*                                      Get the CPU Clock Frequency
*
* Description : This function reads CPU registers to determine the CPU clock frequency
*
* Arguements  : None
*
* Returns     : The CPU Core clock in Hz
*
* Notes       : None
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    CPU_INT32U  msel;
    CPU_INT32U  nsel;
    CPU_INT32U  fin;
    CPU_INT32U  pll_clk_feq;                                    /* When the PLL is enabled, this is Fcco                    */
    CPU_INT32U  clk_div;
    CPU_INT32U  clk_freq;


    switch (CLKSRCSEL & 0x03) {                                 /* Determine the current clock source                       */
        case 0:
             fin        =  IRC_OSC_FRQ;
             break;

        case 1:
             fin        =  MAIN_OSC_FRQ;
             break;

        case 2:
             fin        =  RTC_OSC_FRQ;
             break;

        default:
             fin        =  IRC_OSC_FRQ;
             break;
    }

    if ((PLLSTAT & (1 << 25)) > 0) {                                /* If the PLL is currently enabled and connected        */
        msel        = (CPU_INT32U)(PLLSTAT & 0x3FFF) + 1;           /* Obtain the PLL multiplier                            */
        nsel        = (CPU_INT32U)((PLLSTAT >>   16) & 0x0F) + 1;   /* Obtain the PLL divider                               */
        pll_clk_feq = (2 * msel * fin / nsel);                      /* Compute the PLL output frequency                     */
    } else {
        pll_clk_feq = (fin);                                        /* The PLL is bypassed                                  */
    }

    clk_div         = (CPU_INT08U)(CCLKCFG & 0x0F) + 1;             /* Obtain the CPU core clock divider                    */
    clk_freq        = (CPU_INT32U)(pll_clk_feq / clk_div);          /* Compute the ARM Core clock frequency                 */

    return (clk_freq);
}

/*
*********************************************************************************************************
*                                      Get a Peripheral Clock Frequency
*
* Description : This function reads CPU registers to determine the the clock frequency for the specified
*               peripheral
*
* Arguements  : An ID, one of PCLK_??? defined in bsp.c
*
* Returns     : The peripheral's clock in Hz
*********************************************************************************************************
*/


CPU_INT32U  BSP_CPU_PclkFreq (CPU_INT08U  pclk)
{
    CPU_INT32U  clk_freq;
    CPU_INT32U  selection;


    clk_freq    = BSP_CPU_ClkFreq();

    switch (pclk) {
        case PCLK_WDT:
        case PCLK_TIMER0:
        case PCLK_TIMER1:
        case PCLK_UART0:
        case PCLK_UART1:
        case PCLK_PWM0:
        case PCLK_PWM1:
        case PCLK_I2C0:
        case PCLK_SPI:
        case PCLK_RTC:
        case PCLK_SSP1:
        case PCLK_DAC:
        case PCLK_ADC:
        case PCLK_CAN1:
        case PCLK_CAN2:
        case PCLK_ACF:
             selection = ((PCLKSEL0 >> (pclk * 2)) & 0x03);
             if (selection == 0) {
                 return (clk_freq / 4);
             } else if (selection == 1) {
                 return (clk_freq);
             } else if (selection == 2) {
                 return (clk_freq / 2);
             } else {
                 return (clk_freq / 8);
             }

        case PCLK_BAT_RAM:
        case PCLK_GPIO:
        case PCLK_PCB:
        case PCLK_I2C1:
        case PCLK_SSP0:
        case PCLK_TIMER2:
        case PCLK_TIMER3:
        case PCLK_UART2:
        case PCLK_UART3:
        case PCLK_I2C2:
        case PCLK_MCI:
        case PCLK_SYSCON:
             selection = ((PCLKSEL1 >> ((pclk - 16) * 2)) & 0x03);
             if (selection == 0) {
                 return (clk_freq / 4);
             } else if (selection == 1) {
                 return (clk_freq);
             } else if (selection == 2) {
                 return (clk_freq / 2);
             } else {
                 return (clk_freq / 8);
             }

        default:
             return (0);
    }
}

/*
*********************************************************************************************************
*                                     DISABLE ALL INTERRUPTS
*
* Description : This function disables all interrupts from the interrupt controller.
*
* Arguments   : none
*
* Returns     : None
*********************************************************************************************************
*/

void  BSP_IntDisAll (void)
{
    VICIntEnClr = 0xFFFFFFFFL;                                        /* Disable ALL interrupts                                   */
}

/*
*********************************************************************************************************
*                                          EXCEPTION HANDLER
*
* Description : This function should be used to handle any exceptions.  It is called by
*               OS_CPU_ARM_EXCEPT_HANDLER(), which is declared in os_cpu_a.s
*
* Arguments   : ID, an identifier used to indicate what type of ARM exception has been triggered
*               Possible ID values are shown below.
*                  OS_CPU_ARM_EXCEPT_RESET             0x00
*                  OS_CPU_ARM_EXCEPT_UNDEF_INSTR       0x01
*                  OS_CPU_ARM_EXCEPT_SWI               0x02
*                  OS_CPU_ARM_EXCEPT_PREFETCH_ABORT    0x03
*                  OS_CPU_ARM_EXCEPT_DATA_ABORT        0x04
*                  OS_CPU_ARM_EXCEPT_ADDR_ABORT        0x05
*                  OS_CPU_ARM_EXCEPT_IRQ               0x06
*                  OS_CPU_ARM_EXCEPT_FIQ               0x07
*********************************************************************************************************
*/

void  OS_CPU_ExceptHndlr (CPU_DATA ID)
{
    BSP_FNCT_PTR  pfnct;

                                                                        /* If this exception is either an IRQ or FIQ                */
    if ((ID == OS_CPU_ARM_EXCEPT_IRQ) || (ID == OS_CPU_ARM_EXCEPT_FIQ)) {
        pfnct = (BSP_FNCT_PTR)VICVectAddr;                               /* Read the interrupt vector from the VIC                   */
        if (pfnct != (BSP_FNCT_PTR)0) {                                 /* Make sure we don't have a NULL pointer                   */
          (*pfnct)();                                                   /* Execute the ISR for the interrupting device              */
            VICVectAddr  =  1;                                           /* Acknowlege the VIC interrupt                             */
        }
    }
}


/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                       PB, LED, and ADC Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         GET 'PUSH BUTTON' STATUS
*
* Description : This function is used to get the status of any push button on the board.
*
* Arguments   : pb           is the number of the push button to probe
*                              1    probe the push button B1
*
* Returns     : DEF_TRUE     if the push button is     pressed
*               DEF_FALSE    if the push button is not pressed
*********************************************************************************************************
*/

CPU_BOOLEAN  PB_GetStatus (CPU_INT08U pb)
{
    CPU_INT08U   reg;
    CPU_INT08U   data;
    CPU_BOOLEAN  status;


    status      = DEF_FALSE;

    if (pb >= 1 && pb <= 4) {
        reg     = 0x00;
        data    = 0xFF;
        I2C_ReadReg(&data, 1, reg);
    }


    switch (pb) {
        case 1:
             if ((data & 0x01) == 0) {
                 status = DEF_TRUE;
             }
             break;

        case 2:
             if ((data & 0x02) == 0) {
                 status = DEF_TRUE;
             }
             break;

        case 3:
             if ((data & 0x04) == 0) {
                 status = DEF_TRUE;
             }
             break;

        case 4:
             if ((data & 0x08) == 0) {
                 status = DEF_TRUE;
             }
             break;

        case 5:
             if ((FIO2PIN & DEF_BIT_10) == 0) {
                 status = DEF_TRUE;
             }
             break;

        default:
             break;
    }

    return (status);
}


/*
*********************************************************************************************************
*                                         ADC INITIALIZATION
*
* Description : This function initializes the board's ADC
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

static  void  ADC_Init (void)
{
    CPU_INT08U  div;
    CPU_INT32U  pinsel;


    PCONP      |=  DEF_BIT_12;

    div         = (BSP_CPU_PclkFreq(PCLK_ADC) / 4500000) + 1;
                                                                /* Configure ADC ...                                        */
    AD0CR       = (0x03 <<  0)                                  /*  ... Sample/convert pin AD0.0 and AD0.1 ...              */
                | (div  <<  8)                                  /*  ... Set divider so sample freq. < 4.5 MHz ...           */
                |  DEF_BIT_16                                   /*  ... Use burst mode for continuous conversion ...        */
                | (0x00 << 17)                                  /*  ... Use 11 clocks per conversion for 10-bit accuracy ...*/
                |  DEF_BIT_21;                                  /*  ... Power up the ADC.                                   */

                                                                /* Select AD0.0, AD0.1 function for P0.23, P0.24            */
    pinsel      = PINSEL1;
    pinsel     &= 0xFFFC3FFF;
    pinsel     |= 0x00014000;
    PINSEL1     = pinsel;
}


/*
*********************************************************************************************************
*                                           ADC STATUS
*
* Description : This function initializes the board's ADC
*
* Arguments   : adc     is the number of the ADC to probe.
*                       0  probes AD0.0, the right-hand potentiometer
*                       1  probes AD0.1, the left-hand  potentiometer
*
* Returns     : The numerator of the binary fraction representing the result of the latest ADC conversion.
*               This value will be a 10-bit value between 0x0000 and 0x03FF, inclusive.
*********************************************************************************************************
*/

CPU_INT16U  ADC_GetStatus (CPU_INT08U  adc)
{
	return (0);
//    if (adc == 0) {
//        return ((ADDR0 >> 6) & 0x03FF);
//    } else if (adc == 1) {
//        return ((ADDR1 >> 6) & 0x03FF);
//    } else {
//       return (0);
//    }
}

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                            LCD Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          LCD BACKLIGHT ON
*
* Description : This function turns on the LCD backlight.
*
* Arguments   : None
*
* Returns     : None
*********************************************************************************************************
*/

void  LCD_LightOn (void)
{
 //   FIO3SET     = GPIO3_LCD_LIGHT;
}

/*
*********************************************************************************************************
*                                          LCD BACKLIGHT OFF
*
* Description : This function turns off the LCD backlight.
*
* Arguments   : None
*
* Returns     : None
*********************************************************************************************************
*/

void  LCD_LightOff (void)
{
//    FIO3CLR     = GPIO3_LCD_LIGHT;
}

/*
*********************************************************************************************************
*                                          LCD BACKLIGHT TOGGLE
*
* Description : This function toggles the LCD backlight.
*
* Arguments   : None
*
* Returns     : None
*********************************************************************************************************
*/

void  LCD_LightToggle (void)
{
 /*   CPU_INT32U  pin;


    pin         = FIO3PIN & GPIO3_LCD_LIGHT;

    if (pin == GPIO3_LCD_LIGHT) {

        FIO3CLR = GPIO3_LCD_LIGHT;

    } else {

        FIO3SET = GPIO3_LCD_LIGHT;
    }  */
}

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                          OS-View Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                     INITIALIZE TIMER FOR uC/OS-View
*
* Description : This function is called to by uC/OS-View to initialize the free running timer that is
*               used to make time measurements.
*
* Arguments   : none
*
* Returns     ; none
*********************************************************************************************************
*/

#if OS_VIEW_MODULE > 0
void  OSView_TmrInit (void)
{
    T1PR  = 0;
    T1TCR = 0x00000001;                                         /* Enable the timer                                         */

}
#endif

/*
*********************************************************************************************************
*                                     READ TIMER FOR uC/OS-View
*
* Description : This function is called to read the current counts of a 32 bit free running timer.
*
* Arguments   : none
*
* Returns     ; The 32 bit counts of the timer assuming the timer (MUST be an UP counter).
*********************************************************************************************************
*/

#if OS_VIEW_MODULE > 0
CPU_INT32U  OSView_TmrRd (void)
{
    if (OSRunning == DEF_TRUE) {
        return ((CPU_INT32U)T1TC);
    } else {
        return (0);
    }
}
#endif


/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                         uC/OS-II Timer Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       TICKER INITIALIZATION
*
* Description : This function is called to initialize uC/OS-II's tick source (typically a timer generating
*               interrupts every 1 to 100 mS).
*
* Arguments   : none
*
* Returns     ; none
*********************************************************************************************************
*/

static  void  Tmr_TickInit (void)
{
    CPU_INT32U  pclk_freq;
    CPU_INT32U  rld_cnts;

                                                                /* VIC timer #0 Initialization                              */
    VICIntSelect       &= ~(1 << VIC_TIMER0);                   /* Configure the timer interrupt as an IRQ source           */
    VICVectAddr4        =  (CPU_INT32U)Tmr_TickISR_Handler;     /* Set the vector address                                   */
    VICIntEnable        =  (1 << VIC_TIMER0);                   /* Enable the timer interrupt source                        */

    pclk_freq           =   BSP_CPU_PclkFreq(PCLK_TIMER0);      /* Get the peripheral clock frequency                       */

    rld_cnts            =   pclk_freq / OS_TICKS_PER_SEC;       /* Calculate the # of counts necessary for the OS ticker    */

    T0TCR               =  (1 << 1);                            /* Disable and reset counter 0 and the prescale counter 0   */
    T0TCR               =   0;                                  /* Clear the reset bit                                      */
    T0PC                =   0;                                  /* Prescaler is set to no division                          */

    T0MR0               =   rld_cnts;
    T0MCR               =   3;                                  /* Interrupt on MR0 (reset TC), stop TC                     */

    T0CCR               =   0;                                  /* Capture is disabled.                                     */
    T0EMR               =   0;                                  /* No external match output.                                */
    T0TCR               =   1;                                  /* Enable timer 0                                           */
}


/*
*********************************************************************************************************
*                                         TIMER #0 IRQ HANDLER
*
* Description : This function handles the timer interrupt that is used to generate TICKs for uC/OS-II.
*
* Arguments   : none
*
* Returns     ; none
*********************************************************************************************************
*/

void  Tmr_TickISR_Handler (void)
{
    T0IR        = 0xFF;                                         /* Clear timer #0 interrupt                                 */

    OSTimeTick();                                               /* Call uC/OS-II's OSTimeTick()                             */
}


/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                         DCC Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         DCC INITIALIZATION
*
* Description : This function initializes the interrupts for DCC receive/transmit.
*
* Arguments   : none
*
* Returns     ; none
*********************************************************************************************************
*/

#if OS_CPU_ARM_DCC_EN > 0
static  void  DCC_Init (void)
{
                                                                /* VIC DEBUG RX Initialization                              */
    VICIntEnClear       =  (1 << VIC_DEBUGRX);                  /* Enable the timer interrupt source                        */
    VICIntSelect       &= ~(1 << VIC_DEBUGRX);                  /* Configure the DCC Rx function as an IRQ source           */
    VICVectAddr2        =  (CPU_INT32U)DCC_RxISR_Handler;       /* Set the vector address                                   */
    VICIntEnable        =  (1 << VIC_DEBUGRX);                  /* Enable the timer interrupt source                        */
}
#endif

/*
*********************************************************************************************************
*                                         DCC ISR HANDLERS
*
* Description : These functions are the handlers for DCC receive and transmit.
*
* Arguments   : none
*
* Returns     ; none
*********************************************************************************************************
*/

#if OS_CPU_ARM_DCC_EN > 0
void  DCC_RxISR_Handler (void)
{
    OSDCC_Handler();
}

void  DCC_TxISR_Handler (void)
{
    OSDCC_Handler();
}
#endif

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                Static Board Support Initialization Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      Set the CPU Clock Frequency
*
* Description: This function sets up and activates the PLL
*
* Arguements  : None
*
* Returns     : None
*
* Notes      : 1) The PLL output frequency is calculated by the following formula:
*                     Fcco = 2 * Fin * m / n, where Fin is the PLL input clock. In
*                     this particular case, Fin is set to the Main Oscillator
*                     whose frequency is #define'd in bsp.h. M is the PLL
*                     clock multiplier. M must be written to the PLLCFG register
*                     as the desired multiplier - 1. N is the PLL clock divider
*                     and must be written to PLLCFG as the desired divider - 1.
*
*              2) Fcco must be between 250 and 550 MHz. The ARM Core clock
*                 must never exceed 72 MHz. Use cClkDiv to divide Fcco accordingly.
*
*              3) When using the USB device, you must choose Fcco as a multiple
*                 of 96 MHz, and then use usbClkDiv to divide Fcco to exactly
*                 48 MHz.
*
*              4) In this example, Fin = 12MHz, M = 12, N = 1, cClkDiv = 6 and usbClkDiv = 6.
*                 Therefore, Fcco = 2 * Fin * M / N = (2 * 12 * 12 / 1) = 288MHz.
*                 The processor clock = (Fcco / cClkDiv) = (288MHz / 6) =  48MHz.
*                 Finally, the USB clock = (Fcco / usbClkDib) = (288MHz / 6) = 48MHz.
*
*              5) Early revisions of the part have a PLL errata preventing Fcco from
*                 being greater than 288MHz.
*
*              6) For later revisions, M = 20, cCLKDiv = 8, and usbClkDiv = 10 yield
*                 60MHz for the processor clock and 48MHz for the USB clock.
*********************************************************************************************************
*/

static  void  PLL_Init (void)
{
#if CPU_CFG_CRITICAL_METHOD == CPU_CRITICAL_METHOD_STATUS_LOCAL /* Allocate storage for CPU status register                 */
    CPU_SR  cpu_sr = 0;
#endif

	CPU_INT32U  m;
    CPU_INT32U  n;
    CPU_INT32U  clk_div;
    CPU_INT32U  clk_div_usb;


    m           =       11;                                     /* PLL Multiplier = 20, MSEL bits = 12 - 1 = 11             */
    n           =        0;                                     /* PLL Divider    =  1, NSEL bits =  1 - 1 =  0             */
    clk_div     =        5;                                     /* Configure the  ARM Core clock div to 6. CCLKSEL =  6 - 1 */
    clk_div_usb =        5;                                     /* Configure the USB clock divider to 6, USBSEL  = 6 - 1    */

    if ((PLLSTAT & (1 << 25)) > 0) {                            /* If the PLL is already running                            */
        CPU_CRITICAL_ENTER();
        PLLCON  &= ~(1 << 1);                                   /* Disconnect the PLL                                       */
        PLLFEED  =    0xAA;                                     /* PLL register update sequence, 0xAA, 0x55                 */
        PLLFEED  =    0x55;
        CPU_CRITICAL_EXIT();
    }

    CPU_CRITICAL_ENTER();
    PLLCON     &= ~(1 << 0);                                    /* Disable the PLL                                          */
    PLLFEED     =     0xAA;                                     /* PLL register update sequence, 0xAA, 0x55                 */
    PLLFEED     =     0x55;
    CPU_CRITICAL_EXIT();

    SCS        &= ~(1 << 4);                                    /* OSCRANGE = 0, Main OSC is between 1 and 20 Mhz           */
	SCS        |=  (1 << 5);                                    /* OSCEN = 1, Enable the main oscillator                    */

    while ((SCS &  (1 << 6)) == 0) {                            /* Wait until OSCSTAT is set (Main OSC ready to be used)    */
        ;
    }

    CLKSRCSEL   =  (1 << 0);		                            /* Select main OSC, 12MHz, as the PLL clock source          */

    CPU_CRITICAL_ENTER();
    PLLCFG      =  (m << 0) | (n << 16);                        /* Configure the PLL multiplier and divider                 */
    PLLFEED     =     0xAA;                                     /* PLL register update sequence, 0xAA, 0x55                 */
    PLLFEED     =     0x55;
    CPU_CRITICAL_EXIT();

    CPU_CRITICAL_ENTER();
    PLLCON     |=  (1 << 0);                                    /* Enable the PLL                                           */
    PLLFEED     =     0xAA;                                     /* PLL register update sequence, 0xAA, 0x55                 */
    PLLFEED     =     0x55;
    CPU_CRITICAL_EXIT();

    CCLKCFG     =   clk_div;                                    /* Configure the ARM Core Processor clock divider           */
    USBCLKCFG   =   clk_div_usb;                                /* Configure the USB clock divider                          */

    while ((PLLSTAT & (1 << 26)) == 0) {	                    /* Wait for PLOCK to become set                             */
        ;
    }

    PCLKSEL0    =   0xAAAAAAAA;                                 /* Set peripheral clocks to be half of main clock           */
    PCLKSEL1    =   0x22AAA8AA;

    CPU_CRITICAL_ENTER();
    PLLCON     |=  (1 << 1);                                    /* Connect the PLL. The PLL is now the active clock source  */
    PLLFEED     =     0xAA;                                     /* PLL register update sequence, 0xAA, 0x55                 */
    PLLFEED     =     0x55;
    CPU_CRITICAL_EXIT();

    while ((PLLSTAT & (1 << 25)) == 0) {                        /* Wait PLLC, the PLL connect status bit to become set      */
        ;
    }
}

/*
*********************************************************************************************************
*                                     MAM_Init()
*
* Description : This function initializes the Memory Acceleration Module
*
* Arguements  : None
*
* Returns     : None
*********************************************************************************************************
*/

static  void  MAM_Init (void)
{
    CPU_INT32U  clk_freq;


    clk_freq    = BSP_CPU_ClkFreq();                            /* Get the current core clock frequency                     */

    MAMCR       = 0;                                            /* Disable MAM functionality                                */

    if (clk_freq <  20000000) {                                 /* Compare current clock frequency with MAM modes           */
        MAMTIM  =  1;                                           /* Set MAM fetch cycles to 1 processor clock in duration    */
    }

    if (clk_freq <  40000000) {
        MAMTIM  =  2;                                           /* Set MAM fetch cycles to 2 processor clock in duration    */
    }

    if (clk_freq >= 40000000) {
        MAMTIM  =  3;                                           /* Set MAM fetch cycles to 3 processor clock in duration    */
    }

    MAMCR       =  2;                                           /* Enable full MAM functionality                            */
}

/*
*********************************************************************************************************
*                                          INITIALIZE I/Os
*
* Description : This function consolidates the I/O initialization for various modules.
*
* Arguements  : None
*
* Returns     : None
*
* Note(s)     : (1) Refer to the LPC2468 User Manual for a detailed pin assignment
*********************************************************************************************************
*/

static  void  GPIO_Init (void)
{
}

/*
*********************************************************************************************************
*                                     VIC_Init()
*
* Description : This function initializes the Vectored Interrupt Controller
*
* Arguements  : None
*
* Returns     : None
*
* Notes       : None
*********************************************************************************************************
*/

static  void  VIC_Init (void)
{
    VICSoftIntClr =  0xFFFFFFFF;                                /* Disable ALL interrupts                                   */
    VICVectAddr    =  0;                                         /* Acknowlege any pending VIC interrupt                     */
    VICProtection =  0;                                         /* Allow VIC register access in User of Priviledged modes   */

    VICVectAddr0  = (CPU_INT32U)VIC_DummyWDT;                   /* Set the vector address                                   */
    VICVectAddr1  = (CPU_INT32U)VIC_DummySW;
    VICVectAddr2  = (CPU_INT32U)VIC_DummyDEBUGRX;
    VICVectAddr3  = (CPU_INT32U)VIC_DummyDEBUGTX;
    VICVectAddr4  = (CPU_INT32U)VIC_DummyTIMER0;
    VICVectAddr5  = (CPU_INT32U)VIC_DummyTIMER1;
    VICVectAddr6  = (CPU_INT32U)VIC_DummyUART0;
    VICVectAddr7  = (CPU_INT32U)VIC_DummyUART1;
    VICVectAddr8  = (CPU_INT32U)VIC_DummyPWM01;
    VICVectAddr9  = (CPU_INT32U)VIC_DummyI2C0;
    VICVectAddr10 = (CPU_INT32U)VIC_DummySPI;
    VICVectAddr11 = (CPU_INT32U)VIC_DummySSP1;
    VICVectAddr12 = (CPU_INT32U)VIC_DummyPLL;
    VICVectAddr13 = (CPU_INT32U)VIC_DummyRTC;
    VICVectAddr14 = (CPU_INT32U)VIC_DummyEINT0;
    VICVectAddr15 = (CPU_INT32U)VIC_DummyEINT1;
    VICVectAddr16 = (CPU_INT32U)VIC_DummyEINT2;
    VICVectAddr17 = (CPU_INT32U)VIC_DummyEINT3;
    VICVectAddr18 = (CPU_INT32U)VIC_DummyAD0;
    VICVectAddr19 = (CPU_INT32U)VIC_DummyI2C1;
    VICVectAddr20 = (CPU_INT32U)VIC_DummyBOD;
    VICVectAddr21 = (CPU_INT32U)VIC_DummyETHERNET;
    VICVectAddr22 = (CPU_INT32U)VIC_DummyUSB;
    VICVectAddr23 = (CPU_INT32U)VIC_DummyCAN01;
    VICVectAddr24 = (CPU_INT32U)VIC_DummyMMC;
    VICVectAddr25 = (CPU_INT32U)VIC_DummyGP_DMA;
    VICVectAddr26 = (CPU_INT32U)VIC_DummyTIMER2;
    VICVectAddr27 = (CPU_INT32U)VIC_DummyTIMER3;
    VICVectAddr28 = (CPU_INT32U)VIC_DummyUART2;
    VICVectAddr29 = (CPU_INT32U)VIC_DummyUART3;
    VICVectAddr30 = (CPU_INT32U)VIC_DummyI2C2;
    VICVectAddr31 = (CPU_INT32U)VIC_DummyI2S;
}

/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                       DUMMY INTERRUPT HANDLERS
******************************************************************************************************************************
******************************************************************************************************************************
*/

static  void  VIC_Dummy (void)
{
    while (1) {
        (void)VIC_SpuriousInt;
    }
}

static  void  VIC_DummyWDT (void)
{
    VIC_SpuriousInt = VIC_WDT;
    VIC_Dummy();
}

static  void  VIC_DummySW (void)
{
    VIC_SpuriousInt = VIC_SW;
    VIC_Dummy();
}

static  void  VIC_DummyDEBUGRX (void)
{
    VIC_SpuriousInt = VIC_DEBUGRX;
    VIC_Dummy();
}

static  void  VIC_DummyDEBUGTX (void)
{
    VIC_SpuriousInt = VIC_DEBUGTX;
    VIC_Dummy();
}

static  void  VIC_DummyTIMER0 (void)
{
    VIC_SpuriousInt = VIC_TIMER0;
    VIC_Dummy();
}

static  void  VIC_DummyTIMER1 (void)
{
    VIC_SpuriousInt = VIC_TIMER1;
    VIC_Dummy();
}

static  void  VIC_DummyUART0 (void)
{
    VIC_SpuriousInt = VIC_UART0;
    VIC_Dummy();
}

static  void  VIC_DummyUART1 (void)
{
    VIC_SpuriousInt = VIC_UART1;
    VIC_Dummy();
}

static  void  VIC_DummyPWM01 (void)
{
    VIC_SpuriousInt = VIC_PWM01;
    VIC_Dummy();
}

static  void  VIC_DummyI2C0 (void)
{
    VIC_SpuriousInt = VIC_I2C0;
    VIC_Dummy();
}

static  void  VIC_DummySPI (void)
{
    VIC_SpuriousInt = VIC_SPI;
    VIC_Dummy();
}

static  void  VIC_DummySSP1 (void)
{
    VIC_SpuriousInt = VIC_SSP1;
    VIC_Dummy();
}

static  void  VIC_DummyPLL (void)
{
    VIC_SpuriousInt = VIC_PLL;
    VIC_Dummy();
}

static  void  VIC_DummyRTC (void)
{
    VIC_SpuriousInt = VIC_RTC;
    VIC_Dummy();
}

static  void  VIC_DummyEINT0 (void)
{
    VIC_SpuriousInt = VIC_EINT0;
    VIC_Dummy();
}

static  void  VIC_DummyEINT1 (void)
{
    VIC_SpuriousInt = VIC_EINT1;
    VIC_Dummy();
}

static  void  VIC_DummyEINT2 (void)
{
    VIC_SpuriousInt = VIC_EINT2;
    VIC_Dummy();
}

static  void  VIC_DummyEINT3 (void)
{
    VIC_SpuriousInt = VIC_EINT3;
    VIC_Dummy();
}

static  void  VIC_DummyAD0 (void)
{
    VIC_SpuriousInt = VIC_AD0;
    VIC_Dummy();
}

static  void  VIC_DummyI2C1 (void)
{
    VIC_SpuriousInt = VIC_I2C1;
    VIC_Dummy();
}

static  void  VIC_DummyBOD (void)
{
    VIC_SpuriousInt = VIC_BOD;
    VIC_Dummy();
}

static  void  VIC_DummyETHERNET (void)
{
    VIC_SpuriousInt = VIC_ETHERNET;
    VIC_Dummy();
}

static  void  VIC_DummyUSB (void)
{
    VIC_SpuriousInt = VIC_USB;
    VIC_Dummy();
}

static  void  VIC_DummyCAN01 (void)
{
    VIC_SpuriousInt = VIC_CAN12;
    VIC_Dummy();
}

static  void  VIC_DummyMMC (void)
{
    VIC_SpuriousInt = VIC_MMC;
    VIC_Dummy();
}

static  void  VIC_DummyGP_DMA (void)
{
    VIC_SpuriousInt = VIC_GP_DMA;
    VIC_Dummy();
}

static  void  VIC_DummyTIMER2 (void)
{
    VIC_SpuriousInt = VIC_TIMER2;
    VIC_Dummy();
}

static  void  VIC_DummyTIMER3 (void)
{
    VIC_SpuriousInt = VIC_TIMER3;
    VIC_Dummy();
}

static  void  VIC_DummyUART2 (void)
{
    VIC_SpuriousInt = VIC_UART2;
    VIC_Dummy();
}

static  void  VIC_DummyUART3 (void)
{
    VIC_SpuriousInt = VIC_UART3;
    VIC_Dummy();
}

static  void  VIC_DummyI2C2 (void)
{
    VIC_SpuriousInt = VIC_I2C2;
    VIC_Dummy();
}

static  void  VIC_DummyI2S (void)
{
    VIC_SpuriousInt = VIC_I2S;
    VIC_Dummy();
}


//定义LED的IO控制脚
#define	LED1		1 << 19		//P2.19  
#define	LED2		1 << 30		//P0.30
#define	LED3		1 << 18		//P2.18
#define	LED4		1 << 24		//P3.24
#define	LED5		1 << 25		//P3.25
#define	LED6		1 << 29		//P0.29
#define	LED7		1 << 26		//P3.26
#define	LED8		1 << 25		//P2.25
#define	LED9		1 << 24 	//P2.24
#define	LED10		1 << 12		//P0.12
#define	LED11		0x80000000	//P0.31
#define	LED12		1 << 28		//P2.28

void hd_SetOneLED(INT8U led_num)
{	
	switch(led_num)
	{
	case 0:
		FIO2CLR =LED1;	break;
	case 1:
		FIO0CLR =LED2;	break;			
	case 2:
		FIO2CLR =LED3;	break;	
	case 3:
		FIO3CLR =LED4;	break;
	case 4:
		FIO3CLR =LED5;	break;
	case 5:
		FIO0CLR =LED6;	break;			
	case 6:
		FIO3CLR =LED7;	break;	
	case 7:
		FIO2CLR =LED8;	break;
	case 8:
		FIO2CLR =LED9;	break;
	case 9:
		FIO0CLR =LED10;	break;			
	case 10:
		FIO0CLR =LED11;	break;	
	case 11:
		FIO2CLR =LED12;	break;
	}
}

void hd_ClearOneLED(INT8U led_num)
{	
	switch(led_num)
	{
	case 0:
		FIO2SET =LED1;	break;
	case 1:
		FIO0SET =LED2;	break;			
	case 2:
		FIO2SET =LED3;	break;	
	case 3:
		FIO3SET =LED4;	break;
	case 4:
		FIO3SET =LED5;	break;
	case 5:
		FIO0SET =LED6;	break;			
	case 6:
		FIO3SET =LED7;	break;	
	case 7:
		FIO2SET =LED8;	break;
	case 8:
		FIO2SET =LED9;	break;
	case 9:
		FIO0SET =LED10;	break;			
	case 10:
		FIO0SET =LED11;	break;	
	case 11:
		FIO2SET =LED12;	break;
	}	
}
void SetLed( INT8U uLedID,INT8U bState)
{
	if (bState)
	{
		hd_SetOneLED(uLedID);
	}
	else
	{
		hd_ClearOneLED(uLedID);
	}
}
	  
static void CPU_PIN_Init(void)
{	
	SCS |= 0x01;				
    PINSEL0 = 0x000aa05f;		//PIN设置
    PINSEL1 = 0x01400000;
    PINSEL2 = 0x50150105;
    PINSEL3 = 0x00000005;
    PINSEL4 = 0x05500000;
    PINSEL5 = 0x00c0f000;
    PINSEL9 = 0x0f000a00;
    PINSEL10 = 0x00000000;	    //要保留
   
    PINMODE0=0x00000000;
    PINMODE1=0x00000000;
    PINMODE2=0x00000000;
    PINMODE3=0x00000000;
    PINMODE4=0x00000000;
    PINMODE5=0x00000000;
    PINMODE6=0x00000000;
    PINMODE7=0x00000000;
    PINMODE8=0x00000000;
    PINMODE9=0x00000000;
     
    FIO0DIR = 0xe0019c00;
    FIO1DIR = 0x1c000000;
    FIO2DIR = 0x130c8380;    					 
    FIO3DIR = 0x07270080;
    FIO4DIR = 0x08857000;

	PCONP |= 3<<22;//使能定时器2，3
	PCONP |= 1<<25;//使能Uart3
	PCONP |= 1<<30;//使能Ethnet
}
