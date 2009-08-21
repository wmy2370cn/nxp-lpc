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
*                                           I2C FUNCTIONS
*
*                                             NXP LPC2468
*                                                on the
*                                  Embedded Artists LPC2468 OEM Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : Brian Nagel
* Note(s)       : (1) Adapted from uCLinux distribution.
*********************************************************************************************************
*/

#define  BSP_GLOBALS
#include <includes.h>
#include "iolpc23xx.h"
#include <LPC23xx.H>


/*
*********************************************************************************************************
*                                          #DEFINE CONSTANTS
*********************************************************************************************************
*/

#define  I2CON_AA                   DEF_BIT_02
#define  I2CON_SI                   DEF_BIT_03
#define  I2CON_STO                  DEF_BIT_04
#define  I2CON_STA                  DEF_BIT_05
#define  I2CON_I2EN                 DEF_BIT_06

                                                                /* ---- I2C STATUS CODES: MASTER IN TRANSMITTER MODE ------ */
#define  I2STAT_SLA_W_ACK               0x18
#define  I2STAT_SLA_W_NOACK             0x20
#define  I2STAT_DAT_T_ACK               0x28
#define  I2STAT_DAT_T_NOACK             0x30

                                                                /* ------ I2C STATUS CODES: MASTER IN RECEIVER MODE ------- */
#define  I2STAT_SLA_R_ACK               0x40
#define  I2STAT_SLA_R_NOACK             0x48
#define  I2STAT_DAT_R_ACK               0x50
#define  I2STAT_DAT_R_NOACK             0x58

                                                                /* ---------- I2C STATUS CODES: MASTER AND SLAVE ---------- */
#define  I2STAT_START                   0x08
#define  I2STAT_REPEATED_START          0x10
#define  I2STAT_ARB_LOST                0x38
#define  I2STAT_NOP                     0x58
#define  I2STAT_BUS_ERROR               0x00


#define  PCA9532_ADDRESS                0xC0

#define  PCA9532_INPUT0                 0x00
#define  PCA9532_INPUT1                 0x01
#define  PCA9532_PSC0                   0x02
#define  PCA9532_PWM0                   0x03
#define  PCA9532_PSC1                   0x04
#define  PCA9532_PWM1                   0x05
#define  PCA9532_LS0                    0x06
#define  PCA9532_LS1                    0x07
#define  PCA9532_LS2                    0x08
#define  PCA9532_LS3                    0x09

/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

static  CPU_INT08U      I2C_Address;
static  CPU_INT08U     *I2C_BufPtr;
static  CPU_INT08U      I2C_Count;

static  OS_EVENT       *I2C_SemWait;
static  OS_EVENT       *I2C_SemBusy;

/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/


/*
******************************************************************************************************************************
******************************************************************************************************************************
**                                         Global Functions
******************************************************************************************************************************
******************************************************************************************************************************
*/


/*
*********************************************************************************************************
*                                        I2C_Init()
*
* Description : This function initializes the I2C
*
* Arguements  : None
*
* Returns     : None
*********************************************************************************************************
*/

void  I2C_Init (void)
{
    CPU_INT32U  pinsel;


    VICIntSelect       &= ~(1 << VIC_I2C0);                     /* Configure the I2C interrupt as an IRQ source             */
    VICVectAddr9        =  (CPU_INT32U)I2C_ISR_Handler;         /* Set the vector address                                   */
    VICIntEnClr       =  (1 << VIC_I2C0);                     /* Disable the interrupt source                             */

    pinsel              = PINSEL1;
    pinsel             &= 0xFC3FFFFF;
    pinsel             |= 0x01400000;
    PINSEL1             = pinsel;

    I20CONCLR           = 0x006C;                               /* Clear flags                                              */

    I20SCLH             = 0x00F0;
    I20SCLL             = 0x0160;

    I2C_SemWait         = OSSemCreate(0);
    I2C_SemBusy         = OSSemCreate(1);
}

