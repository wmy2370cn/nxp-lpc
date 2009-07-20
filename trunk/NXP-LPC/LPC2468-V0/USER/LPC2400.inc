;/****************************************Copyright (c***************************************************
;**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
;**                                     
;**                               http://www.embedtools.com
;**
;**--------------File Info--------------------------------------------------------------------------------
;** File name: 		    LPC2400.inc
;** Last modified Date: 2008-04-29 
;** Last Version: 		
;** Descriptions: 		
;**-------------------------------------------------------------------------------------------------------
;** Created by: 		LinEnqiang
;** Created date:   	2008-04-29
;** Version:			1.0
;** Descriptions: 		The original version
;**
;**-------------------------------------------------------------------------------------------------------
;** Modified by: 		
;** Modified date:		
;** Version:			
;** Descriptions:
;********************************************************************************************************/

;/********************************************************************************************************
;  Vectored Interrupt Controller (VIC
;********************************************************************************************************/
VIC_BASE_ADDR                   EQU     0xFFFFF000

VICIRQStatus                    EQU     (VIC_BASE_ADDR + 0x000)
VICFIQStatus                    EQU     (VIC_BASE_ADDR + 0x004)
VICRawIntr                      EQU     (VIC_BASE_ADDR + 0x008)
VICIntSelect                    EQU     (VIC_BASE_ADDR + 0x00C)
VICIntEnable                    EQU     (VIC_BASE_ADDR + 0x010)
VICIntEnClr                     EQU     (VIC_BASE_ADDR + 0x014)
VICSoftInt                      EQU     (VIC_BASE_ADDR + 0x018)
VICSoftIntClr                   EQU     (VIC_BASE_ADDR + 0x01C)
VICProtection                   EQU     (VIC_BASE_ADDR + 0x020)
VICSWPrioMask                   EQU     (VIC_BASE_ADDR + 0x024)

VICVectAddr0                    EQU     (VIC_BASE_ADDR + 0x100)
VICVectAddr1                    EQU     (VIC_BASE_ADDR + 0x104)
VICVectAddr2                    EQU     (VIC_BASE_ADDR + 0x108)
VICVectAddr3                    EQU     (VIC_BASE_ADDR + 0x10C)
VICVectAddr4                    EQU     (VIC_BASE_ADDR + 0x110)
VICVectAddr5                    EQU     (VIC_BASE_ADDR + 0x114)
VICVectAddr6                    EQU     (VIC_BASE_ADDR + 0x118)
VICVectAddr7                    EQU     (VIC_BASE_ADDR + 0x11C)
VICVectAddr8                    EQU     (VIC_BASE_ADDR + 0x120)
VICVectAddr9                    EQU     (VIC_BASE_ADDR + 0x124)
VICVectAddr10                   EQU     (VIC_BASE_ADDR + 0x128)
VICVectAddr11                   EQU     (VIC_BASE_ADDR + 0x12C)
VICVectAddr12                   EQU     (VIC_BASE_ADDR + 0x130)
VICVectAddr13                   EQU     (VIC_BASE_ADDR + 0x134)
VICVectAddr14                   EQU     (VIC_BASE_ADDR + 0x138)
VICVectAddr15                   EQU     (VIC_BASE_ADDR + 0x13C)
VICVectAddr16                   EQU     (VIC_BASE_ADDR + 0x140)
VICVectAddr17                   EQU     (VIC_BASE_ADDR + 0x144)
VICVectAddr18                   EQU     (VIC_BASE_ADDR + 0x148)
VICVectAddr19                   EQU     (VIC_BASE_ADDR + 0x14C)
VICVectAddr20                   EQU     (VIC_BASE_ADDR + 0x150)
VICVectAddr21                   EQU     (VIC_BASE_ADDR + 0x154)
VICVectAddr22                   EQU     (VIC_BASE_ADDR + 0x158)
VICVectAddr23                   EQU     (VIC_BASE_ADDR + 0x15C)
VICVectAddr24                   EQU     (VIC_BASE_ADDR + 0x160)
VICVectAddr25                   EQU     (VIC_BASE_ADDR + 0x164)
VICVectAddr26                   EQU     (VIC_BASE_ADDR + 0x168)
VICVectAddr27                   EQU     (VIC_BASE_ADDR + 0x16C)
VICVectAddr28                   EQU     (VIC_BASE_ADDR + 0x170)
VICVectAddr29                   EQU     (VIC_BASE_ADDR + 0x174)
VICVectAddr30                   EQU     (VIC_BASE_ADDR + 0x178)
VICVectAddr31                   EQU     (VIC_BASE_ADDR + 0x17C)

;/********************************************************************************************************
;  The name convention below is from previous LPC2000 family MCUs, in LPC240x,
;  these registers are known as "VICVectPriority(x)".)
;********************************************************************************************************/

VICVectPri0                     EQU     (VIC_BASE_ADDR + 0x200)
VICVectPri1                     EQU     (VIC_BASE_ADDR + 0x204)
VICVectPri2                     EQU     (VIC_BASE_ADDR + 0x208)
VICVectPri3                     EQU     (VIC_BASE_ADDR + 0x20C)
VICVectPri4                     EQU     (VIC_BASE_ADDR + 0x210)
VICVectPri5                     EQU     (VIC_BASE_ADDR + 0x214)
VICVectPri6                     EQU     (VIC_BASE_ADDR + 0x218)
VICVectPri7                     EQU     (VIC_BASE_ADDR + 0x21C)
VICVectPri8                     EQU     (VIC_BASE_ADDR + 0x220)
VICVectPri9                     EQU     (VIC_BASE_ADDR + 0x224)
VICVectPri10                    EQU     (VIC_BASE_ADDR + 0x228)
VICVectPri11                    EQU     (VIC_BASE_ADDR + 0x22C)
VICVectPri12                    EQU     (VIC_BASE_ADDR + 0x230)
VICVectPri13                    EQU     (VIC_BASE_ADDR + 0x234)
VICVectPri14                    EQU     (VIC_BASE_ADDR + 0x238)
VICVectPri15                    EQU     (VIC_BASE_ADDR + 0x23C)
VICVectPri16                    EQU     (VIC_BASE_ADDR + 0x240)
VICVectPri17                    EQU     (VIC_BASE_ADDR + 0x244)
VICVectPri18                    EQU     (VIC_BASE_ADDR + 0x248)
VICVectPri19                    EQU     (VIC_BASE_ADDR + 0x24C)
VICVectPri20                    EQU     (VIC_BASE_ADDR + 0x250)
VICVectPri21                    EQU     (VIC_BASE_ADDR + 0x254)
VICVectPri22                    EQU     (VIC_BASE_ADDR + 0x258)
VICVectPri23                    EQU     (VIC_BASE_ADDR + 0x25C)
VICVectPri24                    EQU     (VIC_BASE_ADDR + 0x260)
VICVectPri25                    EQU     (VIC_BASE_ADDR + 0x264)
VICVectPri26                    EQU     (VIC_BASE_ADDR + 0x268)
VICVectPri27                    EQU     (VIC_BASE_ADDR + 0x26C)
VICVectPri28                    EQU     (VIC_BASE_ADDR + 0x270)
VICVectPri29                    EQU     (VIC_BASE_ADDR + 0x274)
VICVectPri30                    EQU     (VIC_BASE_ADDR + 0x278)
VICVectPri31                    EQU     (VIC_BASE_ADDR + 0x27C)

VICVectAddr                     EQU     (VIC_BASE_ADDR + 0xF00)

;/********************************************************************************************************
;  Pin Connect Block
;********************************************************************************************************/

PINSEL_BASE_ADDR                EQU     0xE002C000

PINSEL0                         EQU     (PINSEL_BASE_ADDR + 0x00)
PINSEL1                         EQU     (PINSEL_BASE_ADDR + 0x04)
PINSEL2                         EQU     (PINSEL_BASE_ADDR + 0x08)
PINSEL3                         EQU     (PINSEL_BASE_ADDR + 0x0C)
PINSEL4                         EQU     (PINSEL_BASE_ADDR + 0x10)
PINSEL5                         EQU     (PINSEL_BASE_ADDR + 0x14)
PINSEL6                         EQU     (PINSEL_BASE_ADDR + 0x18)
PINSEL7                         EQU     (PINSEL_BASE_ADDR + 0x1C)
PINSEL8                         EQU     (PINSEL_BASE_ADDR + 0x20)
PINSEL9                         EQU     (PINSEL_BASE_ADDR + 0x24)
PINSEL10                        EQU     (PINSEL_BASE_ADDR + 0x28)

PINMODE0                        EQU     (PINSEL_BASE_ADDR + 0x40)            
PINMODE1                        EQU     (PINSEL_BASE_ADDR + 0x44)
PINMODE2                        EQU     (PINSEL_BASE_ADDR + 0x48)            
PINMODE3                        EQU     (PINSEL_BASE_ADDR + 0x4C)
PINMODE4                        EQU     (PINSEL_BASE_ADDR + 0x50)
PINMODE5                        EQU     (PINSEL_BASE_ADDR + 0x54)
PINMODE6                        EQU     (PINSEL_BASE_ADDR + 0x58)
PINMODE7                        EQU     (PINSEL_BASE_ADDR + 0x5C)
PINMODE8                        EQU     (PINSEL_BASE_ADDR + 0x60)
PINMODE9                        EQU     (PINSEL_BASE_ADDR + 0x64)

;/********************************************************************************************************
;  General Purpose Input/Output (GPIO)
;********************************************************************************************************/
GPIO_BASE_ADDR                  EQU     0xE0028000

IO0PIN                          EQU     (GPIO_BASE_ADDR + 0x00)
IO0SET                          EQU     (GPIO_BASE_ADDR + 0x04)
IO0DIR                          EQU     (GPIO_BASE_ADDR + 0x08)
IO0CLR                          EQU     (GPIO_BASE_ADDR + 0x0C)
IO1PIN                          EQU     (GPIO_BASE_ADDR + 0x10)
IO1SET                          EQU     (GPIO_BASE_ADDR + 0x14)
IO1DIR                          EQU     (GPIO_BASE_ADDR + 0x18)
IO1CLR                          EQU     (GPIO_BASE_ADDR + 0x1C)

;/********************************************************************************************************
;  GPIO Interrupt Registers
;********************************************************************************************************/

IO0IntEnR                       EQU     (GPIO_BASE_ADDR + 0x90) 
IO0IntEnF                       EQU     (GPIO_BASE_ADDR + 0x94)
IO0IntStatR                     EQU     (GPIO_BASE_ADDR + 0x84)
IO0IntStatF                     EQU     (GPIO_BASE_ADDR + 0x88)
IO0IntClr                       EQU     (GPIO_BASE_ADDR + 0x8C)

