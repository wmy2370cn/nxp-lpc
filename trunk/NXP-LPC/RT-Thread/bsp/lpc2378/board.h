/*
 * File      : board.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2008-12-11     xuxinming    first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

void rt_hw_board_init(void);
void rt_hw_led_set(rt_uint32_t led);
void rt_hw_led_flash(void);

#ifdef RT_USING_FINSH
void rt_hw_finsh_init(void);
#endif

#define	USE_USB					0

#if USE_USB		/* 1 is USB, 0 is non-USB related */  


#else

#define PLL_NValue			1	
#define PLL_MValue			(((Fcco/Fosc)*(PLL_NValue+1)/2)-1)
#define CCLKDivValue		(Fcco/Fcclk-1)


#define Fosc                12000000			//	OSC = 11.0592MHz
#define Fcclk	(Fosc * 4)							// Ö÷Æµ Fcclk = 44.2368MHz
#define Fcco    (Fcclk* 6)
	
#define Fpclk	(Fcclk / 4)
#endif

#if USE_USB
#define Fpclk	(Fcclk / 2)
#else
#define Fpclk	(Fcclk / 4)
#endif

/* IRQ define */
#define SYS32Mode		0x1F
#define IRQ32Mode		0x12
#define FIQ32Mode		0x11

#define HIGHEST_PRIORITY	0x01
#define LOWEST_PRIORITY		0x0F

#define	WDT_INT			0
#define SWI_INT			1
#define ARM_CORE0_INT	2
#define	ARM_CORE1_INT	3
#define	TIMER0_INT		4
#define TIMER1_INT		5
#define UART0_INT		6
#define	UART1_INT		7
#define	PWM0_1_INT		8
#define I2C0_INT		9
#define SPI0_INT		10			/* SPI and SSP0 share VIC slot */
#define SSP0_INT		10
#define	SSP1_INT		11
#define	PLL_INT			12
#define RTC_INT			13
#define EINT0_INT		14
#define EINT1_INT		15
#define EINT2_INT		16
#define EINT3_INT		17
#define	ADC0_INT		18
#define I2C1_INT		19
#define BOD_INT			20
#define EMAC_INT		21
#define USB_INT			22
#define CAN_INT			23
#define MCI_INT			24
#define GPDMA_INT		25
#define TIMER2_INT		26
#define TIMER3_INT		27
#define UART2_INT		28
#define UART3_INT		29
#define I2C2_INT		30
#define I2S_INT			31

#define VIC_SIZE		32

#define VECT_ADDR_INDEX	0x100
#define VECT_CNTL_INDEX 0x200

/* ---------------------- GPIO0 Pins ---------------------- */
#define  GPIO0_CANRD1                  DEF_BIT_00               /* CAN Transceiver                                          */
#define  GPIO0_CANTD1                  DEF_BIT_01               /* CAN Transceiver                                          */
#define  GPIO0_TXD0                    DEF_BIT_02               /* UART #0: Over Serial Bridge                              */
#define  GPIO0_RXD0                    DEF_BIT_03               /* UART #0: Over Serial Bridge                              */
#define  GPIO0_CANRD2                  DEF_BIT_05               /* CAN Transceiver                                          */
#define  GPIO0_LCD_CS_SEL              DEF_BIT_06               /* LCD (Can be selected)                                    */
#define  GPIO0_CANTD2                  DEF_BIT_07               /* CAN Transceiver                                          */
#define  GPIO0_LCD_RS_SEL              DEF_BIT_07               /* LCD (Can be selected)                                    */
#define  GPIO0_LCD_WE_SEL              DEF_BIT_08               /* LCD (Can be selected)                                    */
#define  GPIO0_USBB_PORT_PWR           DEF_BIT_12               /* USB Channel B                                            */
#define  GPIO0_USBB_LED                DEF_BIT_13               /* USB Channel B                                            */
#define  GPIO0_USBB_HOST_DEV_EN        DEF_BIT_14               /* USB Channel B                                            */
#define  GPIO0_LCD_SCK                 DEF_BIT_15               /* LCD (Can be selected for serial interface)               */
#define  GPIO0_LCD_MISO                DEF_BIT_17               /* LCD (Can be selected for serial interface)               */
#define  GPIO0_LCD_MOSI                DEF_BIT_18               /* LCD (Can be selected for serial interface)               */
#define  GPIO0_PHY_PRWDN_SEL           DEF_BIT_20
#define  GPIO0_LCD_RSTOUT_SEL          DEF_BIT_21               /* LCD (Can be selected)                                    */
#define  GPIO0_MCIDAT0_SEL             DEF_BIT_22               /* SD/MMC (Because of silicon errata)                       */
#define  GPIO0_AD00                    DEF_BIT_23
#define  GPIO0_AD01                    DEF_BIT_24
#define  GPIO0_PBLED_SDA0              DEF_BIT_27               /* PCA9532                                                  */
#define  GPIO0_PBLED_SCL0              DEF_BIT_28               /* PCA9532                                                  */
#define  GPIO0_USBA_DP                 DEF_BIT_29
#define  GPIO0_USBA_DM                 DEF_BIT_30
#define  GPIO0_USBB_DP                 DEF_BIT_31

