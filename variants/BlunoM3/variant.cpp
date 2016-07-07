/*
  Copyright (c) 2014 DFrobot.  All right reserved.
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "variant.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Pins descriptions
 */


extern const PinDescription g_APinDescription[]=
{

  { GPIOA, GPIO_Pin_10,  RCC_APB2Periph_GPIOA, NONE, TIM1, TIM_Channel_3 }, // 0     //     rx1  
  { GPIOA, GPIO_Pin_9,  RCC_APB2Periph_GPIOA, NONE, TIM1, TIM_Channel_2 }, // 1         //   tx1

  { GPIOA, GPIO_Pin_0,  RCC_APB2Periph_GPIOA, ADC_Channel_0, TIM5, TIM_Channel_1 }, // 2

  { GPIOA, GPIO_Pin_1,  RCC_APB2Periph_GPIOA, ADC_Channel_1, TIM2, TIM_Channel_2 }, // 3

  {GPIOB, GPIO_Pin_5, RCC_APB2Periph_GPIOB, NONE, NULL, NONE}, /*4*/

  { GPIOB, GPIO_Pin_6,   RCC_APB2Periph_GPIOB, NONE, NULL, NONE}, // 5

  { GPIOA, GPIO_Pin_8,   RCC_APB2Periph_GPIOA, NONE, TIM1, TIM_Channel_1 }, // 6

  { GPIOA, GPIO_Pin_11,   RCC_APB2Periph_GPIOA, NONE, TIM1, TIM_Channel_4 }, // 7

  { GPIOC, GPIO_Pin_11,  RCC_APB2Periph_GPIOC, NONE, NULL, NONE },  // 8

  
 { GPIOC, GPIO_Pin_10,  RCC_APB2Periph_GPIOC, NONE, NULL, NONE}, //9 

  { GPIOA, GPIO_Pin_4,  RCC_APB2Periph_GPIOA, ADC_Channel_4, NULL, NONE}, //10

  { GPIOA, GPIO_Pin_7,  RCC_APB2Periph_GPIOA, ADC_Channel_7, TIM3, TIM_Channel_2}, // 11

  { GPIOA, GPIO_Pin_6,  RCC_APB2Periph_GPIOA, ADC_Channel_6, TIM3, TIM_Channel_1}, // 12

  { GPIOA, GPIO_Pin_5,  RCC_APB2Periph_GPIOA, ADC_Channel_5, NULL, NONE}, // 13

  { GPIOB, GPIO_Pin_8,   RCC_APB2Periph_GPIOB, NONE, NULL, NONE }, // 14

  { GPIOC, GPIO_Pin_0,  RCC_APB2Periph_GPIOC, ADC_Channel_10, NULL ,NONE }, // 15
  
  { GPIOC, GPIO_Pin_1,  RCC_APB2Periph_GPIOC, ADC_Channel_11, NULL ,NONE }, // 16
   
  { GPIOC, GPIO_Pin_2,  RCC_APB2Periph_GPIOC, ADC_Channel_12, NULL ,NONE }, // 17
  
  { GPIOC, GPIO_Pin_3,  RCC_APB2Periph_GPIOC, ADC_Channel_13, NULL ,NONE }, // 18

  { GPIOC, GPIO_Pin_4,  RCC_APB2Periph_GPIOC, ADC_Channel_14,NULL, NONE }, // 19
  
  { GPIOC, GPIO_Pin_5,  RCC_APB2Periph_GPIOC, ADC_Channel_15,NULL, NONE }, // 20

  { GPIOA, GPIO_Pin_2,  RCC_APB2Periph_GPIOA, ADC_Channel_2, TIM2, TIM_Channel_3 }, //21   //tx2

  { GPIOA, GPIO_Pin_3,  RCC_APB2Periph_GPIOA, ADC_Channel_3, TIM2, TIM_Channel_4 },// 22   //rx2

  { GPIOC, GPIO_Pin_12,  RCC_APB2Periph_GPIOC, NONE, NULL, NONE }, // 23

  { GPIOB, GPIO_Pin_9,   RCC_APB2Periph_GPIOB, NONE, NULL, NONE }, // 24

  { GPIOD, GPIO_Pin_2,   RCC_APB2Periph_GPIOD, NONE, NULL, NONE }, // 25

  { GPIOB, GPIO_Pin_7,   RCC_APB2Periph_GPIOB, NONE, NULL, NONE }, // 26

  { GPIOB, GPIO_Pin_0,  RCC_APB2Periph_GPIOB, ADC_Channel_8, TIM3, TIM_Channel_3}, // 27
  
  { GPIOB, GPIO_Pin_1,  RCC_APB2Periph_GPIOB, ADC_Channel_9, TIM3, TIM_Channel_4}, // 28

  { GPIOB, GPIO_Pin_10,  RCC_APB2Periph_GPIOB, NONE, NULL, NONE }, // 29

  { GPIOB, GPIO_Pin_11,  RCC_APB2Periph_GPIOB, NONE, NULL, NONE }, // 30

  { GPIOB, GPIO_Pin_12,  RCC_APB2Periph_GPIOB, NONE, NULL, NONE }, // 31

  { GPIOB, GPIO_Pin_13,  RCC_APB2Periph_GPIOB, NONE, NULL, NONE }, // 32

  { GPIOB, GPIO_Pin_14,  RCC_APB2Periph_GPIOB, NONE, NULL, NONE }, // 33

  { GPIOB, GPIO_Pin_15,  RCC_APB2Periph_GPIOB, NONE, NULL, NONE }, // 34

  { GPIOC, GPIO_Pin_6,   RCC_APB2Periph_GPIOC, NONE, TIM8, TIM_Channel_1 }, // 35
  
  { GPIOC, GPIO_Pin_7,   RCC_APB2Periph_GPIOC, NONE, TIM8, TIM_Channel_2 }, // 36

  { GPIOC, GPIO_Pin_9,   RCC_APB2Periph_GPIOC, NONE, TIM8, TIM_Channel_4 }, // 37
  
  { GPIOC, GPIO_Pin_8,   RCC_APB2Periph_GPIOC, NONE, TIM8, TIM_Channel_3 }  // 38
} ;

