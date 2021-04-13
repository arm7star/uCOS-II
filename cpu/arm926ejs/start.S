/*
 *************************************************************************
 *
 * Startup Code (reset vector)
 *
 * do important init only if we don't start from memory!
 * setup Memory and board specific bits prior to relocation.
 * relocate armboot to ram
 * setup stack
 *
 *************************************************************************
 */
.global _start
_start:
        ldr pc, ResetAddr
        ldr pc, _undefined_instruction
        ldr pc, _software_interrupt
        ldr pc, _prefetch_abort
        ldr pc, _data_abort
        ldr pc, _not_used
        ldr pc, _irq
        ldr pc, _fiq

ResetAddr:
        .word reset
_undefined_instruction:
        .word undefined_instruction
_software_interrupt:
        .word software_interrupt
_prefetch_abort:
        .word prefetch_abort
_data_abort:
        .word data_abort
_not_used:
        .word 0x12345678
_irq:
        .word irq
_fiq:
        .word fiq
_pad:
        .word 0x12345678 /* now 16*4=64 */

.global _end_vect
_end_vect:
        .balignl 16,0xdeadbeef

.global _TEXT_BASE
_TEXT_BASE :
        .word   TEXT_BASE

.global _DATA_BASE
_DATA_BASE :
        .word   DATA_BASE


.global __start
__start :
        .word _start

/*
 * These are defined in the board-specific linker script.
 */
.global _data_start
_data_start :
        .word __data_start

.global _bss_start
_bss_start :
        .word __bss_start

.global _bss_end
_bss_end :
        .word _end

/* Reset entry */
reset:
        /*
         * set the cpu to SVC32 mode
         */
        mrs     r0, cpsr
        bic     r0, r0, #0x1f
        orr     r0, r0, #(0x13 | 0x80)
        msr     cpsr, r0

        /*
         * Initialize stack and change to System mode
         */
        ldr    sp, =__stack_svc_end__       /* Pickup __stack_svc_end__ value to sp        */
        msr    cpsr_c, #0xd2                /* Switch to Interrupt Request mode (IRQ)      */
        ldr    sp, =__stack_irq_end__       /* Pickup __stack_irq_end__ value to sp        */
        msr    cpsr_c, #0xd1                /* Switch to Fast Interrupt Request mode (FIQ) */
        ldr    sp, =__stack_fiq_end__       /* Pickup __stack_fiq_end__ value to sp        */
        msr    cpsr_c, #0xd7                /* Switch to Data Abort mode (ABT)             */
        ldr    sp, =__stack_abt_end__       /* Pickup __stack_abt_end__ value to sp        */
        msr    cpsr_c, #0xdb                /* Switch to Undefined Instruction mode (UND)  */
        ldr    sp, =__stack_und_end__       /* Pickup __stack_und_end__ value to sp        */
        msr    cpsr_c, #0xdf                /* Switch to System mode (SYS)                 */
        ldr    sp, =__stack_sys_end__       /* Pickup __stack_sys_end__ value to sp        */

        /*
         * Go setup Memory and board specific bits prior to relocation.
         */
        bl  lowlevel_init   /* go setup pll,mux,memory */

        /* Clear bss Section */
clear_bss:
        ldr     r0, _bss_start              /* find start of bss segment        */
        ldr     r1, _bss_end                /* stop here                        */
        mov     r2, #0x00000000             /* clear                            */

clbss_l:
        str     r2, [r0]                    /* clear loop...                    */
        add     r0, r0, #4                  /* Set r0 to next bss address       */
        cmp     r0, r1                      /* Check if r0 to the _bss_end      */
        ble     clbss_l                     /* Continue to clear bss ram        */

        /*
         * Jump to C entry
         */
        mov     lr, pc
        ldr     pc, __main
        b       .

__main:
    .word main

/*
 *************************************************************************
 *
 * Interrupt Code
 *
 *************************************************************************
 */

/*
 * IRQ stack frame.
 */
#define S_FRAME_SIZE    72

#define S_OLD_R0        68
#define S_PSR           64
#define S_PC            60
#define S_LR            56
#define S_SP            52

#define S_IP            48
#define S_FP            44
#define S_R10           40
#define S_R9            36
#define S_R8            32
#define S_R7            28
#define S_R6            24
#define S_R5            20
#define S_R4            16
#define S_R3            12
#define S_R2            8
#define S_R1            4
#define S_R0            0

/*
 * use bad_save_user_regs for abort/prefetch/undef/swi ...
 * use irq_save_user_regs / irq_restore_user_regs for IRQ/FIQ handling
 */
.macro  bad_save_user_regs
        sub    sp, sp, #S_FRAME_SIZE
        stmia  sp, {r0 - r12}

        add    r0, sp, #S_FRAME_SIZE
        mov    r1, lr
        mov    r2, pc
        mrs    r3, spsr
        add    r5, sp, #S_SP
        stmia  r5, {r0 - r3}
        mov    r0, sp
.endm


/*
 * Undefined Instruction interrupt
 */
        .align  5
undefined_instruction:
        bad_save_user_regs
        b      undefined_instruction

/*
 * Software interrupt
 */
        .align  5
