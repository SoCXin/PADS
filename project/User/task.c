/*******************************************************************************************************
*Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : 任务处理模块
*	文件名称 : task.c
*	版    本 : V1.0               
*	描    述 ：
*	日    期 ：2016\12\29             
*	修改记录 :
*			作者      说明
*
* 本软件仅供学习和演示使用，对用户直接引用代码所带来的风险或后果不承担任何法律责任。
*******************************************************************************************************/
#include "includes.h"

/*
********************************************************************************************************
*	函 数 名: KeyScan
*	功能说明: 扫描所有按键。非阻塞，被T0中断周期性的调用
*	形    参:  无
*	返 回 值:  1：按键按下 0：按键未按下
********************************************************************************************************
*/
uint8_t KeyScan(void)
{
	if (KEY0 == 0)
		return 1;
	else
		return 0;
}

/*
*********************************************************************************************************
*	函 数 名: BasicTimer
*	功能说明: 时基，在定时器调用
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Basic_Timer(void)
{	
	static uint8_t Key_Down_Count = 0;
	static uint8_t Key_Up_Count = 0;
	
	g_SysTick ++;	
	
	/***********************按键检测**********************/
	if (FlagGet(g_Tasks_Flag, TASK_FLAG_KEY) == 0)    		//按键事件空闲时处理按键事件，防止在按键处理时再次响应
	{
		if (KeyScan() == 0)
		{ 
			if (Key_Up_Count < 3)         					//只加到3
			Key_Up_Count++;
		}
		else
		{
			if (Key_Down_Count < 3)        					//只加到3
			Key_Down_Count++;
		}

		if ((Key_Up_Count >= 3) && (Key_Down_Count >= 3))  //多次抬起加多次按下才算有效
		{
			FlagSet(g_Tasks_Flag, TASK_FLAG_KEY);			//产生事件标志，给主函数处理
			Key_Up_Count = 0;
			Key_Down_Count = 0;
		}			
	}
	
	/******** 接收处理 uart字节间隔超时或MCU超时重新进入接收状态 ********/
	if (g_dl645.Overtimr_Timer != 0)			//处理超时计时
	{
		g_dl645.Overtimr_Timer--;
		if (g_dl645.Overtimr_Timer == 0)
		{
			g_dl645.Step = ST_START;  			//超时后重新进入等待接受前导码
			UART_RX_ENABLE;						//接收中断使能
			UART_TX_DISABLE;					//发送中断禁止
		}		
	}
	/******** 发送处理 uart 发送延时时间到后发送第一个前导符 **********/
	if (g_dl645.TxDelay_Timer != 0)				//处理超时计时
	{
		g_dl645.TxDelay_Timer--;
		if (g_dl645.TxDelay_Timer == 0)
		{			
			UART0->TXBUF0.TXBUF0 = 0xFE;
			UART_RX_DISABLE;					//接收中断禁止
			UART_TX_ENABLE;						//发送中断使能(发送一个数据中断)	
		}
	}	
}

void SysTick_Task(void)
{
	static uint8_t t_1s = 0;
	static uint8_t t_1min = 0;    

	if (g_SysTick < 100)		
		return;
	g_SysTick = 0;
	/**********************1s事件***********************/		
	FlagSet(g_Tasks_Flag, TASK_FLAG_SEC);			//产生事件标志，给主函数处理
	if (++t_1s < 60)
		return;
	t_1s = 0;
	/**********************1min事件***********************/				
	FlagSet(g_Tasks_Flag, TASK_FLAG_MINUTE);		//产生事件标志，给主函数处理
	if (++t_1min < 10)
		return;			
	t_1min = 0;	    
	/**********************10min事件**********************/		
	
}
/*
********************************************************************************************************
*	函 数 名: KeyTask
*	功能说明: 按键事件
*	形    参:  无
*	返 回 值:  无
********************************************************************************************************
*/
void Key_Task(void)
{
	if (FlagGet(g_Tasks_Flag, TASK_FLAG_KEY))
	{
		NVIC_SystemReset();
		FlagClr(g_Tasks_Flag, TASK_FLAG_KEY);	//清标志
	}
}
/*
********************************************************************************************************
*	函 数 名: SecTask
*	功能说明: 每秒处理的事件。LED灯翻转，读电流电压等信息。
*	形    参:  无
*	返 回 值:  无
********************************************************************************************************
*/
void Sec_Task(void)	
{
	if (FlagGet(g_Tasks_Flag, TASK_FLAG_SEC))
	{
		LED_Toggle();	
		Energy_Add(Get_EnergyPulse());
		FlagClr(g_Tasks_Flag, TASK_FLAG_SEC);	//清标志
	}
}
/*
********************************************************************************************************
*	函 数 名: MinutTask
*	功能说明: 每分钟处理的事件。参数验证
*	形    参:  无
*	返 回 值:  无
********************************************************************************************************
*/
void Minut_Task(void)
{
	if (FlagGet(g_Tasks_Flag, TASK_FLAG_MINUTE))
	{
		Emu_Adjust_Check();    
		FlagClr(g_Tasks_Flag, TASK_FLAG_MINUTE);	//清标志
	}
}

/*
********************************************************************************************************
*	函 数 名: Uart_Task
*	功能说明: 串口任务
*	形    参:  无
*	返 回 值:  无
********************************************************************************************************
*/
void Uart_Task(void)
{
	if (FlagGet(g_Tasks_Flag, TASK_FLAG_UART))
	{
		Uart_Proc();                    		//串口处理程序  
		FlagClr(g_Tasks_Flag, TASK_FLAG_UART);	//清标志
	}
}