IO2IntEnR                       EQU     (GPIO_BASE_ADDR + 0xB0) 
IO2IntEnF                       EQU     (GPIO_BASE_ADDR + 0xB4)
IO2IntStatR                     EQU     (GPIO_BASE_ADDR + 0xA4)
IO2IntStatF                     EQU     (GPIO_BASE_ADDR + 0xA8)
IO2IntClr                       EQU     (GPIO_BASE_ADDR + 0xAC)

IO_INT_STAT                     EQU     (GPIO_BASE_ADDR + 0x80)
IOIntStatus                     EQU     (GPIO_BASE_ADDR + 0x80)

PARTCFG_BASE_ADDR  EQU	  0x3FFF8000

PARTCFG                         EQU     (PARTCFG_BASE_ADDR + 0x00) 

;/********************************************************************************************************
;  Fast I/O setup
;********************************************************************************************************/

FIO_BASE_ADDR	                EQU     0x3FFFC000

FIO0DIR                         EQU     (FIO_BASE_ADDR + 0x00) 
FIO0MASK                        EQU     (FIO_BASE_ADDR + 0x10)
FIO0PIN                         EQU     (FIO_BASE_ADDR + 0x14)
FIO0SET                         EQU     (FIO_BASE_ADDR + 0x18)
FIO0CLR                         EQU     (FIO_BASE_ADDR + 0x1C)

FIO1DIR                         EQU     (FIO_BASE_ADDR + 0x20) 
FIO1MASK                        EQU     (FIO_BASE_ADDR + 0x30)
FIO1PIN                         EQU     (FIO_BASE_ADDR + 0x34)
FIO1SET                         EQU     (FIO_BASE_ADDR + 0x38)
FIO1CLR                         EQU     (FIO_BASE_ADDR + 0x3C)

FIO2DIR                         EQU     (FIO_BASE_ADDR + 0x40) 
FIO2MASK                        EQU     (FIO_BASE_ADDR + 0x50)
FIO2PIN                         EQU     (FIO_BASE_ADDR + 0x54)
FIO2SET                         EQU     (FIO_BASE_ADDR + 0x58)
FIO2CLR                         EQU     (FIO_BASE_ADDR + 0x5C)

FIO3DIR                         EQU     (FIO_BASE_ADDR + 0x60) 
FIO3MASK                        EQU     (FIO_BASE_ADDR + 0x70)
FIO3PIN                         EQU     (FIO_BASE_ADDR + 0x74)
FIO3SET                         EQU     (FIO_BASE_ADDR + 0x78)
FIO3CLR                         EQU     (FIO_BASE_ADDR + 0x7C)

FIO4DIR                         EQU     (FIO_BASE_ADDR + 0x80) 
FIO4MASK                        EQU     (FIO_BASE_ADDR + 0x90)
FIO4PIN                         EQU     (FIO_BASE_ADDR + 0x94)
FIO4SET                         EQU     (FIO_BASE_ADDR + 0x98)
FIO4CLR                         EQU     (FIO_BASE_ADDR + 0x9C)

;/********************************************************************************************************
;  FIOs can be accessed through WORD, HALF-WORD or BYTE.
;********************************************************************************************************/

FIO0DIR0                        EQU     (FIO_BASE_ADDR + 0x00) 
FIO1DIR0                        EQU     (FIO_BASE_ADDR + 0x20) 
FIO2DIR0                        EQU     (FIO_BASE_ADDR + 0x40) 
FIO3DIR0                        EQU     (FIO_BASE_ADDR + 0x60) 
FIO4DIR0                        EQU     (FIO_BASE_ADDR + 0x80) 

FIO0DIR1                        EQU     (FIO_BASE_ADDR + 0x01) 
FIO1DIR1                        EQU     (FIO_BASE_ADDR + 0x21) 
FIO2DIR1                        EQU     (FIO_BASE_ADDR + 0x41) 
FIO3DIR1                        EQU     (FIO_BASE_ADDR + 0x61) 
FIO4DIR1                        EQU     (FIO_BASE_ADDR + 0x81) 

FIO0DIR2                        EQU     (FIO_BASE_ADDR + 0x02) 
FIO1DIR2                        EQU     (FIO_BASE_ADDR + 0x22) 
FIO2DIR2                        EQU     (FIO_BASE_ADDR + 0x42) 
FIO3DIR2                        EQU     (FIO_BASE_ADDR + 0x62) 
FIO4DIR2                        EQU     (FIO_BASE_ADDR + 0x82) 

FIO0DIR3                        EQU     (FIO_BASE_ADDR + 0x03) 
FIO1DIR3                        EQU     (FIO_BASE_ADDR + 0x23) 
FIO2DIR3                        EQU     (FIO_BASE_ADDR + 0x43) 
FIO3DIR3                        EQU     (FIO_BASE_ADDR + 0x63) 
FIO4DIR3                        EQU     (FIO_BASE_ADDR + 0x83) 

FIO0DIRL                        EQU     (FIO_BASE_ADDR + 0x00) 
FIO1DIRL                        EQU     (FIO_BASE_ADDR + 0x20) 
FIO2DIRL                        EQU     (FIO_BASE_ADDR + 0x40) 
FIO3DIRL                        EQU     (FIO_BASE_ADDR + 0x60) 
FIO4DIRL                        EQU     (FIO_BASE_ADDR + 0x80) 

FIO0DIRU                        EQU     (FIO_BASE_ADDR + 0x02) 
FIO1DIRU                        EQU     (FIO_BASE_ADDR + 0x22) 
FIO2DIRU                        EQU     (FIO_BASE_ADDR + 0x42) 
FIO3DIRU                        EQU     (FIO_BASE_ADDR + 0x62) 
FIO4DIRU                        EQU     (FIO_BASE_ADDR + 0x82) 

FIO0MASK0                       EQU     (FIO_BASE_ADDR + 0x10) 
FIO1MASK0                       EQU     (FIO_BASE_ADDR + 0x30) 
FIO2MASK0                       EQU     (FIO_BASE_ADDR + 0x50) 
FIO3MASK0                       EQU     (FIO_BASE_ADDR + 0x70) 
FIO4MASK0                       EQU     (FIO_BASE_ADDR + 0x90) 

FIO0MASK1                       EQU     (FIO_BASE_ADDR + 0x11) 
FIO1MASK1                       EQU     (FIO_BASE_ADDR + 0x31) 
FIO2MASK1                       EQU     (FIO_BASE_ADDR + 0x51) 
FIO3MASK1                       EQU     (FIO_BASE_ADDR + 0x71) 
FIO4MASK1                       EQU     (FIO_BASE_ADDR + 0x91) 

FIO0MASK2                       EQU     (FIO_BASE_ADDR + 0x12) 
FIO1MASK2                       EQU     (FIO_BASE_ADDR + 0x32) 
FIO2MASK2                       EQU     (FIO_BASE_ADDR + 0x52) 
FIO3MASK2                       EQU     (FIO_BASE_ADDR + 0x72) 
FIO4MASK2                       EQU     (FIO_BASE_ADDR + 0x92) 

FIO0MASK3                       EQU     (FIO_BASE_ADDR + 0x13) 
FIO1MASK3                       EQU     (FIO_BASE_ADDR + 0x33) 
FIO2MASK3                       EQU     (FIO_BASE_ADDR + 0x53) 
FIO3MASK3                       EQU     (FIO_BASE_ADDR + 0x73) 
FIO4MASK3                       EQU     (FIO_BASE_ADDR + 0x93) 

FIO0MASKL                       EQU     (FIO_BASE_ADDR + 0x10) 
FIO1MASKL                       EQU     (FIO_BASE_ADDR + 0x30) 
FIO2MASKL                       EQU     (FIO_BASE_ADDR + 0x50) 
FIO3MASKL                       EQU     (FIO_BASE_ADDR + 0x70) 
FIO4MASKL                       EQU     (FIO_BASE_ADDR + 0x90) 

FIO0MASKU                       EQU     (FIO_BASE_ADDR + 0x12) 
FIO1MASKU                       EQU     (FIO_BASE_ADDR + 0x32) 
FIO2MASKU                       EQU     (FIO_BASE_ADDR + 0x52) 
FIO3MASKU                       EQU     (FIO_BASE_ADDR + 0x72) 
FIO4MASKU                       EQU     (FIO_BASE_ADDR + 0x92) 

FIO0PIN0                        EQU     (FIO_BASE_ADDR + 0x14) 
FIO1PIN0                        EQU     (FIO_BASE_ADDR + 0x34) 
FIO2PIN0                        EQU     (FIO_BASE_ADDR + 0x54) 
FIO3PIN0                        EQU     (FIO_BASE_ADDR + 0x74) 
FIO4PIN0                        EQU     (FIO_BASE_ADDR + 0x94) 

FIO0PIN1                        EQU     (FIO_BASE_ADDR + 0x15) 
FIO1PIN1                        EQU     (FIO_BASE_ADDR + 0x35) 
FIO2PIN1                        EQU     (FIO_BASE_ADDR + 0x55) 
FIO3PIN1                        EQU     (FIO_BASE_ADDR + 0x75) 
FIO4PIN1                        EQU     (FIO_BASE_ADDR + 0x95) 

FIO0PIN2                        EQU     (FIO_BASE_ADDR + 0x16) 
FIO1PIN2                        EQU     (FIO_BASE_ADDR + 0x36) 
FIO2PIN2                        EQU     (FIO_BASE_ADDR + 0x56) 
FIO3PIN2                        EQU     (FIO_BASE_ADDR + 0x76) 
FIO4PIN2                        EQU     (FIO_BASE_ADDR + 0x96) 

FIO0PIN3                        EQU     (FIO_BASE_ADDR + 0x17) 
FIO1PIN3                        EQU     (FIO_BASE_ADDR + 0x37) 
FIO2PIN3                        EQU     (FIO_BASE_ADDR + 0x57) 
FIO3PIN3                        EQU     (FIO_BASE_ADDR + 0x77) 
FIO4PIN3                        EQU     (FIO_BASE_ADDR + 0x97) 

FIO0PINL                        EQU     (FIO_BASE_ADDR + 0x14) 
FIO1PINL                        EQU     (FIO_BASE_ADDR + 0x34) 
FIO2PINL                        EQU     (FIO_BASE_ADDR + 0x54) 
FIO3PINL                        EQU     (FIO_BASE_ADDR + 0x74) 
FIO4PINL                        EQU     (FIO_BASE_ADDR + 0x94) 

FIO0PINU                        EQU     (FIO_BASE_ADDR + 0x16) 
FIO1PINU                        EQU     (FIO_BASE_ADDR + 0x36) 
FIO2PINU                        EQU     (FIO_BASE_ADDR + 0x56) 
FIO3PINU                        EQU     (FIO_BASE_ADDR + 0x76) 
FIO4PINU                        EQU     (FIO_BASE_ADDR + 0x96) 