#ifdef __cplusplus
}
#endif

/*
 * UART objects
 */
//RingBuffer rx_buffer1;

//UARTClass Serial1(UART, UART_IRQn, id_uart, &rx_buffer1);
//void serialEvent() __attribute__((weak));
//void serialEvent() { }
//
//// IT handlers
//void UART_Handler(void)
//{
//  //Serial1.IrqHandler();
//}

// ----------------------------------------------------------------------------
/*
 * USART objects
 */
RingBuffer rx_buffer1;
RingBuffer rx_buffer2;
RingBuffer rx_buffer3;
RingBuffer rx_buffer4;
RingBuffer rx_buffer5;



USARTClass Serial1(USART1, USART1_IRQn, id_serial1, &rx_buffer1);
void serialEvent1() __attribute__((weak));
void serialEvent1() { }


USARTClass Serial2(USART2, USART2_IRQn, id_serial2, &rx_buffer2);
void serialEvent2() __attribute__((weak));
void serialEvent2() { }


USARTClass Serial3(USART3, USART3_IRQn, id_serial3, &rx_buffer3);
void serialEvent3() __attribute__((weak));
void serialEvent3() { }



USARTClass Serial4(UART4, UART4_IRQn, id_serial4, &rx_buffer4);
void serialEvent4() __attribute__((weak));
void serialEvent4() { }



USARTClass Serial5(UART5, UART5_IRQn, id_serial5, &rx_buffer5);
void serialEvent5() __attribute__((weak));
void serialEvent5() { }



// IT handlers
void USART1_IRQHandler(void) 
{

  Serial1.IrqHandler();//USART1 must be Serial1,for usart flash programming.
}

void USART2_IRQHandler(void) 
{
  Serial2.IrqHandler();
}

void USART3_IRQHandler(void) 
{
  Serial3.IrqHandler();
}

void UART4_IRQHandler(void) 
{

  Serial4.IrqHandler();
  
}

void UART5_IRQHandler(void)
{
	
  Serial5.IrqHandler();


}

// ----------------------------------------------------------------------------

void serialEventRun(void)
{
 
  if (Serial1.available()) serialEvent1();
  if (Serial2.available()) serialEvent2();
  if (Serial3.available()) serialEvent3();
  if (Serial4.available()) serialEvent4();
  if (Serial5.available()) serialEvent5();

}

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

