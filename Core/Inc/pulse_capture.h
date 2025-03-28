/*
 * pulse_capture.h
 *
 *  Created on: Mar 26, 2025
 *      Author: andrey.hiemer
 */

#ifndef INC_PULSE_CAPTURE_H_
#define INC_PULSE_CAPTURE_H_

#include "stm32c0xx_hal.h"
#include <stdint.h>

#define PULSE_BUFFER_SIZE 50
#define PINO_INTERRUPT GPIO_PIN_14

#define START_PULSE_WIDTH 700  // Pulso de início esperado
#define PULSE_TOLERANCE 100     // Margem de erro permitida para o pulso

// Macros
#define RESET_TIMER(htim) __HAL_TIM_SET_COUNTER(htim, 0)

extern uint32_t pulse_buffer[PULSE_BUFFER_SIZE];
// extern volatile uint32_t pulse_width[PULSE_BUFFER_SIZE-1];
extern uint8_t data_ready;
extern uint16_t pulse_index;
extern uint32_t idleTimer;
extern uint8_t buffer_cleared;

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin);

const volatile uint32_t *get_width_pulses(void);
void processPulses(void);
void setTimerPulses(TIM_HandleTypeDef *htimChosed);

void resetTimer(void);
void get_Sample(void);

#ifdef DEBUG
extern uint32_t sample_count;
#endif

#endif /* INC_PULSE_CAPTURE_H_ */
