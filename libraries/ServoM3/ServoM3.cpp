/*
 Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <Arduino.h> 

#include "ServoM3.h"

#define usToTicks(_us)    (( clockCyclesPerMicrosecond()* _us) / 32)     // converts microseconds to tick (assumes prescale of 8)  
#define ticksToUs(_ticks) (( (unsigned)_ticks * 32)/ clockCyclesPerMicrosecond() ) // converts from ticks back to microseconds


#define TRIM_DURATION       2                               // compensation ticks to trim adjust for digitalWrite delays // 12 August 

//#define NBR_TIMERS        (MAX_SERVOS / SERVOS_PER_TIMER)

static servo_t servos[MAX_SERVOS];                          // static array of servo structures
static volatile int8_t Channel[_Nbr_16timers ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

static uint8_t ServoCounter = 0;                                     // the total number of attached servos


// convenience macros
#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER)) // returns the timer controlling this servo
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)       // returns the index of the servo on this timer
#define SERVO_INDEX(_timer,_channel)  ((_timer*SERVOS_PER_TIMER) + _channel)     // macro to access servo index by timer and channel
#define SERVO(_timer,_channel)  (servos[SERVO_INDEX(_timer,_channel)])            // macro to access servo class by timer and channel

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)  // maximum value in uS for this servo 

/************ static functions common to all instances ***********************/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

void INT_DISABLE(void) 
{ 
	st( asm("CPSID I"););
} 
 
void INT_ENABLE(void) 
{ 
	st( asm("CPSIE I"););
}

static inline void handle_interrupts(timer16_Sequence_t timer)
{
	TIM_TypeDef* TIMx;
	switch(timer)
	{
		case	_timer2:
			TIM_Cmd(TIM2, DISABLE);
			TIMx = TIM2;
			break;
		case	_timer3:
		    TIM_Cmd(TIM3, DISABLE);
			TIMx = TIM3;
			break;
		case	_timer4:
		    TIM_Cmd(TIM4, DISABLE);
			TIMx = TIM4;
			break;
	}
	if( Channel[timer] < 0 )
	{
		TIMx->CNT = 0; // channel set to -1 indicated that refresh interval completed so reset the timer 
	}
	else
	{
		if( SERVO_INDEX(timer,Channel[timer]) < ServoCounter && SERVO(timer,Channel[timer]).Pin.isActive == true )
		{	
			digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,LOW); // pulse this channel low if activated   
		}
	}
	Channel[timer]++;    // increment to the next channel<12
	//SERVO_INDEX(timer,Channel[timer])hannel[timer]=13
	if( SERVO_INDEX(timer,Channel[timer]) < ServoCounter && Channel[timer] < SERVOS_PER_TIMER) 
	{ 
		TIM_TimeBaseStructure.TIM_Period = TIM_GetCounter(TIMx) + SERVO(timer,Channel[timer]).ticks;//接着计数的次数=现有次数+接下来的ticks次数
		if(SERVO(timer,Channel[timer]).Pin.isActive == true)     // check if activated
		{
			digitalWrite( SERVO(timer,Channel[timer]).Pin.nbr,HIGH); // its an active channel so pulse it high   
		}
	}  
	else 
	{ 
	// finished all channels so wait for the refresh period to expire before starting over 
	if( ((unsigned)TIM_GetCounter(TIMx)) + 4 < usToTicks(REFRESH_INTERVAL) )  // allow a few ticks to ensure the next OCR1A not missed
	{
		TIM_TimeBaseStructure.TIM_Period = (unsigned int)usToTicks(REFRESH_INTERVAL);  //
	}
	else 
	{
		TIM_TimeBaseStructure.TIM_Period = TIM_GetCounter(TIMx) + 4;  // at least REFRESH_INTERVAL has elapsed
	}
		Channel[timer] = -1; // this will get incremented at the end of the refresh period to start again at the first channel
	}
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); 	 
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMx, ENABLE); 
}



#ifndef WIRING // Wiring pre-defines signal handlers so don't define any if compiling for the Wiring platform
// Interrupt handlers for Arduino 
	
	#if defined(_useTimerARMtimer2)
	void TIM2_IRQHandler(void)
	{	
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
		{
			handle_interrupts(_timer2); 
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  
		}
	}
	#endif
	#if defined(_useTimerARMtimer3)
	void TIM3_IRQHandler(void)
	{	
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
		{
			handle_interrupts(_timer3); 
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  
		}
	}
	#endif
	#if defined(_useTimerARMtimer4)
	void TIM4_IRQHandler(void)
	{	
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) 
		{
			handle_interrupts(_timer4); 
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  
		}
	}
	#endif
#endif