FIO0SET0                        EQU     (FIO_BASE_ADDR + 0x18) 
FIO1SET0                        EQU     (FIO_BASE_ADDR + 0x38) 
FIO2SET0                        EQU     (FIO_BASE_ADDR + 0x58) 
FIO3SET0                        EQU     (FIO_BASE_ADDR + 0x78) 
FIO4SET0                        EQU     (FIO_BASE_ADDR + 0x98) 

FIO0SET1                        EQU     (FIO_BASE_ADDR + 0x19) 
FIO1SET1                        EQU     (FIO_BASE_ADDR + 0x39) 
FIO2SET1                        EQU     (FIO_BASE_ADDR + 0x59) 
FIO3SET1                        EQU     (FIO_BASE_ADDR + 0x79) 
FIO4SET1                        EQU     (FIO_BASE_ADDR + 0x99) 

FIO0SET2                        EQU     (FIO_BASE_ADDR + 0x1A) 
FIO1SET2                        EQU     (FIO_BASE_ADDR + 0x3A) 
FIO2SET2                        EQU     (FIO_BASE_ADDR + 0x5A) 
FIO3SET2                        EQU     (FIO_BASE_ADDR + 0x7A) 
FIO4SET2                        EQU     (FIO_BASE_ADDR + 0x9A) 

FIO0SET3                        EQU     (FIO_BASE_ADDR + 0x1B) 
FIO1SET3                        EQU     (FIO_BASE_ADDR + 0x3B) 
FIO2SET3                        EQU     (FIO_BASE_ADDR + 0x5B) 
FIO3SET3                        EQU     (FIO_BASE_ADDR + 0x7B) 
FIO4SET3                        EQU     (FIO_BASE_ADDR + 0x9B) 

FIO0SETL                        EQU     (FIO_BASE_ADDR + 0x18) 
FIO1SETL                        EQU     (FIO_BASE_ADDR + 0x38) 
FIO2SETL                        EQU     (FIO_BASE_ADDR + 0x58) 
FIO3SETL                        EQU     (FIO_BASE_ADDR + 0x78) 
FIO4SETL                        EQU     (FIO_BASE_ADDR + 0x98) 

FIO0SETU                        EQU     (FIO_BASE_ADDR + 0x1A) 
FIO1SETU                        EQU     (FIO_BASE_ADDR + 0x3A) 
FIO2SETU                        EQU     (FIO_BASE_ADDR + 0x5A) 
FIO3SETU                        EQU     (FIO_BASE_ADDR + 0x7A) 
FIO4SETU                        EQU     (FIO_BASE_ADDR + 0x9A) 

FIO0CLR0                        EQU     (FIO_BASE_ADDR + 0x1C) 
FIO1CLR0                        EQU     (FIO_BASE_ADDR + 0x3C) 
FIO2CLR0                        EQU     (FIO_BASE_ADDR + 0x5C) 
FIO3CLR0                        EQU     (FIO_BASE_ADDR + 0x7C) 
FIO4CLR0                        EQU     (FIO_BASE_ADDR + 0x9C) 

FIO0CLR1                        EQU     (FIO_BASE_ADDR + 0x1D) 
FIO1CLR1                        EQU     (FIO_BASE_ADDR + 0x3D) 
FIO2CLR1                        EQU     (FIO_BASE_ADDR + 0x5D) 
FIO3CLR1                        EQU     (FIO_BASE_ADDR + 0x7D) 
FIO4CLR1                        EQU     (FIO_BASE_ADDR + 0x9D) 

FIO0CLR2                        EQU     (FIO_BASE_ADDR + 0x1E) 
FIO1CLR2                        EQU     (FIO_BASE_ADDR + 0x3E) 
FIO2CLR2                        EQU     (FIO_BASE_ADDR + 0x5E) 
FIO3CLR2                        EQU     (FIO_BASE_ADDR + 0x7E) 
FIO4CLR2                        EQU     (FIO_BASE_ADDR + 0x9E) 

FIO0CLR3                        EQU     (FIO_BASE_ADDR + 0x1F) 
FIO1CLR3                        EQU     (FIO_BASE_ADDR + 0x3F) 
FIO2CLR3                        EQU     (FIO_BASE_ADDR + 0x5F) 
FIO3CLR3                        EQU     (FIO_BASE_ADDR + 0x7F) 
FIO4CLR3                        EQU     (FIO_BASE_ADDR + 0x9F) 

FIO0CLRL                        EQU     (FIO_BASE_ADDR + 0x1C) 
FIO1CLRL                        EQU     (FIO_BASE_ADDR + 0x3C) 
FIO2CLRL                        EQU     (FIO_BASE_ADDR + 0x5C) 
FIO3CLRL                        EQU     (FIO_BASE_ADDR + 0x7C) 
FIO4CLRL                        EQU     (FIO_BASE_ADDR + 0x9C) 

FIO0CLRU                        EQU     (FIO_BASE_ADDR + 0x1E) 
FIO1CLRU                        EQU     (FIO_BASE_ADDR + 0x3E) 
FIO2CLRU                        EQU     (FIO_BASE_ADDR + 0x5E) 
FIO3CLRU                        EQU     (FIO_BASE_ADDR + 0x7E) 
FIO4CLRU                        EQU     (FIO_BASE_ADDR + 0x9E) 

;/********************************************************************************************************
;    System Control Block(SCB) modules include Memory Accelerator Module,
;    Phase Locked Loop, VPB divider, Power Control, External Interrupt, 
;    Reset, and Code Security/Debugging
;********************************************************************************************************/
SCB_BASE_ADDR	                EQU     0xE01FC000

;/********************************************************************************************************
;    Memory Accelerator Module (MAM)
;********************************************************************************************************/

MAMCR                           EQU     (SCB_BASE_ADDR + 0x000)
MAMTIM                          EQU     (SCB_BASE_ADDR + 0x004)
MEMMAP                          EQU     (SCB_BASE_ADDR + 0x040)

;/********************************************************************************************************
;    Phase Locked Loop (PLL)
;********************************************************************************************************/

PLLCON                          EQU     (SCB_BASE_ADDR + 0x080)
PLLCFG                          EQU     (SCB_BASE_ADDR + 0x084)
PLLSTAT                         EQU     (SCB_BASE_ADDR + 0x088)
PLLFEED                         EQU     (SCB_BASE_ADDR + 0x08C)

;/********************************************************************************************************
;    Power Control
;********************************************************************************************************/

PCON                            EQU     (SCB_BASE_ADDR + 0x0C0)
PCONP                           EQU     (SCB_BASE_ADDR + 0x0C4)

;/********************************************************************************************************
;    Clock Divider
;********************************************************************************************************/

;APBDIV                         EQU     (SCB_BASE_ADDR + 0x100)
CCLKCFG                         EQU     (SCB_BASE_ADDR + 0x104)
USBCLKCFG                       EQU     (SCB_BASE_ADDR + 0x108)
CLKSRCSEL                       EQU     (SCB_BASE_ADDR + 0x10C)
PCLKSEL0                        EQU     (SCB_BASE_ADDR + 0x1A8)
PCLKSEL1                        EQU     (SCB_BASE_ADDR + 0x1AC)

;/********************************************************************************************************
;    External Interrupts
;********************************************************************************************************/

EXTINT                          EQU     (SCB_BASE_ADDR + 0x140)
INTWAKE                         EQU     (SCB_BASE_ADDR + 0x144)
EXTMODE                         EQU     (SCB_BASE_ADDR + 0x148)
EXTPOLAR                        EQU     (SCB_BASE_ADDR + 0x14C)

;/********************************************************************************************************
;    Reset, reset source identification 
;********************************************************************************************************/

RSIR                            EQU     (SCB_BASE_ADDR + 0x180)

;/********************************************************************************************************
;    RSID, code security protection
;********************************************************************************************************/

CSPR                            EQU     (SCB_BASE_ADDR + 0x184)

;/********************************************************************************************************
;    AHB configuration
;********************************************************************************************************/

AHBCFG1                         EQU     (SCB_BASE_ADDR + 0x188)
AHBCFG2                         EQU     (SCB_BASE_ADDR + 0x18C)

;/********************************************************************************************************
;    System Controls and Status
;********************************************************************************************************/

SCS                             EQU     (SCB_BASE_ADDR + 0x1A0)	

;/********************************************************************************************************
;    External Memory Controller (EMC)
;********************************************************************************************************/
EMC_BASE_ADDR			        EQU     0xFFE08000

EMCControl				        EQU     (EMC_BASE_ADDR + 0x000)
EMCStatus				        EQU     (EMC_BASE_ADDR + 0x004)
EMCConfig				        EQU     (EMC_BASE_ADDR + 0x008)

;/********************************************************************************************************
;    Static Dynamic RAM access registers
;********************************************************************************************************/

EMCDynamicControl		        EQU     (EMC_BASE_ADDR + 0x020)
EMCDynamicRefresh		        EQU     (EMC_BASE_ADDR + 0x024)
EMCDynamicReadConfig	        EQU     (EMC_BASE_ADDR + 0x028)
EMCDynamictRP    		        EQU     (EMC_BASE_ADDR + 0x030)
EMCDynamictRAS  		        EQU     (EMC_BASE_ADDR + 0x034)
EMCDynamictSREX 		        EQU     (EMC_BASE_ADDR + 0x038)
EMCDynamictAPR   		        EQU     (EMC_BASE_ADDR + 0x03C)
EMCDynamictDAL  		        EQU     (EMC_BASE_ADDR + 0x040)
EMCDynamictWR   		        EQU     (EMC_BASE_ADDR + 0x044)
EMCDynamictRC   		        EQU     (EMC_BASE_ADDR + 0x048)
EMCDynamictRFC  		        EQU     (EMC_BASE_ADDR + 0x04C)
EMCDynamictXSR  		        EQU     (EMC_BASE_ADDR + 0x050)
EMCDynamictRRD  		        EQU     (EMC_BASE_ADDR + 0x054)
EMCDynamictMRD  		        EQU     (EMC_BASE_ADDR + 0x058)

EMCDynamicConfig0		        EQU     (EMC_BASE_ADDR + 0x100)
EMCDynamicContro1		        EQU     (EMC_BASE_ADDR + 0x120)
EMCDynamicContro2		        EQU     (EMC_BASE_ADDR + 0x140)
EMCDynamicContro3		        EQU     (EMC_BASE_ADDR + 0x160)

EMCDynamicRASCAS0		        EQU     (EMC_BASE_ADDR + 0x104)
EMCDynamicRASCAS1		        EQU     (EMC_BASE_ADDR + 0x124)
EMCDynamicRASCAS2		        EQU     (EMC_BASE_ADDR + 0x144)
EMCDynamicRASCAS3		        EQU     (EMC_BASE_ADDR + 0x164)

;/********************************************************************************************************
;    Static RAM access registers
;********************************************************************************************************/

