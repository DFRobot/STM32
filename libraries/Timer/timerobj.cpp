#include "timerobj.h"

Timer::Timer(void)
{
	conf_flag = 0;
}

Timer::Timer(uint32_t tms, Func tfunc, tKind_t mode, void *tdata)
{
	asm("CPSIE	I");   //¿ªÖÐ¶Ï

	num = Timer_Add(tms, tfunc, mode, tdata);
	
	msec = tms;
	
	type = mode;
	
    conf_flag = 1;

}



Timer::~Timer()
{
	

   
   Timer_delete(num);
  
}

void Timer::config(uint32_t tms, Func tfunc, tKind_t mode, void *tdata)
{
    if (!conf_flag)
   	{
   	    
		num = Timer_Add(tms, tfunc, mode, tdata);
		msec = tms;
	
		type = mode;
	
    }
	
	conf_flag = 1;



}





uint8_t Timer::get_tNum()
{

		return num;

}





uint32_t Timer::get_resTime()
{


 	return get_resitime(this->num);
	



}



void Timer::change_callbackFunc(Func ttFunc)
{

	
	change_cbFunc(this->num, ttFunc);
}


void Timer::change_type( tKind_t mode)
{

    change_ttype(this->num, mode);

}
















	 

