#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"

void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t *pData,uint16_t Size);
void my_USART1_Init(void);

int main(void)
{
	my_USART1_Init();
	Delay_Init();
//	//发送数据a
//	uint8_t a[]={1,2,3,4,5,6};
//	My_USART_SendBytes(USART1,a,6);
	
//	printf("HELLO WORD!I'm a robot. \r\n");
	
	while(1)
	{
		uint32_t currentTick = GetTick();
		uint32_t miliseconds = currentTick % 1000; 
		currentTick/=1000;
		uint32_t seconds = currentTick % 60; 
		currentTick/=60;
		uint32_t minute = currentTick % 60;
		currentTick/=60;
		uint32_t hour = currentTick; 
		printf("%02u:%02u:%02u.%03u\r\n", hour, minute, seconds, miliseconds);
	}
}

//1.作用：使用串口一次性发送多次字节
//2.参数：pData：要发的数据 Size：字节的数量

//
// @简介：使串口一次发送多个字节
// @参数：USATRTX：要发送的串口
// @参数：PData：要发送的数据
// @参数：Size：要发送的数据数量，以字节为单位
//
void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t *pData,uint16_t Size)
{
	for(uint32_t i=0;i<Size;i++)
		{
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)==RESET);
			USART_SendData(USARTx,pData[i]);
		}
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);
}
//
// @简介：对USART1初始化
// 				TX：PA9 RX：PA10
//				115200、双向、8bit、NO、1
void my_USART1_Init(void)
{
//		#1 初始化PA9和PA10
    //开启USARTA的串口	
    //重映射设置
    //	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    //	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	//开启串口A的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//设置串口：TX、PA9、全双工（复用输出推挽）
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//设置串口：RX、PA10、全双工（输入上拉）	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	  
//		#2.初始化USART1
  //设置USARTA的串口	
	// 开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	
	//设置USART1的BaudRate、Mode、WordLength、Parity、StopBits
	USART_InitStruct.USART_BaudRate=115200;//波特率设置115200HZ
	USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;//双向
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//长度8bit
	USART_InitStruct.USART_Parity=USART_Parity_No;//无校验位
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//1位停止位
	
	USART_Init(USART1,&USART_InitStruct);
	//使能USART1
	USART_Cmd(USART1,ENABLE);
}

int fputc(int ch,FILE *f)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	USART_SendData(USART1,(uint8_t)ch);
	return ch;
}
