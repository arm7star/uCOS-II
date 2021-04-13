/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                  uC/OS-II Configuration File for V2.8x
*
*                               (c) Copyright 2005-2007, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
* File    : OS_CFG.H
* By      : Jean J. Labrosse
* Version : V2.86
*
* LICENSING TERMS:
* ---------------
*   uC/OS-II is provided in source form for FREE evaluation, for educational use or for peaceful research.
* If you plan on using  uC/OS-II  in a commercial product you need to contact Micri祄 to properly license
* its use in your product. We provide ALL the source code for your convenience and to help you experience
* uC/OS-II.   The fact that the  source is provided does  NOT  mean that you can use it without  paying a
* licensing fee.
*********************************************************************************************************
*/

#ifndef OS_CFG_H
#define OS_CFG_H


                                       /* ---------------------- MISCELLANEOUS ----------------------- */
//应用程序勾子函数                      
#define OS_APP_HOOKS_EN           0    /* Application-defined hooks are called from the uC/OS-II hooks */

//参数检查
#define OS_ARG_CHK_EN             1    /* Enable (1) or Disable (0) argument checking                  */

//CPU勾子函数
#define OS_CPU_HOOKS_EN           1    /* uC/OS-II hooks are found in the processor port files         */

//调试
#define OS_DEBUG_EN               0    /* Enable(1) debug variables                                    */

//多事件等待
#define OS_EVENT_MULTI_EN         1    /* Include code for OSEventPendMulti()                          */

//事件名长
#define OS_EVENT_NAME_SIZE        8    /* Determine the size of the name of a Sem, Mutex, Mbox or Q    */

//最低优先级
#define OS_LOWEST_PRIO           20    /* Defines the lowest priority that can be assigned ...         */
                                       /* ... MUST NEVER be higher than 254!                           */
//最大事件数
#define OS_MAX_EVENTS            20    /* Max. number of event control blocks in your application      */

//最大旗语数(用得不多)
#define OS_MAX_FLAGS              1    /* Max. number of Event Flag Groups    in your application      */

//最大内存分区数(用得不多)
#define OS_MAX_MEM_PART           1    /* Max. number of memory partitions                             */

//最大消息队列数(Lwip要用消息队列)
#define OS_MAX_QS                10    /* Max. number of queue control blocks in your application      */

//最大任务数
#define OS_MAX_TASKS             15    /* Max. number of tasks in your application, MUST be >= 2       */

//调度器上锁
#define OS_SCHED_LOCK_EN          1    /* Include code for OSSchedLock() and OSSchedUnlock()           */

//系统定时器步进
#define OS_TICK_STEP_EN           0    /* Enable tick stepping feature for uC/OS-View                  */

//系统定时器频率
#define OS_TICKS_PER_SEC        100    /* Set the number of ticks in one second                        */


                                       /* --------------------- TASK STACK SIZE ---------------------- */
/* 定时器\统计\空闲任务堆栈大小 */                                       
#define OS_TASK_TMR_STK_SIZE     64    /* Timer      task stack size (# of OS_STK wide entries)        */
#define OS_TASK_STAT_STK_SIZE    64    /* Statistics task stack size (# of OS_STK wide entries)        */
#define OS_TASK_IDLE_STK_SIZE    64    /* Idle       task stack size (# of OS_STK wide entries)        */


                                       /* --------------------- TASK MANAGEMENT ---------------------- */
//可改变任务优先级                                       
#define OS_TASK_CHANGE_PRIO_EN    1    /*     Include code for OSTaskChangePrio()                      */

//任务可创建
#define OS_TASK_CREATE_EN         1    /*     Include code for OSTaskCreate()                          */

//任务可扩展创建
#define OS_TASK_CREATE_EXT_EN     1    /*     Include code for OSTaskCreateExt()                       */

//任务可删除
#define OS_TASK_DEL_EN            1    /*     Include code for OSTaskDel()                             */

//任务名长
#define OS_TASK_NAME_SIZE         8    /*     Determine the size of a task name                        */

//???
#define OS_TASK_PROFILE_EN        0    /*     Include variables in OS_TCB for profiling                */

//任务查询
#define OS_TASK_QUERY_EN          1    /*     Include code for OSTaskQuery()                           */

//统计CPU占用率
#define OS_TASK_STAT_EN           0    /*     Enable (1) or Disable(0) the statistics task             */

//统计任务进行任务堆栈检查
#define OS_TASK_STAT_STK_CHK_EN   0    /*     Check task stacks from statistic task                    */

//任务挂起
#define OS_TASK_SUSPEND_EN        1    /*     Include code for OSTaskSuspend() and OSTaskResume()      */

//任务切换勾子函数
#define OS_TASK_SW_HOOK_EN        1    /*     Include code for OSTaskSwHook()                          */


                                       /* ----------------------- EVENT FLAGS ------------------------ */
