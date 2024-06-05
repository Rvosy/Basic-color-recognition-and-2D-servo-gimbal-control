/******************************** Copyright (c) ***************************************
 * @file: Remote.h
 * @author: Rvosy
 * @date: 2024-06-05
 * @version: V1.0
 * @brief: 遥控器与NEC解码驱动
 * @note: 
 *************************************************************************************/
#ifndef __REMOTE_H
#define __REMOTE_H

#define REMOTE_ID 0x00
#define RDATA GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
void Remote_Init(void);
uint8_t Remote_Scan(void);
uint8_t Remote_GetEvent_Up(void);
uint8_t Remote_GetEvent_Down(void);
uint8_t Remote_GetEvent_Enter(void);
uint8_t Remote_GetEvent_Back(void);
#endif // __REMOTE_H