/* ---------------------- GPIO1 Pins ---------------------- */
#define  GPIO1_E_TXD0                  DEF_BIT_00
#define  GPIO1_E_TXD1                  DEF_BIT_01
#define  GPIO1_MCICLK                  DEF_BIT_02               /* SD/MMC                                                   */
#define  GPIO1_MCICMD                  DEF_BIT_03               /* SD/MMC                                                   */
#define  GPIO1_E_TX_EN                 DEF_BIT_04
#define  GPIO1_MCIPWR                  DEF_BIT_05               /* SD/MMC                                                   */
#define  GPIO1_MCIDAT0                 DEF_BIT_06               /* SD/MMC (Cannot be used due to errata)                    */
#define  GPIO1_MCIDAT1                 DEF_BIT_07               /* SD/MMC                                                   */
#define  GPIO1_E_CRS0                  DEF_BIT_08
#define  GPIO1_E_RXD0                  DEF_BIT_09
#define  GPIO1_E_RXD1                  DEF_BIT_10
#define  GPIO1_MCIDAT2                 DEF_BIT_11               /* SD/MMC                                                   */
#define  GPIO1_MCIDAT3                 DEF_BIT_12               /* SD/MMC                                                   */
#define  GPIO1_E_RX_ER                 DEF_BIT_14
#define  GPIO1_E_RX_CLK                DEF_BIT_15
#define  GPIO1_E_MDC                   DEF_BIT_16
#define  GPIO1_E_MDIO                  DEF_BIT_17
#define  GPIO1_USBA_LED                DEF_BIT_18               /* USB Channel A (OTG)                                      */
#define  GPIO1_USBA_SDA                DEF_BIT_27
#define  GPIO1_USBA_SCL                DEF_BIT_28
#define  GPIO1_USBA_INT                DEF_BIT_29
#define  GPIO1_USBB_PWR_VBUS           DEF_BIT_30               /* USB Channel B                                            */
#define  GPIO1_USBB_OVERCRNT           DEF_BIT_31               /* USB Channel B                                            */


                                                                /* ---------------------- GPIO1 Pins ---------------------- */
#define  GPIO1_SPICLK                  DEF_BIT_20				//zhaoyk22081124
#define  GPIO1_SPInCS                  DEF_BIT_21				//zhaoyk22081124
#define  GPIO1_SPIMISO                 DEF_BIT_23				//zhaoyk22081124
#define  GPIO1_SPIMOSI                 DEF_BIT_24				//zhaoyk22081124
#define  GPIO1_BUZZER                  DEF_BIT_27				//zhaoyk22081124


/* ---------------------- GPIO2 Pins ---------------------- */
#define  GPIO2_ETM              ((CPU_INT32U)(0x000000FF))
#define  GPIO2_EINT0                   DEF_BIT_10
#define  GPIO2_PHY_INT_SEL             DEF_BIT_11               /* PHY Interrupt output                                     */
#define  GPIO2_NAND_BUSY_SEL           DEF_BIT_12
#define  GPIO2_CS2                     DEF_BIT_14               /* DBUS_EN                                                  */
#define  GPIO2_RAS                     DEF_BIT_16
#define  GPIO2_CAS                     DEF_BIT_17
#define  GPIO2_CLKOUT0                 DEF_BIT_18
#define  GPIO2_CLKOUT1                 DEF_BIT_19
#define  GPIO2_DYCS0                   DEF_BIT_20
#define  GPIO2_DYCS1                   DEF_BIT_21               /* DBUS_EN                                                  */
#define  GPIO2_CKE0                    DEF_BIT_24
#define  GPIO2_CKE1                    DEF_BIT_25
#define  GPIO2_DQM0                    DEF_BIT_28
#define  GPIO2_DQM1                    DEF_BIT_29

/* ---------------------- GPIO4 Pins ---------------------- */
#define  GPIO3_LCD_D            ((CPU_INT32U)(0x0000FFFF))      /* LCD Data Bus                                             */
#define  GPIO3_TXD1                    DEF_BIT_16               /* UART #1: RS232 Full Modem or WLAN Add-On Module          */
#define  GPIO3_RXD1                    DEF_BIT_17               /* UART #1: RS232 Full Modem or WLAN Add-On Module          */
#define  GPIO3_CTS1                    DEF_BIT_18               /* UART #1: RS232 Full Modem or WLAN Add-On Module          */
#define  GPIO3_DCD1                    DEF_BIT_19               /* UART #1: RS232 Full Modem or WLAN Add-On Module          */
#define  GPIO3_DSR1                    DEF_BIT_20               /* UART #1: RS232 Full Modem or WLAN Add-On Module          */
#define  GPIO3_DTR1                    DEF_BIT_21               /* UART #1: RS232 Full Modem or WLAN Add-On Module          */
#define  GPIO3_RI1                     DEF_BIT_22               /* UART #1: RS232 Full Modem or WLAN Add-On Module          */
#define  GPIO3_LCD_LIGHT               DEF_BIT_28               /* LCD Backlight                                            */
#define  GPIO3_RTS1                    DEF_BIT_30               /* UART #1: RS232 Full Modem or WLAN Add-On Module          */


/* ---------------------- GPIO4 Pins ---------------------- */
#define  GPIO4_LCD_A            ((CPU_INT32U)(0x00FFFFFF))      /* LCD address bus                                          */
#define  GPIO4_LCD_OE                  DEF_BIT_24               /* LCD RD_E Input                                           */
#define  GPIO4_LCD_WE                  DEF_BIT_25               /* LCD WR_RW Input                                          */
#define  GPIO4_BLS0                    DEF_BIT_26
#define  GPIO4_BLS1                    DEF_BIT_27
#define  GPIO4_CS0                     DEF_BIT_30
#define  GPIO4_CS1                     DEF_BIT_31


/******************************************************************************
** Function name:		TargetInit
**
** Descriptions:		Initialize the target board; it is called in a 
**				necessary place, change it as needed
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern void TargetInit(void);
extern void ConfigurePLL( void );
extern void TargetResetInit(void);

#endif
