/**
  ****************************(C) COPYRIGHT 2020-2022 HCRT****************************
  * @file       bsp_usart.c/h
  * @brief      空闲中断配置和中断处理函数支持包
  * @note
  * @history
  *  Version        Date           Author          Modification
  *  V1.0.0     2020-02-01          YLT             V1.0 done
  *  V1.0.1     2020-03-16   		YLT               update
  *  V1.1.0     2022-04-05          ZXY               Update
  *  V1.1.1     2022-04-06          ZXY             bug fixed
  * @verbatim
  * [V1.1.0]
  * 2022-04-05:
  * 1. 删去了uart_receive_dma_no_it()函数，直接调用HAL库中的HAL_UART_Receive_DMA
  * 2. 重写了函数名字
  * 3. 添加了使用说明
  * ==================================================================================
  *                                 如何使用该支持包
  * ==================================================================================
  *  因为Cube在生成时默认不会使能中断，且HAL官方库中并没有对于空闲中断的处理函数，所以需要进行以下
  *  配置(eg. USART1)：
  *  1. 在main.c中，添加空闲中断初始化函数：
  *  Usart_IdleIRQ_Init(&huart1);
  *
  *  2. 在stm32f4xx_it.c的USART1_IRQHandler中添加空闲中断用户处理函数：
  *  Usart_IdleIRQ_Handler(&huart1);
  *
  *  3. 在bsp_usart.h中配置串口数据的接收长度
  * ====================================
  *  [V1.0.1]
  *  2020.3.16(update):增加详细的注释 优化程序逻辑 将中断入口函数全部添加到
  *  stm32f4xx_it.c 增加 空闲中断+DMA 接收数据的流程说明
  *
  *  空闲中断介绍 :
  *  接收中断													空闲中断
  *  处理函数 	USARTx_IRQHandler								USARTx_IRQHandler
  *  回调函数 	HAL_UART_RxCpltCallback 				HAL库没有提供
  *  USART 状态寄存器中的位 UART_FLAG_RXNE 		UART_FLAG_IDLE
  *  触发条件 完成一帧数据的接收之后触发一次		串口接收完一帧数据后又过了一个字节的时间没有接收到任何数据
  *	 串口空闲中断即每当串口接收完一帧数据后又过了一个字节的时间没有接收到任何数据则
  *	 触发一次中断，中断处理函数同样为 USARTx_IRQHandler，可以通过 USART 状态寄存器
  *	 中的 UART_FLAG_IDLE 判断是否发生了空闲中断。
  *
  *	 对于DMA UART 发送来说 通道剩余数据个数 = 总共需要接收的个数 - 目前已发送出的数据的个数
  *	 对于DMA UART 接收来说 通道剩余数据个数 = 总共需要接收的个数 - 目前已接收到的数据的个数
  *
  *	 空闲中断的数据长度判断 为什么 USARTX_BUFLEN = USARTX_MAX_LEN - dma_stream->NDTR
  *	 在DMA开启时填充的期望接收到的数据的个数为 USARTX_MAX_LEN 即NDTR寄存器=USARTX_MAX_LEN
  *  而每当DMA搬运完一个数据 NDTR寄存器的值就会减一
  *  因为我们定义了最大接收数设置为数据长度的两倍 USARTX_MAX_LEN = USARTX_BUFLEN * 2
  *  因此当dma_stream->NDTR 的值由USARTX_MAX_LEN 变为 USARTX_BUFLEN时 就满足了判断条件
  *  为什么要将最大接收数设置为数据长度的两倍？
  *  1.留出接收余量 保证数据能能够被全部接收到
  *  2.为了好判断 写判断条件的时候比较简洁
  *  ===================================
  *  [V1.0.0]
  *  写完了所有串口的空闲中断，按需使用
  *  需要在stm32f4xx_it.c 中添加入口函数 uart_receive_handler
  ****************************(C) COPYRIGHT 2020-2022 HCRT****************************
  */
#include "bsp_usart.h"
#include "bsp_buzzer.h"
#include "bsp_led.h"
#include "bsp_ops.h"
#include "bsp_ops.h"
/****** 串口数据储存数组定义 ******/
uint8_t usart1_buff[USART1_BUFFLEN];
//uint8_t usart2_buff[USART2_BUFFLEN];
//uint8_t usart3_buff[USART3_BUFFLEN];
//uint8_t usart6_buff[USART6_BUFFLEN];
uint8_t uart7_buff[UART7_BUFFLEN];
uint8_t uart8_buff[UART8_BUFFLEN];

/******** 数据结构体 ********/
rc_rx_t rc_rx;

