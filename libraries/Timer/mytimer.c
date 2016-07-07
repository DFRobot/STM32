#include "mytimer.h"
#include <stdio.h>
#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_rcc.h"





__IO uint32_t TIM7_TimingMillis;
struct TimerManager_t TManager;
void timer_handler(void);





void TIM7_IRQHandler(void)
{
  
  if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
  {
   
     TIM7_TimingMillis++;
     timer_handler();
     TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
	 
  }
}



void TIM7_Configuration(void)
{
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
      
      
    
  // RCC_Configuration();

  /* NVIC Configuration */
 NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM7 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&NVIC_InitStructure);
  
 
  
  TIM_TimeBaseStructure.TIM_Period = 1000;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM7, 71, TIM_PSCReloadMode_Immediate);

 
  TIM_ClearFlag(TIM7, TIM_FLAG_Update);
  /* TIM IT enable */  
  TIM_ITConfig(TIM7, TIM_IT_Update , ENABLE);
  
  /* TIM7 enable counter */
  TIM_Cmd(TIM7, ENABLE);

  
}



void timer_handler(void)
{
       Node_t *tail , *temp;
	   //aabb=12;

       if (TManager.Head.prev != NULL)
       {
            for (tail = TManager.Head.next; tail != &TManager.Head; tail = temp)
            {
                    temp = tail->next;
                    //aabb++;
                    if (TIM7_TimingMillis - tail->Timer->start == tail->Timer->msec && tail->Timer->flag == 1)
                    {
                            tail->Timer->tFunc(tail->Timer->data);
                            if (tail->Timer->type == t_single)
                            {
                                    tail->Timer->flag = 0;
                                    //Timer_delete(tail->Timer->num);
                                    Timer_delete(tail->Timer->num);
                            }
                            else
                            {
                                    tail->Timer->start = TIM7_TimingMillis;
                            }
                    }
            
            
            }
       }







}


uint8_t Timer_Add(uint32_t ms, Func tfunc, tKind_t type, void *data)
{

	Timer_t *timer;
	Node_t *newtimer;
	Node_t *tail;

	if (TManager.TotalNum == 0)
	{
		   
	      
		TManager.Head.prev = &TManager.Head;
		TManager.Head.next = &TManager.Head;
		 
        TIM7_Configuration();
	

	}
    
    timer = (Timer_t *)malloc(sizeof(Timer_t));
	if (timer == NULL)
		return -1;
	

	timer->msec = ms;
	timer->tFunc = tfunc;
	timer->type = type;
	timer->data = data;
    timer->start = TIM7_TimingMillis;
	timer->num = TManager.TotalNum;
    timer->flag = 1;
    newtimer = (Node_t *)malloc(sizeof(Node_t));
    if (timer == NULL)
		return -1;
	
    newtimer->Timer = timer;
	for (tail = &TManager.Head; tail->next != &TManager.Head; tail = tail ->next)
	{
		
	
	}

	newtimer->next = &TManager.Head;
    newtimer->prev = tail;

	tail->next = newtimer;
	TManager.Head.prev = newtimer;
    TManager.TotalNum++;

	return  timer->num;
	

}

void Timer_delete(uint8_t Num)
{
    int i = 0;
    Node_t *tail, *temp;
  
    
     for (tail = TManager.Head.next; tail != &TManager.Head; tail = temp)
     {

	 
	
          temp = tail->next;
		 
          if (tail->Timer->num == Num)
          {
           
              temp->prev = tail->prev;
			     
              tail->prev->next = temp;
          
              if(tail->Timer->data != NULL)
                free(tail->Timer->data);
              free(tail->Timer);
              free(tail);
              TManager.TotalNum--;
          }
     }
	
    
}


uint32_t get_resitime(uint8_t num)
{
	 Node_t *tail;
    
    
     for (tail = TManager.Head.next; tail != &TManager.Head; tail = tail->next)
     {
     	if (tail->Timer->num == num)
     	{
			return tail->Timer->msec - (TIM7_TimingMillis - tail->Timer->start);
		}


	 }
	

}

void change_cbFunc(uint8_t num, Func ttFunc)
{
	Node_t *tail;
	   
	   
		for (tail = TManager.Head.next; tail != &TManager.Head; tail = tail->next)
		{
		   if (tail->Timer->num == num)
		   {
		   	  tail->Timer->tFunc = ttFunc;
			  return;
		   }
	
	
		}


}


void change_ttype(uint8_t num, tKind_t type)
{

		Node_t *tail;
	   
	   
		for (tail = TManager.Head.next; tail != &TManager.Head; tail = tail->next)
		{
		   if (tail->Timer->num == num)
		   {
		   	  tail->Timer->type = type;
			  return;
		   }
	
	
		}



}



