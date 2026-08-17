/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern SD_HandleTypeDef uSdHandle;
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_dac1;
extern DAC_HandleTypeDef hdac;
extern I2C_HandleTypeDef hi2c1;
extern LTDC_HandleTypeDef hltdc;
extern TIM_HandleTypeDef htim5;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim6;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void) {
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1) {
    }
    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void) {
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void) {
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void) {
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while (1) {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void) {
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void) {
    /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

    /* USER CODE END DMA1_Stream5_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_dac1);
    /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

    /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles I2C1 event interrupt.
  */
void I2C1_EV_IRQHandler(void) {
    /* USER CODE BEGIN I2C1_EV_IRQn 0 */

    /* USER CODE END I2C1_EV_IRQn 0 */
    HAL_I2C_EV_IRQHandler(&hi2c1);
    /* USER CODE BEGIN I2C1_EV_IRQn 1 */

    /* USER CODE END I2C1_EV_IRQn 1 */
}

/**
  * @brief This function handles I2C1 error interrupt.
  */
void I2C1_ER_IRQHandler(void) {
    /* USER CODE BEGIN I2C1_ER_IRQn 0 */

    /* USER CODE END I2C1_ER_IRQn 0 */
    HAL_I2C_ER_IRQHandler(&hi2c1);
    /* USER CODE BEGIN I2C1_ER_IRQn 1 */

    /* USER CODE END I2C1_ER_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void) {
    /* USER CODE BEGIN USART1_IRQn 0 */

    /* USER CODE END USART1_IRQn 0 */
    HAL_UART_IRQHandler(&huart1);
    /* USER CODE BEGIN USART1_IRQn 1 */

    /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles TIM5 global interrupt.
  */
void TIM5_IRQHandler(void) {
    /* USER CODE BEGIN TIM5_IRQn 0 */

    /* USER CODE END TIM5_IRQn 0 */
    HAL_TIM_IRQHandler(&htim5);
    /* USER CODE BEGIN TIM5_IRQn 1 */

    /* USER CODE END TIM5_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void) {
    /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

    /* USER CODE END TIM6_DAC_IRQn 0 */
    if (hdac.State != HAL_DAC_STATE_RESET) {
        HAL_DAC_IRQHandler(&hdac);
    }
    HAL_TIM_IRQHandler(&htim6);
    /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

    /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream5 global interrupt.
  */
void DMA2_Stream5_IRQHandler(void) {
    /* USER CODE BEGIN DMA2_Stream5_IRQn 0 */

    /* USER CODE END DMA2_Stream5_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart1_rx);
    /* USER CODE BEGIN DMA2_Stream5_IRQn 1 */

    /* USER CODE END DMA2_Stream5_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream7_IRQHandler(void) {
    /* USER CODE BEGIN DMA2_Stream7_IRQn 0 */

    /* USER CODE END DMA2_Stream7_IRQn 0 */
    HAL_DMA_IRQHandler(&hdma_usart1_tx);
    /* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

    /* USER CODE END DMA2_Stream7_IRQn 1 */
}

/**
  * @brief This function handles LTDC global interrupt.
  */
void LTDC_IRQHandler(void) {
    /* USER CODE BEGIN LTDC_IRQn 0 */

    /* USER CODE END LTDC_IRQn 0 */
    HAL_LTDC_IRQHandler(&hltdc);
    /* USER CODE BEGIN LTDC_IRQn 1 */

    /* USER CODE END LTDC_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void SDMMC1_IRQHandler(void) {
    HAL_SD_IRQHandler(&uSdHandle);
}

/**
* @brief  This function handles DMA2 Stream 6 interrupt request.
* @param  None
* @retval None
*/
void DMA2_Stream6_IRQHandler(void) {
    HAL_DMA_IRQHandler(uSdHandle.hdmatx);
}

/**
* @brief  This function handles DMA2 Stream 3 interrupt request.
* @param  None
* @retval None
*/
void DMA2_Stream3_IRQHandler(void) {
    HAL_DMA_IRQHandler(uSdHandle.hdmarx);
}

__attribute__((used))
void HardFault_Handler_C(uint32_t *stack_frame) {
    // Дамп регистров из стека
    uint32_t r0 = stack_frame[0];
    uint32_t r1 = stack_frame[1];
    uint32_t r2 = stack_frame[2];
    uint32_t r3 = stack_frame[3];
    uint32_t r12 = stack_frame[4];
    uint32_t lr = stack_frame[5];
    uint32_t pc = stack_frame[6];
    uint32_t psr = stack_frame[7];

    // Анализ причины HardFault
    uint32_t hfsr = SCB->HFSR;
    uint32_t cfsr = SCB->CFSR;
    uint32_t mmfar = SCB->MMFAR;
    uint32_t bfar = SCB->BFAR;

    while (1) {
        __asm("BKPT #0");
    }
}

__attribute__((naked))
void HardFault_Handler(void) {
    __asm volatile(
            "TST LR, #4\n"
            "ITE EQ\n"
            "MRSEQ R0, MSP\n"
            "MRSNE R0, PSP\n"
            "B HardFault_Handler_C\n" // Переход на C-обработчик
            );
}

/* USER CODE END 1 */
