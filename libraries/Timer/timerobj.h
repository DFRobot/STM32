#ifndef __TIMEROBJ_H
#define __TIMEROBJ_H


#include "mytimer.h"

class Timer{
public:
     Timer(void);
     Timer(uint32_t ms, Func tfunc, tKind_t type, void *data);
	  ~Timer();
	 void config(uint32_t ms, Func tfunc, tKind_t type, void *data);
     uint8_t get_tNum();
	 uint32_t get_resTime();
	 void change_callbackFunc(Func ttFunc);
	 void change_type(tKind_t mode);

	 
private:

	uint32_t msec;

	tKind_t type;
    uint8_t num;
	uint8_t conf_flag;





};




#endif /*TIMER_CLASS_H*/










