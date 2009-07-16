;/****************************************Copyright (c)***************************************************
;**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
;**                                     
;**                               http://www.embedtools.com
;**
;**--------------File Info-------------------------------------------------------------------------------
;** File name: 			vicControl.s
;** Last modified Date: 2007-06-12
;** Last Version: 		1.0
;** Descriptions: 		Provide VIC_Control in prerogative mode
;**------------------------------------------------------------------------------------------------------
;** Created by: 		LinEnqiang
;** Created date:   	2007-06-12
;** Version:			1.0
;** Descriptions: 		The original version
;**
;**------------------------------------------------------------------------------------------------------
;** Modified by: 		LinEnqiang
;** Modified date:		2008-04-30
;** Version:			1.1
;** Descriptions:       统一于代码规范V1.2
;********************************************************************************************************/
                                 INCLUDE     LPC2400.INC                ; Include the head file 引入头文件

;/********************************************************************************************************
; 宏定义
;********************************************************************************************************/

NoInt       EQU 0x80
NoFIQ		EQU	0x40

SVC32Mode   EQU 0x13
SYS32Mode   EQU 0x1f


		;	IMPORT  StackUsr 
		
	 		EXPORT  RunFirst
	 		EXPORT  vicControl
 PRESERVE8 			 	
	CODE32
    AREA    |RUNFIRST|, CODE, READONLY     