/*
*********************************************************************************************************
*                                        I2C_Read()
*
* Description : This function reads from the I2C bus.
*
* Arguements  : p_buf   is a pointer to the buffer into which the bytes will be stored
*               count   is the number of bytes to read
*
* Returns     : None
*********************************************************************************************************
*/

void  I2C_Read (CPU_INT08U *p_buf, CPU_INT08U count)
{
    CPU_INT08U      err;


    OSSemPend(I2C_SemBusy, 0, &err);                            /* Wait until I2C is free                                   */

    I20CONCLR           = I2CON_I2EN
                        | I2CON_STA
                        | I2CON_SI
                        | I2CON_AA;

    I20CONSET           = I2CON_I2EN;

    I2C_Address         = (PCA9532_ADDRESS | 0x01);             /* Set lower bit of address                                 */
    I2C_BufPtr          = p_buf;
    I2C_Count           = count;

    I20CONSET           = I2CON_STA
                        | I2CON_AA;


    VICIntEnable        =  (1 << VIC_I2C0);                     /* Enable the interrupt source                              */
    OSSemPend(I2C_SemWait, 0, &err);                            /* Wait until complete                                      */

    OSSemPost(I2C_SemBusy);                                     /* Free I2C                                                 */
}

/*
*********************************************************************************************************
*                                        I2C_ReadReg()
*
* Description : This function reads a register on an I2C device.
*
* Arguements  : p_buf   is a pointer to the buffer into which the bytes will be stored
*               count   is the number of bytes to read
*               reg     is the register to read
*
* Returns     : None
*********************************************************************************************************
*/

void  I2C_ReadReg (CPU_INT08U *p_buf, CPU_INT08U count, CPU_INT08U reg)
{
    CPU_INT08U      err;


    OSSemPend(I2C_SemBusy, 0, &err);                            /* Wait until I2C is free                                   */



    I20CONCLR           = I2CON_I2EN
                        | I2CON_STA
                        | I2CON_SI
                        | I2CON_AA;

    I20CONSET           = I2CON_I2EN;

    I2C_Address         = (PCA9532_ADDRESS & 0xFE);             /* Clear lower bit of address                               */
    I2C_BufPtr          = &reg;
    I2C_Count           = 1;

    I20CONSET           = I2CON_STA
                        | I2CON_AA;

    VICIntEnable        =  (1 << VIC_I2C0);                     /* Enable the interrupt source                              */
    OSSemPend(I2C_SemWait, 0, &err);                            /* Wait until complete                                      */



    I20CONCLR           = I2CON_I2EN
                        | I2CON_STA
                        | I2CON_SI
                        | I2CON_AA;

    I20CONSET           = I2CON_I2EN;

    I2C_Address         = (PCA9532_ADDRESS | 0x01);             /* Set lower bit of address                                 */
    I2C_BufPtr          = p_buf;
    I2C_Count           = count;

    I20CONSET           = I2CON_STA
                        | I2CON_AA;


    VICIntEnable        =  (1 << VIC_I2C0);                     /* Enable the interrupt source                              */
    OSSemPend(I2C_SemWait, 0, &err);                            /* Wait until complete                                      */


    OSSemPost(I2C_SemBusy);                                     /* Free I2C                                                 */
}

/*
*********************************************************************************************************
*                                        I2C_Write()
*
* Description : This function writes to the I2C bus.
*
* Arguements  : p_buf   is a pointer to the buffer storing the data to be written
*               count   is the number of bytes to write
*
* Returns     : None
*********************************************************************************************************
*/

