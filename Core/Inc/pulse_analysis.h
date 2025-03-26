/*
 * pulse_analysis.h
 *
 *  Created on: Mar 26, 2025
 *      Author: andrey.hiemer
 */

#ifndef INC_PULSE_ANALYSIS_H_
#define INC_PULSE_ANALYSIS_H_

#include <stdint.h>

#define DUTY_CYCLE_SIZE (PULSE_BUFFER_SIZE / 2)

void analyze_pulses(void);
const float *get_duty_cycles(void);

#endif /* INC_PULSE_ANALYSIS_H_ */
