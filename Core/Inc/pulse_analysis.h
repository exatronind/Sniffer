/*
 * pulse_analysis.h
 *
 *  Created on: Mar 26, 2025
 *      Author: andrey.hiemer
 */

#ifndef INC_PULSE_ANALYSIS_H_
#define INC_PULSE_ANALYSIS_H_

#include <stdint.h>

#define DUTY_CYCLE_SIZE 20
#define MAX_TIME_LOW 90
#define BIT_FINAL 19
#define DUTY_CYCLE_VALUE_HIGH 68
#define SIZE_VECTOR_BITS 19
#define EXPECTED_BIT_0 1
#define EXPECTED_BIT_1 0
#define EXPECTED_BIT_18 0

extern uint8_t decode_ready;
extern uint8_t payload_ready;


void analyze_pulses(void);
void convert_duty_to_bits(void);
const uint16_t *get_duty_cycles(void);
void analyze_payload(void);

#endif /* INC_PULSE_ANALYSIS_H_ */
