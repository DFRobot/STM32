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

#include "Arduino.h"



#ifdef __cplusplus
 extern "C" {
#endif
extern uint8_t pinEnabled[PINS_COUNT];
extern void analogOutputInit(void);
extern int _readResolution ;
extern int _writeResolution;


extern inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to);
extern void pinMode( uint32_t ulPin, uint32_t ulMode )
{
	
	if ( ulPin > PINS_COUNT )
    {
        return ;
    }

  GPIO_TypeDef *gpio_port = g_APinDescription[ulPin].pPort;
  uint16_t gpio_pin = g_APinDescription[ulPin].ulPin;

  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(g_APinDescription[ulPin].ulPeripheral,ENABLE);

  GPIO_InitStructure.GPIO_Pin = gpio_pin;

	switch ( ulMode )
    {
        case INPUT:
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        break ;

        case INPUT_PULLUP:
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        break ;

        case INPUT_PULLDOWN:
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
        break;
	

        case OUTPUT:
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        break ;

        case AF_OUTPUT_PUSHPULL:	//Used internally for Alternate Function Output PushPull(TIM, UART, SPI etc)
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        break;

        case AF_OUTPUT_DRAIN:		//Used internally for Alternate Function Output Drain(I2C etc)
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        break;

        case AN_INPUT:				//Used internally for ADC Input
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
        break;
	 case OUTPUT_OD	:			//Used internally for ADC Input
          GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
          GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        break;
	#if 0	
     case TIMER_PWM:
	 {
		  uint32_t ulValue = 0;
 		  uint32_t ulChannel;
	 	
		if ( g_APinDescription[ulPin].ulTimerPeripheral == NULL)
			{
			// Defaults to digital write
			pinMode(ulPin, OUTPUT);
			ulValue = mapResolution(ulValue, _writeResolution, 8);
			if (ulValue < 128)
			  digitalWrite(ulPin, LOW);
			else
			  digitalWrite(ulPin, HIGH);
		
			return;
		  }
		
			TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
			TIM_OCInitTypeDef  TIM_OCInitStructure;
		    uint16_t TIM_CCR = 0;
		    ulValue = mapResolution(ulValue, _writeResolution, PWM_RESOLUTION);		//对于PWM 模块来说 该函数直接返回ulValue.
		
			//PWM Frequency : 1000 Hz,Timer counter clk:1MHz
			uint16_t TIM_Prescaler = (uint16_t)(SystemCoreClock / 1000000) - 1;
			uint16_t TIM_ARR = (uint16_t)(1000000 / PWM_FREQUENCY) - 1;
		
		
		   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
           GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		   GPIO_Init(gpio_port, &GPIO_InitStructure);  
		   
		  if (!pinEnabled[ulPin]) {
			// Setup PWM for this pin
		
			// AFIO clock enable
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		
			// TIM clock enable
			if(g_APinDescription[ulPin].ulTimerPeripheral == TIM1)
			  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
			else if(g_APinDescription[ulPin].ulTimerPeripheral == TIM2)
			{
			  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			}
			else if(g_APinDescription[ulPin].ulTimerPeripheral == TIM3)
			{
			  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			}
			else if(g_APinDescription[ulPin].ulTimerPeripheral == TIM4)
			{
			  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			}
			else if(g_APinDescription[ulPin].ulTimerPeripheral == TIM5)
			{
			  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			}
			else if(g_APinDescription[ulPin].ulTimerPeripheral == TIM8)
			  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE);
		
			// Time base configuration
			TIM_TimeBaseStructure.TIM_Period = TIM_ARR;
			TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//0
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		
			//for TIM1 and TIM8
			TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
			TIM_TimeBaseInit(g_APinDescription[ulPin].ulTimerPeripheral, &TIM_TimeBaseStructure);
			pinEnabled[ulPin] = 1;
		  }
		
			// PWM1 Mode configuration
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			TIM_OCInitStructure.TIM_Pulse = TIM_CCR;
		
		  //for TIM1 and TIM8
		  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
		  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
		  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
		  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
		
			if(g_APinDescription[ulPin].ulTimerChannel == TIM_Channel_1)
			{
				// PWM1 Mode configuration: Channel1
				TIM_OC1Init(g_APinDescription[ulPin].ulTimerPeripheral, &TIM_OCInitStructure);
				TIM_OC1PreloadConfig(g_APinDescription[ulPin].ulTimerPeripheral, TIM_OCPreload_Enable);
			}
			else if(g_APinDescription[ulPin].ulTimerChannel == TIM_Channel_2)
			{
				// PWM1 Mode configuration: Channel2
				TIM_OC2Init(g_APinDescription[ulPin].ulTimerPeripheral, &TIM_OCInitStructure);
				TIM_OC2PreloadConfig(g_APinDescription[ulPin].ulTimerPeripheral, TIM_OCPreload_Enable);
			}
			else if(g_APinDescription[ulPin].ulTimerChannel == TIM_Channel_3)
			{
				// PWM1 Mode configuration: Channel3
				TIM_OC3Init(g_APinDescription[ulPin].ulTimerPeripheral, &TIM_OCInitStructure);
				TIM_OC3PreloadConfig(g_APinDescription[ulPin].ulTimerPeripheral, TIM_OCPreload_Enable);
			}
			else if(g_APinDescription[ulPin].ulTimerChannel == TIM_Channel_4)
			{
				// PWM1 Mode configuration: Channel4
				TIM_OC4Init(g_APinDescription[ulPin].ulTimerPeripheral, &TIM_OCInitStructure);
				TIM_OC4PreloadConfig(g_APinDescription[ulPin].ulTimerPeripheral, TIM_OCPreload_Enable);
			}
		
			TIM_ARRPreloadConfig(g_APinDescription[ulPin].ulTimerPeripheral, ENABLE);
		
			// TIM enable counter
			TIM_Cmd(g_APinDescription[ulPin].ulTimerPeripheral, ENABLE);
		
		  //for TIM1 and TIM8
		  TIM_CtrlPWMOutputs(g_APinDescription[ulPin].ulTimerPeripheral, ENABLE);
	 }
		  return;
#endif

		
        default:
        break ;
    }


  GPIO_Init(gpio_port, &GPIO_InitStructure);  
}

extern void digitalWrite( uint32_t ulPin, uint32_t ulVal )
{
  /* Handle */
	if ( ulPin > PINS_COUNT )
  {
    return ;
  }

  if ( ulVal == HIGH )
  {
    g_APinDescription[ulPin].pPort->BSRR = g_APinDescription[ulPin].ulPin;
  }
  else
  {
    g_APinDescription[ulPin].pPort->BRR = g_APinDescription[ulPin].ulPin;
  }
}

extern int digitalRead( uint32_t ulPin )
{
	if ( ulPin > PINS_COUNT )
    {
        return LOW ;
    }

	return GPIO_ReadInputDataBit(g_APinDescription[ulPin].pPort,g_APinDescription[ulPin].ulPin) ;
}

#ifdef __cplusplus
}
#endif

