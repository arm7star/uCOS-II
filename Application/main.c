/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                             (c) Copyright 1998-2004, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                            ARM7 Sample Code
*
* File : APP.C
* By   : Eric Shufro
*********************************************************************************************************
*/

#include "ucos_ii.h"

/*
*********************************************************************************************************
*                                                CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE    128
#define  TASK_START_PRIO    5

/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

OS_STK        AppStartTaskStk[TASK_STK_SIZE];

/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppStartTask(void *p_arg);
static  void  Task0(void *p_arg);
static  void  Task1(void *p_arg);
static  void  Task2(void *p_arg);

OS_STK  Task0Stk [256];
OS_STK  Task1Stk [256];
OS_STK  Task2Stk [256];

#if OS_VIEW_MODULE > 0
static  void  AppTerminalRx(INT8U rx_data);
#endif

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  
* Arguments   : none
*********************************************************************************************************
*/

void main(int argc, char *argv[])
{
    INT8U  err;


#if 0
    BSP_IntDisAll();                       /* For an embedded target, disable all interrupts until we are ready to accept them */
#endif

    sp804_init();

    OSInit();                              /* Initialize "uC/OS-II, The Real-Time Kernel"                                      */

    OSTaskCreateExt(AppStartTask,
                    (void *)0,
                    (OS_STK *)&AppStartTaskStk[TASK_STK_SIZE-1],
                    TASK_START_PRIO,
                    TASK_START_PRIO,
                    (OS_STK *)&AppStartTaskStk[0],
                    TASK_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
					
    OSTaskCreate (Task0,(void *)0, &Task0Stk[256 - 1], TASK_START_PRIO - 1);
    OSTaskCreate (Task1,(void *)0, &Task1Stk[256 - 1], TASK_START_PRIO - 2);
    OSTaskCreate (Task2,(void *)0, &Task2Stk[256 - 1], TASK_START_PRIO - 3);

#if OS_TASK_NAME_SIZE > 11
    OSTaskNameSet(APP_TASK_START_PRIO, (INT8U *)"Start Task", &err);
#endif

#if OS_TASK_NAME_SIZE > 14
    OSTaskNameSet(OS_IDLE_PRIO, (INT8U *)"uC/OS-II Idle", &err);
#endif

#if (OS_TASK_NAME_SIZE > 14) && (OS_TASK_STAT_EN > 0)
    OSTaskNameSet(OS_STAT_PRIO, "uC/OS-II Stat", &err);
#endif

    OSStart();                             /* Start multitasking (i.e. give control to uC/OS-II)                               */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/

void  AppStartTask (void *p_arg)
{
    p_arg = p_arg;
	unsigned int i = 0;
#if OS_TASK_STAT_EN > 0
    OSStatInit();                                /* Determine CPU capacity                                                     */
#endif
    
    while (1)                                   /* Task body, always written as an infinite loop.                             */
    {
        printf("Delay 1 second and print\n");   /* your code here. Create more tasks, etc.                                    */
        OSTimeDlyHMSM(0, 0, 1, 0);       
    }
}

void Task0(void *pdata)
{
    static int tsk0_i;
    tsk0_i = 0;
    while (1)
    {
        printf("this is from task0 %d\n", tsk0_i++);
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

void Task1(void *pdata)
{
    static int tsk1_i;
    tsk1_i = 0;
    while (1)
    {
        printf("this is from task1 %d\n", tsk1_i++);
        OSTimeDlyHMSM(0, 0, 2, 0);
    }
}

void Task2(void *pdata)
{
    static int tsk2_i;
    tsk2_i = 0;
    while (1)
    {
        printf("this is from task2 %d\n", tsk2_i++);
        OSTimeDlyHMSM(0, 0, 3, 0);
    }
}
