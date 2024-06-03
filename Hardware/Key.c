#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
 * @brief 初始化按键
 */
void Key_Init(void){
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//初始化GPIOB上拉输入
}

uint8_t Key_GetEvent_Up(){
	// if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键1按下
	// {
	// 	Delay_ms(20);											//延时消抖
	// 	while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0);	//等待按键松手
	// 	Delay_ms(20);											//延时消抖
	// 	return 1;
	// }
	return 0;
}

uint8_t Key_GetEvent_Down(){
	// if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)			//读PB12输入寄存器的状态，如果为0，则代表按键1按下
	// {
	// 	Delay_ms(20);											//延时消抖
	// 	while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0);	//等待按键松手
	// 	Delay_ms(20);											//延时消抖
	// 	return 1;
	// }
	return 0;
}

uint8_t Key_GetEvent_Enter(){
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)			//读PB15输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		return 1;
	}
	return 0;
}
uint8_t Key_GetEvent_Back(){
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0)			//读PA10输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		return 1;
	}
	return 0;
}
