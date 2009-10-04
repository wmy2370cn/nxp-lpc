/******************************************************************************
 *   LPC230X.h:  Header file for NXP LPC230x Family Microprocessors
 *   The header file is the super set of all hardware definition of the 
 *   peripherals for the LPC230x family microprocessor.
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/

#ifndef __LPC24xx_H_
#define __LPC24xx_H_
 

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

#endif  // __LPC230x_H

