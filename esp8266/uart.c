/*
 * uart.c
 *
 *  Created on: May 5, 2022
 *      Author: Burak Duysak
 */
#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "delay.h"
#include "uart.h"


bool _wait_for_tx_available(uint16_t timeout_val);
bool _wait_for_rx_available(uint16_t timeout_val);

bool uart_send_data(char tx_buffer[],uint16_t buffer_len, uint16_t tx_timeout, bool aCRLF)
{

    uint16_t buffer_idx = 0;

    for(buffer_idx=0; buffer_idx < buffer_len; buffer_idx++)
    {
        if(!uart_send_char(tx_buffer[buffer_idx], tx_timeout))
            return false;
    }

    if(aCRLF == true) //append CRLF
    {
        if(!uart_send_char(0x0D, tx_timeout))
            return false;
        if(!uart_send_char(0x0A, tx_timeout))
            return false;
    }

    return true;
}

char* uart_receive_data(uint16_t rx_timeout)
{
    static char rec_buff[50];
    char rec_char;
    uint16_t buff_index = 0;
    do
    {
        rec_char = uart_receive_char(rx_timeout);
        rec_buff[buff_index] = rec_char;
        buff_index++;
    }while(rec_char != 0x0D);

    return &rec_buff[0];
}


/* Setup UART */
void uart_init(void)
{
// We did Initializing or reconfiguring the eUSCI_A module
// according to the user guide

  // Configure UART as 115200 baud
  UCA0CTLW0 |= UCSWRST;       // Put eUSCI in reset Modify the UCA0CTLW0 reg only when UCSWRST = 1
  // Use SMCLK - 1MHz clock
  UCA0CTLW0 |= UCSSEL__SMCLK;
   UCA0BRW    =  8;     // 1000000/115200 = 8.68  Set prescaler to 8
   UCA0MCTLW  =  0xD600;// 1000000/115200 - INT(1000000/115200)=0.68
    //UCBRSx value = 0xD6 (See UG) + low freq
   //framing values are default

  // Set TXD, and RXD
  P1SEL0 |= BIT0 | BIT1; // Primary function selected

  UCA0IE |= 0x00;
  UCA0CTLW0 &= ~UCSWRST; // Initialize eUSCI
  UCA0IE &= ~UCRXIE;     // Disable USCI_A0 RX interrupt
}



bool uart_send_char(char tx_char, uint16_t tx_timeout)
{
    if(_wait_for_tx_available(tx_timeout))
    {
        // if tx available initiate transmission

        UCA0TXBUF = tx_char;
        return true;
    }
    else
    {
        // Timeout
        return false;
    }
}

char uart_receive_char(uint16_t rx_timeout)
{
    if(_wait_for_rx_available(rx_timeout))
    {
        // if rx available initiate transmission
        return UCA0RXBUF;
    }
    else
    {
        // Timeout
        return false;
    }
}


bool _wait_for_rx_available(uint16_t timeout_val)
{
    while(!(UCA0IFG & UCRXIFG))
    {
        if (timeout_val == 1)
            return false; // Timeout
        else if(timeout_val == 0) //no timeout
            timeout_val = 0;
        else
            timeout_val--;
    }

    return true; // TX available
}

bool _wait_for_tx_available(uint16_t timeout_val)
{
    while(!(UCA0IFG & UCTXIFG))
    {
        if (timeout_val > 0)
            timeout_val--;
        else
            return false; // Timeout
    }

    return true; // TX available
}



