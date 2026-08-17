#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Ensure definitions are only used by the compiler, and not by the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)

#include <stdint.h>

extern uint32_t SystemCoreClock;
#endif

/******************************************************************************/
/* Hardware description related definitions. **********************************/
/******************************************************************************/
#define configCPU_CLOCK_HZ                       200000000  // Частота CPU (Гц)
#define configTICK_RATE_HZ                       ((TickType_t)1000)  // Частота системного тика (Гц)

/******************************************************************************/
/* Scheduling behaviour related definitions. **********************************/
/******************************************************************************/
#define configUSE_PREEMPTION                     1  ///< 1 - вытесняющая многозадачность, 0 - кооперативная
#define configUSE_TIME_SLICING                   1  ///< Включает квантование времени для задач с одинаковым приоритетом
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  1  ///< Оптимизированный алгоритм выбора задач (использует CLZ на Cortex-M)
#define configMAX_PRIORITIES                     7  ///< Число приоритетов задач (0 - самый низкий)
#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)  ///< Размер стека idle-задачи (в словах)
#define configMAX_TASK_NAME_LEN                  16  ///< Макс. длина имени задачи (с NULL-терминатором)

/* TickType_t width */
#define configUSE_16_BIT_TICKS                   0  ///< 0: TickType_t - uint32_t (лучше для Cortex-M), 1: uint16_t (макс. ~65 сек)

/* Memory allocation */
#define configSUPPORT_STATIC_ALLOCATION          1  ///< Поддержка статического выделения памяти
#define configSUPPORT_DYNAMIC_ALLOCATION         1  ///< Поддержка динамического выделения памяти
#define configTOTAL_HEAP_SIZE                    ((size_t)512)  ///< Общий размер кучи (байт)

/* Queue registry */
#define configQUEUE_REGISTRY_SIZE                8  ///< Для vQueueAddToRegistry() (назначение имён очередям/семафорам)

/******************************************************************************/
/* Hook and callback function related definitions. ****************************/
/******************************************************************************/
#define configUSE_IDLE_HOOK                      1  ///< 1: Включить vApplicationIdleHook() для фоновых операций
#define configUSE_TICK_HOOK                      1  ///< 1: Включить vApplicationTickHook() (вызывается в прерывании)
#define configUSE_MALLOC_FAILED_HOOK             1  ///< 1: Включить vApplicationMallocFailedHook()
#define configCHECK_FOR_STACK_OVERFLOW           2  ///< 0: Отключено, 1: Быстрая проверка, 2: Точная проверка

/******************************************************************************/
/* Run time and task stats gathering related definitions. *********************/
/******************************************************************************/
#define configGENERATE_RUN_TIME_STATS            1  ///< Сбор статистики времени выполнения задач
#define configUSE_TRACE_FACILITY                 1  ///< Включить отладочную информацию (номера задач, типы очередей)
#define configUSE_STATS_FORMATTING_FUNCTIONS     1  ///< Включить vTaskList() и vTaskGetRunTimeStats()
#define configRECORD_STACK_HIGH_ADDRESS          1  ///< Запись верхнего адреса стека для отладки

#if (configGENERATE_RUN_TIME_STATS == 1)
void ConfigureTimerForRunTimeStats(void);
uint32_t GetRuntimeCounterValue(void);
#endif

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() ConfigureTimerForRunTimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE()         GetRuntimeCounterValue()

/******************************************************************************/
/* Software timer related definitions. ****************************************/
/******************************************************************************/

#define configUSE_TIMERS                         1  ///< Включить программные таймеры
#define configTIMER_TASK_PRIORITY                (configMAX_PRIORITIES - 1)  ///< Приоритет задачи таймеров
#define configTIMER_TASK_STACK_DEPTH             configMINIMAL_STACK_SIZE    ///< Размер стека задачи таймеров
#define configTIMER_QUEUE_LENGTH                 10  ///< Длина очереди команд таймеров

/******************************************************************************/
/* Co-routine definitions. ***************************************************/
/******************************************************************************/
#define configUSE_CO_ROUTINES                    0  ///< 1: Включить co-routines (устарело для Cortex-M)
#define configMAX_CO_ROUTINE_PRIORITIES          2  ///< Число приоритетов co-routines (если включены)

