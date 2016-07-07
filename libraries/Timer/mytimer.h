#ifndef __MYTIMER_H
#define __MYTIMER_H




//#include "variant.h"
#include "stm32f10x.h"
#ifdef __cplusplus
 extern "C" {
#endif

typedef  void (*Func)(void *);

typedef enum { t_period = 0, t_single}tKind_t;  // the variable types of the timer class . 


typedef struct{
	
	    Func tFunc;
		uint32_t start;
		uint32_t msec;
		void *data;
		tKind_t type;
        uint8_t num;          //timer num;
		uint8_t flag;
}Timer_t;

typedef struct Node{

		struct Node *prev;
		struct Node *next;
		Timer_t *Timer;

}Node_t;

typedef struct TimerManager_t{
	Node_t Head;
	uint8_t TotalNum;

};

void timer_handler(void);
extern uint8_t Timer_Add(uint32_t ms, Func tfunc, tKind_t type, void *data);
extern void Timer_delete(uint8_t Num);
extern uint32_t get_resitime(uint8_t num);
extern void change_cbFunc(uint8_t num, Func ttFunc);
extern void change_ttype(uint8_t num, tKind_t type);

extern struct TimerManager_t TManager;




#ifdef __cplusplus
}
#endif





#endif /*__MYTIMER_H*/



