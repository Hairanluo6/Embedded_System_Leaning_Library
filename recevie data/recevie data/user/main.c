#include "stm32f10x.h"

void my_USART1_Init(void);
void My_OnBoardLED_Init(void);

int main(void)
{
	my_USART1_Init();
	My_OnBoardLED_Init();
	while(1)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
		uint8_t byteRcvd=USART_ReceiveData(USART1);
		
		if(byteRcvd =='0')
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
		}
		else if(byteRcvd =='1')
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
		}
	}
}


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

void My_OnBoardLED_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
	
}