#define OS_FLAG_EN                1    /* Enable (1) or Disable (0) code generation for EVENT FLAGS    */
#define OS_FLAG_ACCEPT_EN         1    /*     Include code for OSFlagAccept()                          */
#define OS_FLAG_DEL_EN            1    /*     Include code for OSFlagDel()                             */
//旗语名长
#define OS_FLAG_NAME_SIZE         8    /*     Determine the size of the name of an event flag group    */
#define OS_FLAG_QUERY_EN          1    /*     Include code for OSFlagQuery()                           */
#define OS_FLAG_WAIT_CLR_EN       1    /* Include code for Wait on Clear EVENT FLAGS                   */
//旗语位数
#define OS_FLAGS_NBITS           16    /* Size in #bits of OS_FLAGS data type (8, 16 or 32)            */


                                       /* -------------------- MESSAGE MAILBOXES --------------------- */
#define OS_MBOX_EN                1    /* Enable (1) or Disable (0) code generation for MAILBOXES      */
#define OS_MBOX_ACCEPT_EN         1    /*     Include code for OSMboxAccept()                          */
#define OS_MBOX_DEL_EN            1    /*     Include code for OSMboxDel()                             */
#define OS_MBOX_PEND_ABORT_EN     1    /*     Include code for OSMboxPendAbort()                       */
#define OS_MBOX_POST_EN           1    /*     Include code for OSMboxPost()                            */
#define OS_MBOX_POST_OPT_EN       1    /*     Include code for OSMboxPostOpt()                         */
#define OS_MBOX_QUERY_EN          1    /*     Include code for OSMboxQuery()                           */


                                       /* --------------------- MEMORY MANAGEMENT -------------------- */
#define OS_MEM_EN                 1    /* Enable (1) or Disable (0) code generation for MEMORY MANAGER */
//内存分区名长
#define OS_MEM_NAME_SIZE          8    /*     Determine the size of a memory partition name            */
#define OS_MEM_QUERY_EN           1    /*     Include code for OSMemQuery()                            */


                                       /* ---------------- MUTUAL EXCLUSION SEMAPHORES --------------- */
#define OS_MUTEX_EN               1    /* Enable (1) or Disable (0) code generation for MUTEX          */
#define OS_MUTEX_ACCEPT_EN        1    /*     Include code for OSMutexAccept()                         */
#define OS_MUTEX_DEL_EN           1    /*     Include code for OSMutexDel()                            */
#define OS_MUTEX_QUERY_EN         1    /*     Include code for OSMutexQuery()                          */


                                       /* ---------------------- MESSAGE QUEUES ---------------------- */
#define OS_Q_EN                   1    /* Enable (1) or Disable (0) code generation for QUEUES         */
#define OS_Q_ACCEPT_EN            1    /*     Include code for OSQAccept()                             */
#define OS_Q_DEL_EN               1    /*     Include code for OSQDel()                                */
#define OS_Q_FLUSH_EN             1    /*     Include code for OSQFlush()                              */
#define OS_Q_PEND_ABORT_EN        1    /*     Include code for OSQPendAbort()                          */
#define OS_Q_POST_EN              1    /*     Include code for OSQPost()                               */
#define OS_Q_POST_FRONT_EN        1    /*     Include code for OSQPostFront()                          */
#define OS_Q_POST_OPT_EN          1    /*     Include code for OSQPostOpt()                            */
#define OS_Q_QUERY_EN             1    /*     Include code for OSQQuery()                              */


                                       /* ------------------------ SEMAPHORES ------------------------ */
#define OS_SEM_EN                 1    /* Enable (1) or Disable (0) code generation for SEMAPHORES     */
#define OS_SEM_ACCEPT_EN          1    /*    Include code for OSSemAccept()                            */
#define OS_SEM_DEL_EN             1    /*    Include code for OSSemDel()                               */
#define OS_SEM_PEND_ABORT_EN      1    /*    Include code for OSSemPendAbort()                         */
#define OS_SEM_QUERY_EN           1    /*    Include code for OSSemQuery()                             */
#define OS_SEM_SET_EN             1    /*    Include code for OSSemSet()                               */


                                       /* --------------------- TIME MANAGEMENT ---------------------- */
#define OS_TIME_DLY_HMSM_EN       1    /*     Include code for OSTimeDlyHMSM()                         */
#define OS_TIME_DLY_RESUME_EN     1    /*     Include code for OSTimeDlyResume()                       */
#define OS_TIME_GET_SET_EN        1    /*     Include code for OSTimeGet() and OSTimeSet()             */
#define OS_TIME_TICK_HOOK_EN      1    /*     Include code for OSTimeTickHook()                        */


                                       /* --------------------- TIMER MANAGEMENT --------------------- */
//定时器使能                                       
#define OS_TMR_EN                 0    /* Enable (1) or Disable (0) code generation for TIMERS         */

//最大定时器数
#define OS_TMR_CFG_MAX            4    /*     Maximum number of timers                                 */

//定时器名长
#define OS_TMR_CFG_NAME_SIZE      8    /*     Determine the size of a timer name                       */

//???
#define OS_TMR_CFG_WHEEL_SIZE     8    /*     Size of timer wheel (#Spokes)                            */

//定时器频率
#define OS_TMR_CFG_TICKS_PER_SEC 10    /*     Rate at which timer management task runs (Hz)            */

#endif
