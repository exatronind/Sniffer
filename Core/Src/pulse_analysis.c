/*
 * pulse_analysis.c
 *
 *  Created on: Mar 26, 2025
 *      Author: andrey.hiemer
 */

#include "pulse_analysis.h"
#include "pulse_capture.h"

static uint16_t duty_cycles[DUTY_CYCLE_SIZE];
static uint8_t bit_sequence[SIZE_VECTOR_BITS]; // BIT_SEQUENCE_SIZE deve ser 19
uint8_t decode_ready = 0;
uint8_t payload_ready = 0;
uint8_t message_done = 0;
int16_t erro_payload = 0;
int16_t valor_PIR = 0;

uint8_t verify_payload(void);

void analyze_pulses(void)
{
    const volatile uint32_t *pulse_widths = get_width_pulses();
    uint32_t highTime, lowTime, totalTime;

    for (uint8_t i = 0, j = 0; i < PULSE_BUFFER_SIZE - 1; i += 2, j++)
    {
        highTime = pulse_widths[i];
        lowTime = pulse_widths[i + 1];
        totalTime = highTime + lowTime;

        /* if (totalTime > 0)
         {
             // Se não for a última posição, calcula normalmente
             if (j != BIT_FINAL)
             {
                 duty_cycles[j] = (highTime * 100) / totalTime;
             }
             // Última posição tem tratamento especial
             else
             {
                 duty_cycles[j] = (highTime < MAX_TIME_LOW) ? 50 : 90;
             }
         }
         else
         {
             duty_cycles[j] = 0; // Evita divisão por zero
         }*/

        // Outra Estrategia

        if (j == BIT_FINAL)
        {
            duty_cycles[j] = (highTime <= LAST_BIT_LENGTH) ? 50 : 90;
        }
        else
        {

            duty_cycles[j] = (highTime <= MAX_TIME_LOW) ? 50 : 90;
        }
    }

    decode_ready = 1;
}

const uint16_t *get_duty_cycles(void)
{
    return duty_cycles;
}

void convert_duty_to_bits(void)
{

    for (uint8_t i = 1, j = 0; i <= 19; i++, j++)
    {
        bit_sequence[j] = (duty_cycles[i] >= DUTY_CYCLE_VALUE_HIGH) ? 1 : 0;
    }
    payload_ready = 1;
}

const uint8_t *get_bit_sequence(void)
{
    return bit_sequence;
}

uint8_t verify_payload(void)
{

    return (bit_sequence[0] == EXPECTED_BIT_0) && (bit_sequence[1] == EXPECTED_BIT_1) && (bit_sequence[18] == EXPECTED_BIT_18);
}

void analyze_payload(void)
{
    // Verifica os bits de controle (posição 0, 1 e 19)
    if (!verify_payload())
    {
        erro_payload++;
        return; // Se o payload estiver incorreto, aborta
    }

    // Extrai os bits de 2 a 18 e armazena em um uint16_t
    valor_PIR = 0;
    for (uint8_t i = 2; i <= 17; i++)
    {
        valor_PIR <<= 1;
        valor_PIR |= bit_sequence[i];
    }

    message_done = 1;
}

int16_t getErroPIR(void){
    return erro_payload;
}
int16_t getPirValue(void){
    return valor_PIR;
}
