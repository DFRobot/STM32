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
//abort
#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	uint32_t curPin;     //当前用到的管脚
	int16_t presc;      //根据传入pwmMode的频率计算得到的预分频，此值决定当前产生的pwm的频率
	uint16_t arr;        //根据传入pwmMode的模式得到的分辨率，8位或者16位----对应的值分别为255、4096.
	
}PWM_Manager_t;

static PWM_Manager_t PWM_Manager;


int _readResolution = 10;
int _writeResolution = 8;

void analogReadResolution(int res) {
	_readResolution = res;
}

void analogWriteResolution(int res) {
	_writeResolution = res;
}

 inline uint32_t mapResolution(uint32_t value, uint32_t from, uint32_t to) {
	if (from == to)
		return value;
	if (from > to)
		return value >> (from-to);
	else
		return value << (to-from);
}

eAnalogReference analog_reference = AR_DEFAULT;

uint8_t MapArr[][1] = {
				{15},
				{16},
				{17},
				{18},
				{19},
				{20},
				{2},
				{3},
				{10},
				{11},
				{12},
				{13}
};










void analogReference(eAnalogReference ulMode)
{
	analog_reference = ulMode;
}

uint32_t analogRead(uint32_t ulPin)
{
  uint32_t ulValue = 0;
  uint32_t ulChannel;
  uint32_t temp;

/*  if (ulPin < A0)
    ulPin += A0;*/

 ulPin = MapArr[ulPin][0];
  
  
  

 


  ulChannel = g_APinDescription[ulPin].ulADCChannelNumber ;

  
	if ( ulChannel == NONE )
	{
		return -1;
	}

	ADC_RegularChannelConfig(ADC1, g_APinDescription[ulPin].ulADCChannelNumber, 1, ADC_SampleTime_55Cycles5);
	//Start ADC1 Software Conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	// Wait until conversion completion
	// while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

  // Read the value
  ulValue = ADC_GetConversionValue(ADC1);
  ulValue = mapResolution(ulValue, ADC_RESOLUTION, _readResolution);

	return ulValue;
}

#if 0
static void TC_SetCMR_ChannelA(Tc *tc, uint32_t chan, uint32_t v)
{
	tc->TC_CHANNEL[chan].TC_CMR = (tc->TC_CHANNEL[chan].TC_CMR & 0xFFF0FFFF) | v;
}

static void TC_SetCMR_ChannelB(Tc *tc, uint32_t chan, uint32_t v)
{
	tc->TC_CHANNEL[chan].TC_CMR = (tc->TC_CHANNEL[chan].TC_CMR & 0xF0FFFFFF) | v;
}
#endif

uint8_t pinEnabled[PINS_COUNT];



