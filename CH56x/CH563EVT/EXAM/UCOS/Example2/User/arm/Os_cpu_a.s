;
;********************************************************************************************************
;                                                uC/OS-II
;                                          The Real-Time Kernel
;                                      primary task switch asm code
;
;                          (c) Copyright 1992-2001, Jean J. Labrosse, Weston, FL
;                                           All Rights Reserved
;
; File : os_cpu_a.s
; By   : Jean J. Labrosse
;********************************************************************************************************

                PRESERVE8
                

        AREA  UCOS_ARM, CODE, READONLY

;*********************************************************************************************************
;                                          START MULTITASKING
;                                       void OSStartHighRdy(void)
;
; Note : OSStartHighRdy() MUST:
;           a) Call OSTaskSwHook() then,
;           b) Set OSRunning to TRUE,
;           c) Switch to the highest priority task.
; �˺�������osstart��������������֮�󣬸����������ȼ������tcb���ƿ��л�ø�����Ķ�ջָ��sp,
; ͨ��sp ���ν�cpu �ֳ��ָ�����ʱϵͳ�ͽ�����Ȩ�����û������ĸ�������̣� 
; ֱ���������������߱������������ȼ���������ռcpu�� 
; �ú��������ڶ���������ʱ��ִ��һ�Σ�����������һ����Ҳ����������ȼ�������ִ�У� 
; ֮�������ĵ��Ⱥ��л�����������ĺ�����ʵ�֡� 
;*********************************************************************************************************

        IMPORT  OSTaskSwHook
        IMPORT  OSRunning
        IMPORT  OSTCBHighRdy

        EXPORT  OSStartHighRdy

OSStartHighRdy  

        bl OSTaskSwHook             ; Call user defined task switch hook

        ldr r4,=OSRunning           ; Indicate that multitasking has started
        mov r5,#1
        strb r5,[r4]

        ldr r4,=OSTCBHighRdy        ; Get highest priority task TCB address

        ldr r4,[r4]                 ; get stack pointer
        ldr sp,[r4]                 ; switch to the new stack

        ldmfd sp!,{r4}              ; pop new task's spsr
        msr SPSR_cxsf,r4
        ldmfd sp!,{r4}              ; pop new task's psr
        msr CPSR_cxsf,r4
        ldmfd sp!,{r0-r12,lr,pc}    ; pop new task's r0-r12,lr & pc

;*********************************************************************************************************
;                                PERFORM A CONTEXT SWITCH (From task level)
;                                           void OSCtxSw(void)
;
; Note(s):    Upon entry, 
;             OSTCBCur     points to the OS_TCB of the task to suspend
;             OSTCBHighRdy points to the OS_TCB of the task to resume
;
; ���񼶵��������л������ǵ�������Ϊ����������������cpu ����ʱ��ִ�У� 
; ���ڴ�ʱ�������л������ڷ��쳣ģʽ�½��еģ�����������жϼ���������л���   
; ���Ĺ������Ƚ���ǰ�����cpu �ֳ����浽�������ջ�У�Ȼ����������ȼ�����Ķ�ջָ�룬	 
; �Ӹö�ջ�лָ��������cpu �ֳ���ʹ֮����ִ�С������������һ�������л���	 
;*********************************************************************************************************

        IMPORT  OSTCBCur
        IMPORT  OSTaskSwHook
        IMPORT  OSTCBHighRdy
        IMPORT  OSPrioCur
        IMPORT  OSPrioHighRdy

        EXPORT  OSCtxSw

OSCtxSw
; Special optimised code below:
        stmfd sp!,{lr}              ; push pc (lr should be pushed in place of PC)
        stmfd sp!,{r0-r12,lr}       ; push lr & register file
        mrs r4,cpsr
        stmfd sp!,{r4}              ; push current psr
        mrs r4,spsr
        stmfd sp!,{r4}              ; push current spsr

        ; OSPrioCur = OSPrioHighRdy
        ldr r4,=OSPrioCur
        ldr r5,=OSPrioHighRdy
        ldrb r6,[r5]
        strb r6,[r4]
        
        ; Get current task TCB address
        ldr r4,=OSTCBCur
        ldr r5,[r4]
        str sp,[r5]                 ; store sp in preempted tasks's TCB

        bl OSTaskSwHook             ; call Task Switch Hook

        ; Get highest priority task TCB address
        ldr r6,=OSTCBHighRdy
        ldr r6,[r6]
        ldr sp,[r6]                 ; get new task's stack pointer

        ; OSTCBCur = OSTCBHighRdy
        str r6,[r4]                 ; set new current task TCB address

        ldmfd sp!,{r4}              ; pop new task's spsr
        msr SPSR_cxsf,r4
        ldmfd sp!,{r4}              ; pop new task's psr
        msr CPSR_cxsf,r4
        ldmfd sp!,{r0-r12,lr,pc}    ; pop new task's r0-r12,lr & pc


