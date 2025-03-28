/*
 * uart_send.c
 *
 *  Created on: Mar 28, 2025
 *      Author: andrey.hiemer
 */


#include "uart_send.h"

// Declaração de variáveis externas
UART_HandleTypeDef *huart;

static int16_t last_error_value = 0;
static uint8_t new_error_flag = 0;

void send_uart_data(void)
{
    uint8_t message[6]; // 2 bytes de payload inicial + 2 bytes erro + 2 bytes PIR
    int16_t error_value, pir_value;

    error_value = getErroPIR();
    pir_value = getPirValue();

    if(error_value > last_error_value){
    	new_error_flag = 1;
    	last_error_value = error_value;
    }

    // Definição do payload inicial
    message[0] = 0xAA; // Primeiro byte do cabeçalho
    message[1] = 0x55; // Segundo byte do cabeçalho

    // Copia os valores de erro e PIR para o buffer (little-endian)
    message[2] = (uint8_t)(error_value & 0xFF);
    message[3] = (uint8_t)((error_value >> 8) & 0xFF);

    message[4] = (uint8_t)(pir_value & 0xFF);
    message[5] = (uint8_t)((pir_value >> 8) & 0xFF);

    if(new_error_flag){
    	// Copia os valores de erro e PIR para o buffer (little-endian)
    	message[4] = (0x00);
    	message[5] = (0x00);
    	new_error_flag = 0;
    }
    // Envio pela UART
    HAL_UART_Transmit_DMA(huart, message, 6);
}

void setUart(UART_HandleTypeDef *huartChosed){
	huart = huartChosed;
}
