#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Key.h"
#include "Encoder.h"
#include "User_Menu.h"

int main(){
	OLED_Init();
	Key_Init();
	Encoder_Init();
	while (1){
         MENU_RunMainMenu();//运行主菜单
	}
}