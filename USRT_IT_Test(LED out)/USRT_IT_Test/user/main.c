#include "stm32f10x.h"
#include "usart.h"

#include "delay.h"

void App_USART1_Init(void);

void APP_OnBoardLED_Init(void);

uint32_t blinkInterval=1000;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	APP_OnBoardLED_Init();
	App_USART1_Init();
	
//	My_USART_SendString(USART1,"Hello word\r\n");
	
	while(1)
	{
		if(blinkInterval>3)
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
			Delay(blinkInterval);
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
			Delay(blinkInterval);			
		}else if(blinkInterval<=3)
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
			Delay(1000);
		}

	}
}

void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
	{
			uint8_t rcvd=USART_ReceiveData(USART1);
		if(rcvd =='0')
		{
			blinkInterval=1000;
		}else if(rcvd == '1')
		{
			blinkInterval=200;
		}else if(rcvd == '2')
		{
			blinkInterval=50;
		}else if(rcvd=='3')
		{
			blinkInterval=3;
		}
	}
	
}	

void APP_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
}

void App_USART1_Init(void)
{
	//#1.初始化串口
	GPIO_InitTypeDef GPIO_InitStruct;
	//PA9 TX 全双工（复用输出推挽） 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//?
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//PA10 RX 全双工（输入上拉）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//#2.初始化USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//?
	USART_InitStruct.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	
	USART_Init(USART1,&USART_InitStruct);
	
	USART_Cmd(USART1,ENABLE);
	
	//#3.配置中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	//#4.配置NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//0~3
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStruct);
}	
