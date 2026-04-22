/* generated configuration header file - do not edit */
#ifndef FREERTOSCONFIG_H_
#define FREERTOSCONFIG_H_
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Ensure stdint is only used by the compiler, and not the assembler. */
#include <stdint.h>
#include <stdbool.h>
#include "bsp_api.h"
#include "custom_config_qspi.h" // TODO get rid of this dependency

extern uint32_t SystemCoreClock;

#define STRIP_PARENS(...)    __VA_ARGS__

#if FSP_TEST_LOGGING
	    #define configPRINTF(x)     SEGGER_RTT_printf(0, STRIP_PARENS x)
	    #else
#define configPRINTF(...)
#endif

#define configUSE_PREEMPTION (1)
#ifndef configUSE_IDLE_HOOK
#define configUSE_IDLE_HOOK (1)
#endif /* configUSE_IDLE_HOOK */
#define configUSE_TICK_HOOK (0)
#define configCPU_CLOCK_HZ	(SystemCoreClock)

#if (DEVICE_FAMILY == DA1640X)
#if (BSP_CFG_LP_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_LP_CLK_RCX)
#define configSYSTICK_CLOCK_HZ                  ( rcx_clock_hz )
#define configTICK_RATE_HZ                      ( ( TickType_t ) rcx_tick_rate_hz )
#define TICK_PERIOD                             ( (uint32_t)rcx_tick_period )
#else
	        #define configSYSTICK_CLOCK_HZ                  32768
	        #define configTICK_RATE_HZ                      ( ( TickType_t ) 512 )
	        #define TICK_PERIOD   ((configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ))
	    #endif
#endif /* DEVICE_FAMILY */

/* ============================================================================ */

#define configENABLE_FPU                        1
#define configENABLE_MPU                        0
/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#define configENABLE_TRUSTZONE                  0

/* ============================================================================ */

#define configMAX_PRIORITIES (32)
#define configMINIMAL_STACK_SIZE (200)
#ifndef configTOTAL_HEAP_SIZE
#define configTOTAL_HEAP_SIZE (7168)
#endif
#define configMAX_TASK_NAME_LEN (16)
#define configUSE_TRACE_FACILITY (1)
#define configRECORD_STACK_HIGH_ADDRESS (1)
#define configGENERATE_RUN_TIME_STATS (0)
#ifdef configGENERATE_RUN_TIME_STATS
extern unsigned long vGetRunTimeCounterValue(void);
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() {}
#define portGET_RUN_TIME_COUNTER_VALUE() vGetRunTimeCounterValue()
#endif
#define configUSE_16_BIT_TICKS (0)
#define configIDLE_SHOULD_YIELD (1)
#define configUSE_MUTEXES (1)
#define configQUEUE_REGISTRY_SIZE (8)
#define configCHECK_FOR_STACK_OVERFLOW (2)
#define configUSE_RECURSIVE_MUTEXES	(1)
#define configUSE_MALLOC_FAILED_HOOK (1)
#define configUSE_APPLICATION_TASK_TAG	        0
#define configUSE_COUNTING_SEMAPHORES (1)
#define configUSE_QUEUE_SETS (1)
#define configUSE_POSIX_ERRNO                   1
#ifndef configSUPPORT_STATIC_ALLOCATION
#define configSUPPORT_STATIC_ALLOCATION (1)
#endif
#ifndef configSUPPORT_DYNAMIC_ALLOCATION
#define configSUPPORT_DYNAMIC_ALLOCATION (1)
#endif

#if configSUPPORT_DYNAMIC_ALLOCATION
#define FSP_USE_HEAP_1 (FSP_NOT_DEFINED)
#define FSP_USE_HEAP_2 (FSP_NOT_DEFINED)
#define FSP_USE_HEAP_3 (FSP_NOT_DEFINED)
#define FSP_USE_HEAP_4 (FSP_NOT_DEFINED)
#define FSP_USE_HEAP_5 (1)
#endif

#ifndef configUSE_TICKLESS_IDLE
#define configUSE_TICKLESS_IDLE (2)
#endif
/* The minimum allowed value for configEXPECTED_IDLE_TIME_BEFORE_SLEEP is 2. */
#define configEXPECTED_IDLE_TIME_BEFORE_SLEEP   2
#define portSUPPRESS_TICKS_AND_SLEEP( x )       prvSystemSleep( x )
#define configPRE_STOP_PROCESSING()
#define configPRE_SLEEP_PROCESSING( x )
#define configPOST_SLEEP_PROCESSING( x )
#define configPRE_IDLE_ENTRY( x )               /*cm_lower_all_clocks()*/
#define configPOST_IDLE_ENTRY( x )              /*cm_restore_all_clocks()*/

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES (0)
#define configMAX_CO_ROUTINE_PRIORITIES (2)

