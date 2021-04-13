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
* File      : OS_CPU.H
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

#ifndef  OS_CPU_H
#define  OS_CPU_H


#ifdef   OS_CPU_GLOBALS
#define  OS_CPU_EXT
#else
#define  OS_CPU_EXT  extern
#endif

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

typedef unsigned int   OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef unsigned int   OS_CPU_SR;                /* Define size of CPU status register (PSR = 32 bits) */

/*
*********************************************************************************************************
*                                           	ARM 7
*                                      Critical Section Management
*********************************************************************************************************
*/

#define  OS_CRITICAL_METHOD   3


#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}

/*
*********************************************************************************************************
*                                         ARM Miscellaneous
*********************************************************************************************************
*/

#define  OS_STK_GROWTH        1                   /* Stack grows from HIGH to LOW memory on ARM        */

#define  OS_TASK_SW()         OSCtxSw()

#define  ARM_SYS_MODE    	  (0x0000001FL)

//任务运行在系统模式
#define  TASK_RUN_MODE   	  ARM_SYS_MODE

/*
*********************************************************************************************************
*                                              PROTOTYPES
*                                           (see OS_CPU_A.S)
*********************************************************************************************************
*/

#if OS_CRITICAL_METHOD == 3
OS_CPU_SR  OS_CPU_SR_Save(void);
void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
#endif

void       OSCtxSw(void);
void       OSStartHighRdy(void);

//中断退出后并不立即做任务切换, 将在中断句柄里实现任务切换, 所以定义它为空
//void       OSIntCtxSw(void);
#define    OSIntCtxSw()


#endif
