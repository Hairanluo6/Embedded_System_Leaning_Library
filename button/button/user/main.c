#include "stm32f10x.h"

int main(void)
{
	//打开GPIOC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//初始化GPIO,设置GPIOA0为通用输出开漏，2MHz
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//初始化GPIOA1
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	
//写入
//	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET );//写0
//	
//	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET );//写1

	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==Bit_RESET)
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET );
		}
		else
		{
			GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET );
		}
		
	}
}
