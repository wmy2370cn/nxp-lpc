                PRESERVE8
				AREA  	UCOS_ARM,CODE,READONLY

;*********************************************************************************************************
;                                          START MULTITASKING
;                                       void OSStartHighRdy(void)
;
; Note : 	OSStartHighRdy() MUST:
;           a) Call OSTaskSwHook() then,
;           b) Set OSRunning to TRUE,
;           c) Switch to the highest priority task.
;
;*********************************************************************************************************

        		IMPORT	OSTaskSwHook
        		IMPORT  OSRunning
        		IMPORT  OSTCBHighRdy
        		EXPORT  OSStartHighRdy
OSStartHighRdy
        		BL 		OSTaskSwHook        ; Call user defined task switch hook
        		LDR 	R4,=OSRunning       ; Indicate that multitasking has started
        		MOV 	R5,#1
        		STRB 	R5,[R4]
        		LDR 	r4,=OSTCBHighRdy    ; Get highest priority task TCB address
        		LDR 	R4,[R4]             ; get stack pointer
        		LDR 	SP,[R4]             ; switch to the new stack
        		LDMFD 	SP!,{R4}            ; pop new task's SPSR
        		MSR 	SPSR_cxsf,R4
        		LDMFD 	SP!,{R4}            ; pop new task's PSR
        		MSR 	CPSR_cxsf,R4
        		LDMFD 	SP!,{R0-R12,LR,PC}	; pop new task's R0-R12,LR & PC

;*********************************************************************************************************
;                                PERFORM A CONTEXT SWITCH (From task level)
;                                           void OSCtxSw(void)
;
; Note(s):    Upon entry, 
;             OSTCBCur     points to the OS_TCB of the task to suspend
;             OSTCBHighRdy points to the OS_TCB of the task to resume
;
;	 
;*********************************************************************************************************

        		IMPORT	OSTCBCur
        		IMPORT  OSTaskSwHook
        		IMPORT  OSTCBHighRdy
        		IMPORT  OSPrioCur
        		IMPORT  OSPrioHighRdy
        		EXPORT  OSCtxSw
OSCtxSw										; Special optimised code below:
        		STMFD 	SP!,{LR}        	; push PC (lr should be pushed in place of PC)
        		STMFD 	SP!,{R0-R12,LR}     ; push LR & register file
        		MRS 	R4,CPSR
        		STMFD 	SP!,{R4}            ; push current PSR
        		MRS 	R4,SPSR
        		STMFD 	SP!,{R4}            ; push current SPSR        									
        		LDR 	R4,=OSPrioCur		; OSPrioCur = OSPrioHighRdy
        		LDR 	R5,=OSPrioHighRdy
        		LDRB 	R6,[R5]
        		STRB 	R6,[R4]			
        		LDR 	R4,=OSTCBCur		; Get current task TCB address
        		LDR 	R5,[R4]
        		STR 	SP,[R5]             ; store sp in preempted tasks's TCB
        		BL 		OSTaskSwHook        ; call Task Switch Hook											
        		LDR 	R6,=OSTCBHighRdy	; Get highest priority task TCB address
        		LDR 	R6,[R6]
        		LDR 	SP,[R6]             ; get new task's stack pointer
        		STR 	R6,[R4]             ; set new current task TCB address
        		LDMFD 	SP!,{R4}            ; pop new task's SPSR
        		LDMFD 	SP!,{R4}            ; pop new task's PSR
        		MSR 	CPSR_cxsf,R4
        		LDMFD 	SP!,{R0-R12,LR,PC}  ; pop new task's R0-R12,LR & PC

;*********************************************************************************************************
;                                PERFORM A CONTEXT SWITCH (From an ISR)
;                                        void OSIntCtxSw(void)
;
; Note(s): This function only flags a context switch to the ISR Handler
;  
;*********************************************************************************************************

        		IMPORT  OSIntCtxSwFlag
        		EXPORT  OSIntCtxSw
OSIntCtxSw        
        		LDR 	R0,=OSIntCtxSwFlag	;OSIntCtxSwFlag = True
        		MOV 	R1,#1
        		STR 	R1,[R0]     
				BX		LR

