
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartLEDTask1(void *argument);
void StartLEDTask2(void *argument);

// Define two RTOS task handles
osThreadId_t ledTask1Handle;
osThreadId_t ledTask2Handle;

// Task attributes
const osThreadAttr_t ledTask1_attributes = {
  .name = "LED_Task1",
  .priority = osPriorityNormal,
  .stack_size = 128 * 4
};

const osThreadAttr_t ledTask2_attributes = {
  .name = "LED_Task2",
  .priority = osPriorityNormal,
  .stack_size = 128 * 4
};

int main(void)
{
  HAL_Init(); // Initialize HAL library

  SystemClock_Config(); // Configure system clock

  MX_GPIO_Init(); // Initialize GPIO (PA0 and PA1)

  osKernelInitialize(); // Initialize FreeRTOS kernel

  // Create two LED control tasks
  ledTask1Handle = osThreadNew(StartLEDTask1, NULL, &ledTask1_attributes);
  ledTask2Handle = osThreadNew(StartLEDTask2, NULL, &ledTask2_attributes);

  osKernelStart(); // Start FreeRTOS scheduler

  while (1) {} // Should never reach here
}

// Task1: Toggle PA0 every 1000ms
void StartLEDTask1(void *argument)
{
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0); // Toggle PA0
    osDelay(1000); // Delay 1000ms
  }
}

// Task2: Toggle PA1 every 500ms
void StartLEDTask2(void *argument)
{
  for(;;)
  {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1); // Toggle PA1
    osDelay(500); // Delay 500ms
  }
}

// Configure system clock
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler(); // Handle clock config error
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler(); // Handle clock config error
  }
}

// Initialize GPIO pins PA0 and PA1
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock

  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET); // Set output low
}

// Handle errors
void Error_Handler(void)
{
  __disable_irq(); // Disable interrupts
  while (1) {} // Stay here if error occurs
}

#ifdef USE_FULL_ASSERT
// Report assert errors
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif



