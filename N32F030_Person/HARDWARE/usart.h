#ifndef _USART_H_
#define _USART_H_
#include "n32g030.h"

#define _USART1_COM_

#ifdef _USART1_COM_
#define USARTx            USART1
#define USARTx_GPIO       GPIOA
#define USARTx_CLK        RCC_APB2_PERIPH_USART1
#define USARTx_GPIO_CLK   RCC_APB2_PERIPH_GPIOA
#define USARTx_RxPin      GPIO_PIN_10
#define USARTx_TxPin      GPIO_PIN_9
#define USARTx_Rx_GPIO_AF GPIO_AF4_USART1
#define USARTx_Tx_GPIO_AF GPIO_AF4_USART1

#define GPIO_APBxClkCmd   RCC_EnableAPB2PeriphClk
#define USART_APBxClkCmd  RCC_EnableAPB2PeriphClk
#endif




#endif