EMCStaticConfig0		        EQU     (EMC_BASE_ADDR + 0x200)
EMCStaticWaitWen0		        EQU     (EMC_BASE_ADDR + 0x204)
EMCStaticWaitOen0		        EQU     (EMC_BASE_ADDR + 0x208)
EMCStaticWaitRd0		        EQU     (EMC_BASE_ADDR + 0x20C)
EMCStaticWaitPage0		        EQU     (EMC_BASE_ADDR + 0x210)
EMCStaticWaitWr0		        EQU     (EMC_BASE_ADDR + 0x214)
EMCStaticWaitTurn0		        EQU     (EMC_BASE_ADDR + 0x218)

EMCStaticConfig1		        EQU     (EMC_BASE_ADDR + 0x220)
EMCStaticWaitWen1		        EQU     (EMC_BASE_ADDR + 0x224)
EMCStaticWaitOen1		        EQU     (EMC_BASE_ADDR + 0x228)
EMCStaticWaitRd1		        EQU     (EMC_BASE_ADDR + 0x22C)
EMCStaticWaitPage1		        EQU     (EMC_BASE_ADDR + 0x230)
EMCStaticWaitWr1		        EQU     (EMC_BASE_ADDR + 0x234)
EMCStaticWaitTurn1		        EQU     (EMC_BASE_ADDR + 0x238)

EMCStaticConfig2		        EQU     (EMC_BASE_ADDR + 0x240)
EMCStaticWaitWen2		        EQU     (EMC_BASE_ADDR + 0x244)
EMCStaticWaitOen2		        EQU     (EMC_BASE_ADDR + 0x248)
EMCStaticWaitRd2		        EQU     (EMC_BASE_ADDR + 0x24C)
EMCStaticWaitPage2		        EQU     (EMC_BASE_ADDR + 0x250)
EMCStaticWaitWr2		        EQU     (EMC_BASE_ADDR + 0x254)
EMCStaticWaitTurn2		        EQU     (EMC_BASE_ADDR + 0x258)

EMCStaticConfig3		        EQU     (EMC_BASE_ADDR + 0x260)
EMCStaticWaitWen3		        EQU     (EMC_BASE_ADDR + 0x264)
EMCStaticWaitOen3		        EQU     (EMC_BASE_ADDR + 0x268)
EMCStaticWaitRd3		        EQU     (EMC_BASE_ADDR + 0x26C)
EMCStaticWaitPage3		        EQU     (EMC_BASE_ADDR + 0x270)
EMCStaticWaitWr3		        EQU     (EMC_BASE_ADDR + 0x274)
EMCStaticWaitTurn3		        EQU     (EMC_BASE_ADDR + 0x278)

EMCStaticExtendedWait           EQU     (EMC_BASE_ADDR + 0x880)	

;/********************************************************************************************************
;    Timer 0
;********************************************************************************************************/
TMR0_BASE_ADDR                  EQU     0xE0004000

T0IR                            EQU     (TMR0_BASE_ADDR + 0x00)
T0TCR                           EQU     (TMR0_BASE_ADDR + 0x04)
T0TC                            EQU     (TMR0_BASE_ADDR + 0x08)
T0PR                            EQU     (TMR0_BASE_ADDR + 0x0C)
T0PC                            EQU     (TMR0_BASE_ADDR + 0x10)
T0MCR                           EQU     (TMR0_BASE_ADDR + 0x14)
T0MR0                           EQU     (TMR0_BASE_ADDR + 0x18)
T0MR1                           EQU     (TMR0_BASE_ADDR + 0x1C)
T0MR2                           EQU     (TMR0_BASE_ADDR + 0x20)
T0MR3                           EQU     (TMR0_BASE_ADDR + 0x24)
T0CCR                           EQU     (TMR0_BASE_ADDR + 0x28)
T0CR0                           EQU     (TMR0_BASE_ADDR + 0x2C)
T0CR1                           EQU     (TMR0_BASE_ADDR + 0x30)
T0CR2                           EQU     (TMR0_BASE_ADDR + 0x34)
T0CR3                           EQU     (TMR0_BASE_ADDR + 0x38)
T0EMR                           EQU     (TMR0_BASE_ADDR + 0x3C)
T0CTCR                          EQU     (TMR0_BASE_ADDR + 0x70)

;/********************************************************************************************************
;    Timer 1
;********************************************************************************************************/
TMR1_BASE_ADDR	                EQU     0xE0008000

T1IR                            EQU     (TMR1_BASE_ADDR + 0x00)
T1TCR                           EQU     (TMR1_BASE_ADDR + 0x04)
T1TC                            EQU     (TMR1_BASE_ADDR + 0x08)
T1PR                            EQU     (TMR1_BASE_ADDR + 0x0C)
T1PC                            EQU     (TMR1_BASE_ADDR + 0x10)
T1MCR                           EQU     (TMR1_BASE_ADDR + 0x14)
T1MR0                           EQU     (TMR1_BASE_ADDR + 0x18)
T1MR1                           EQU     (TMR1_BASE_ADDR + 0x1C)
T1MR2                           EQU     (TMR1_BASE_ADDR + 0x20)
T1MR3                           EQU     (TMR1_BASE_ADDR + 0x24)
T1CCR                           EQU     (TMR1_BASE_ADDR + 0x28)
T1CR0                           EQU     (TMR1_BASE_ADDR + 0x2C)
T1CR1                           EQU     (TMR1_BASE_ADDR + 0x30)
T1CR2                           EQU     (TMR1_BASE_ADDR + 0x34)
T1CR3                           EQU     (TMR1_BASE_ADDR + 0x38)
T1EMR                           EQU     (TMR1_BASE_ADDR + 0x3C)
T1CTCR                          EQU     (TMR1_BASE_ADDR + 0x70)

;/********************************************************************************************************
;    Timer 2
;********************************************************************************************************/
TMR2_BASE_ADDR	                EQU     0xE0070000

T2IR                            EQU     (TMR2_BASE_ADDR + 0x00)
T2TCR                           EQU     (TMR2_BASE_ADDR + 0x04)
T2TC                            EQU     (TMR2_BASE_ADDR + 0x08)
T2PR                            EQU     (TMR2_BASE_ADDR + 0x0C)
T2PC                            EQU     (TMR2_BASE_ADDR + 0x10)
T2MCR                           EQU     (TMR2_BASE_ADDR + 0x14)
T2MR0                           EQU     (TMR2_BASE_ADDR + 0x18)
T2MR1                           EQU     (TMR2_BASE_ADDR + 0x1C)
T2MR2                           EQU     (TMR2_BASE_ADDR + 0x20)
T2MR3                           EQU     (TMR2_BASE_ADDR + 0x24)
T2CCR                           EQU     (TMR2_BASE_ADDR + 0x28)
T2CR0                           EQU     (TMR2_BASE_ADDR + 0x2C)
T2CR1                           EQU     (TMR2_BASE_ADDR + 0x30)
T2CR2                           EQU     (TMR2_BASE_ADDR + 0x34)
T2CR3                           EQU     (TMR2_BASE_ADDR + 0x38)
T2EMR                           EQU     (TMR2_BASE_ADDR + 0x3C)
T2CTCR                          EQU     (TMR2_BASE_ADDR + 0x70)

;/********************************************************************************************************
;    Timer 3
;********************************************************************************************************/
TMR3_BASE_ADDR		            EQU     0xE0074000

T3IR                            EQU     (TMR3_BASE_ADDR + 0x00)
T3TCR                           EQU     (TMR3_BASE_ADDR + 0x04)
T3TC                            EQU     (TMR3_BASE_ADDR + 0x08)
T3PR                            EQU     (TMR3_BASE_ADDR + 0x0C)
T3PC                            EQU     (TMR3_BASE_ADDR + 0x10)
T3MCR                           EQU     (TMR3_BASE_ADDR + 0x14)
T3MR0                           EQU     (TMR3_BASE_ADDR + 0x18)
T3MR1                           EQU     (TMR3_BASE_ADDR + 0x1C)
T3MR2                           EQU     (TMR3_BASE_ADDR + 0x20)
T3MR3                           EQU     (TMR3_BASE_ADDR + 0x24)
T3CCR                           EQU     (TMR3_BASE_ADDR + 0x28)
T3CR0                           EQU     (TMR3_BASE_ADDR + 0x2C)
T3CR1                           EQU     (TMR3_BASE_ADDR + 0x30)
T3CR2                           EQU     (TMR3_BASE_ADDR + 0x34)
T3CR3                           EQU     (TMR3_BASE_ADDR + 0x38)
T3EMR                           EQU     (TMR3_BASE_ADDR + 0x3C)
T3CTCR                          EQU     (TMR3_BASE_ADDR + 0x70)

;/********************************************************************************************************
;    Pulse Width Modulator (PWM)
;********************************************************************************************************/
PWM0_BASE_ADDR		            EQU     0xE0014000

PWM0IR                          EQU     (PWM0_BASE_ADDR + 0x00)
PWM0TCR                         EQU     (PWM0_BASE_ADDR + 0x04)
PWM0TC                          EQU     (PWM0_BASE_ADDR + 0x08)
PWM0PR                          EQU     (PWM0_BASE_ADDR + 0x0C)
PWM0PC                          EQU     (PWM0_BASE_ADDR + 0x10)
PWM0MCR                         EQU     (PWM0_BASE_ADDR + 0x14)
PWM0MR0                         EQU     (PWM0_BASE_ADDR + 0x18)
PWM0MR1                         EQU     (PWM0_BASE_ADDR + 0x1C)
PWM0MR2                         EQU     (PWM0_BASE_ADDR + 0x20)
PWM0MR3                         EQU     (PWM0_BASE_ADDR + 0x24)
PWM0CCR                         EQU     (PWM0_BASE_ADDR + 0x28)
PWM0CR0                         EQU     (PWM0_BASE_ADDR + 0x2C)
PWM0CR1                         EQU     (PWM0_BASE_ADDR + 0x30)
PWM0CR2                         EQU     (PWM0_BASE_ADDR + 0x34)
PWM0CR3                         EQU     (PWM0_BASE_ADDR + 0x38)
PWM0EMR                         EQU     (PWM0_BASE_ADDR + 0x3C)
PWM0MR4                         EQU     (PWM0_BASE_ADDR + 0x40)
PWM0MR5                         EQU     (PWM0_BASE_ADDR + 0x44)
PWM0MR6                         EQU     (PWM0_BASE_ADDR + 0x48)
PWM0PCR                         EQU     (PWM0_BASE_ADDR + 0x4C)
PWM0LER                         EQU     (PWM0_BASE_ADDR + 0x50)
PWM0CTCR                        EQU     (PWM0_BASE_ADDR + 0x70)

PWM1_BASE_ADDR		            EQU     0xE0018000

