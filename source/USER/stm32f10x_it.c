/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include	"BSP_config.h"
#include 	"app.h"
#include	"irq.h"
#include 	"serve_test.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  
  /* 发生错误时必须关闭高压很重要 */
  irq_stop_relay_motion();
  
  while (1)
  {
    if(BUZZER_EN > 0)
    {
        int i = 0;
        BUZZER = !BUZZER;
        for(i=0;i < 1000000;i++);
    }
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	OSIntEnter();
	
	OSTimeTick(); /*CalluC/OS-II'sOSTimeTick()*/
	
	OSIntExit();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/* 中断延时常数 */
#define SOFT_DELAY_STOP		200
#define SOFT_DELAY_SHORT	1
#define SOFT_DELAY_GFI		5
#define SOFT_DELAY_ARC		0

/*
 * 函数名：EXTI9_5_IRQHandler
 * 描述  ：急停按键中断服务程序+功放中断服务程序
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void EXTI9_5_IRQHandler(void)
{
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)/* 复位按键中断 */
	{
		soft_delay_10us(SOFT_DELAY_STOP);
		if(STOP_PIN == 0)
		{
			stop_irq();
			close_test_timer();
		}
		EXTI_ClearFlag(EXTI_Line5);
	}
    /* 出现异常情况清除所有的中断标志,防止死机 */
    {
        EXTI_ClearFlag(EXTI_Line5);
        EXTI_ClearFlag(EXTI_Line6);
        EXTI_ClearFlag(EXTI_Line7);
        EXTI_ClearFlag(EXTI_Line8);
        EXTI_ClearFlag(EXTI_Line9);
    }
	
	/* 功放采用轮询方法 */
// 	if(EXTI_GetITStatus(EXTI_Line9) != RESET)/* 功放报警中断 */
// 	{
// 		soft_delay_us(SOFT_DELAY);
// 		if(AMP_PIN == 0)
// 		{
// 			amp_irq();
// 		}
// 		EXTI_ClearFlag(EXTI_Line9);
// 	}
	OSIntExit();
}

/*
 * 函数名：EXTI15_10_IRQHandler
 * 描述  ：ARC,SHORT,GFI中断服务程序
 * 输入  ：无
 * 输出  ：无
 * 返回  ：无
 */
void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();
	
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)/* ARC中断 */
	{
		/*********************************************/
		//2016.10.8 经测试发现，ARC应该响应比较窄的脉冲信号，所以去掉延时
// 		soft_delay_10us(SOFT_DELAY_ARC);
		/*********************************************/
		if(ARC_PIN == 0)
		{
			/* 直流耐压 电弧侦测关闭 触发电弧中断作短路处理 */
// 			if(cur_mode == DCW && 0 == cur_arc_gear)
// 			{
// 				short_irq();
// 			}
// 			else
			{
				arc_irq();
			}
		}
		EXTI_ClearFlag(EXTI_Line11);
	}
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)/* 短路中断 */
	{
		soft_delay_10us(g_short_delay);
        
		if(SHORT_PIN == 0)
		{
			short_irq();
		}
        
		EXTI_ClearFlag(EXTI_Line10);
	}
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)/* GFI中断 */
	{
		soft_delay_10us(SOFT_DELAY_GFI);
		if(GFI_PIN == 0)
		{
			gfi_irq();
		}
		EXTI_ClearFlag(EXTI_Line12);
	}
	
    /* 出现异常情况清除所有的中断标志 */
    {
        EXTI_ClearFlag(EXTI_Line10);
        EXTI_ClearFlag(EXTI_Line11);
        EXTI_ClearFlag(EXTI_Line12);
        EXTI_ClearFlag(EXTI_Line13);
        EXTI_ClearFlag(EXTI_Line14);
        EXTI_ClearFlag(EXTI_Line15);
    }
    
	OSIntExit();
}


/*******************************************************************************
 * Function Name  : USART2_IRQHandler
 * Description    : This function handles USART2 global interrupt request.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
int debug_led;
void USART2_IRQHandler(void)
{
	OSIntEnter();
    
	//接收中断
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		p_prvvUARTRxISR();
		debug_led++;
	}
	//发送中断
	else if (USART_GetITStatus(USART2, USART_IT_TXE) == SET)
	{
		p_prvvUARTTxReadyISR();
	}
	else
	{
	}
    
	OSIntExit();
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
