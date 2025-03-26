/*
 * pulse_capture.c
 *
 *  Created on: Mar 26, 2025
 *      Author: andrey.hiemer
 */

#include "pulse_capture.h"

// Prototipo privado
void get_Sample(void);

// As variáveis abaixo são declaradas como 'volatile' porque são modificadas
// dentro de interrupções. Isso evita otimizações indesejadas do compilador,
// garantindo que sempre sejam lidas da memória corretamente.
//
// - pulse_buffer[] e pulse_width[]: Modificados pela interrupção e lidos no loop principal.
// - data_ready: Flag usada para indicar que os dados estão prontos.
//
// Sem 'volatile', o compilador poderia armazenar valores em registradores,
// impedindo a detecção correta das mudanças feitas pelas interrupções.

volatile uint32_t pulse_buffer[PULSE_BUFFER_SIZE];
volatile uint32_t pulse_width[PULSE_BUFFER_SIZE - 1];
volatile uint8_t data_ready = 0;
static uint16_t pulse_index = 0;
static TIM_HandleTypeDef *htim = NULL;

void processPulses()
{
    if (data_ready)
    {

        for (uint8_t i = 0, j = 0; i < PULSE_BUFFER_SIZE - 1; i++, j++)
        {
            pulse_width[j] = (pulse_buffer[i + 1] >= pulse_buffer[i]) ? (pulse_buffer[i + 1] - pulse_buffer[i]) : ((0xFFFF - pulse_buffer[i]) + pulse_buffer[i + 1]);
        }

        data_ready = 0; // Reseta a flag para a próxima captura
    }
}

void get_Sample(void)
{
    if (!data_ready)
    {

        if (pulse_index < PULSE_BUFFER_SIZE)
        {
            pulse_buffer[pulse_index++] = __HAL_TIM_GET_COUNTER(htim);
        }

        else
        {
            data_ready = 1;  // Indica que os dados estão prontos para processamento
            pulse_index = 0; // Reinicia o índice para a próxima captura
        }
    }
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == PINO_INTERRUPT)
    {
        get_Sample();
    }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == PINO_INTERRUPT)
    {

        get_Sample();
    }
}

const volatile uint32_t *get_width_pulses(void)
{
    return pulse_width;
}

void setTimerPulses(TIM_HandleTypeDef *htimChosed)
{
    htim = htimChosed;
}

// void resetTimer(void)
// {
//     __HAL_TIM_SET_COUNTER(htim, 0);
// }