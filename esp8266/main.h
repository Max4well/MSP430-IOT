/*
 * main.h
 *
 *  Created on: May 5, 2022
 *      Author: Burak Duysak
 */

#ifndef MAIN_H_
#define MAIN_H_


#define TXD BIT0 // TXD on P1.2 (RX of ESP)
#define RXD BIT1 // RXD on P1.1 (TX of ESP)

#define LED0 BIT2   // P1.2 LED
#define LED1 BIT6   // P2.6 LED

#define WIFI_NETWORK_SSID "burak"
#define WIFI_NETWORK_PASS "senveben"

#define TCP_SERVER "iot.ee.hacettepe.edu.tr"
#define TCP_PORT "1883"


#endif /* MAIN_H_ */
