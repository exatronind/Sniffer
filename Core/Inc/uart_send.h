/*
 * uart_send.h
 *
 *  Created on: Mar 28, 2025
 *      Author: andrey.hiemer
 */

#ifndef INC_UART_SEND_H_
#define INC_UART_SEND_H_


#include "main.h"

void send_uart_data(void);
void setUart(UART_HandleTypeDef *huartChosed);

#endif /* INC_UART_SEND_H_ */