void __libc_init_array(void);
#if 1
void init( void )
{
  SystemInit();

  // Set Systick to 1ms interval, common to all SAM3 variants
  if (SysTick_Config(SystemCoreClock / 1000))
  {
    // Capture error
    while (true);
  }
	/* Configure the SysTick Handler Priority: Preemption priority and subpriority */
	NVIC_SetPriority(SysTick_IRQn, 15);	

  // Disable watchdog
  //WDT_Disable(WDT);

  // Initialize C library
  __libc_init_array();

  // default 13pin led will off.
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);

  /*
  // Enable parallel access on PIo output data registers
  PIOA->PIO_OWER = 0xFFFFFFFF;
  PIOB->PIO_OWER = 0xFFFFFFFF;
  PIOC->PIO_OWER = 0xFFFFFFFF;
  PIOD->PIO_OWER = 0xFFFFFFFF;

  // Initialize Serial1 port U(S)Art pins
  PIO_Configure(
    g_APinDescription[PINS_UART].pport,
    g_APinDescription[PINS_UART].ulpintype,
    g_APinDescription[PINS_UART].ulpin,
    g_APinDescription[PINS_UART].ulpinconfiguration);
  digitalWrite(0, HIGH); // Enable pullup for rx0
  PIO_Configure(
    g_APinDescription[PINS_USART0].pport,
    g_APinDescription[PINS_USART0].ulpintype,
    g_APinDescription[PINS_USART0].ulpin,
    g_APinDescription[PINS_USART0].ulpinconfiguration);
  PIO_Configure(
    g_APinDescription[PINS_USART1].pport,
    g_APinDescription[PINS_USART1].ulpintype,
    g_APinDescription[PINS_USART1].ulpin,
    g_APinDescription[PINS_USART1].ulpinconfiguration);
  PIO_Configure(
    g_APinDescription[PINS_USART3].pport,
    g_APinDescription[PINS_USART3].ulpintype,
    g_APinDescription[PINS_USART3].ulpin,
    g_APinDescription[PINS_USART3].ulpinconfiguration);

  // Initialize USB pins
  PIO_Configure(
    g_APinDescription[PINS_USB].pport,
    g_APinDescription[PINS_USB].ulpintype,
    g_APinDescription[PINS_USB].ulpin,
    g_APinDescription[PINS_USB].ulpinconfiguration);

  // Initialize CAN pins
  PIO_Configure(
    g_APinDescription[PINS_CAN0].pport,
    g_APinDescription[PINS_CAN0].ulpintype,
    g_APinDescription[PINS_CAN0].ulpin,
    g_APinDescription[PINS_CAN0].ulpinconfiguration);
  PIO_Configure(
    g_APinDescription[PINS_CAN1].pport,
    g_APinDescription[PINS_CAN1].ulpintype,
    g_APinDescription[PINS_CAN1].ulPin,
    g_APinDescription[PINS_CAN1].ulPinConfiguration);
*/

  //disable JTAG-DP,release pin 29(PB3),30(PB4),23(PA15)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  //remap Timer4
  GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
  //remap USART3
  GPIO_PinRemapConfig(GPIO_FullRemap_USART3, DISABLE);
  //remap USART2
 // GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
  //remap CAN1,to PD0,PD1
  GPIO_PinRemapConfig(GPIO_Remap2_CAN1,ENABLE);

  // Initialize Analog Controller

	ADC_InitTypeDef ADC_InitStructure;

	// ADCCLK = PCLK2/4
	// RCC_ADCCLKConfig(RCC_PCLK2_Div4);
	// ADCCLK = PCLK2/6 = 72/6 = 12MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	// Enable ADC1 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_DeInit(ADC1);

	// ADC1 Configuration
	// ADC1 operate independently
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	// Disable the scan conversion so we do one at a time
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	// Don't do continuous conversions - do them on demand
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	// Start conversion by software, not an external trigger
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	// Conversions are 12 bit - put them in the lower 12 bits of the result
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	// Say how many channels would be used by the sequencer
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	// Now do the setup
	ADC_Init(ADC1, &ADC_InitStructure);

	// Enable ADC1
	ADC_Cmd(ADC1, ENABLE);

	// Enable ADC1 reset calibration register
	ADC_ResetCalibration(ADC1);

	// Check the end of ADC1 reset calibration register
	while(ADC_GetResetCalibrationStatus(ADC1));

	// Start ADC1 calibration
	ADC_StartCalibration(ADC1);

	// Check the end of ADC1 calibration
	while(ADC_GetCalibrationStatus(ADC1));

  // Initialize analogOutput module
  analogOutputInit();
	pinMode(0,INPUT);
	pinMode(1,INPUT);
	/* Configure the NVIC Preemption Priority Bits */
	/* 4 bits for pre-emption priority(0-15 PreemptionPriority) and 0 bits for subpriority(0 SubPriority) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	SCB->VTOR = 0x08005000; /* Vector Table Relocation in Internal FLASH. */
}
#endif
#ifdef __cplusplus
}
#endif