/**
 * @brief   空闲中断初始化函数
 * @param   huart: 串口
 * @retval
 */
void Usart_IdleIRQ_Init(UART_HandleTypeDef *huart)
{
    if (USART1 == huart->Instance)
    {
        /** 使能串口中断 **/
        __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
        /** 开启DMA接收 **/
        HAL_UART_Receive_DMA(&huart1, usart1_buff, USART1_MAX_LEN);
    }
    else if (huart == &huart7)
    {
        /** 使能串口中断 **/
        __HAL_UART_ENABLE_IT(&huart7, UART_IT_IDLE);
        /** 开启DMA接收 **/
        HAL_UART_Receive_DMA(&huart7, uart7_buff, UART7_MAX_LEN);
    }
    else if (huart == &huart8)
    {
        /** 使能串口中断 **/
        __HAL_UART_ENABLE_IT(&huart8, UART_IT_IDLE);
        /** 开启DMA接收 **/
        HAL_UART_Receive_DMA(&huart8, uart8_buff, UART8_MAX_LEN);
    }
}

/**
 * @brief   空闲中断用户处理函数
 * @param   huart: 串口
 * @retval
 */
void Usart_IdleIRQ_Handler(UART_HandleTypeDef *huart)
{
    if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
    {
        /** 清除空闲中断标识位 否则会一直不断进入中断 **/
        __HAL_UART_CLEAR_IDLEFLAG(huart);

        /** 关闭DMA传输 **/
//        HAL_UART_DMAStop(huart);
        __HAL_DMA_DISABLE(huart->hdmarx);

        /** 进入空闲中断回调函数 **/
        Usart_IdleIRQ_Callback(huart);

        /** 重新启动DMA传输 **/
        __HAL_DMA_ENABLE(huart->hdmarx);
//        HAL_UART_Receive_DMA(huart, usart1_buff, USART1_MAX_LEN);
    }
}

/**
 * @brief   串口中断回调函数
 * @note    满足数据要求的空闲中断触发后会调用对应串口的回调函数
 * @param   huart: 串口
 * @retval
 */
void Usart1_Idle_Callback(uint8_t *buff)
{

}

void Uart7_Idle_Callback(uint8_t *buff)
{
    switch(buff[0])
    {
        case 0x43:    //CTIN
            //warn_buzzer();
            break;
        case 0x57:    //WALK
            HAL_GPIO_TogglePin(LED_1_PORT,LED_1_PIN);
            break;
        case 0x53:    //STOP
            HAL_GPIO_TogglePin(LED_5_PORT,LED_5_PIN);
            break;
        case 0x46:    //FOLW
            HAL_GPIO_TogglePin(LED_6_PORT,LED_6_PIN);
            break;
        case 0x52:    //RIGH
            HAL_GPIO_TogglePin(LED_7_PORT,LED_7_PIN);
            break;
        case 0x4c:    //LEFT
            HAL_GPIO_TogglePin(LED_8_PORT,LED_8_PIN);
            break;
        default:
            break;
    }
}

void Uart8_Idle_Callback(uint8_t *buff)
{
    Ops_Frame_Parse(&ops_data,buff);
}
/**
 * @brief   空闲中断回调函数
 * @param   huart: 串口
 * @retval
 */
void Usart_IdleIRQ_Callback(UART_HandleTypeDef *huart)
{
    if (USART1 == huart->Instance)
    {
        /** 判断接收到的数据是否为期望的长度 如不是则不进入回调函数 直接开启下一次接收 **/
        if ((USART1_BUFFLEN == USART1_MAX_LEN - __HAL_DMA_GET_COUNTER(huart->hdmarx)))
        {
            Usart1_Idle_Callback(usart1_buff);
        }
        //HAL_UART_Receive_DMA(huart, usart1_buff, USART1_MAX_LEN);
        __HAL_DMA_SET_COUNTER(huart->hdmarx,USART1_MAX_LEN);
    }
    else if (UART7 == huart->Instance)
    {
        if (UART7_BUFFLEN == UART7_MAX_LEN - __HAL_DMA_GET_COUNTER(huart->hdmarx))
        {
            Uart7_Idle_Callback(uart7_buff);
        }
        __HAL_DMA_SET_COUNTER(huart->hdmarx,UART7_MAX_LEN);
    }
    else if (UART8 == huart->Instance)
    {
        if (UART8_BUFFLEN == UART8_MAX_LEN - __HAL_DMA_GET_COUNTER(huart->hdmarx))
        {
            Uart8_Idle_Callback(uart8_buff);
        }
        __HAL_DMA_SET_COUNTER(huart->hdmarx,UART8_MAX_LEN);
    }
}

