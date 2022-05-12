/*
 * delay.h
 *
 *  Created on: May 5, 2022
 *      Author: Burak Duysak
 */

#ifndef DELAY_H_
#define DELAY_H_
#include <stdint.h>

void delay_us(uint16_t us);
void delay_ms(uint16_t ms);
void delay_init(uint16_t fclk);




#endif /* DELAY_H_ */