;*********************************************************************************************************
;                                PERFORM A CONTEXT SWITCH (From an ISR)
;                                        void OSIntCtxSw(void)
;
; Note(s): This function only flags a context switch to the ISR Handler
;
; �жϼ��������л���������ʱ���ж�isr���жϷ������̣��з����и����ȼ�����ȴ���ʱ���źŵ�����   
; ����Ҫ���ж��˳��󲢲����ر��ж����񣬶���ֱ�ӵ��Ⱦ����ĸ����ȼ�����ִ�С�		    
; ��������Ŀ����Ҫ���ܹ�������ø����ȼ�������õ���Ӧ����֤ϵͳ��ʵʱ���ܡ�		  
; ����ԭ������������񼶵��л���ͬ���������ڽ����ж�ʱ�Ѿ�������˱��ж������cpu �ֳ���	 
; �������Ͳ����ٽ������ƵĲ�����ֻ��Ҫ�Զ�ջָ������Ӧ�ĵ�����ԭ���Ǻ�����Ƕ�ס�  
;*********************************************************************************************************

        IMPORT  OSIntCtxSwFlag

        EXPORT  OSIntCtxSw

OSIntCtxSw


        ;OSIntCtxSwFlag = True
        ldr r0,=OSIntCtxSwFlag
        mov r1,#1
        str r1,[r0]
        ;mov pc,lr
		BX	LR
        

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

NOINT   EQU 0xc0


_IntCtxSw
        mov r1,#0
        str r1,[r0]

        ldmfd sp!,{r0-r3,r12,lr}
        stmfd sp!,{r0-r3}
        mov r1,sp
        add sp,sp,#16
        sub r2,lr,#4

        mrs r3,spsr
        orr r0,r3,#NOINT
        msr spsr_c,r0

        ldr r0,=.+8
        movs pc,r0

        stmfd sp!,{r2}              ; push old task's pc
        stmfd sp!,{r4-r12,lr}       ; push old task's lr,r12-r4
        mov r4,r1                   ; Special optimised code below
        mov r5,r3
        ldmfd r4!,{r0-r3}
        stmfd sp!,{r0-r3}           ; push old task's r3-r0
        stmfd sp!,{r5}              ; push old task's psr
        mrs r4,spsr
        stmfd sp!,{r4}              ; push old task's spsr
        
        ; OSPrioCur = OSPrioHighRdy
        ldr r4,=OSPrioCur
        ldr r5,=OSPrioHighRdy
        ldrb r5,[r5]
        strb r5,[r4]
        
        ; Get current task TCB address
        ldr r4,=OSTCBCur
        ldr r5,[r4]
        str sp,[r5]                 ; store sp in preempted tasks's TCB

        bl OSTaskSwHook             ; call Task Switch Hook

        ; Get highest priority task TCB address
        ldr r6,=OSTCBHighRdy
        ldr r6,[r6]
        ldr sp,[r6]                 ; get new task's stack pointer

        ; OSTCBCur = OSTCBHighRdy
        str r6,[r4]                 ; set new current task TCB address

        ldmfd sp!,{r4}              ; pop new task's spsr
        msr SPSR_cxsf,r4
        ldmfd sp!,{r4}              ; pop new task's psr
        msr CPSR_cxsf,r4

        ldmfd sp!,{r0-r12,lr,pc}    ; pop new task's r0-r12,lr & pc

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
;*********************************************************************************************************

        EXPORT  OSCPUSaveSR
OSCPUSaveSR

        mrs r0,CPSR
        orr r1,r0,#NOINT
        msr CPSR_c,r1
        MRS R1,CPSR                     ; Confirm that CPSR contains the proper interrupt disable flags
        AND R1,R1,#NOINT
        CMP R1,#NOINT
        BNE OSCPUSaveSR              ; Not properly disabled (try again)
        ;mov pc,lr
		BX	LR


        EXPORT  OSCPURestoreSR
OSCPURestoreSR

        msr CPSR_c,r0
        ;mov pc,lr
		BX	LR


        END