PWM1IR                          EQU     (PWM1_BASE_ADDR + 0x00)
PWM1TCR                         EQU     (PWM1_BASE_ADDR + 0x04)
PWM1TC                          EQU     (PWM1_BASE_ADDR + 0x08)
PWM1PR                          EQU     (PWM1_BASE_ADDR + 0x0C)
PWM1PC                          EQU     (PWM1_BASE_ADDR + 0x10)
PWM1MCR                         EQU     (PWM1_BASE_ADDR + 0x14)
PWM1MR0                         EQU     (PWM1_BASE_ADDR + 0x18)
PWM1MR1                         EQU     (PWM1_BASE_ADDR + 0x1C)
PWM1MR2                         EQU     (PWM1_BASE_ADDR + 0x20)
PWM1MR3                         EQU     (PWM1_BASE_ADDR + 0x24)
PWM1CCR                         EQU     (PWM1_BASE_ADDR + 0x28)
PWM1CR0                         EQU     (PWM1_BASE_ADDR + 0x2C)
PWM1CR1                         EQU     (PWM1_BASE_ADDR + 0x30)
PWM1CR2                         EQU     (PWM1_BASE_ADDR + 0x34)
PWM1CR3                         EQU     (PWM1_BASE_ADDR + 0x38)
PWM1EMR                         EQU     (PWM1_BASE_ADDR + 0x3C)
PWM1MR4                         EQU     (PWM1_BASE_ADDR + 0x40)
PWM1MR5                         EQU     (PWM1_BASE_ADDR + 0x44)
PWM1MR6                         EQU     (PWM1_BASE_ADDR + 0x48)
PWM1PCR                         EQU     (PWM1_BASE_ADDR + 0x4C)
PWM1LER                         EQU     (PWM1_BASE_ADDR + 0x50)
PWM1CTCR                        EQU     (PWM1_BASE_ADDR + 0x70)

;/********************************************************************************************************
;    Universal Asynchronous Receiver Transmitter 0 (UART0)
;********************************************************************************************************/
UART0_BASE_ADDR	                EQU     0xE000C000

U0RBR                           EQU     (UART0_BASE_ADDR + 0x00)
U0THR                           EQU     (UART0_BASE_ADDR + 0x00)
U0DLL                           EQU     (UART0_BASE_ADDR + 0x00)
U0DLM                           EQU     (UART0_BASE_ADDR + 0x04)
U0IER                           EQU     (UART0_BASE_ADDR + 0x04)
U0IIR                           EQU     (UART0_BASE_ADDR + 0x08)
U0FCR                           EQU     (UART0_BASE_ADDR + 0x08)
U0LCR                           EQU     (UART0_BASE_ADDR + 0x0C)

U0LSR                           EQU     (UART0_BASE_ADDR + 0x14)
U0SCR                           EQU     (UART0_BASE_ADDR + 0x1C)
U0ACR                           EQU     (UART0_BASE_ADDR + 0x20)

U0FDR                           EQU     (UART0_BASE_ADDR + 0x28)
U0TER                           EQU     (UART0_BASE_ADDR + 0x30)

;/********************************************************************************************************
;    Universal Asynchronous Receiver Transmitter 1 (UART1)
;********************************************************************************************************/
UART1_BASE_ADDR	                EQU     0xE0010000

U1RBR                           EQU     (UART1_BASE_ADDR + 0x00)
U1THR                           EQU     (UART1_BASE_ADDR + 0x00)
U1DLL                           EQU     (UART1_BASE_ADDR + 0x00)
U1DLM                           EQU     (UART1_BASE_ADDR + 0x04)
U1IER                           EQU     (UART1_BASE_ADDR + 0x04)
U1IIR                           EQU     (UART1_BASE_ADDR + 0x08)
U1FCR                           EQU     (UART1_BASE_ADDR + 0x08)
U1LCR                           EQU     (UART1_BASE_ADDR + 0x0C)
U1MCR                           EQU     (UART1_BASE_ADDR + 0x10)
U1LSR                           EQU     (UART1_BASE_ADDR + 0x14)
U1MSR                           EQU     (UART1_BASE_ADDR + 0x18)
U1SCR                           EQU     (UART1_BASE_ADDR + 0x1C)
U1ACR                           EQU     (UART1_BASE_ADDR + 0x20)

U1FDR                           EQU     (UART1_BASE_ADDR + 0x28)
U1TER                           EQU     (UART1_BASE_ADDR + 0x30)

;/********************************************************************************************************
;    Universal Asynchronous Receiver Transmitter 2 (UART2)
;********************************************************************************************************/
UART2_BASE_ADDR	                EQU     0xE0078000

U2RBR                           EQU     (UART2_BASE_ADDR + 0x00)
U2THR                           EQU     (UART2_BASE_ADDR + 0x00)
U2DLL                           EQU     (UART2_BASE_ADDR + 0x00)
U2DLM                           EQU     (UART2_BASE_ADDR + 0x04)
U2IER                           EQU     (UART2_BASE_ADDR + 0x04)
U2IIR                           EQU     (UART2_BASE_ADDR + 0x08)
U2FCR                           EQU     (UART2_BASE_ADDR + 0x08)
U2LCR                           EQU     (UART2_BASE_ADDR + 0x0C)

U2LSR                           EQU     (UART2_BASE_ADDR + 0x14)
U2SCR                           EQU     (UART2_BASE_ADDR + 0x1C)
U2ACR                           EQU     (UART2_BASE_ADDR + 0x20)

U2FDR                           EQU     (UART2_BASE_ADDR + 0x28)
U2TER                           EQU     (UART2_BASE_ADDR + 0x30)

;/********************************************************************************************************
;    Universal Asynchronous Receiver Transmitter 3 (UART3)
;********************************************************************************************************/
UART3_BASE_ADDR	                EQU     0xE007C000

U3RBR                           EQU     (UART3_BASE_ADDR + 0x00)
U3THR                           EQU     (UART3_BASE_ADDR + 0x00)
U3DLL                           EQU     (UART3_BASE_ADDR + 0x00)
U3DLM                           EQU     (UART3_BASE_ADDR + 0x04)
U3IER                           EQU     (UART3_BASE_ADDR + 0x04)
U3IIR                           EQU     (UART3_BASE_ADDR + 0x08)
U3FCR                           EQU     (UART3_BASE_ADDR + 0x08)
U3LCR                           EQU     (UART3_BASE_ADDR + 0x0C)

U3LSR                           EQU     (UART3_BASE_ADDR + 0x14)
U3SCR                           EQU     (UART3_BASE_ADDR + 0x1C)
U3ACR                           EQU     (UART3_BASE_ADDR + 0x20)
U3ICR                           EQU     (UART3_BASE_ADDR + 0x24)
U3FDR                           EQU     (UART3_BASE_ADDR + 0x28)
U3TER                           EQU     (UART3_BASE_ADDR + 0x30)

;/********************************************************************************************************
;    I2C Interface
;********************************************************************************************************/
I2C_BASE_ADDR	                EQU     0xE001C000

I2CONSET                        EQU     (I2C_BASE_ADDR + 0x00)
I2STAT                          EQU     (I2C_BASE_ADDR + 0x04)
I2DAT                           EQU     (I2C_BASE_ADDR + 0x08)
I2ADR                           EQU     (I2C_BASE_ADDR + 0x0C)
I2SCLH                          EQU     (I2C_BASE_ADDR + 0x10)
I2SCLL                          EQU     (I2C_BASE_ADDR + 0x14)
I2CONCLR                        EQU     (I2C_BASE_ADDR + 0x18)

;/********************************************************************************************************
;    I2C Interface 0
;********************************************************************************************************/
I2C0_BASE_ADDR	                EQU     0xE001C000

I20CONSET                       EQU     (I2C0_BASE_ADDR + 0x00)
I20STAT                         EQU     (I2C0_BASE_ADDR + 0x04)
I20DAT                          EQU     (I2C0_BASE_ADDR + 0x08)
I20ADR                          EQU     (I2C0_BASE_ADDR + 0x0C)
I20SCLH                         EQU     (I2C0_BASE_ADDR + 0x10)
I20SCLL                         EQU     (I2C0_BASE_ADDR + 0x14)
I20CONCLR                       EQU     (I2C0_BASE_ADDR + 0x18)

;/********************************************************************************************************
;    I2C Interface 1
;********************************************************************************************************/
I2C1_BASE_ADDR	                EQU     0xE005C000

I21CONSET                       EQU     (I2C1_BASE_ADDR + 0x00)
I21STAT                         EQU     (I2C1_BASE_ADDR + 0x04)
I21DAT                          EQU     (I2C1_BASE_ADDR + 0x08)
I21ADR                          EQU     (I2C1_BASE_ADDR + 0x0C)
I21SCLH                         EQU     (I2C1_BASE_ADDR + 0x10)
I21SCLL                         EQU     (I2C1_BASE_ADDR + 0x14)
I21CONCLR                       EQU     (I2C1_BASE_ADDR + 0x18)

;/********************************************************************************************************
;    I2C Interface 2
;********************************************************************************************************/
I2C2_BASE_ADDR	                EQU     0xE0080000

I22CONSET                       EQU     (I2C2_BASE_ADDR + 0x00)
I22STAT                         EQU     (I2C2_BASE_ADDR + 0x04)
I22DAT                          EQU     (I2C2_BASE_ADDR + 0x08)
I22ADR                          EQU     (I2C2_BASE_ADDR + 0x0C)
I22SCLH                         EQU     (I2C2_BASE_ADDR + 0x10)
I22SCLL                         EQU     (I2C2_BASE_ADDR + 0x14)
I22CONCLR                       EQU     (I2C2_BASE_ADDR + 0x18)

;/********************************************************************************************************
;    SPI0 (Serial Peripheral Interface 0)
;********************************************************************************************************/
SPI0_BASE_ADDR	                EQU     0xE0020000

S0SPCR                          EQU     (SPI0_BASE_ADDR + 0x00)
S0SPSR                          EQU     (SPI0_BASE_ADDR + 0x04)
S0SPDR                          EQU     (SPI0_BASE_ADDR + 0x08)
S0SPCCR                         EQU     (SPI0_BASE_ADDR + 0x0C)
S0SPINT                         EQU     (SPI0_BASE_ADDR + 0x1C)

;/********************************************************************************************************
;    SSP0 Controller
;********************************************************************************************************/
SSP0_BASE_ADDR	                EQU     0xE0068000

