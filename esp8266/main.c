#include <msp430.h> 
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "uart.h"
#include "delay.h"

#include "main.h"
/**
 *  Created on: May 5, 2022
 *      Author: Burak Duysak
 */
uint16_t dummy = 0;

char rx_buffer[500],tx_buffer[150];
uint16_t ProtocolNameLength;
uint16_t ClientIDLength;


const uint8_t connect = 0x10;
const uint8_t publishCon = 0x30;
const uint8_t subscribeCon = 0x82;
char *protocolName = "MQTT";
const uint8_t level = 0x04;
const uint8_t flag = 0x02;   // 02--> sifresiz
const uint16_t keepAlive =60;
const uint16_t packetID = 0x01;
const uint8_t Qos = 0x00;
char *clientID = "g40";


volatile char uart_recieve;



void wifi_connect(void){
    uart_send_data("AT+CWMODE=1\r\n",strlen("AT+CWMODE=1\r\n"),1000,true);
    delay_ms(10);
    uart_send_data("AT+CWQAP\r\n",strlen("AT+CWQAP\r\n"),1000,true);
    delay_ms(10);
    uart_send_data("AT+CWJAP=\"burak\",\"senveben\"\r\n",strlen("AT+CWJAP=\"burak\",\"senveben\"\r\n"),1000, true);
    delay_ms(2000);
//This should reply something like "wifi connected"
}
void Connect_Broker(char *Ip ,char *Port)
{
    uint16_t Remainlength;
    uart_send_data("AT+CIPSTART=\"TCP\",\"iot.ee.hacettepe.edu.tr\",1883\r\n",strlen("AT+CIPSTART=\"TCP\",\"iot.ee.hacettepe.edu.tr\",1883\r\n"),1000, true);
    delay_ms(20);
    //connect packet
    ProtocolNameLength = strlen(protocolName);
    ClientIDLength     = strlen(clientID);

    Remainlength = 2 + ProtocolNameLength + 6 + ClientIDLength;
    uint16_t length = sprintf(tx_buffer,"%c%c%c%c%s%c%c%c%c%c%c%s",(char)connect,(char)Remainlength,(char)(ProtocolNameLength << 8),(char)ProtocolNameLength,protocolName,(char)level,(char)flag,(char)(keepAlive << 8),(char)keepAlive,(char)(ClientIDLength << 8),(char)ClientIDLength,clientID);
    sprintf(tx_buffer,"AT+CIPSEND=%d\r\n",length);
    uart_send_data(tx_buffer,sprintf(tx_buffer,"AT+CIPSEND=%d\r\n",length),1000,true);
    delay_ms(20);
    sprintf(tx_buffer,"%c%c%c%c%s%c%c%c%c%c%c%s",(char)connect,(char)Remainlength,(char)(ProtocolNameLength << 8),(char)ProtocolNameLength,protocolName,(char)level,(char)flag,(char)(keepAlive << 8),(char)keepAlive,(char)(ClientIDLength << 8),(char)ClientIDLength,clientID);
    uart_send_data(tx_buffer,sprintf(tx_buffer,"%c%c%c%c%s%c%c%c%c%c%c%s",(char)connect,(char)Remainlength,(char)(ProtocolNameLength << 8),(char)ProtocolNameLength,protocolName,(char)level,(char)flag,(char)(keepAlive << 8),(char)keepAlive,(char)(ClientIDLength << 8),(char)ClientIDLength,clientID),35000,true);
    delay_ms(10);
}

void publish(char *topic, char *message)
{

    uint16_t topiclength = strlen(topic);
    uint16_t remainlength = 2+topiclength+strlen(message);
    int length = sprintf(tx_buffer,"%c%c%c%c%s%s",(char)publishCon,(char)remainlength,(char)(topiclength << 8),(char)topiclength,topic,message);
    sprintf(tx_buffer,"AT+CIPSEND=%d\r\n",length);
    uart_send_data(tx_buffer,sprintf(tx_buffer,"AT+CIPSEND=%d\r\n",length),1000,true);
    delay_ms(10);
    sprintf(tx_buffer,"%c%c%c%c%s%s",(char)publishCon,(char)remainlength,(char)(topiclength << 8),(char)topiclength,topic,message);
    uart_send_data(tx_buffer, sprintf(tx_buffer,"%c%c%c%c%s%s",(char)publishCon,(char)remainlength,(char)(topiclength << 8),(char)topiclength,topic,message),1000,true);

    sprintf(tx_buffer,"%c%c%c%c%s%s",(char)publishCon,(char)remainlength,(char)(topiclength << 8),(char)topiclength,topic,message);
    uart_send_data(tx_buffer, sprintf(tx_buffer,"%c%c%c%c%s%s",(char)publishCon,(char)remainlength,(char)(topiclength << 8),(char)topiclength,topic,message),1000,true);

    delay_ms(10);
}

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5; // TURN ON I/O

	P2DIR |=  BIT7; // ESP8266 RESET PIN
	P2OUT &= ~BIT7; // INITILIAZED IT 0
	P4DIR |=  BIT0; // LED
	P4OUT &= ~BIT0;

	    uart_init();
	    delay_init(1);

	    wifi_connect();


    UCA0IE |= UCRXIE;
    __enable_interrupt();



while(1){
    Connect_Broker("iot.ee.hacettepe.edu.tr","1883");
    publish("g40temp","23");
}
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_RX_ISR(void){
    dummy = UCA0RXBUF;
    __bic_SR_register_on_exit(LPM3_bits);                    //Out lowpower on exit


}
