/*
 *********************************************************************************************************
 *                                               uC/OS-II
 *                                         The Real-Time Kernel
 *
 *
 *                                (c) Copyright 2006, Micrium, Weston, FL
 *                                          All Rights Reserved
 *
 *                                           	ARM 7 Port
 *
 * File      : OS_CPU_A.S
 * Version   : V2.86
 * By        : Jean J. Labrosse
 *
 * For       : ARMv4
 * Mode      : ARM
 * Toolchain : RealView Development Suite
 *             RealView Microcontroller Development Kit (MDK)
 *             ARM Developer Suite (ADS)
 *             Keil uVision
 *********************************************************************************************************
 */


/*
 *********************************************************************************************************
 *										External references
 *********************************************************************************************************
 */
			.extern  OSRunning                   
			.extern  OSPrioCur
			.extern  OSPrioHighRdy
			.extern  OSTCBCur
			.extern  OSTCBHighRdy
			.extern  OSIntNesting
			.extern  OSIntExit
			.extern  OSTaskSwHook

/*			
 *********************************************************************************************************
 *									Functions declared in this file
 *********************************************************************************************************
 */
            .globl  OS_CPU_SR_Save         
            .globl  OS_CPU_SR_Restore
            .globl  OSStartHighRdy
            .globl  OSCtxSw
            .globl  OSIntCtxSw

/*
 *********************************************************************************************************
 *												Config
 *********************************************************************************************************
 */
#define OS_TASK_SW_HOOK			0

/*
 *********************************************************************************************************
 *									ARM Core Mode and Status Bits
 *********************************************************************************************************
 */
#define ARM_MODE_USER	0x10
#define ARM_MODE_FIQ	0x11
#define ARM_MODE_IRQ	0x12
#define ARM_MODE_SVC	0x13
#define ARM_MODE_ABORT	0x17
#define ARM_MODE_UNDEF	0x1B
#define ARM_MODE_SYS	0x1F

#define I_BIT			0x80
#define F_BIT			0x40
#define T_BIT			0x20

/*
 *********************************************************************************************************
 *									CRITICAL SECTION METHOD 3 FUNCTIONS
 * Prototypes :     OS_CPU_SR  OS_CPU_SR_Save(void);
 *                  void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
 *********************************************************************************************************
 */
        .text

OS_CPU_SR_Save:
        MRS     R0, CPSR                    // Set IRQ and FIQ bits in CPSR to disable all interrupts
        ORR     R1, R0, #(I_BIT | F_BIT)
        MSR     CPSR_c, R1
        BX      LR                          // return the original CPSR contents in R0

OS_CPU_SR_Restore:
        MSR     CPSR_c,R0
        BX      LR

/*
*********************************************************************************************************
										START MULTITASKING
									void OSStartHighRdy(void)
*********************************************************************************************************
*/

        .text

OSStartHighRdy:

#if (OS_TASK_SW_HOOK >= 1)
        BL		OS_TaskSwHook     	  	// OSTaskSwHook();
#endif

        MSR     CPSR_c, #(I_BIT | F_BIT | ARM_MODE_SYS)	// Switch to SYS mode with IRQ and FIQ disabled

        LDR     R4, OS_Running          // OSRunning = TRUE
        MOV     R5, #1
        STRB    R5, [R4]

                                        // SWITCH TO HIGHEST PRIORITY TASK
        LDR     R4, OS_TCBHighRdy       //    Get highest priority task TCB address
        LDR     R6, [R4]                //    get stack pointer

        B       OSCtxSw2

/*
 *********************************************************************************************************
 *							PERFORM A CONTEXT SWITCH (From task level) - OSCtxSw()
 *********************************************************************************************************
 */
        .text

OSCtxSw:
                                        // SAVE CURRENT TASK'S CONTEXT
        STMFD   SP!, {LR}               //     Push return address
        STMFD   SP!, {R0-R12, LR}       //     Push registers
        MRS     R4,  CPSR               //     Push current CPSR
        STMFD   SP!, {R4}

