;
;********************************************************************************************************
;                                                uC/OS-II
;                                          The Real-Time Kernel
;                                        interrupt remap and macro
;
;                          (c) Copyright 1992-2001, Jean J. Labrosse, Weston, FL
;                                           All Rights Reserved
;
; File : os_int_a.s
; By   : Jean J. Labrosse
;********************************************************************************************************

                PRESERVE8
                

        AREA  UCOS_ARM_ASM_INT, CODE, READONLY
           

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
        IMPORT  _IntCtxSw

  
        MACRO     
$IRQ_AsmEntery HANDLER $IRQ_CEntry
        EXPORT  $IRQ_AsmEntery              ; ����ı��
        IMPORT  $IRQ_CEntry                 ; ���õ��ⲿ���
                
$IRQ_AsmEntery
        stmfd sp!,{r0-r3,r12,lr}

        bl OSIntEnter
        bl $IRQ_CEntry
        bl OSIntExit

        ldr r0,=OSIntCtxSwFlag
        ldr r1,[r0]
        cmp r1,#1
        beq _IntCtxSw

        ldmfd sp!,{r0-r3,r12,lr}
        subs pc,lr,#4
        
        MEND
 
;���е��жϰ����µĸ�ʽ��ע��(��������ĺ궨��)
;$IRQ_AsmEntery HANDLER $IRQ_CEntry


;��ʱ��0�ж�
IRQASMTimer0  HANDLER IRQ_C_Timer0

;IRQ�������ж�
;IRQASMHandler HANDLER IRQCHandler
IRQ_Handler HANDLER IRQ_C_Handler

;FIQ�������ж�
;FIQASMHandler HANDLER FIQCHandler
FIQ_Handler HANDLER FIQ_C_Handler

		END
   
                        
