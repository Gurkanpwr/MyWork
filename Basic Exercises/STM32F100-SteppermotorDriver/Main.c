#include "stm32f10x.h"
#include "delay.h"

uint16_t buttonState2=0;
uint16_t buttonState=0;
uint16_t counter=0;
void gpioConfig(){

	
	GPIO_InitTypeDef GPIOInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//for led clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//for button clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//motor
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOInitStructure);
	//LED
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIOInitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInitStructure);
	
	//BUTTON
	GPIOInitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIOInitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 ;
	GPIO_Init(GPIOC,&GPIOInitStructure);

}
void delay(uint32_t time){
	
	while(time--);



}
void fullStep(){
	for(int i=0; i<512; i++){
			
			GPIO_SetBits(GPIOA,GPIO_Pin_0);//in1
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_1);//in2
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_SetBits(GPIOA,GPIO_Pin_2);//in3
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_SetBits(GPIOA,GPIO_Pin_3);//in4
			
			delay_ms(2);
		
		}


}

void halfStep(){
	
	for(int i=0; i<512; i++){
			
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_SetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_SetBits(GPIOA,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
			GPIO_SetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_SetBits(GPIOA,GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_SetBits(GPIOA,GPIO_Pin_2);
			GPIO_SetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
			GPIO_ResetBits(GPIOA,GPIO_Pin_2);
			GPIO_SetBits(GPIOA,GPIO_Pin_3);
			
			delay_ms(2);
		
		}


}
int main(){
	gpioConfig();
	
	while(1){
		buttonState=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0);
		buttonState2=GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0);
		fullStep();
	if(buttonState==1||buttonState2==1){
			
		}
	else{
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	
	}
}



}