/* Software timer definitions. */
#define configUSE_TIMERS (1)
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#ifndef configTIMER_QUEUE_LENGTH
#define configTIMER_QUEUE_LENGTH (48)
#endif
#ifndef configTIMER_TASK_STACK_DEPTH
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE)
#endif
#define configUSE_DAEMON_TASK_STARTUP_HOOK (0)

/*lwip configuration*/
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS (1)

/* Set the following definitions to 1 to include the API function, or zero
 to exclude the API function. */
#define INCLUDE_vTaskPrioritySet (1)
#define INCLUDE_uxTaskPriorityGet (1)
#define INCLUDE_vTaskDelete	(1)
#define INCLUDE_vTaskCleanUpResources (1)
#define INCLUDE_vTaskSuspend (1)
#define INCLUDE_vTaskDelayUntil (1)
#define INCLUDE_vTaskDelay (1)
#define INCLUDE_eTaskGetState (1)
#define INCLUDE_xEventGroupSetBitFromISR (1)
#define INCLUDE_xTimerPendFunctionCall (1)
#define INCLUDE_xTaskAbortDelay (1)
#if (dg_configTRACK_OS_HEAP == 1) || (dg_configENABLE_TASK_MONITORING == 1)
	    #define INCLUDE_uxTaskGetStackHighWaterMark (1)
	    #endif /* (dg_configTRACK_OS_HEAP == 1) */
#define INCLUDE_xTaskGetCurrentTaskHandle (1)

/* $code_snippet START code_defining_the_config_ASSERT_macro */
/* Normal assert() semantics without relying on the provision of an assert.h header file. */
#if (dg_configIMAGE_SETUP == DEVELOPMENT_MODE)
#include <stdio.h>
#define configASSERT( x ) if( ( x ) == 0 ) { printf(" >> ASSERT %s:%d \n", __func__, __LINE__); assert( x ); }
#else
	    #define configASSERT( x ) if( ( x ) == 0 ) { }
	    #endif
/* $code_snippet END */

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
 standard names - or at least those used in the unmodified vector table. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#if (dg_configENABLE_TASK_MONITORING == 1)
	    #define INCLUDE_pcTaskGetTaskName               1
	    #if (dg_configFREERTOS_ENABLE_THREAD_AWARENESS == 0)
	    #undef configUSE_TRACE_FACILITY
	    #define configUSE_TRACE_FACILITY                1
	    #endif
	    #endif /*dg_configENABLE_TASK_MONITORING */

#define INCLUDE_xTaskGetIdleTaskHandle (1)
#if (dg_configSYSTEMVIEW == 1)
	    #define INCLUDE_pxTaskGetStackStart                             1
	    #include "SEGGER_SYSVIEW_FreeRTOS.h"
	    #endif /* (dg_configSYSTEMVIEW == 1) */

/* -------------------------------------------------------------------- */
/* Cortex-M specific definitions. */
#if (DEVICE_FAMILY == DA1640X)
#define configPRIO_BITS                         __NVIC_PRIO_BITS
#endif /* DEVICE_FAMILY */

/* The lowest interrupt priority that can be used in a call to a "set priority" function. */
#if (DEVICE_FAMILY == DA1640X)
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY ((1 << __NVIC_PRIO_BITS) - 1)
#endif /* DEVICE_FAMILY */

/* The highest interrupt priority that can be used by any interrupt service
 routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
 INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
 PRIORITY THAN THIS! (higher priorities are lower numeric values on an ARM Cortex-M). */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY ((1))

/* Interrupt priorities used by the kernel port layer itself.  These are generic
 to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY ((configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - __NVIC_PRIO_BITS))
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
 See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - __NVIC_PRIO_BITS))

#ifndef OS_TASK_PRIORITY_LOWEST
#define OS_TASK_PRIORITY_LOWEST		(tskIDLE_PRIORITY)				//0
#endif //OS_TASK_PRIORITY_LOWEST
#define OS_TASK_PRIORITY_USER		(tskIDLE_PRIORITY+1)			//1
#define OS_TASK_PRIORITY_USER_MAX	(OS_TASK_PRIORITY_USER+9)		//10
#define OS_TASK_PRIORITY_SYSTEM		(OS_TASK_PRIORITY_USER_MAX+1)	//11
#ifndef OS_TASK_PRIORITY_HIGHEST
#define OS_TASK_PRIORITY_HIGHEST	(configMAX_PRIORITIES-1)		//31
#endif //OS_TASK_PRIORITY_HIGHEST
#ifndef LIBRARY_LOG_NAME
#define LIBRARY_LOG_NAME                           "FSP_NOT_DEFINED"
#endif
#ifndef LIBRARY_LOG_LEVEL
#define LIBRARY_LOG_LEVEL                          (FSP_NOT_DEFINED)
#endif

#endif /* FREERTOS_CONFIG_H */
#endif /* FREERTOSCONFIG_H_ */
