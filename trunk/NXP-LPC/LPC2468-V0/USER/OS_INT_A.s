                PRESERVE8
        		AREA  	UCOS_ARM_ASM_INT,CODE,READONLY           

;*********************************************************************************************************
;                                            IRQ HANDLER
;
;        This handles all the IRQs
;        Note: FIQ Handler should be written similar to this
;
;*********************************************************************************************************

;In ¦ÌC/OS-II, ISRs includes several parts: save CPU registers, call function OSIntEnter(), 
;execute user code, call function OSIntExit(), restore CPU registers and return.
;Function OSIntEnter() is used to notify ¦ÌC/OS-II that you are about to service an interrupt (ISR) 
;and function OSIntExit() is used to notify ¦ÌC/OS-II that you have completed serving an ISR. 
;With OSIntEnter() and OSIntExit(), ¦ÌC/OS-II can keep track of interrupt nesting and thus only perform rescheduling at the last nested ISR.
;Sometime when the last nested ISR is completed, the interrupted task will be no longer the task that needs to run because a new,
; higher-priority task is now ready. In this case, interrupt level context switch is needed. This is done by function _IntCtxSw(). 
; Then after return, the new, higher-priority task is running and the old one is pending.
;These codes should be written in assembly language because you cannot access CPU registers directly from C.
; But user code can be written in C. Here we use macro code to implement ISR in file irq_handler.s. 
; The code can be shown as following and should be duplicated for each ISR you have in your system.

        		IMPORT	OSIntEnter
        		IMPORT  OSIntExit
        		IMPORT  OSIntCtxSwFlag
        		IMPORT  _IntCtxSw  
        		MACRO     
$IRQ_AsmEntery 	HANDLER $IRQ_CEntry
        		EXPORT  $IRQ_AsmEntery 
        		IMPORT  $IRQ_CEntry                 
$IRQ_AsmEntery  STMFD 	SP!,{R0-R3,R12,LR}
        		BL 		OSIntEnter          ; Interrupt Nest++
        		BL 		$IRQ_CEntry         ; User ISR Subroutine
        		BL 		OSIntExit
        		LDR 	R0,=OSIntCtxSwFlag
        		LDR 	R1,[R0]
        		CMP 	R1,#1
        		BEQ 	_IntCtxSw          ; interrupt level context switch
        		LDMFD 	SP!,{R0-R3,R12,LR}
        		SUBS 	PC,LR,#4           ; return    
        		MEND 

IRQASMTimer0  	HANDLER IRQCTimer0
IRQASMTimer3  	HANDLER IRQCTimer3
IRQASMUart0  	HANDLER uart0Isr
IRQASMUart3  	HANDLER uart3Isr

        		END
