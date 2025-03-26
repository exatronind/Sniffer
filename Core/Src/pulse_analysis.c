/*
 * pulse_analysis.c
 *
 *  Created on: Mar 26, 2025
 *      Author: andrey.hiemer
 */

#include "pulse_analysis.h"
#include "pulse_capture.h"

static float duty_cycles[DUTY_CYCLE_SIZE];

void analyze_pulses(void)
{
    const volatile uint32_t *pulse_widths = get_width_pulses();
    uint32_t highTime, lowTime, totalTime;

    for (uint8_t i = 0, j = 0; i < PULSE_BUFFER_SIZE - 1; i += 2, j++)
    {
        highTime = pulse_widths[i];
        lowTime = pulse_widths[i + 1];
        totalTime = highTime + lowTime;
        
        if (totalTime > 0)
        {
            duty_cycles[j] = ((float)highTime / totalTime) * 100.0f;
        }
        else
        {
            duty_cycles[j] = 0.0f; // Evita divisão por zero
        }
    }
}

const float *get_duty_cycles(void)
{
    return duty_cycles;
}

