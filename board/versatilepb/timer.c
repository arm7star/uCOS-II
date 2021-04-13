/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*
*                                (c) Copyright 2006, Micrium, Weston, FL
*                                          All Rights Reserved
*
*                                               ARM 7 Port
*
* File      : timer.c
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

#include "os_cfg.h"

#define SP804_T1_LOAD       (*((volatile unsigned int *) 0x101e3000))
#define SP804_T1_VALUE      (*((volatile unsigned int *) 0x101e3004))
#define SP804_T1_CTRL       (*((volatile unsigned int *) 0x101e3008))
#define SP804_T1_INTCTRL    (*((volatile unsigned int *) 0x101e300c))
#define SP804_T1_RIS        (*((volatile unsigned int *) 0x101e3010))
#define SP804_T1_MIS        (*((volatile unsigned int *) 0x101e3014))
#define SP804_T1_BGLOAD     (*((volatile unsigned int *) 0x101e3018))

#define VIC_INT_ENABLE 		(*((volatile unsigned int *) 0x10140010))

/*
;*****************************************************************************************************
;* �������� : Timer0_Exception
;* ��    �� : ��ʱ��0�жϷ����ӳ��� 
;* �䡡    �� : ��
;*        
;* �䡡    �� : ��
;*****************************************************************************************************
;*/
void Timer0_Exception(void)
{  
    sp804_clr_interrupt();	/* clear interrupt flag MR0 */
    irq_enable();
    OSTimeTick();
}

/*
;*****************************************************************************************************
;* �������� : sp804_clr_interrupt
;* ��    �� : �����ʱ���ж�
;* �䡡    �� : ��
;*        
;* �䡡    �� : ��
;*****************************************************************************************************
;*/
void sp804_clr_interrupt(void)
{
	SP804_T1_INTCTRL = 0;	/* clear interrupt flag MR0 */
}

/*
;*****************************************************************************************************
;* �������� : sp804_init
;* ��    �� : ��ʼ����ʱ��
;* �䡡    �� : ��
;*        
;* �䡡    �� : ��
;*****************************************************************************************************
;*/
void sp804_init(void)
{
    SP804_T1_CTRL = 0;
    SP804_T1_BGLOAD = 0;
    SP804_T1_LOAD = 1000000 / 1000 * 10;
    SP804_T1_CTRL = 0xe2;
	irq_enable();
	unmask_irq(5);
}

/*
;*****************************************************************************************************
;*                                      End Of File
;*****************************************************************************************************
;*/
