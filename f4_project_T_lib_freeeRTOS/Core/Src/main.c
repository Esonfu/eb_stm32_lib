/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_task.h"
#include "stdio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "test.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
t_i2c_t *a_test;
t_spi_t *a_spi_test;
uint8_t w_rt[20]="hello world!";
uint8_t r_rt[20];
uint8_t tdata;
uint32_t id_data;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
//uint8_t rh = 11;
//	float x = 1.1234;
//	double y = 2.345;
//	
//	printf("hello!你好！\r\n");
//	printf("rh:%d\r\n",rh);
//	printf("x:%.2f\n\r",x);
//	printf("y:%f\n\r",y);
   //app_create();//开启调度后以下代码将不被执行
	 
	 
	 
	 a_test=t_i2c_create("i2c",
	                    hal_gpio_outcfg,
	                    hal_gpio_incfg,
											i2c_dealy,
											hal_write,
											hal_read,
											&T_gpio[0]
											);
											
		a_spi_test=t_mspi_create("spi",
		                 hal_gpio_outcfg,
	                   hal_gpio_incfg,
										 NULL,
										 hal_write,
										 hal_read,
		                 &T_spi_gpio[0]
		);								
											
//											
//		tdata=t_i2c_24Cxx_read_byte(a_test,0x13,T_I2C_Block2)	;								
//		t_i2c_24Cxx_write_byte(a_test,0x13,0x0c,T_I2C_Block2);	
//    tdata=t_i2c_24Cxx_read_byte(a_test,0x13,T_I2C_Block2)	;				
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		t_i2c_start(a_test); 
//    t_i2c_send_byte(a_test,(0X68<<1)|0);
//		if(t_i2c_wait_ack(a_test)==1)
//		{
//		  printf("hhhh");
//		}
//		t_i2c_send_byte(a_test,0x75);
//		if(t_i2c_wait_ack(a_test)==1)
//		{
//		  printf("hhhh");
//		}
//		
//		t_i2c_start(a_test);
//		
//		 t_i2c_send_byte(a_test,(0X68<<1)|1);
//		if(t_i2c_wait_ack(a_test)==1)
//		{
//		  printf("hhhh");
//		}
//		tdata=t_i2c_read_byte(a_test);//读取数据,发送nACK 
//		t_i2c_stop(a_test);
//		
//		//HAL_UART_Transmit_DMA(&huart1,(uint8_t*)"a",1);

//    /* USER CODE END WHILE */

//    /* USER CODE BEGIN 3 */
//		
//		tdata=t_i2c_CheckDevice(a_test,0xA0);
//		tdata=t_i2c_CheckDevice(a_test,0xA0);
//		
//		id_data=t_mspi_id_ReadNorFlash(a_spi_test);
		id_data=t_mspi_id_ReadNorFlash(a_spi_test);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void vApplicationMallocFailedHook(void)
{
		/*确保分配内存正常,出现堆溢出和malloc失败进入该函数*/

	
    taskDISABLE_INTERRUPTS();
    for(;;);
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void)
{
		/*通过空闲任务钩子函数(或称回调，hook, or call-back)，可以直接在空闲任务中添加应用程序相关的功能。空闲任务钩子函数会被空闲任务每循环一次就自动调用一次*/
		
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook (TaskHandle_t xTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) xTask;
		/*运行时堆栈溢出检查,进入该函数*/

    taskDISABLE_INTERRUPTS();
    for(;;);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
