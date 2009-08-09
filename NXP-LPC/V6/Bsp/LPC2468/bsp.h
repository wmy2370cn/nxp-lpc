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
* Filename      : bsp.h
* Version       : V1.00
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/

#ifndef  __BSP_H__
#define  __BSP_H__

#define  MAIN_OSC_FRQ              11059200L
#define  IRC_OSC_FRQ               11059200L
#define  RTC_OSC_FRQ                  32768L

#ifdef   BSP_GLOBALS
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


/***************************************************************************
 **
 **  VIC Interrupt channels
 **
 ***************************************************************************/
#define VIC_WDT          0  /* Watchdog                           */
#define VIC_SW           1  /* Software interrupts                */
#define VIC_DEBUGRX      2  /* Embedded ICE, DbgCommRx            */
#define VIC_DEBUGTX      3  /* Embedded ICE, DbgCommTx            */
#define VIC_TIMER0       4  /* Timer 0 (Match 0-3 Capture 0-3)    */
#define VIC_TIMER1       5  /* Timer 1 (Match 0-3 Capture 0-3)    */
#define VIC_UART0        6  /* UART 0  (RLS, THRE, RDA, CTI)      */
#define VIC_UART1        7  /* UART 1  (RLS, THRE, RDA, CTI, MSI) */
#define VIC_PWM01        8  /* PWM 01  (Match 0-6 Capture 0-3)    */
#define VIC_I2C0         9  /* I2C 0   (SI)                       */
#define VIC_SPI         10  /* SPI 0, SSP 0                       */
#define VIC_SSP1         11 /* SSP 1                              */
#define VIC_PLL         12  /* PLL lock (PLOCK)                   */
#define VIC_RTC         13  /* RTC      (RTCCIF, RTCALF)          */
#define VIC_EINT0       14  /* External interrupt 0 (EINT0)       */
#define VIC_EINT1       15  /* External interrupt 1 (EINT1)       */
#define VIC_EINT2       16  /* External interrupt 2 (EINT2)       */
#define VIC_EINT3       17  /* External interrupt 3 (EINT3)       */
#define VIC_AD0         18  /* A/D converter 0                    */
#define VIC_I2C1        19  /* I2C 1                              */
#define VIC_BOD         20  /* Brown out detect                   */
#define VIC_ETHERNET    21  /* Ethernet                           */
#define VIC_USB         22  /* USB Low and High priority          */
#define VIC_CAN12       23  /* CAN1,2 Tx, Rx                      */
#define VIC_MMC         24  /* SD/MMC                             */
#define VIC_GP_DMA      25  /* DMA channel 0, DMA channel 1       */
#define VIC_TIMER2      26  /* Timer 2 (Match 0-3 Capture 0-3)    */
#define VIC_TIMER3      27  /* Timer 3 (Match 0-3 Capture 0-3)    */
#define VIC_UART2       28  /* UART 2  (RLS, THRE, RDA, CTI)      */
#define VIC_UART3       29  /* UART 3  (RLS, THRE, RDA, CTI, MSI) */
#define VIC_I2C2        30  /* I2C 0   (SI)                       */
#define VIC_I2S         31  /* I2S Rx, Tx                         */
/*
*********************************************************************************************************
*                                             #define CONSTANTS
*********************************************************************************************************
*/

#define  PCLK_WDT           ( 0)
#define  PCLK_TIMER0        ( 1)
#define  PCLK_TIMER1        ( 2)
#define  PCLK_UART0         ( 3)
#define  PCLK_UART1         ( 4)
#define  PCLK_PWM0          ( 5)
#define  PCLK_PWM1          ( 6)
#define  PCLK_I2C0          ( 7)
#define  PCLK_SPI           ( 8)
#define  PCLK_RTC           ( 9)
#define  PCLK_SSP1          (10)
#define  PCLK_DAC           (11)
#define  PCLK_ADC           (12)
#define  PCLK_CAN1          (13)
#define  PCLK_CAN2          (14)
#define  PCLK_ACF           (15)
#define  PCLK_BAT_RAM       (16)
#define  PCLK_GPIO          (17)
#define  PCLK_PCB           (18)
#define  PCLK_I2C1          (19)
#define  PCLK_SSP0          (21)
#define  PCLK_TIMER2        (22)
#define  PCLK_TIMER3        (23)
#define  PCLK_UART2         (24)
#define  PCLK_UART3         (25)
#define  PCLK_I2C2          (26)
#define  PCLK_MCI           (27)
#define  PCLK_SYSCON        (29)

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void            BSP_Init                (void);
void            BSP_IntDisAll           (void);
CPU_INT32U      BSP_CPU_ClkFreq         (void);
CPU_INT32U      BSP_CPU_PclkFreq        (CPU_INT08U  id);
 

/*
*********************************************************************************************************
*                                            ADC SERVICES
*********************************************************************************************************
*/

CPU_INT16U      ADC_GetStatus           (CPU_INT08U  adc);

/*
*********************************************************************************************************
*                                             TICK SERVICES
*********************************************************************************************************
*/

void            Tmr_TickISR_Handler     (void);

/*
*********************************************************************************************************
*                                             I2C SERVICES
*********************************************************************************************************
*/

void            I2C_Init                (void);
void            I2C_Read                (CPU_INT08U *p_buf, CPU_INT08U count);
void            I2C_ReadReg             (CPU_INT08U *p_buf, CPU_INT08U count, CPU_INT08U reg);
void            I2C_Write               (CPU_INT08U *p_buf, CPU_INT08U count);
void            I2C_ISR_Handler         (void);

/*
*********************************************************************************************************
*                                          LCD LIGHT SERVICES
*********************************************************************************************************
*/

void            LCD_LightOn             (void);
void            LCD_LightOff            (void);
void            LCD_LightToggle         (void);

/*
*********************************************************************************************************
*                                          DCC SERVICES
*********************************************************************************************************
*/

void            DCC_TxISR_Handler       (void);
void            DCC_RxISR_Handler       (void);
 


#endif
