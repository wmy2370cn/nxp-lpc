/*
*********************************************************************************************************
*                                              uC/TCP-IP
*                                      The Embedded TCP/IP Suite
*
*                          (c) Copyright 2003-2005; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                BOARD SUPPORT PACKAGE (BSP) FUNCTIONS
*
*                                             NXP LPC2468
*                                                on the
*                                  Embedded Artists LPC2468 OEM Board
*
* Filename      : net_isr.c
* Version       : V1.89
* Programmer(s) : Brian Nagel
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <includes.h>
#include  <net.h>
#include  <net_phy.h>
#include <LPC24xx.H>
#include <bsp.h>
/*
*********************************************************************************************************
*                                   NETWORK INTERRUPT INITIALIZATION
*
* Description : This function is called to initialize the interrupt controller associated with the NIC.
*
* Arguments   : None.
*
* Return(s)   : None.
*********************************************************************************************************
*/

void  NetNIC_IntInit  (void)
{
    VICIntSelect       &= ~(1 << VIC_ETHERNET);                         /* Configure the Ethernet VIC interrupt source for IRQ      */
    VICVectAddr21       =  (CPU_INT32U)NetNIC_ISR_Handler;              /* Set the vector address                                   */
    VICIntEnable        =  (1 << VIC_ETHERNET);                         /* Enable the VIC interrupt source, but no local sources    */

}

/*
*********************************************************************************************************
*                                   NETWORK PHY INTERRUPT INITIALIZATION
*
* Description : This function is called to initialize the interrupt controller associated with the NIC.
*
* Arguments   : None.
*
* Return(s)   : None.
*********************************************************************************************************
*/

#if (EMAC_CFG_PHY_INT == DEF_ON)
void  NetNIC_PhyIntInit  (void)
{
//    IO1INTCLR              |=  DEF_BIT_11;
//    IO1INTENR              |=  DEF_BIT_11;

    VICIntSelect           &= ~(1 << VIC_EINT3);                        /* Enable interrupts                                        */
    VICVectAddr17           = (CPU_INT32U)NetNIC_PhyISR_Handler;        /* Set the vector address                                   */
    VICIntEnable            =  (1 << VIC_EINT3);                        /* Enable Interrupts                                        */
}
#endif

/*
*********************************************************************************************************
*                                        NETWORK PHY INTERRUPT CLEAR
*
* Description : This function is called to clear the interrupt controller associated with the PHY.
*
* Arguments   : None.
*
* Return(s)   : None.
*********************************************************************************************************
*/

#if (EMAC_CFG_PHY_INT == DEF_ON)
void  NetNIC_PhyIntClr (void)
{
 //   IO1INTCLR               =  DEF_BIT_11;
}
#endif