SSP0CR0                         EQU     (SSP0_BASE_ADDR + 0x00)
SSP0CR1                         EQU     (SSP0_BASE_ADDR + 0x04)
SSP0DR                          EQU     (SSP0_BASE_ADDR + 0x08)
SSP0SR                          EQU     (SSP0_BASE_ADDR + 0x0C)
SSP0CPSR                        EQU     (SSP0_BASE_ADDR + 0x10)
SSP0IMSC                        EQU     (SSP0_BASE_ADDR + 0x14)
SSP0RIS                         EQU     (SSP0_BASE_ADDR + 0x18)
SSP0MIS                         EQU     (SSP0_BASE_ADDR + 0x1C)
SSP0ICR                         EQU     (SSP0_BASE_ADDR + 0x20)
SSP0DMACR                       EQU     (SSP0_BASE_ADDR + 0x24)

;/********************************************************************************************************
;    SSP1 Controller
;********************************************************************************************************/
SSP1_BASE_ADDR	                EQU     0xE0030000

SSP1CR0                         EQU     (SSP1_BASE_ADDR + 0x00)
SSP1CR1                         EQU     (SSP1_BASE_ADDR + 0x04)
SSP1DR                          EQU     (SSP1_BASE_ADDR + 0x08)
SSP1SR                          EQU     (SSP1_BASE_ADDR + 0x0C)
SSP1CPSR                        EQU     (SSP1_BASE_ADDR + 0x10)
SSP1IMSC                        EQU     (SSP1_BASE_ADDR + 0x14)
SSP1RIS                         EQU     (SSP1_BASE_ADDR + 0x18)
SSP1MIS                         EQU     (SSP1_BASE_ADDR + 0x1C)
SSP1ICR                         EQU     (SSP1_BASE_ADDR + 0x20)
SSP1DMACR                       EQU     (SSP1_BASE_ADDR + 0x24)

;/********************************************************************************************************
;    Real Time Clock
;********************************************************************************************************/
RTC_BASE_ADDR	                EQU     0xE0024000

ILR                             EQU     (RTC_BASE_ADDR + 0x00)
CTC                             EQU     (RTC_BASE_ADDR + 0x04)
CCR                             EQU     (RTC_BASE_ADDR + 0x08)
CIIR                            EQU     (RTC_BASE_ADDR + 0x0C)
AMR                             EQU     (RTC_BASE_ADDR + 0x10)
CTIME0                          EQU     (RTC_BASE_ADDR + 0x14)
CTIME1                          EQU     (RTC_BASE_ADDR + 0x18)
CTIME2                          EQU     (RTC_BASE_ADDR + 0x1C)
SEC                             EQU     (RTC_BASE_ADDR + 0x20)
MIN                             EQU     (RTC_BASE_ADDR + 0x24)
HOUR                            EQU     (RTC_BASE_ADDR + 0x28)
DOM                             EQU     (RTC_BASE_ADDR + 0x2C)
DOW                             EQU     (RTC_BASE_ADDR + 0x30)
DOY                             EQU     (RTC_BASE_ADDR + 0x34)
MONTH                           EQU     (RTC_BASE_ADDR + 0x38)
YEAR                            EQU     (RTC_BASE_ADDR + 0x3C)
CISS                            EQU     (RTC_BASE_ADDR + 0x40)
ALSEC                           EQU     (RTC_BASE_ADDR + 0x60)
ALMIN                           EQU     (RTC_BASE_ADDR + 0x64)
ALHOUR                          EQU     (RTC_BASE_ADDR + 0x68)
ALDOM                           EQU     (RTC_BASE_ADDR + 0x6C)
ALDOW                           EQU     (RTC_BASE_ADDR + 0x70)
ALDOY                           EQU     (RTC_BASE_ADDR + 0x74)
ALMON                           EQU     (RTC_BASE_ADDR + 0x78)
ALYEAR                          EQU     (RTC_BASE_ADDR + 0x7C)
PREINT                          EQU     (RTC_BASE_ADDR + 0x80)
PREFRAC                         EQU     (RTC_BASE_ADDR + 0x84)

;/********************************************************************************************************
;    A/D Converter 0 (AD0)
;********************************************************************************************************/
AD0_BASE_ADDR	                EQU     0xE0034000

AD0CR                           EQU     (AD0_BASE_ADDR + 0x00)
AD0GDR                          EQU     (AD0_BASE_ADDR + 0x04)
AD0INTEN                        EQU     (AD0_BASE_ADDR + 0x0C)
ADDR0                           EQU     (AD0_BASE_ADDR + 0x10)
ADDR1                           EQU     (AD0_BASE_ADDR + 0x14)
ADDR2                           EQU     (AD0_BASE_ADDR + 0x18)
ADDR3                           EQU     (AD0_BASE_ADDR + 0x1C)
ADDR4                           EQU     (AD0_BASE_ADDR + 0x20)
ADDR5                           EQU     (AD0_BASE_ADDR + 0x24)
ADDR6                           EQU     (AD0_BASE_ADDR + 0x28)
ADDR7                           EQU     (AD0_BASE_ADDR + 0x2C)
AD0STAT                         EQU     (AD0_BASE_ADDR + 0x30)

;/********************************************************************************************************
;    D/A Converter
;********************************************************************************************************/
DAC_BASE_ADDR	                EQU     0xE006C000

DACR                            EQU     (DAC_BASE_ADDR + 0x00)

;/********************************************************************************************************
;    Watchdog Converter
;********************************************************************************************************/
WDG_BASE_ADDR	                EQU     0xE0000000

WDMOD                           EQU     (WDG_BASE_ADDR + 0x00)
WDTC                            EQU     (WDG_BASE_ADDR + 0x04)
WDFEED                          EQU     (WDG_BASE_ADDR + 0x08)
WDTV                            EQU     (WDG_BASE_ADDR + 0x0C)
WDCLKSEL                        EQU     (WDG_BASE_ADDR + 0x10)

;/********************************************************************************************************
;    CAN CONTROLLERS AND ACCEPTANCE FILTER 
;********************************************************************************************************/
CAN_ACCEPT_BASE_ADDR	EQU     0xE003C000

CAN_AFMR 			            EQU     (CAN_ACCEPT_BASE_ADDR + 0x00)  	
CAN_SFF_SA 			            EQU     (CAN_ACCEPT_BASE_ADDR + 0x04)  	
CAN_SFF_GRP_SA 		            EQU     (CAN_ACCEPT_BASE_ADDR + 0x08)
CAN_EFF_SA 			            EQU     (CAN_ACCEPT_BASE_ADDR + 0x0C)
CAN_EFF_GRP_SA 		            EQU     (CAN_ACCEPT_BASE_ADDR + 0x10)  	
CAN_EOT 			            EQU     (CAN_ACCEPT_BASE_ADDR + 0x14)
CAN_LUT_ERR_ADR		            EQU     (CAN_ACCEPT_BASE_ADDR + 0x18)  	
CAN_LUT_ERR 		            EQU     (CAN_ACCEPT_BASE_ADDR + 0x1C)

CAN_CENTRAL_BASE_ADDR	        EQU     0xE0040000  	

CAN_TX_SR 			            EQU     (CAN_CENTRAL_BASE_ADDR + 0x00)  	
CAN_RX_SR 			            EQU     (CAN_CENTRAL_BASE_ADDR + 0x04)  	
CAN_MSR 			            EQU     (CAN_CENTRAL_BASE_ADDR + 0x08)

CAN1_BASE_ADDR		            EQU     0xE0044000

CAN1MOD 	                    EQU     (CAN1_BASE_ADDR + 0x00)  	
CAN1CMR 	                    EQU     (CAN1_BASE_ADDR + 0x04)  	
CAN1GSR 	                    EQU     (CAN1_BASE_ADDR + 0x08)  	
CAN1ICR 	                    EQU     (CAN1_BASE_ADDR + 0x0C)  	
CAN1IER 	                    EQU     (CAN1_BASE_ADDR + 0x10)
CAN1BTR 	                    EQU     (CAN1_BASE_ADDR + 0x14)  	
CAN1EWL 	                    EQU     (CAN1_BASE_ADDR + 0x18)  	
CAN1SR 		                    EQU     (CAN1_BASE_ADDR + 0x1C)  	
CAN1RFS 	                    EQU     (CAN1_BASE_ADDR + 0x20)  	
CAN1RID 	                    EQU     (CAN1_BASE_ADDR + 0x24)
CAN1RDA 	                    EQU     (CAN1_BASE_ADDR + 0x28)  	
CAN1RDB 	                    EQU     (CAN1_BASE_ADDR + 0x2C)
  	
CAN1TFI1 	                    EQU     (CAN1_BASE_ADDR + 0x30)  	
CAN1TID1 	                    EQU     (CAN1_BASE_ADDR + 0x34)  	
CAN1TDA1 	                    EQU     (CAN1_BASE_ADDR + 0x38)
CAN1TDB1 	                    EQU     (CAN1_BASE_ADDR + 0x3C)  	
CAN1TFI2 	                    EQU     (CAN1_BASE_ADDR + 0x40)  	
CAN1TID2 	                    EQU     (CAN1_BASE_ADDR + 0x44)  	
CAN1TDA2 	                    EQU     (CAN1_BASE_ADDR + 0x48)  	
CAN1TDB2 	                    EQU     (CAN1_BASE_ADDR + 0x4C)
CAN1TFI3 	                    EQU     (CAN1_BASE_ADDR + 0x50)  	
CAN1TID3 	                    EQU     (CAN1_BASE_ADDR + 0x54)  	
CAN1TDA3 	                    EQU     (CAN1_BASE_ADDR + 0x58)  	
CAN1TDB3 	                    EQU     (CAN1_BASE_ADDR + 0x5C)

CAN2_BASE_ADDR		            EQU     0xE0048000

CAN2MOD 	                    EQU     (CAN2_BASE_ADDR + 0x00)  	
CAN2CMR 	                    EQU     (CAN2_BASE_ADDR + 0x04)  	
CAN2GSR 	                    EQU     (CAN2_BASE_ADDR + 0x08)  	
CAN2ICR 	                    EQU     (CAN2_BASE_ADDR + 0x0C)  	
CAN2IER 	                    EQU     (CAN2_BASE_ADDR + 0x10)
CAN2BTR 	                    EQU     (CAN2_BASE_ADDR + 0x14)  	
CAN2EWL 	                    EQU     (CAN2_BASE_ADDR + 0x18)  	
CAN2SR 		                    EQU     (CAN2_BASE_ADDR + 0x1C)  	
CAN2RFS 	                    EQU     (CAN2_BASE_ADDR + 0x20)  	
CAN2RID 	                    EQU     (CAN2_BASE_ADDR + 0x24)
CAN2RDA 	                    EQU     (CAN2_BASE_ADDR + 0x28)  	
CAN2RDB 	                    EQU     (CAN2_BASE_ADDR + 0x2C)
  	
