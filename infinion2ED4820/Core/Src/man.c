/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
//#include "adc.h"
//#include "spi.h"
//#include "tim.h"
//#include "usart.h"
//#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "registers.h"
#include "driver.h"
//#include "analog.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct ADC_Data {
    uint8_t CHid;
    uint32_t timestamp;
    uint32_t adc_value;
    uint8_t readcpltflag;
} ADC_Data;

SMART_FET_SPI spibus; // spi bus for the driver




/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ADC_BUFFER_SIZE 4


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t channels[] = {ADC_CHANNEL_2,ADC_CHANNEL_3,ADC_CHANNEL_4,ADC_CHANNEL_5};//
uint8_t numChannels = sizeof(channels) / sizeof(channels[0]);
ADC_Data adc_data[ADC_BUFFER_SIZE];
volatile uint8_t current_buffer_index = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  //MX_ADC1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
//  saveInitConfig();

//  HAL_GPIO_WritePin(GPIOA, ENAB_Pin, GPIO_PIN_SET); // ENABLE
//  HAL_GPIO_WritePin(GPIOA, SAFES_Pin, GPIO_PIN_RESET); // SAFESTATE

//  setRegisterDefaultValues();
  driverInit();

  MX_ADC1_Init_multichannel(channels, numChannels);  // comment again  //MX_ADC1_Init(); if regenerate ioc MX_ADC1_Init();
  timInitat100ms();
  HAL_ADC_Start_IT(&hadc1);
  HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  turnOnMOSFETs();
	HAL_Delay(3000);

  while (1)
  {
	    HAL_GPIO_WritePin(GPIOA, ENAB_Pin, GPIO_PIN_SET); // ENABLE
//	    turnOnMOSFETs();

		setBits(MOS_CHS_CTRL, MOSONCH_A);
		setBits(MOS_CHS_CTRL, MOSONCH_B); // switch on chan B
		HAL_GPIO_WritePin(GPIOA, LED_G_Pin, GPIO_PIN_SET);
		HAL_Delay(3000);
		clearBits(MOS_CHS_CTRL, MOSONCH_A); // switch off chan A
		clearBits(MOS_CHS_CTRL, MOSONCH_B); // switch off chan B



		HAL_GPIO_WritePin(GPIOA, LED_G_Pin, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(GPIOA, ENAB_Pin, GPIO_PIN_RESET); // ENABLE
		clearFailures();

		HAL_Delay(3000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void saveInitConfig(void)
{
	setInitRegisterData(VDSTHA_B, getRegisterData(VDSTHA_B));
	setInitRegisterData(MOSFLTBLKA_B, getRegisterData(MOSFLTBLKA_B));
	setInitRegisterData(CSAG_OCTH, getRegisterData(CSAG_OCTH));
	setInitRegisterData(VBATOVUVRST, getRegisterData(VBATOVUVRST));
	// channel cross control configuration is just one bit in register MOS_CHS_CTRL
	setInitRegisterData(MOS_CHS_CTRL, (getRegisterData(MOS_CHS_CTRL) & CHCRCTRL));
}

#define MOSFET_A_CTRL_REG 0x10
#define MOSFET_B_CTRL_REG 0x11

// ...

void turnOnMOSFETs(void)
{
  // Turn on MOSFET A
  SPI_WriteRegister(MOSFET_A_CTRL_REG, 0x01);

  // Turn on MOSFET B
  SPI_WriteRegister(MOSFET_B_CTRL_REG, 0x01);
}

void SPI_WriteRegister(uint8_t reg, uint8_t data)
{
  uint8_t txData[2] = {reg, data};  // Combine the register address and data into a single array
  HAL_GPIO_WritePin(GPIOB, GPIO_NSS, GPIO_PIN_RESET);  // Activate the chip select (CS) pin

  HAL_SPI_Transmit(&hspi1, txData, 2, HAL_MAX_DELAY);  // Send the register address and data

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  // Deactivate the chip select (CS) pin
}


//****************************************************************************
// driverInit
//
// initialise the driver with the configuration stored in flash memory
//****************************************************************************
void driverInit(void)
{
	// save current configuration for initialisation
//	if (!DIGITAL_IO_GetInput(&ENABLE))
		saveInitConfig();

	// disable interrupt to suppress vdd undervoltage message
//	PIN_INTERRUPT_SetEdgeSensitivity(&PIN_INT, PIN_INTERRUPT_EDGE_NONE);
	// PIN_INTERRUPT_Disable(&PIN_INT);

	// set enable pin high
//	DIGITAL_IO_SetOutputHigh(&ENABLE);
	HAL_GPIO_WritePin(GPIOA, ENAB_Pin, GPIO_PIN_SET); // ENABLE
//	HAL_GPIO_WritePin(GPIOA, SAFES_Pin, GPIO_PIN_SET); // SAFESTATE
	setRegisterDefaultValues();

	// wait for the driver to power up
//	wait(5000);
	HAL_Delay(5000);
	// driver is enabled and switches are still off (load current = 0)
	// this is a good time to measure the actual cso output reference voltage (typ. vdd/2)
	// to avoid offset errors
//	setCsoReferenceVoltage();

	// clear VDD_UV failure after reset
	clearFailures();

	// apply init configuration
	spiWrite(VDSTHA_B, getInitRegisterData(VDSTHA_B));
	spiWrite(MOSFLTBLKA_B, getInitRegisterData(MOSFLTBLKA_B));
	spiWrite(CSAG_OCTH, getInitRegisterData(CSAG_OCTH));
	spiWrite(VBATOVUVRST, getInitRegisterData(VBATOVUVRST));
	// XCTRL setting is just one bit in register MOS_CHS_CTRL
	spiWrite(MOS_CHS_CTRL, ((getRegisterData(MOS_CHS_CTRL) & ~CHCRCTRL) | getInitRegisterData(MOS_CHS_CTRL)));

	// enable interrupt again
//	PIN_INTERRUPT_SetEdgeSensitivity(&PIN_INT, PIN_INTERRUPT_EDGE_RISING);
	// PIN_INTERRUPT_Enable(&PIN_INT);
}

//****************************************************************************
// clearFailures
//****************************************************************************
void clearFailures(void)
{
	static uint8_t reg_data;

//	if (DIGITAL_IO_GetInput(&ENABLE))
//	{
		// brute force approach:
		// set all bits that have to do with failure clearing
		spiWrite(FAILURECLEAN, 0xFF);
		spiWrite(MOS_CHS_CTRL, (getRegisterData(MOS_CHS_CTRL) | MOS_CTRL_FAILURE_CLEAR));

		// check if there are still failures present
		readAllRegisters();
		reg_data = getRegisterData(STDIAG);

//		if (reg_data & FAILURE) // still failures present
//		{
//			uartPrintf("\r\n"); // new line
//			uartPrintf("Couldn't clear all failures");
//			failureBitWasSet = 1; // trigger reporting of remaining failures
//		}
//	}
}
static void MX_ADC1_Init_multichannel(uint32_t channels[], uint8_t numChannels)
{
  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  // Common ADC configuration
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = numChannels;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;

  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  // Configure the ADC multi-mode
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  for (uint8_t i = 0; i < numChannels; i++)
  {
    // Configure Regular Channels
    sConfig.Channel = channels[i];
    sConfig.Rank = i + 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
      Error_Handler();
    }
  }
}
static void timInitat100ms(void)
{
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 1600 - 1;  // Assuming the clock frequency is 16 MHz
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 10000 - 1;  // 100 ms = 10000 * (1/16000000) s
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM1)
  {
    HAL_ADC_Start_IT(&hadc1);
//  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if (hadc->Instance == ADC1)
  {  // Process the data in the current buffer
      for (int i = 0; i < ADC_BUFFER_SIZE; i++)
      {
          adc_data[current_buffer_index].adc_value =HAL_ADC_GetValue(&hadc1);
          adc_data[current_buffer_index].timestamp = HAL_GetTick();
          adc_data[current_buffer_index].CHid = i + 1;
          adc_data[current_buffer_index].readcpltflag = 1;
          current_buffer_index = (current_buffer_index + 1) % ADC_BUFFER_SIZE;
      }

    // Process the ADC value here
  }
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