//init timers
static void initISR(timer16_Sequence_t timer)
{  
	//TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
#if defined (_useTimerARMtimer2)
	if(timer == _timer2)
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

		TIM_TimeBaseStructure.TIM_Period = 999; 
		TIM_TimeBaseStructure.TIM_Prescaler =(36-1); 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 

		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);  

		TIM_Cmd(TIM2, ENABLE); 
	}
#endif
	
#if defined (_useTimerARMtimer3)
	if(timer == _timer3)
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

		TIM_TimeBaseStructure.TIM_Period = 999; 
		TIM_TimeBaseStructure.TIM_Prescaler =(36-1); 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 

		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

		
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);  

		TIM_Cmd(TIM3, ENABLE); 
	}
#endif

#if defined (_useTimerARMtimer4)
	if(timer == _timer4)
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 

		TIM_TimeBaseStructure.TIM_Period = 999; 
		TIM_TimeBaseStructure.TIM_Prescaler =(36-1); 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 

		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;  
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);  

		TIM_Cmd(TIM4, ENABLE); 
	}
#endif
} 

static void finISR(timer16_Sequence_t timer)
{
    //disable use of the given timer
#if defined WIRING   // Wiring
  if(timer == _timer1) {
    #if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
    TIMSK1 &=  ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt
    #else 
    TIMSK &=  ~_BV(OCIE1A) ;  // disable timer 1 output compare interrupt   
    #endif
    timerDetach(TIMER1OUTCOMPAREA_INT); 
  }
  else if(timer == _timer3) {     
    #if defined(__AVR_ATmega1281__)||defined(__AVR_ATmega2561__)
    TIMSK3 &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
    #else
    ETIMSK &= ~_BV(OCIE3A);    // disable the timer3 output compare A interrupt
    #endif
    timerDetach(TIMER3OUTCOMPAREA_INT);
  }
#else
    //For arduino - in future: call here to a currently undefined function to reset the timer
#endif
}

static boolean isTimerActive(timer16_Sequence_t timer)
{
  // returns true if any servo is active on this timer
  for(uint8_t channel=0; channel < SERVOS_PER_TIMER; channel++)
  {
    if(SERVO(timer,channel).Pin.isActive == true)
      return true;
  }
  return false;
}


/****************** end of static functions ******************************/

Servo::Servo()
{

	if( ServoCounter < MAX_SERVOS)
	{
		this->servoIndex = ServoCounter++;                    // assign a servo index to this instance
		servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values  - 12 Aug 2009
		servos[this->servoIndex].Pin.isActive = false;//
	}
	else
	{
		this->servoIndex = INVALID_SERVO ;  // too many servos 
	}
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}


uint8_t Servo::attach(int pin, int min, int max)
{
  if(this->servoIndex < MAX_SERVOS ) 
  {
    pinMode( pin, OUTPUT);                                   // set servo pin to output
    servos[this->servoIndex].Pin.nbr = pin;  //
    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128 
    this->min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
    this->max  = (MAX_PULSE_WIDTH - max)/4; 
    // initialize the timer if it has not already been initialized 
    timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(this->servoIndex);
    if(isTimerActive(timer) == false)
	{
		initISR(timer);    
	}
    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  } 
  return this->servoIndex ;
}

void Servo::detach()  
{
  servos[this->servoIndex].Pin.isActive = false;  
  timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(servoIndex);
  if(isTimerActive(timer) == false)
  {
    finISR(timer);
  }
}

void Servo::write(int value)
{  
  if(value < MIN_PULSE_WIDTH)
  {  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    if(value < 0)
	{	
		value = 0;
	}
    if(value > 180)
	{
		value = 180;
	}
    value = map(value, 0, 180, SERVO_MIN(),  SERVO_MAX());//544~2400
  }
  this->writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {  
    if( value < SERVO_MIN() )          // ensure pulse width is valid
      value = SERVO_MIN();
    else if( value > SERVO_MAX() )
      value = SERVO_MAX();   
    
  	value = value - TRIM_DURATION; //TRIM_DURATION=>2
    value = usToTicks(value);  // convert to ticks after compensating for interrupt overhead - 12 Aug 2009

    INT_DISABLE();
    servos[channel].ticks = value;  // 
    INT_ENABLE();
  } 
}

int Servo::read() // return the value as degrees
{
  return  map( this->readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);     
}

int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
  if( this->servoIndex != INVALID_SERVO )
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks)  + TRIM_DURATION ;   // 12 aug 2009
  else 
    pulsewidth  = 0;

  return pulsewidth;   
}

bool Servo::attached()
{
  return servos[this->servoIndex].Pin.isActive ;
}

