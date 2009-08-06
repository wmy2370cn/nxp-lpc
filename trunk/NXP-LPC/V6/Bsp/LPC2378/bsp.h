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

#define  MAIN_OSC_FRQ              12000000L
#define  IRC_OSC_FRQ               12000000L
#define  RTC_OSC_FRQ                  32768L

#ifdef   BSP_GLOBALS
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif

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
*                                             LED SERVICES
*********************************************************************************************************
*/

void            LED_On                  (CPU_INT08U led);
void            LED_Off                 (CPU_INT08U led);
void            LED_Toggle              (CPU_INT08U led);

/*
*********************************************************************************************************
*                                         PUSH BUTTON SERVICES
*********************************************************************************************************
*/

CPU_BOOLEAN     PB_GetStatus            (CPU_INT08U pb);

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

/*
*********************************************************************************************************
*                                             RS-232 SERVICES
*********************************************************************************************************
*/

void            Ser_Init            (void);
void            Ser_WrByte          (      CPU_CHAR  tx_byte);
void            Ser_WrStr           (const CPU_CHAR *tx_str);
CPU_INT08U      Ser_RdByte          (void);
void            Ser_RdStr           (      CPU_CHAR *rx_str, CPU_INT32U len);
void            Ser_Printf          (const CPU_CHAR *format, ...);




#endif
