//
// Created by 13763 on 2022/4/5.
//

#ifndef BSP_USART_H
#define BSP_USART_H

#include "usart.h"

/******** 配置串口数据的长度 ********/
#define USART1_BUFFLEN 25
#define USART1_MAX_LEN USART1_BUFFLEN * 2

//#define USART2_BUFFLEN 28
//#define USART2_MAX_LEN USART2_BUFFLEN * 2

//#define USART3_BUFFLEN 28
//#define USART3_MAX_LEN USART3_BUFFLEN * 2

//#define USART6_BUFFLEN 28
//#define USART6_MAX_LEN USART6_BUFFLEN * 2

#define UART7_BUFFLEN 4
#define UART7_MAX_LEN UART7_BUFFLEN*2

//#define UART8_BUFFLEN 28
//#define UART8_MAX_LEN UART8_BUFFLEN * 2

/******** Struct ********/
typedef struct
{
    int16_t ch[10];
} rc_rx_t;

/******** Extern ********/
extern uint8_t usart1_buff[USART1_BUFFLEN];
//extern uint8_t usart2_buff[USART2_BUFFLEN];
//extern uint8_t usart3_buff[USART3_BUFFLEN];
//extern uint8_t usart6_buff[USART6_BUFFLEN];
extern uint8_t uart7_buff[UART7_BUFFLEN];
//extern uint8_t uart8_buff[UART8_BUFFLEN];
extern rc_rx_t rc_rx;
/******** Function *********/
void Usart_IdleIRQ_Init(UART_HandleTypeDef *huart);
void Usart_IdleIRQ_Handler(UART_HandleTypeDef *huart);
void Usart_IdleIRQ_Callback(UART_HandleTypeDef *huart);

#endif //ZXX_RM_A_TEMPLATE_BSP_USART_H
