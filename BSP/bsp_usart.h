#ifndef BSP_USART_H
#define BSP_USART_H

#include "stm32f4xx.h"
#include "usart.h"
#include "dma.h"

#define UART_RX_DMA_SIZE (1024)


#define USART1_BUFLEN 25
#define USART1_MAX_LEN USART1_BUFLEN * 2

//#define USART2_BUFLEN 28
//#define USART2_MAX_LEN USART2_BUFLEN * 2

//#define USART3_BUFLEN 28
//#define USART3_MAX_LEN USART3_BUFLEN * 2

//#define USART6_BUFLEN 28
//#define USART6_MAX_LEN USART6_BUFLEN * 2

#define UART7_BUFLEN 4
#define UART7_MAX_LEN UART7_BUFLEN * 2

//#define UART8_BUFLEN 28
//#define UART8_MAX_LEN UART8_BUFLEN * 2
void Uart_RX_Init(UART_HandleTypeDef *);

////ACTION定位模块数据联合体
//typedef union _imu_data
//{
//    uint8_t data[24];
//    float ActVal[6];
//} imudata_t;

//typedef struct
//{
//	int16_t ch[10];
//}sbus_rx_t;
//extern sbus_rx_t sbus_rx;

//extern imudata_t imudata;

void uart_receive_handler(UART_HandleTypeDef *huart);
void uart_receive_init(UART_HandleTypeDef *huart);

#define ABS(x) ((x > 0) ? (x) : (-x))

#endif