void  I2C_Write (CPU_INT08U *p_buf, CPU_INT08U count)
{
    CPU_INT08U      err;


    OSSemPend(I2C_SemBusy, 0, &err);                            /* Wait until I2C is free                                   */

    I20CONCLR           = I2CON_I2EN
                        | I2CON_STA
                        | I2CON_SI
                        | I2CON_AA;

    I20CONSET           = I2CON_I2EN;

    I2C_Address         = (PCA9532_ADDRESS & 0xFE);             /* Clear lower bit of address                               */
    I2C_BufPtr          = p_buf;
    I2C_Count           = count;

    I20CONSET           = I2CON_STA
                        | I2CON_AA;

    VICIntEnable        =  (1 << VIC_I2C0);                     /* Enable the interrupt source                              */
    OSSemPend(I2C_SemWait, 0, &err);                            /* Wait until complete                                      */

    OSSemPost(I2C_SemBusy);                                     /* Free I2C                                                 */
}

/*
*********************************************************************************************************
*                                        I2C_ISR_Handler()
*
* Description : This function is the I2C ISR handler.
*
* Arguements  : None
*
* Returns     : None
*********************************************************************************************************
*/

void  I2C_ISR_Handler (void)
{
    CPU_INT32U  status;
    CPU_INT32U  temp;


    status      = I20STAT;
    temp        = 0;

    switch (status & 0xF8) {

        case I2STAT_START:
        case I2STAT_REPEATED_START:
             I20DAT             = I2C_Address;
             I20CONCLR          = I2CON_STA | I2CON_SI;
             break;

        case I2STAT_SLA_W_ACK:
             temp               = *I2C_BufPtr;
             I20DAT             = temp;
             I2C_BufPtr        += 1;
             I2C_Count         -= 1;
             I20CONCLR          = I2CON_STA | I2CON_SI;
             break;

        case I2STAT_DAT_T_ACK:
             if (I2C_Count > 0) {
                 temp           = *I2C_BufPtr;
                 I20DAT         = temp;
                 I2C_BufPtr    += 1;
                 I2C_Count     -= 1;
                 I20CONCLR      = I2CON_STA | I2CON_SI;
             } else {
                 I20CONCLR      = I2CON_STA | I2CON_SI;
                 I20CONSET      = I2CON_STO;
                 OSSemPost(I2C_SemWait);
                 VICIntEnClr  =  (1 << VIC_I2C0);
             }
             break;

        case I2STAT_SLA_W_NOACK:
        case I2STAT_DAT_T_NOACK:
        case I2STAT_SLA_R_NOACK:
             I20CONSET          = I2CON_STO;
             I20CONCLR          = I2CON_STA | I2CON_SI;
             OSSemPost(I2C_SemWait);
             VICIntEnClr      =  (1 << VIC_I2C0);
             break;

        case I2STAT_ARB_LOST:
             I20CONCLR          = I2CON_STA | I2CON_SI;
             OSSemPost(I2C_SemWait);
             VICIntEnClr      =  (1 << VIC_I2C0);
             break;


        case I2STAT_SLA_R_ACK:
             if (I2C_Count <= 1) {
                 I20CONSET      = I2CON_AA;
             } else {
                 I20CONCLR      = I2CON_AA;
             }
             I20CONCLR          = I2CON_STA | I2CON_SI;
             break;

        case I2STAT_DAT_R_ACK:
             temp               = I20DAT;
            *I2C_BufPtr         = temp;
             I2C_BufPtr        += 1;
             I2C_Count         -= 1;
             if (I2C_Count <= 1) {
                 I20CONCLR      = I2CON_AA;
             }
             I20CONCLR          = I2CON_STA | I2CON_SI;
             break;

        case I2STAT_DAT_R_NOACK:
             temp               = I20DAT;
            *I2C_BufPtr         = temp;
             I2C_Count         -= 1;
             I20CONSET          = I2CON_STO;
             I20CONCLR          = I2CON_STA | I2CON_SI;
             OSSemPost(I2C_SemWait);
             VICIntEnClr      =  (1 << VIC_I2C0);
             break;

        default:
             I20CONCLR          = I2CON_STA | I2CON_SI;
             break;
    }
}
