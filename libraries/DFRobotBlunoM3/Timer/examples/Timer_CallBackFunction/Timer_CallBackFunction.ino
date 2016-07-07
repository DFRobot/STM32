
#include "timerobj.h"
#include <stdlib.h>
unsigned char td;

/*define the objects of the Timer */
Timer timer1(1000, myfunc, t_period, NULL );
Timer timer2(3000, myfunc1, t_period, NULL );

void myfunc(void *data)
{
     Serial1.println("it is processing the first call back function! on timer1\n");  
}

void myfunc1(void *data)
{
     static int i;
     i++;
     Serial1.println(i);
     Serial1.println("it is processing the second call back function! on timer2\n");  
   
     if (i == 3)
     {
         Serial1.println("changing the callback function for the timer1\n");  
         timer1.change_callbackFunc(myfunc2);  // changing the callback function for the object timer1
      }  
  
}

void myfunc2(void *data)
{
  
      static int i;
      i++;
      Serial1.println(i);
      Serial1.println("myfunc2");   
      if (i == 2)
      {
         Serial1.println("changing the kind of type for timer2\n");  
         timer2.change_type(t_single);  // changing the mode for the timer2 from t_period to t_single.
      }
}

void setup() {
  
      Serial1.begin(9600);
  
}

void loop() {
   
 
}