void analogWrite(uint32_t ulPin, uint32_t ulValue) {

	/*if (ulPin >= PINS_COUNT )
	{
		return;
	}*/

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

  ulValue = mapResolution(ulValue, _writeResolution, PWM_RESOLUTION);       //对于PWM 模块来说 该函数直接返回ulValue.

	//PWM Frequency : 1000 Hz,Timer counter clk:1MHz
	uint16_t TIM_Prescaler = (uint16_t)(SystemCoreClock / 1000000) - 1;
	uint16_t TIM_ARR = (uint16_t)(1000000 / PWM_FREQUENCY) - 1;

	uint16_t Duty_Cycle = (uint16_t)((ulValue * 100) / 255);
	// TIM Channel Duty Cycle(%) = (TIM_CCR / TIM_ARR + 1) * 100
	uint16_t TIM_CCR = (uint16_t)((Duty_Cycle * (TIM_ARR + 1)) / 100);

  pinMode(ulPin, AF_OUTPUT_PUSHPULL);

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




void analogOutputInit(void) {
	uint8_t i;
	for (i=0; i<PINS_COUNT; i++)
		pinEnabled[i] = 0;
}

void pwmMode( uint32_t ulPin, uint32_t pwmFre, uint32_t pwmMode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
        
        
        
    if ( ulPin > PINS_COUNT )
    {
        return ;
    }

  GPIO_TypeDef *gpio_port = g_APinDescription[ulPin].pPort;
  uint16_t gpio_pin = g_APinDescription[ulPin].ulPin;

	if ( g_APinDescription[ulPin].ulTimerPeripheral == NULL)
	{
		/*
			// Defaults to digital write
			pinMode(ulPin, OUTPUT);
			ulValue = mapResolution(ulValue, _writeResolution, 8);
			if (ulValue < 128)
			  digitalWrite(ulPin, LOW);
			else
			  digitalWrite(ulPin, HIGH);
		
			return;
			*/
	}
	

	 if (!pinEnabled[ulPin]) {
			
			
			PWM_Manager.curPin = ulPin;
	
			if (pwmMode == PWM_8_BIT){
				
				PWM_Manager.arr = 255;
				}
			else if (pwmMode == PWM_12_BIT) {
				PWM_Manager.arr = 4095;
				}
			else
				{

			}
				
			
			PWM_Manager.presc =(SystemCoreClock / (pwmFre * PWM_Manager.arr) ) - 1;

			if (PWM_Manager.arr == 4095 && (pwmFre * PWM_Manager.arr) > 72000000)
				PWM_Manager.presc = 0;

		    
			if (PWM_Manager.arr == 255 && (pwmFre * PWM_Manager.arr) > 72000000)
							PWM_Manager.presc = 0;
				
			// Setup PWM for this pin
                     RCC_APB2PeriphClockCmd(g_APinDescription[ulPin].ulPeripheral,ENABLE);

                        GPIO_InitStructure.GPIO_Pin = gpio_pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(g_APinDescription[ulPin].pPort, &GPIO_InitStructure); 
			
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
			TIM_TimeBaseStructure.TIM_Period = PWM_Manager.arr;
			TIM_TimeBaseStructure.TIM_Prescaler = PWM_Manager.presc;
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
			TIM_OCInitStructure.TIM_Pulse = 100;
		
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

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void pwmWrite(uint32_t ulPin, uint32_t ulValue)
{
	
		  //PWM Frequency : 1000 Hz,Timer counter clk:1MHz
	
	
	uint16_t TIM_CCR = ulValue;
	   
	   if(g_APinDescription[ulPin].ulTimerChannel == TIM_Channel_1)
		   {
			   // PWM1 Mode configuration: Channel1
			  g_APinDescription[ulPin].ulTimerPeripheral->CCR1 = TIM_CCR;
		   }
		   else if(g_APinDescription[ulPin].ulTimerChannel == TIM_Channel_2)
		   {
			   // PWM1 Mode configuration: Channel2
			   g_APinDescription[ulPin].ulTimerPeripheral->CCR2 = TIM_CCR;
		   }
		   else if(g_APinDescription[ulPin].ulTimerChannel == TIM_Channel_3)
		   {
			   // PWM1 Mode configuration: Channel3
			   g_APinDescription[ulPin].ulTimerPeripheral->CCR3 = TIM_CCR;
		   }
		   else if(g_APinDescription[ulPin].ulTimerChannel == TIM_Channel_4)
		   {
			   // PWM1 Mode configuration: Channel4
			  g_APinDescription[ulPin].ulTimerPeripheral->CCR4 = TIM_CCR;
		   }
	

	return;
}


void adcMode(uint32_t ulPin, uint8_t Mode)
{

  	  ADC_InitTypeDef ADC_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;
	


      


	  ulPin = MapArr[ulPin][0];
	   
	   
	   



	  
	  /*config GPIO, ADC RCC*/
	  RCC_APB2PeriphClockCmd(g_APinDescription[ulPin].ulPeripheral,ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE); //制定为 adc1模块


	  /* Configure GPIO as analog input -------------------------*/
	  GPIO_InitStructure.GPIO_Pin = g_APinDescription[ulPin].ulPin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	  GPIO_Init(g_APinDescription[ulPin].pPort, &GPIO_InitStructure);

      
	  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
      ADC_InitStructure.ADC_ScanConvMode = DISABLE;
      ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
      ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
      ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
      ADC_InitStructure.ADC_NbrOfChannel = 1;
      ADC_Init(ADC1, &ADC_InitStructure);


      /* ADC1 regular channel14 configuration */ 
	  ADC_RegularChannelConfig(ADC1, g_APinDescription[ulPin].ulADCChannelNumber, 1, ADC_SampleTime_55Cycles5);

	  /*enable ADC1 module*/
      ADC_Cmd(ADC1, ENABLE);
	
	  
	  
	  /* Enable ADC1 reset calibration register */	 
	  ADC_ResetCalibration(ADC1);
	  /* Check the end of ADC1 reset calibration register */
	  while(ADC_GetResetCalibrationStatus(ADC1));

		  
	  /* Start ADC1 calibration */
	  ADC_StartCalibration(ADC1);
	  /* Check the end of ADC1 calibration */
	  while(ADC_GetCalibrationStatus(ADC1));

      switch(Mode)	  
      {
		case ADC_8_BIT:  _readResolution = 8; break;
		case ADC_10_BIT:  _readResolution = 10; break;
		case ADC_12_BIT: _readResolution = 12; break;
		default: 
			 _readResolution = 10; break;
	  }


}

uint32_t adcRead(uint32_t ulPin)
{
	uint32_t ulValue = 0;
	uint32_t ulChannel;

	
	  ulPin = MapArr[ulPin][0];
	   
	   
	   

	ulChannel = g_APinDescription[ulPin].ulADCChannelNumber ;

	  
	if ( ulChannel == NONE )
	{
		return -1;
	}

	
	//Start ADC1 Software Conversion
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);

	// Wait until conversion completion
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

	// Read the value
	ulValue = ADC_GetConversionValue(ADC1);
	ulValue = mapResolution(ulValue, ADC_RESOLUTION, _readResolution);

	return ulValue;



}








#ifdef __cplusplus
}
#endif
