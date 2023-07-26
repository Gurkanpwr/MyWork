
   
#include "delay.h"

// ticks counter
static __IO uint32_t counter;

void SysTick_Handler()  
{
   if(counter > 0)
      counter--;   
}

void delay_ms(uint32_t time)
{
   counter = time;

   while(counter);
}

void DelayInit()
{
   SystemCoreClockUpdate();
   SysTick_Config(SystemCoreClock / 1000);
}