/******************************************************************************/
/* Cortex-M specific definitions. ********************************************/
/******************************************************************************/
#ifdef __NVIC_PRIO_BITS
#define configPRIO_BITS         __NVIC_PRIO_BITS  ///< Число битов приоритета прерываний
#else
#define configPRIO_BITS         4
#endif

/* Приоритеты прерываний */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   15  ///< Самый низкий приоритет прерывания
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 5  ///< Макс. приоритет для вызовов FreeRTOS API из прерываний
#define configKERNEL_INTERRUPT_PRIORITY        (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY   (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

/* Assert */
#define configASSERT(x) if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );}

/* Маппинг обработчиков прерываний FreeRTOS на CMSIS */
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler ///< Закомментировать для CubeMX, если SysTick обрабатывается HAL

/******************************************************************************/
/* Additional features from FreeRTOS V11. ************************************/
/******************************************************************************/
#define configUSE_TASK_NOTIFICATIONS           1 ///< Включить уведомления задач
#define configUSE_MUTEXES                      1 ///< Включить мьютексы
#define configUSE_RECURSIVE_MUTEXES            1 ///< Включить рекурсивные мьютексы
#define configUSE_COUNTING_SEMAPHORES          1 ///< Включить счётчиковые семафоры
#define configUSE_EVENT_GROUPS                 1 ///< Включить группы событий
#define configUSE_STREAM_BUFFERS               1 ///< Включить потоковые буферы

/* API includes */
#define INCLUDE_vTaskPrioritySet               1 ///< Разрешает использование vTaskPrioritySet() — изменяет приоритет задачи
#define INCLUDE_uxTaskPriorityGet              1 ///< Разрешает использование uxTaskPriorityGet() — возвращает приоритет задачи
#define INCLUDE_vTaskDelete                    1 ///< Разрешает использование vTaskDelete() — удаляет задачу
#define INCLUDE_vTaskSuspend                   1 ///< Разрешает использование vTaskSuspend() — приостанавливает выполнение задачи
#define INCLUDE_xResumeFromISR                 1 ///< Разрешает использование xResumeFromISR() — возобновляет задачу из обработчика прерывания (устаревшее, используйте xTaskResumeFromISR)
#define INCLUDE_vTaskDelayUntil                1 ///< Разрешает использование vTaskDelayUntil() — задержка до определённого времени (tick-ориентированная задержка)
#define INCLUDE_vTaskDelay                     1 ///< Разрешает использование vTaskDelay() — задержка на заданное количество тиков
#define INCLUDE_xTaskGetSchedulerState         1 ///< Разрешает использование xTaskGetSchedulerState() — возвращает состояние планировщика (запущен/остановлен)
#define INCLUDE_xTaskGetCurrentTaskHandle      1 ///< Разрешает использование xTaskGetCurrentTaskHandle() — возвращает дескриптор текущей задачи
#define INCLUDE_xEventGroupSetBitFromISR       1 ///< Разрешает использование xEventGroupSetBitFromISR() — установка флага события из ISR
#define INCLUDE_xTaskResumeFromISR             1 ///< Разрешает использование xTaskResumeFromISR() — возобновляет задачу из ISR

/* Отключенные API (для экономии места) */
#define INCLUDE_uxTaskGetStackHighWaterMark    1 ///< Отключает uxTaskGetStackHighWaterMark() — возвращает минимальный запас стека задачи (для отладки)
#define INCLUDE_xTaskGetIdleTaskHandle         0 ///< Отключает xTaskGetIdleTaskHandle() — возвращает дескриптор задачи простаивания (idle task)
#define INCLUDE_eTaskGetState                  0 ///< Отключает eTaskGetState() — возвращает состояние задачи (Running, Ready, Blocked и т.д.)
#define INCLUDE_xTimerPendFunctionCall         0 ///< Отключает xTimerPendFunctionCall() — отложенный вызов функции из ISR в контексте задачи таймера
#define INCLUDE_xTaskAbortDelay                0 ///< Отключает xTaskAbortDelay() — прерывает задержку задачи (например, vTaskDelay)
#define INCLUDE_xTaskGetHandle                 1 ///< Разрешает использование xTaskGetHandle() — получение дескриптора задачи по имени

#define configCOMMAND_INT_MAX_OUTPUT_SIZE 256
#define configAPPLICATION_PROVIDES_cOutputBuffer 0

#ifdef __cplusplus
}
#endif

#endif /* FREERTOS_CONFIG_H */