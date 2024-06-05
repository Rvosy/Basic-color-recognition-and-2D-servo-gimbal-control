#include "stm32f10x.h"                  // Device header
#include "MENU.h"
#include "User_MENU.h"
#include "OLED.h"
#include "Remote.h"
void MENU_RunMainMenu(void){
    static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                   {"开始识别", RunMain},
                                                   {"阈值设置", NULL},
                                                   {"测试", MENU_RunTestMenu},
                                                   {"关于", MENU_Information},
                                                   {".."}};

    static MENU_HandleTypeDef MENU = {.OptionList = MENU_OptionList};

    MENU_RunMenu(&MENU);
}

void RunMain(void){
    menu_command_callback(BUFFER_CLEAR);
    menu_command_callback(SHOW_STRING, 5, 0, "MainFun");
    menu_command_callback(BUFFER_DISPLAY);
    while (1){
        if (menu_command_callback(GET_EVENT_ENTER) || menu_command_callback(GET_EVENT_BACK)) return;
    }
}

void MENU_RunTestMenu(void){
    static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                   {"喇叭", NULL},
                                                   {"舵机", MENU_RunSevroMenu},
                                                   {"遥控器", RunNECTest},
                                                   {".."}};

    static MENU_HandleTypeDef MENU = {.OptionList = MENU_OptionList};

    MENU_RunMenu(&MENU);
}

void MENU_RunSevroMenu(void){
    static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                   {"调试", MENU_RunSevroDebugMenu},
                                                   {"画图", MENU_RunSevroDrawMenu},
                                                   {".."}};

    static MENU_HandleTypeDef MENU = {.OptionList = MENU_OptionList};

    MENU_RunMenu(&MENU);
}
void MENU_RunSevroDebugMenu(void){
    static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                   {"自检", NULL},
                                                   {"水平", NULL},
                                                   {"垂直", NULL},
                                                   {".."}};

    static MENU_HandleTypeDef MENU = {.OptionList = MENU_OptionList};

    MENU_RunMenu(&MENU);
}
void MENU_RunSevroDrawMenu(void){
    static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                   {"画圆", NULL},
                                                   {"画矩形", NULL},
                                                   {"画三角形", NULL},
                                                   {".."}};

    static MENU_HandleTypeDef MENU = {.OptionList = MENU_OptionList};
    MENU_RunMenu(&MENU);
}

void RunNECTest(void){
    menu_command_callback(BUFFER_CLEAR);
    menu_command_callback(SHOW_STRING, 5, 0, "遥控器测试");
    menu_command_callback(SHOW_STRING, 5, 20, "键码:");
    menu_command_callback(BUFFER_DISPLAY);
    while (1){
        u8 key = Remote_Scan();
        OLED_ShowHexNum(45, 20, key, 2, OLED_8X16);
        menu_command_callback(BUFFER_DISPLAY);
        if (menu_command_callback(GET_EVENT_BACK)) return;
    }
}

void MENU_Information(void){
    menu_command_callback(BUFFER_CLEAR);
    menu_command_callback(SHOW_STRING, 5, 0, "识别+控制题");
    menu_command_callback(BUFFER_DISPLAY);
    while (1){
        if (menu_command_callback(GET_EVENT_ENTER) || menu_command_callback(GET_EVENT_BACK)) return;
    }
    
   

}