OSCtxSw1:
        LDR     R4, OS_TCBCur         	// OSTCBCur->OSTCBStkPtr = SP;
        LDR     R5, [R4]
        STR     SP, [R5]

#if (OS_TASK_SW_HOOK >= 1)
        BL		OS_TaskSwHook     	  	// OSTaskSwHook();
#endif

        LDR     R4, OS_PrioCur        	// OSPrioCur = OSPrioHighRdy
        LDR     R5, OS_PrioHighRdy
        LDRB    R6, [R5]
        STRB    R6, [R4]

        LDR     R4, OS_TCBCur         	// OSTCBCur  = OSTCBHighRdy;
        LDR     R6, OS_TCBHighRdy
        LDR     R6, [R6]
        STR     R6, [R4]

OSCtxSw2:
        LDR     R4, [R6]                // R4(SP) = OSTCBHighRdy->OSTCBStkPtr;
        ADD     SP, R4, #16*4           // Adjust SYS stack pointer
        LDR     LR, [SP, #-2*4]         // get new task's register LR_sys
        MSR     CPSR_c, #(I_BIT | F_BIT | ARM_MODE_UNDEF)	// Switch to UNDEF mode
        MOV     SP, R4
                                        // RESTORE NEW TASK'S CONTEXT
        LDMFD   SP!, {R4}               //    Pop new task's CPSR
        MSR     SPSR_cxsf, R4           // has SPSR only at UNDEF mode
        LDMFD   SP!, {R0-R12,LR,PC}^    //    pop new task's context

/*
 *********************************************************************************************************
 *						PERFORM A CONTEXT SWITCH (From interrupt level) - OSIntCtxSw()
 *********************************************************************************************************
 */
        .text

OSIntCtxSw:
        LDR     R2, [SP, #4*5]                              // 从IRQ栈获取PC(中断返回地址)
        LDR     R12, [SP, #4*4]                             // 从IRQ栈获取获取R12
        MRS     R0, CPSR							        // 等下还要回到IRQ模式

        MSR     CPSR_c, #(I_BIT | F_BIT | ARM_MODE_SYS)		// 切换到SYS系统模式并禁止IRQ/FIQ
        MOV     R1, LR
        STMFD   SP!, {R1-R2}                                // 保存LR,PC到当前任务的栈里面(此时的系统模式栈使用的是被中断的任务栈)
        STMFD   SP!, {R4-R12}                               // 保存R4-R12到当前任务的栈里面

        MSR     CPSR_c, R0							        // 切换回IRQ模式
        LDMFD   SP!, {R4-R7}                                // 从IRQ栈获取R0-R3
        ADD     SP, SP, #4*2                                // 出栈R12,PC(此次不需要获取内核，之前已经获取了r12及pc的值，此次仅恢复IRQ栈即可)
        
        MSR     CPSR_c, #(I_BIT | F_BIT | ARM_MODE_SYS)		// 切换到SYS系统模式
        STMFD   SP!, {R4-R7}                                // 保存R0-R3到当前任务的栈里面
        
        STMFD   SP!, {R3}                       	        // 保存CPSR到当前任务的栈里面(此时当前任务栈保存的内容为: cpsr, r0-r12, lr, pc)

		B		OSCtxSw1

/*
 *********************************************************************************************************
 *                                     POINTERS TO VARIABLES
 *********************************************************************************************************
 */
#if (OS_TASK_SW_HOOK >= 1)
OS_TaskSwHook:
        .word	OSTaskSwHook
#endif

OS_IntExit:
        .word	OSIntExit

OS_IntNesting:
        .word	OSIntNesting

OS_PrioCur:
        .word	OSPrioCur

OS_PrioHighRdy:
        .word	OSPrioHighRdy

OS_Running:
        .word	OSRunning

OS_TCBCur:
        .word	OSTCBCur

OS_TCBHighRdy:
        .word	OSTCBHighRdy

 
/*       
 *********************************************************************************************************
 *                                     END OF FILE
 *********************************************************************************************************
 */