;*********************************************************************************************************
;                                            IRQ HANDLER
;
;        This handles all the IRQs
;        Note: FIQ Handler should be written similar to this
;
;*********************************************************************************************************
        		IMPORT  OSIntEnter
        		IMPORT  OSIntExit
        		IMPORT  OSIntCtxSwFlag
        		IMPORT  OSTCBCur
        		IMPORT  OSTaskSwHook
        		IMPORT  OSTCBHighRdy
        		IMPORT  OSPrioCur
        		IMPORT  OSPrioHighRdy
        		EXPORT  _IntCtxSw
NOINT			EQU 	0xC0
_IntCtxSw
        		MOV 	R1,#0
        		STR 	R1,[R0]
        		LDMFD 	SP!,{R0-R3,R12,LR}
        		STMFD 	SP!,{R0-R3}
        		MOV 	R1,SP
        		ADD 	SP,SP,#16
        		SUB 	R2,LR,#4
        		MRS 	R3,SPSR
        		ORR 	R0,R3,#NOINT
        		MSR 	SPSR_c,R0
        		LDR 	R0,=.+8
				BX		R0
				MRS		R0,SPSR
				MSR		CPSR_cxsf,R0
        		STMFD 	SP!,{R2}			; push old task's PC
        		STMFD 	SP!,{R4-R12,LR}     ; push old task's LR,R12-R4
        		MOV 	R4,R1               ; Special optimised code below
        		MOV 	R5,R3
        		LDMFD 	R4!,{R0-R3}
        		STMFD 	SP!,{R0-R3}         ; push old task's R3-R0
        		STMFD 	SP!,{R5}            ; push old task's PSR
        		MRS 	R4,SPSR
        		STMFD 	SP!,{R4}            ; push old task's SPSR        
        		LDR 	R4,=OSPrioCur		; OSPrioCur = OSPrioHighRdy
        		LDR 	R5,=OSPrioHighRdy
        		LDRB 	R5,[R5]
        		STRB 	R5,[R4]        
        		LDR 	R4,=OSTCBCur		; Get current task TCB address
        		LDR 	R5,[R4]
        		STR 	SP,[R5]             ; store sp in preempted tasks's TCB
       		 	BL 		OSTaskSwHook        ; call Task Switch Hook        
        		LDR 	R6,=OSTCBHighRdy	; Get highest priority task TCB address
        		LDR 	R6,[R6]
        		LDR 	SP,[R6]             ; get new task's stack pointer
        		STR 	R6,[R4]             ; set new current task TCB address
        		LDMFD 	SP!,{R4}            ; pop new task's SPSR
        		MSR 	SPSR_cxsf,r4
        		LDMFD 	SP!,{R4}            ; pop new task's PSR
        		MSR 	CPSR_cxsf,R4
        		LDMFD 	SP!,{R0-R12,LR,PC}  ; pop new task's R0-R12,LR & PC

;*********************************************************************************************************
;                                   CRITICAL SECTION METHOD 3 FUNCTIONS
;
; Description: Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
;              would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
;              disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to 
;              disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
;              into the CPU's status register.
;
;              OS_CPU_SR OSCPUSaveSR()
; Arguments  : none
;
; Returns    : OS_CPU_SR
;
;              OSCPURestoreSR(OS_CPU_SR cpu_sr)
; Arguments  : OS_CPU_SR
;
; Returns    : none
;
; Note(s)    : These functions are used in general like this,
;
;            void Task (void *data)
;            {
;                    #if OS_CRITICAL_METHOD == 3 /* Allocate storage for CPU status register */
;                          OS_CPU_SR  cpu_sr;
;                    #endif
;                         :
;                         :
;                    OS_ENTER_CRITICAL(); /* cpu_sr = OSCPUSaveSR(); */
;                         :
;                         :
;                    OS_EXIT_CRITICAL();  /* OSCPURestoreSR(cpu_sr); */
;                         :
;                         :
;            }
;
;*********************************************************************************************************

        		EXPORT	OSCPUSaveSR
OSCPUSaveSR
        		MRS 	R0,CPSR
        		ORR 	R1,R0,#NOINT
        		MSR 	CPSR_c,R1
        		MRS 	R1,CPSR				; Confirm that CPSR contains the proper interrupt disable flags
        		AND 	R1,R1,#NOINT
        		CMP 	R1,#NOINT
        		BNE 	OSCPUSaveSR         ; Not properly disabled (try again)
        		BX		LR
        		EXPORT  OSCPURestoreSR
OSCPURestoreSR
        		MSR 	CPSR_c,R0
        		BX		LR
        		END