CAN2TFI1 	                    EQU     (CAN2_BASE_ADDR + 0x30)  	
CAN2TID1 	                    EQU     (CAN2_BASE_ADDR + 0x34)  	
CAN2TDA1 	                    EQU     (CAN2_BASE_ADDR + 0x38)
CAN2TDB1 	                    EQU     (CAN2_BASE_ADDR + 0x3C)  	
CAN2TFI2 	                    EQU     (CAN2_BASE_ADDR + 0x40)  	
CAN2TID2 	                    EQU     (CAN2_BASE_ADDR + 0x44)  	
CAN2TDA2 	                    EQU     (CAN2_BASE_ADDR + 0x48)  	
CAN2TDB2 	                    EQU     (CAN2_BASE_ADDR + 0x4C)
CAN2TFI3 	                    EQU     (CAN2_BASE_ADDR + 0x50)  	
CAN2TID3 	                    EQU     (CAN2_BASE_ADDR + 0x54)  	
CAN2TDA3 	                    EQU     (CAN2_BASE_ADDR + 0x58)  	
CAN2TDB3 	                    EQU     (CAN2_BASE_ADDR + 0x5C)

;/********************************************************************************************************
;    MultiMedia Card Interface(MCI) Controller 
;********************************************************************************************************/
MCI_BASE_ADDR		            EQU     0xE008C000

MCIPower                        EQU     (MCI_BASE_ADDR + 0x00)
MCIClock                        EQU     (MCI_BASE_ADDR + 0x04)
MCIArgument                     EQU     (MCI_BASE_ADDR + 0x08)
MCICommand                      EQU     (MCI_BASE_ADDR + 0x0C)
MCIRespCmd                      EQU     (MCI_BASE_ADDR + 0x10)
MCIResponse0                    EQU     (MCI_BASE_ADDR + 0x14)
MCIResponse1                    EQU     (MCI_BASE_ADDR + 0x18)
MCIResponse2                    EQU     (MCI_BASE_ADDR + 0x1C)
MCIResponse3                    EQU     (MCI_BASE_ADDR + 0x20)
MCIDataTimer                    EQU     (MCI_BASE_ADDR + 0x24)
MCIDataLength                   EQU     (MCI_BASE_ADDR + 0x28)
MCIDataCtrl                     EQU     (MCI_BASE_ADDR + 0x2C)
MCIDataCnt                      EQU     (MCI_BASE_ADDR + 0x30)
MCIStatus                       EQU     (MCI_BASE_ADDR + 0x34)
MCIClear                        EQU     (MCI_BASE_ADDR + 0x38)
MCIMask0                        EQU     (MCI_BASE_ADDR + 0x3C)
MCIMask1                        EQU     (MCI_BASE_ADDR + 0x40)
MCIFifoCnt                      EQU     (MCI_BASE_ADDR + 0x48)
MCIFIFO                         EQU     (MCI_BASE_ADDR + 0x80)

;/********************************************************************************************************
;    I2S Interface Controller (I2S)
;********************************************************************************************************/
I2S_BASE_ADDR		            EQU     0xE0088000

I2SDAO                          EQU     (I2S_BASE_ADDR + 0x00)
I2SDAI                          EQU     (I2S_BASE_ADDR + 0x04)
I2STXFIFO                       EQU     (I2S_BASE_ADDR + 0x08)
I2SRXFIFO                       EQU     (I2S_BASE_ADDR + 0x0C)
I2SSTATE                        EQU     (I2S_BASE_ADDR + 0x10)
I2SDMA1                         EQU     (I2S_BASE_ADDR + 0x14)
I2SDMA2                         EQU     (I2S_BASE_ADDR + 0x18)
I2SIRQ                          EQU     (I2S_BASE_ADDR + 0x1C)
I2STXRATE                       EQU     (I2S_BASE_ADDR + 0x20)
I2SRXRATE                       EQU     (I2S_BASE_ADDR + 0x24)

;/********************************************************************************************************
;    General-purpose DMA Controller
;********************************************************************************************************/

DMA_BASE_ADDR		            EQU     0xFFE04000

DMACIntStatus                   EQU     (DMA_BASE_ADDR + 0x000)
DMACIntTCStatus                 EQU     (DMA_BASE_ADDR + 0x004)
DMACIntTCClear                  EQU     (DMA_BASE_ADDR + 0x008)
DMACIntErrorStatus              EQU     (DMA_BASE_ADDR + 0x00C)
DMACIntErrClr                   EQU     (DMA_BASE_ADDR + 0x010)
DMACRawIntTCStatus              EQU     (DMA_BASE_ADDR + 0x014)
DMACRawIntErrorStatus           EQU     (DMA_BASE_ADDR + 0x018)
DMACEnbldChns                   EQU     (DMA_BASE_ADDR + 0x01C)
DMACSoftBReq                    EQU     (DMA_BASE_ADDR + 0x020)
DMACSoftSReq                    EQU     (DMA_BASE_ADDR + 0x024)
DMACSoftLBReq                   EQU     (DMA_BASE_ADDR + 0x028)
DMACSoftLSReq                   EQU     (DMA_BASE_ADDR + 0x02C)
DMACConfiguration               EQU     (DMA_BASE_ADDR + 0x030)
DMACSync                        EQU     (DMA_BASE_ADDR + 0x034)

;/********************************************************************************************************
;    DMA channel 0 registers 
;********************************************************************************************************/

DMACC0SrcAddr                   EQU     (DMA_BASE_ADDR + 0x100)
DMACC0DestAddr                  EQU     (DMA_BASE_ADDR + 0x104)
DMACC0LLI                       EQU     (DMA_BASE_ADDR + 0x108)
DMACC0Control                   EQU     (DMA_BASE_ADDR + 0x10C)
DMACC0Configuration             EQU     (DMA_BASE_ADDR + 0x110)

;/********************************************************************************************************
;    DMA channel 1 registers 
;********************************************************************************************************/

DMACC1SrcAddr                   EQU     (DMA_BASE_ADDR + 0x120)
DMACC1DestAddr                  EQU     (DMA_BASE_ADDR + 0x124)
DMACC1LLI                       EQU     (DMA_BASE_ADDR + 0x128)
DMACC1Control                   EQU     (DMA_BASE_ADDR + 0x12C)
DMACC1Configuration             EQU     (DMA_BASE_ADDR + 0x130)

;/********************************************************************************************************
;    USB Controller
;********************************************************************************************************/

USB_INT_BASE_ADDR	            EQU      0xE01FC1C0
USB_BASE_ADDR		            EQU      0xFFE0C200		

USBClkCtrl                      EQU     (0xFFE0CFF4)  
USBClkSt                        EQU     (0xFFE0CFF8) 
USBPortSel                      EQU     (0xFFE0C110)	

USBIntSt			            EQU     (USB_INT_BASE_ADDR + 0x00)

;/********************************************************************************************************
;    USB Device Interrupt Registers
;********************************************************************************************************/

USBDevIntSt			            EQU     (USB_BASE_ADDR + 0x00)
USBDevIntEn			            EQU     (USB_BASE_ADDR + 0x04)
USBDevIntClr		            EQU     (USB_BASE_ADDR + 0x08)
USBDevIntSet		            EQU     (USB_BASE_ADDR + 0x0C)
USBDevIntPri		            EQU     (USB_BASE_ADDR + 0x2C)

;/********************************************************************************************************
;    USB Device Endpoint Interrupt Registers
;********************************************************************************************************/

USBEpIntSt			            EQU     (USB_BASE_ADDR + 0x30)
USBEpIntEn			            EQU     (USB_BASE_ADDR + 0x34)
USBEpIntClr			            EQU     (USB_BASE_ADDR + 0x38)
USBEpIntSet			            EQU     (USB_BASE_ADDR + 0x3C)
USBEpIntPri			            EQU     (USB_BASE_ADDR + 0x40)

;/********************************************************************************************************
;    USB Device Endpoint Realization Registers
;********************************************************************************************************/

USBReEp				            EQU     (USB_BASE_ADDR + 0x44)
USBEpInd			            EQU     (USB_BASE_ADDR + 0x48)
USBMaxPSize			            EQU     (USB_BASE_ADDR + 0x4C)

;/********************************************************************************************************
;    USB Device Command Reagisters
;********************************************************************************************************/
        
USBCmdCode			            EQU     (USB_BASE_ADDR + 0x10)
USBCmdData			            EQU     (USB_BASE_ADDR + 0x14)

;/********************************************************************************************************
;    USB Device Data Transfer Registers
;********************************************************************************************************/

USBRxData			            EQU     (USB_BASE_ADDR + 0x18)
USBTxData			            EQU     (USB_BASE_ADDR + 0x1C)
USBRxPLen			            EQU     (USB_BASE_ADDR + 0x20)
USBTxPLen			            EQU     (USB_BASE_ADDR + 0x24)
USBCtrl				            EQU     (USB_BASE_ADDR + 0x28)

;/********************************************************************************************************
;    USB Device DMA Registers
;********************************************************************************************************/

USBDMARSt                       EQU     (USB_BASE_ADDR + 0x50)
USBDMARClr                      EQU     (USB_BASE_ADDR + 0x54)
USBDMARSet                      EQU     (USB_BASE_ADDR + 0x58)
USBUDCAH                        EQU     (USB_BASE_ADDR + 0x80)
USBEpDMASt                      EQU     (USB_BASE_ADDR + 0x84)
USBEpDMAEn                      EQU     (USB_BASE_ADDR + 0x88)
USBEpDMADis                     EQU     (USB_BASE_ADDR + 0x8C)
USBDMAIntSt                     EQU     (USB_BASE_ADDR + 0x90)
USBDMAIntEn                     EQU     (USB_BASE_ADDR + 0x94)
USBEoTIntSt                     EQU     (USB_BASE_ADDR + 0xA0)
USBEoTIntClr                    EQU     (USB_BASE_ADDR + 0xA4)
USBEoTIntSet                    EQU     (USB_BASE_ADDR + 0xA8)
USBNDDRIntSt                    EQU     (USB_BASE_ADDR + 0xAC)
USBNDDRIntClr                   EQU     (USB_BASE_ADDR + 0xB0)
USBNDDRIntSet                   EQU     (USB_BASE_ADDR + 0xB4)
USBSysErrIntSt                  EQU     (USB_BASE_ADDR + 0xB8)
USBSysErrIntClr                 EQU     (USB_BASE_ADDR + 0xBC)
USBSysErrIntSet                 EQU     (USB_BASE_ADDR + 0xC0)


;/********************************************************************************************************
;    USB Host and OTG registers are for LPC24xx only
;   USB Host Controller
;********************************************************************************************************/

USBHC_BASE_ADDR		            EQU     0xFFE0C000

