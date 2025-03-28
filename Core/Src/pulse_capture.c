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

static uint32_t pulse_buffer[PULSE_BUFFER_SIZE];
static uint32_t pulse_width[40];
uint8_t data_ready = 0;
static uint16_t pulse_index = 0;
static TIM_HandleTypeDef *htim = NULL;
static uint32_t idleTimer = 0;
static uint8_t buffer_cleared = 0; // Flag para indicar se o buffer já foi limpo

void find_start_pulse_and_realign(void)
{
    if (data_ready)
        return;

    int8_t start_index = -1;

    // Passo 1: Encontrar o índice do primeiro pulso de 100µs
    for (uint8_t i = 0; i < PULSE_BUFFER_SIZE - 1; i++)
    {
        uint32_t width = (pulse_buffer[i + 1] >= pulse_buffer[i])
                             ? (pulse_buffer[i + 1] - pulse_buffer[i])
                             : ((0xFFFF - pulse_buffer[i]) + pulse_buffer[i + 1]);

        if (width >= (START_PULSE_WIDTH - PULSE_TOLERANCE) &&
            width <= (START_PULSE_WIDTH + PULSE_TOLERANCE))
        {
            start_index = i;
            break;
        }
    }

    // Passo 2: Se encontrou, recalcular os pulse_widths a partir deste ponto
    if (start_index != (-1) && (start_index + 40) < PULSE_BUFFER_SIZE)
    {
        for (uint8_t j = 0, i = start_index; j < 40; i++, j++)
        {
            pulse_width[j] = (pulse_buffer[i + 1] >= pulse_buffer[i])
                                 ? (pulse_buffer[i + 1] - pulse_buffer[i])
                                 : ((0xFFFF - pulse_buffer[i]) + pulse_buffer[i + 1]);
        }

        data_ready = 1; // Marca os dados como prontos
        pulse_index = 0;
    }
    else
    {
        // Se não encontrou um pulso válido, reseta buffer para evitar dados errados
        pulse_index = 0;
        idleTimer = 0;
        data_ready = 0;
    }
}

void processPulses()
{
    //    if (data_ready)
    //        return;

    if ((++idleTimer) > 1) // Garante que pelo menos passou 2 ms da ultima leitura de borda e entao habilita para a leitura
    {
        if (pulse_index >= 40) // Certifica que temos pelo menos 40 pulsos, comeca no zero
        {
            find_start_pulse_and_realign();
            // RESET_TIMER(htim);
        }

        else if (!buffer_cleared) // Só limpa se ainda não foi limpo
        {
            pulse_index = 0;
            idleTimer = 0;
            buffer_cleared = 1; // Marca que o buffer foi limpo

            for (uint8_t i = 0; i < PULSE_BUFFER_SIZE; i++)
                pulse_buffer[i] = 0;
        }
        // pulse_index = 0; // Reinicia o índice para a próxima captura
        // idleTimer = 0;
    }
}

void get_Sample(void)
{

    if (data_ready)
        return; // Se os dados já estão prontos, ignora novas capturas

    if (pulse_index < PULSE_BUFFER_SIZE)
    {
        pulse_buffer[pulse_index++] = TIM3->CNT;
        idleTimer = 0;      // Reset o tempo de inatividade quando um novo pulso chega
        buffer_cleared = 0; // Se chegou um novo pulso, indica que o buffer não está limpo
    }
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    //     if (GPIO_Pin == PINO_INTERRUPT)
    //     {
    //         get_Sample();
    // //        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_SET);
    //         GPIOA->ODR |= GPIO_PIN_9;
    //     }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    //     if (GPIO_Pin == PINO_INTERRUPT)
    //     {

    //         get_Sample();
    // //        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,GPIO_PIN_RESET);
    //         GPIOA->ODR &= ~GPIO_PIN_9;
    //     }
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
