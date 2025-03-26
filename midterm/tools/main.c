#include "main.h"
#include "cmsis_os.h"

// Task handles
osThreadId_t LED1_Handle;
osThreadId_t LED2_Handle;

// Function prototypes
void LED1_Task(void *argument);
void LED2_Task(void *argument);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void) {
    HAL_Init();                        // 初始化 HAL 庫
    SystemClock_Config();              // 設定系統時鐘
    MX_GPIO_Init();                    // 初始化 GPIO
    
    // 建立 FreeRTOS 任務
    osKernelInitialize();
    LED1_Handle = osThreadNew(LED1_Task, NULL, NULL);
    LED2_Handle = osThreadNew(LED2_Task, NULL, NULL);
    osKernelStart();                   // 啟動 FreeRTOS 核心
    
    while (1) {}
}

// Blink LED on PA0 every 500ms
void LED1_Task(void *argument) {
    for (;;) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
        osDelay(500);
    }
}

// Blink LED on PA1 every 1000ms
void LED2_Task(void *argument) {
    for (;;) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
        osDelay(1000);
    }
}

// GPIO 設定 (PA0 與 PA1)
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE(); // 啟用 GPIOA 時鐘

    // 設定 PA0 和 PA1 為輸出模式
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// 系統時鐘設定
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK 
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}

// 發生錯誤時的處理
void Error_Handler(void) {
    __disable_irq();
    while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
}
#endif