HcRevision                      EQU     (USBHC_BASE_ADDR + 0x00)
HcControl                       EQU     (USBHC_BASE_ADDR + 0x04)
HcCommandStatus                 EQU     (USBHC_BASE_ADDR + 0x08)
HcInterruptStatus               EQU     (USBHC_BASE_ADDR + 0x0C)
HcInterruptEnable               EQU     (USBHC_BASE_ADDR + 0x10)
HcInterruptDisable              EQU     (USBHC_BASE_ADDR + 0x14)
HcHcca                          EQU     (USBHC_BASE_ADDR + 0x18)
HcPeriodCurrentED               EQU     (USBHC_BASE_ADDR + 0x1C)
HcControlHeadED                 EQU     (USBHC_BASE_ADDR + 0x20)
HcControlCurrentED              EQU     (USBHC_BASE_ADDR + 0x24)
HcBulkHeadED                    EQU     (USBHC_BASE_ADDR + 0x28)
HcBulkCurrentED                 EQU     (USBHC_BASE_ADDR + 0x2C)
HcDoneHead                      EQU     (USBHC_BASE_ADDR + 0x30)
HcFmInterval                    EQU     (USBHC_BASE_ADDR + 0x34)
HcFmRemaining                   EQU     (USBHC_BASE_ADDR + 0x38)
HcFmNumber                      EQU     (USBHC_BASE_ADDR + 0x3C)
HcPeriodicStart                 EQU     (USBHC_BASE_ADDR + 0x40)
HcLSThreshold                   EQU     (USBHC_BASE_ADDR + 0x44)
HcRhDescriptorA                 EQU     (USBHC_BASE_ADDR + 0x48)
HcRhDescriptorB                 EQU     (USBHC_BASE_ADDR + 0x4C)
HcRhStatus                      EQU     (USBHC_BASE_ADDR + 0x50)
HcRhPortStatus1                 EQU     (USBHC_BASE_ADDR + 0x54)
HcRhPortStatus2                 EQU     (USBHC_BASE_ADDR + 0x58)
Module_VerRev_ID                EQU     (USBHC_BASE_ADDR + 0xFC)

;/********************************************************************************************************
;    USB OTG Controller
;********************************************************************************************************/

USBOTG_BASE_ADDR	            EQU     0xFFE0C100

OTGIntSt                        EQU     (USBOTG_BASE_ADDR + 0x00)
OTGIntEn                        EQU     (USBOTG_BASE_ADDR + 0x04)
OTGIntSet                       EQU     (USBOTG_BASE_ADDR + 0x08)
OTGIntClr                       EQU     (USBOTG_BASE_ADDR + 0x0C)

;/********************************************************************************************************
;    On LPC23xx, the name is USBPortSel, on LPC24xx, the name is OTG_STAT_CTRL
;********************************************************************************************************/

OTGStCtrl                       EQU     (USBOTG_BASE_ADDR + 0x10)
OTGTmr                          EQU     (USBOTG_BASE_ADDR + 0x14)

USBOTG_I2C_BASE_ADDR	        EQU     0xFFE0C300

I2C_RX                          EQU     (USBOTG_I2C_BASE_ADDR + 0x00)
I2C_TX                          EQU     (USBOTG_I2C_BASE_ADDR + 0x00)
I2C_STS                         EQU     (USBOTG_I2C_BASE_ADDR + 0x04)
I2C_CTL                         EQU     (USBOTG_I2C_BASE_ADDR + 0x08)
I2C_CLKHI                       EQU     (USBOTG_I2C_BASE_ADDR + 0x0C)
I2C_CLKLO                       EQU     (USBOTG_I2C_BASE_ADDR + 0x10)

;/********************************************************************************************************
;    On LPC23xx, the names are USBClkCtrl and USBClkSt; on LPC24xx, the names are 
;    OTG_CLK_CTRL and OTG_CLK_STAT respectively.
;********************************************************************************************************/

USBOTG_CLK_BASE_ADDR	        EQU     0xFFE0CFF0

OTGClkCtrl                      EQU     (USBOTG_CLK_BASE_ADDR + 0x04)
OTGClkSt                        EQU     (USBOTG_CLK_BASE_ADDR + 0x08)

;/********************************************************************************************************
;    Note: below three register name convention is for LPC23xx USB device only, match
;    with the spec. update in USB Device Section.
;********************************************************************************************************/
;USBPortSel                     EQU     (USBOTG_BASE_ADDR + 0x10)
;USBClkCtrl                     EQU     (USBOTG_CLK_BASE_ADDR + 0x04)
;USBClkSt                       EQU     (USBOTG_CLK_BASE_ADDR + 0x08)

;/********************************************************************************************************
;    Ethernet MAC (32 bit data bus) -- all registers are RW unless indicated in parentheses
;********************************************************************************************************/

MAC_BASE_ADDR		            EQU     0xFFE00000                      ;  AHB Peripheral # 0 
MAC_MAC1                        EQU     (MAC_BASE_ADDR + 0x000)        ;  MAC config reg 1 
MAC_MAC2                        EQU     (MAC_BASE_ADDR + 0x004)        ;  MAC config reg 2 
MAC_IPGT                        EQU     (MAC_BASE_ADDR + 0x008)        ;  b2b InterPacketGap reg 
MAC_IPGR                        EQU     (MAC_BASE_ADDR + 0x00C)        ;  non b2b InterPacketGap reg 
MAC_CLRT                        EQU     (MAC_BASE_ADDR + 0x010)        ;  CoLlision window/ReTry reg 
MAC_MAXF                        EQU     (MAC_BASE_ADDR + 0x014)        ;  MAXimum Frame reg 
MAC_SUPP                        EQU     (MAC_BASE_ADDR + 0x018)        ;  PHY SUPPort reg 
MAC_TEST                        EQU     (MAC_BASE_ADDR + 0x01C)        ;  TEST reg 
MAC_MCFG                        EQU     (MAC_BASE_ADDR + 0x020)        ;  MII Mgmt ConFiG reg 
MAC_MCMD                        EQU     (MAC_BASE_ADDR + 0x024)        ;  MII Mgmt CoMmanD reg 
MAC_MADR                        EQU     (MAC_BASE_ADDR + 0x028)        ;  MII Mgmt ADdRess reg 
MAC_MWTD                        EQU     (MAC_BASE_ADDR + 0x02C)        ;  MII Mgmt WriTe Data reg (WO) 
MAC_MRDD                        EQU     (MAC_BASE_ADDR + 0x030)        ;  MII Mgmt ReaD Data reg (RO) 
MAC_MIND                        EQU     (MAC_BASE_ADDR + 0x034)        ;  MII Mgmt INDicators reg (RO) 

MAC_SA0                         EQU     (MAC_BASE_ADDR + 0x040)        ;  Station Address 0 reg 
MAC_SA1                         EQU     (MAC_BASE_ADDR + 0x044)        ;  Station Address 1 reg 
MAC_SA2                         EQU     (MAC_BASE_ADDR + 0x048)        ;  Station Address 2 reg

MAC_COMMAND                     EQU     (MAC_BASE_ADDR + 0x100)        ;  Command reg 
MAC_STATUS                      EQU     (MAC_BASE_ADDR + 0x104)        ;  Status reg (RO) 
MAC_RXDESCRIPTOR                EQU     (MAC_BASE_ADDR + 0x108)        ;  Rx descriptor base address reg 
MAC_RXSTATUS                    EQU     (MAC_BASE_ADDR + 0x10C)        ;  Rx status base address reg 
MAC_RXDESCRIPTORNUM             EQU     (MAC_BASE_ADDR + 0x110)        ;  Rx number of descriptors reg 
MAC_RXPRODUCEINDEX              EQU     (MAC_BASE_ADDR + 0x114)        ;  Rx produce index reg (RO) 
MAC_RXCONSUMEINDEX              EQU     (MAC_BASE_ADDR + 0x118)        ;  Rx consume index reg 
MAC_TXDESCRIPTOR                EQU     (MAC_BASE_ADDR + 0x11C)        ;  Tx descriptor base address reg 
MAC_TXSTATUS                    EQU     (MAC_BASE_ADDR + 0x120)        ;  Tx status base address reg 
MAC_TXDESCRIPTORNUM             EQU     (MAC_BASE_ADDR + 0x124)        ;  Tx number of descriptors reg 
MAC_TXPRODUCEINDEX              EQU     (MAC_BASE_ADDR + 0x128)        ;  Tx produce index reg 
MAC_TXCONSUMEINDEX              EQU     (MAC_BASE_ADDR + 0x12C)        ;  Tx consume index reg (RO) 

MAC_TSV0                        EQU     (MAC_BASE_ADDR + 0x158)        ;  Tx status vector 0 reg (RO) 
MAC_TSV1                        EQU     (MAC_BASE_ADDR + 0x15C)        ;  Tx status vector 1 reg (RO) 
MAC_RSV                         EQU     (MAC_BASE_ADDR + 0x160)        ;  Rx status vector reg (RO) 

MAC_FLOWCONTROLCNT              EQU     (MAC_BASE_ADDR + 0x170)         ;  Flow control counter reg 
MAC_FLOWCONTROLSTS              EQU     (MAC_BASE_ADDR + 0x174)        ;  Flow control status reg 

MAC_RXFILTERCTRL                EQU     (MAC_BASE_ADDR + 0x200)        ;  Rx filter ctrl reg 
MAC_RXFILTERWOLSTS              EQU     (MAC_BASE_ADDR + 0x204)        ;  Rx filter WoL status reg (RO) 
MAC_RXFILTERWOLCLR              EQU     (MAC_BASE_ADDR + 0x208)        ;  Rx filter WoL clear reg (WO) 

MAC_HASHFILTERL                 EQU     (MAC_BASE_ADDR + 0x210)        ;  Hash filter LSBs reg 
MAC_HASHFILTERH                 EQU     (MAC_BASE_ADDR + 0x214)        ;  Hash filter MSBs reg

MAC_INTSTATUS                   EQU     (MAC_BASE_ADDR + 0xFE0)        ;  Interrupt status reg (RO) 
MAC_INTENABLE                   EQU     (MAC_BASE_ADDR + 0xFE4)        ;  Interrupt enable reg  
MAC_INTCLEAR                    EQU     (MAC_BASE_ADDR + 0xFE8)        ;  Interrupt clear reg (WO) 
MAC_INTSET                      EQU     (MAC_BASE_ADDR + 0xFEC)        ;  Interrupt set reg (WO) 

MAC_POWERDOWN                   EQU     (MAC_BASE_ADDR + 0xFF4)        ;  Power-down reg 
;MAC_MODULEID                   EQU     (MAC_BASE_ADDR + 0xFFC)        ;  Module ID reg (RO) 

;/********************************************************************************************************
;    Define firmware Functions
;********************************************************************************************************/

rm_init_entry                   EQU     0x7fffff91
rm_undef_handler                EQU     0x7fffffa0
rm_prefetchabort_handler        EQU     0x7fffffb0
rm_dataabort_handler            EQU     0x7fffffc0
rm_irqhandler                   EQU     0x7fffffd0
rm_irqhandler2                  EQU     0x7fffffe0
iap_entry                       EQU     0x7ffffff1

    END
;/********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/