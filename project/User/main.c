/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V1.0
*	描    述 ：
*	日    期 ：2016\12\29             
*	修改记录 :
*			作者	版本	说明
*			9527	V1.0	1、波特率9600，8,n,1.偶校验	
* 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************************/
#include "includes.h"
/* 全局变量 */
volatile uint8_t  g_Tasks_Flag = 0;		//任务标志
volatile uint16_t g_SysTick    = 0;		//系统时间

/*
********************************************************************************************************
*	函 数 名: main
*	功能说明: 系统入口函数
*	形    参：无
*	返 回 值: 错误代码(无需处理)
********************************************************************************************************
*/
int main(void)
{	
	DISABLE_INTERRUPT();
	MCU_Init();
	SYS_Init();
	ENABLE_INTERRUPT();

	while(1)
	{
//		WDT_CLEAR();
        SysTick_Task();
		Key_Task();
		Uart_Task();
		Sec_Task() ;	
		Minut_Task();
		Read_UIP();	
	}	
}
