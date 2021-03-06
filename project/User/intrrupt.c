/*******************************************************************************************************
*Copyright (C), 2016, Shanghai Eastsoft Microelectronics Co., Ltd   
*	模块名称 : 中断模块。
*	文件名称 : interrupt.c
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
*********************************************************************************************************
*	函 数 名: T16N0INT_IRQHandler
*	功能说明: UART中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void T16N0INT_IRQHandler(void)
{
	if (T16N0->INT.MAT0IF == 1)
	{
		T16N0->INT.MAT0IF = 0;		
		Basic_Timer();
	}
}
/*
*********************************************************************************************************
*	函 数 名: UART0TINT_IRQHandler
*	功能说明: UART发送中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void UART0TINT_IRQHandler(void)
{
	if (UART0->INT.TXIF)
	{
		UART0->INT.TXIF = 0;		
		if (Uart_Tx())
		{
		
		}
		else									//发送完成
		{
			UART_RX_ENABLE;
			UART_TX_DISABLE;
			g_dl645.Cnt = 0;
			g_dl645.Step = ST_START;			//接收状态
		}		
	}
}
/*
*********************************************************************************************************
*	函 数 名: UART0TINT_IRQHandler
*	功能说明: UART发送中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void UART0RINT_IRQHandler(void)
{
	if(UART0->INT.RXIF)   
	{
		UART0->INT.RXIF = 0;
		Uart_Rx(UART0->RXBUF0.RXBUF0);
	}
	
}