;/********************************************************************************************************
;** Function name:           RunFirst
;** Descriptions:            VIC管理初始化
;** input parameters:        none
;** output parameters:       none
;** Returned value:          none
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
		
RunFirst			
		LDR     R2, =VICIntEnClr
		MVN     R3, #0x00
		STR	    R3, [R2] 
		LDR     R2, =VICIntSelect
		MOV     R3, #0x00
		STR	    R3, [R2] 

        LDR     R0, =VICVectAddr0
        MOV     R1, #0
        MOV     R2, #0
        MOV     R3, #0
        MOV     R4, #0
        MOV     R5, #0
        MOV     R6, #0
        MOV     R7, #0
        MOV     R8, #0
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}
        LDR     R0, =VICVectPri0
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}
        STMIA   R0!, {R1-R8}    		
		
		LDR     R2, =VICVectAddr
		STR	    R3, [R2] 
		LDR     R2, =VICProtection
        MOV     R3, #0x01
        STR	    R3, [R2]              
		;MOV     PC, LR
		BX      LR 
;/********************************************************************************************************
;** Function name:           vicControl
;** Descriptions:            软件中断，用于提供VIC管理服务
;** input parameters:        依功能而定
;** output parameters:       依功能而定
;** Returned value:          依功能而定
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;********************************************************************************************************/
vicControl 
        SUB     R0, R0, #0x100
        CMP     R0, #7
        LDRLO   PC, [PC, R0, LSL #2]
        MOVS    PC, LR
        
SwiFunctionAdd
        DCD     SetvicIrqFunc                                           ;  0
        DCD     ClrvicIrqFunc                                           ;  1
        DCD     EnablevicIrq                                            ;  2
        DCD     DisablevicIrq                                           ;  3
        DCD     GetvicIrqStatus                                         ;  4
        DCD     SetvicFiq                                               ;  5
        DCD     ClrvicFiq                                               ;  6 

;/*********************************************************************************************************
;** Function name:           SetvicIrqFunc
;** Descriptions:            设置所选外设的中断优先级、中断服务函数地址，并使能中断
;** input parameters:        R0:         外设对应的中断通道号
;**                          R1:         中断优先级
;**                          R2:         中断服务函数地址
;** output parameters:       none
;** Returned value:          1:          成功
;**                          0:          失败
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************/

SetvicIrqFunc
        CMP     R1, #32                                                 ;  if (通道号 >=32) return FALSE
        MOVCS   R0, #0
        MOVCSS  PC, LR
        CMP     R2, #16                                                 ;  if (优先级 >=16) return FALSE
        MOVCS   R0, #0 
        MOVCSS  PC, LR 
        CMP     R3, #0                                                  ;  if (处理函数 ==0) return FALSE
        MOVEQ   R0, #0
        MOVEQS  PC, LR
		     
 		MSR     CPSR_c, #(NoFIQ | NoInt | SYS32Mode)
        STMFD   SP!, {R2, R3}
		MOV     R2, #1                                                  ;  if (Enable) return FALSE
        MOV     R3, R2, lsl R1 
       
        LDR     R0, =VICIntEnable
        LDR     R2, [R0]
        ANDS    R2, R2, R3
        BNE     SetvicIrqFunc_j
        
        LDR     R0, =VICVectAddr0                                       ;  if (IRQ已经使能) return FALSE
        LDR     R3, [R0, R1, lsl #2]
        CMP     R3, #0         
SetvicIrqFunc_j
        LDMFD   SP!, {R2, R3}
        MSR     CPSR_c, #(NoFIQ | NoInt | SVC32Mode)
        MOVNE   R0, #0
        MOVNES  PC, LR 
        
        STR     R3, [R0, R1, lsl #2]
        LDR     R0, =VICVectPri0
        STR     R2, [R0, R1, lsl #2] 
        
        LDR     R0, =VICIntEnable
        MOV     R2, #1
        MOV     R3, R2, lsl R1 
        STR     R3, [R0]
                                              
        MOV     R0, #1
        MOVS    PC, LR   
;/*********************************************************************************************************
;** Function name:           ClrvicIrqFunc
;** Descriptions:            清除所选外设的IRQ资源
;** input parameters:        R0:        外设对应的中断通道号
;** output parameters:       none
;** Returned value:          1:         成功
;**                          0:         失败
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************/

ClrvicIrqFunc
        CMP     R1, #32 						                        ;  if (通道号 >=32) return FALSE
        MOVCS   R0, #0
		MOVCSS  PC, LR
		
		LDR     R0, =VICIntSelect      			                        ;  if (FIQ) return FALSE
        MOV     R3, #1
        MOV     R3, R3, LSL R1 
        LDR     R2, [R0]
        ANDS    R2, R2, R3        
        MOVNE   R0, #0
		MOVNES  PC, LR
        
        LDR     R0, =VICVectAddr0				                        ;  if (IRQ wasnt Set) return FALSE
        LDR     R2,[R0,R1,LSL #2] 
        CMP     R2,#0
        MOVEQ   R0, #0
		MOVEQS  PC, LR
                
		LDR     R0, =VICIntEnClr       			                        ;  Disable IRQ
        STR     R3, [R0]             
        
        LDR     R0, =VICVectAddr0      			                        ;  Clear VICVectAddr0
        MOV     R3, #0
        STR     R3, [R0, R1, lsl #2]        
        MOV     R0, #1        
		MOVS    PC, LR
;/*********************************************************************************************************
;** Function name:           EnablevicIrq
;** Descriptions:            使能相应外设的中断
;** input parameters:        R0:         外设对应的中断通道号
;** output parameters:       none
;** Returned value:          1:          成功
;**                          0:          失败
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************/

EnablevicIrq
        CMP     R1, #32                                                 ;  if (通道号 >=32) return FALSE
        MOVCS   R0, #0
        MOVCSS  PC, LR 
        
        LDR     R0, =VICVectAddr0                                       ;  if (未加载) return FALSE
        LDR     R3, [R0, R1, lsl #2]
        CMP     R3, #0
        MOVEQ   R0, #0
        MOVEQS  PC, LR
		
		LDR     R0, =VICIntEnable
        MOV     R2, #1
        MOV     R3, R2, lsl R1 
        STR     R3, [R0]
        MOV     R0, #1
        MOVS    PC, LR                

;/*********************************************************************************************************
;** Function name:           DisablevicIrq
;** Descriptions:            禁止相应外设的中断
;** input parameters:        uiChannel:  外设对应的中断通道号
;** output parameters:       none
;** Returned value:          1:          成功
;**                          0:          失败
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

DisablevicIrq
        CMP     R1, #32                                                 ;  if (通道号 >=32) return FALSE
        MOVCS   R0, #0
        MOVCSS  PC, LR 
        
        LDR     R0, =VICVectAddr0                                       ;  if (未加载) return FALSE
        LDR     R3, [R0, R1, lsl #2]
        CMP     R3, #0
        MOVEQ   R0, #0
        MOVEQS  PC, LR
        
        LDR     R0, =VICIntEnClr
        MOV     R2, #1
        MOV     R3, R2, lsl R1 
        STR     R3, [R0]
        MOV     R0, #1
        MOVS    PC, LR
;/*********************************************************************************************************
;** Function name:           GetvicIrqStatus
;** Descriptions:            获取所选外设的中断通道号、优先级、中断服务函数地址及中断使能状态
;** input parameters:        R0:         外设对应的中断通道号
;** output parameters:       R2:         配置信息
;** Returned value:          1:          成功
;**                          0:          失败
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
;*********************************************************************************************************/

GetvicIrqStatus
        CMP     R1, #32							                        ;  if (通道号 >=32) return FALSE
        MOVCS   R0, #0
		MOVCSS  PC, LR
        
        LDR     R0, =VICVectAddr0				                        ;  if (未加载) return FALSE
        LDR     R3, [R0, R1, lsl #2]
        CMP     R3, #0
        MOVEQ   R0, #0
		MOVEQS  PC, LR       
        STMIA   R2!, {R1}						                        ;  通道号

        LDR     R0, =VICVectPri0
        LDR     R0, [R0, R1, lsl #2]
        STMIA   R2!, {R0}       				                        ;  优先级
        STMIA   R2!, {R3}						                        ;  处理函数地址

        LDR     R0, =VICIntEnable
        MOV     R3, #1
        MOV     R1, R3, lsl R1 
        LDR     R3, [R0]
        AND     R3, R3, R1            
        CMP     R3, #0
        MOVHI   R3, #1
        STMIA   R2!,{R3}						                        ;  IRQ使能与否的状态
        
		MOV     R0, #1
        MOVS    PC, LR        
;/*********************************************************************************************************
;** Function name:           SetvicFiq
;** Descriptions:            设置并使能所选中断通道号为FIQ中断
;** input parameters:        R0:        外设对应的中断通道号
;** output parameters:       none
;** Returned value:          1:          成功
;**                          0:          失败
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
SetvicFiq
       	MSR     CPSR_c, #(NoFIQ | NoInt| SVC32Mode)
       	
        CMP     R1, #32							                        ;  if (通道号 >=32) return FALSE
        MOVCS   R0, #0
		MOVCSS  PC, LR
        
        LDR     R0, =VICVectAddr0				                        ;  if (IRQ已加载) return FALSE
        LDR     R3,[R0,R1,LSL #2] 
        CMP     R3,#0
        MOVNE   R0, #0
		MOVNES  PC, LR
        
        LDR     R0, =VICIntSelect				
        LDR     R3, [R0]
        MOV     R2, #1
        MOV     R2, R2, lsl R1 
        ORR     R3, R3, R2                
        STR     R3, [R0]
        
        LDR     R0, =VICIntEnable
        STR     R3, [R0]
        MOV     R0, #1
        MOVS    PC, LR
;/*********************************************************************************************************
;** Function name:           ClrvicFiq
;** Descriptions:            清除所选中断通道号的FIQ中断
;** input parameters:        R0:         外设对应的中断通道号
;** output parameters:       none
;** Returned value:          1:          成功
;**                          0:          失败;
;** Created by:              LinEnqiang
;** Created Date:            2008/04/30
;**-------------------------------------------------------------------------------------------------------
;** Modified by:
;** Modified date:
;**-------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

ClrvicFiq
		MSR     CPSR_c, #(NoFIQ | NoInt| SVC32Mode)
		
        CMP     R1, #32							                        ;  if (通道号 >=32) return FALSE
        MOVCS   R0, #0
		MOVCSS  PC, LR       
             
        LDR     R0, =VICIntSelect				                        ;  if(FIQ未使能)return FALSE
        LDR     R3, [R0]
        MOV     R2, #1
        MOV     R2, R2, LSL R1        
        ANDS    R2, R3, R2        
        MOVEQ   R0, #0
		MOVEQS  PC, LR         
       
        LDR     R0, =VICIntEnClr
        MOV     R3, R2 
        STR     R3, [R0]  						                        ;  Disable FIQ
        
        LDR     R0, =VICIntSelect
        LDR     R3, [R0]
        BIC     R3, R3,R2
        STR     R3, [R0]						       
        
        MOV     R0, #1
        MOVS    PC, LR            
                 
    END
;/********************************************************************************************************
;**                            End Of File
;********************************************************************************************************/