software_interrupt:
        bad_save_user_regs
        b   software_interrupt

/*
 * Pre fetch abort interrupt
 */
        .align  5
prefetch_abort:
        bad_save_user_regs
        b   prefetch_abort

/*
 * Data abort interrupt
 */
        .align  5
data_abort:
        bad_save_user_regs
        b   data_abort

        .align  5
not_used:
        b   .

/*
 * IRQ interrupt
 */
        .align  5
irq:
        sub    lr, lr, #4               // 调整返回地址
        stmfd  sp!, {r0-r3, r12, lr}    // 保存任务环境 (中断前SYS系统模式的r0-r3, r12, pc保存到IRQ栈里面；被调用的函数执行时不会保存/恢复r0-r3，由上级函数保存，被调用函数只保存/恢复其它会修改的寄存器，因此r0-r3, r12(ip), pc之外的寄存器暂时不需要保存)
        mrs    r3, spsr                 // 保存中断前的程序状态寄存器cpsr_sys
        stmfd  sp!, {r3, sp, lr}^       // 保存系统模式的r3,sp,lr(中断前系统模式的cpsr, sp, lr保存到IRQ栈里面)

        msr    cpsr_c, #0xdf            // 切换到SYS系统模式，并禁止IRQ/FIQ

        ldr    r2, =OSIntNesting        // OSIntNesting++
        ldrb   r1, [r2]
        add    r1, r1, #1
        strb   r1, [r2]
        cmp    r1, #1                   // 第一次进入IRQ中断
        ldreq  sp, =__stack_isr_end__   // 则调整sp = __stack_isr_end__, 中断处理程序使用特定的堆栈空间(中断处理程序运行在系统模式，与任务运行在一个模式下，不能使用任务的栈!!!)

        bl     Timer0_Exception         // 执行中断处理程序(定时器中断处理使能IRQ，允许中断嵌套!!!)
        msr    cpsr_c, #0xdf            // 禁止IRQ/FIQ(之前已经切换到了SYS系统模式，此处仅起禁止IRQ/FIQ的作用)
        bl     OSIntExit                // 中断退出处理(OSIntNesting减1，如果没有中断嵌套，则更新OSTCBHighRdy；有中断嵌套，则不检查(优先处理被打断的中断处理程序))

        msr    cpsr_c, #0xd2            // 切换回IRQ模式，并禁止IRQ/FIQ

        ldr    r0, =OSTCBHighRdy        // if (OSTCBHighRdy != OSTCBCur)
        ldr    r0, [r0]
        ldr    r1, =OSTCBCur
        ldr    r1, [r1]
        cmp    r0, r1

        ldmfd  sp!, {r3, sp, lr}^       // 恢复SYS系统模式的r3,sp,lr(中断前的cpsr恢复到r3寄存器，虽然是恢复到系统模式寄存器，但是系统模式/中断模式r3寄存器是共用的)
        msr    spsr_cxsf, r3            // r3 -> spsr_cxsf，下一条指令带有pc寄存器，会将spsr恢复到cpsr即切换cpu模式
        ldmeqfd sp!, {r0-r3, r12, pc}^  // 不用进行任务切换，恢复任务环境(OSTCBHighRdy == OSTCBCur 任务不需要切换或者有中断嵌套没有更新OSTCBHighRdy)
        ldr    pc, =OSIntCtxSw          // 进行任务切换(此时保存在IRQ栈里面的内容为{r0-r3, r12, lr(pc_sys)})

/*
 * FIQ interrupt
 */
        .align  5
fiq:
        stmfd  sp!, {r0-r3,r12,lr}
        // bl     do_fiq
        ldmfd  sp!, {r0-r3,r12,lr}
        subs   pc,  lr,  #4

/*
 *************************************************************************
 *
 * IRQ/FIQ enable/disable
 *
 *************************************************************************
 */

#define LOCKOUT    0xC0                 // Interrupt lockout value
#define MODE_MASK  0x1F                 // Processor Mode Mask
#define MODE_SVC   0x13                 // Supervisor Mode (SVC)

#define I_BIT      0x80                 // Interrupt bit of CPSR and SPSR
#define F_BIT      0x40                 // Interrupt bit of CPSR and SPSR

/*
 * Enable IRQ interrupt
 */
        .align  5
.global irq_enable
irq_enable:
        mrs    r0, cpsr
        bic    r0, r0, #I_BIT
        msr    cpsr_c, r0
        mov    pc, lr

/*
 * Disable IRQ interrupt
 */
        .align  5
.global irq_disable
irq_disable:
        mrs    r0, cpsr
        orr    r0, r0, #I_BIT
        msr    cpsr_c, r0
        mov    pc, lr

/*
 * Disable FIQ interrupt
 */
        .align  5
fiq_disable:
        mrs    r0, spsr
        orr    r0, r0, #F_BIT
        msr    spsr_c, r0
        movs   pc, lr

/*
 * Enable FIQ interrupt
 */
        .align  5
fiq_enable:
        mrs    r0, spsr
        bic    r0, r0, #F_BIT
        msr    spsr_c, r0
        movs   pc, lr

