
#pragma once



/*
*********************************************************************************************************
*                                          #DEFINE CONSTANTS
*********************************************************************************************************
*/
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
#define  GPIO1_SPICLK                  DEF_BIT_20				//zhaoyk22081124
#define  GPIO1_SPInCS                  DEF_BIT_21				//zhaoyk22081124
#define  GPIO1_SPIMISO                 DEF_BIT_23				//zhaoyk22081124
#define  GPIO1_SPIMOSI                 DEF_BIT_24				//zhaoyk22081124
#define  GPIO1_BUZZER                  DEF_BIT_27				//zhaoyk22081124


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
