/*
 * uart.h
 *
 *  Created on: May 5, 2022
 *      Author: Burak Duysak
 */

#ifndef UART_H_
#define UART_H_




#include <stdbool.h>
#include <stdint.h>

bool uart_send_data(char tx_buffer[],uint16_t buffer_len, uint16_t tx_timeout, bool aCRLF);
bool uart_send_char(char tx_char, uint16_t tx_timeout);
char* uart_receive_data(uint16_t rx_timeout);
char uart_receive_char(uint16_t rx_timeout);
void uart_init(void);


#endif /* UART_H_ */
