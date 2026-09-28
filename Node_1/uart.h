#ifndef UART_H
#define UART_H

#include <stdio.h>

void uart_init(void);

int uart_transmit(char data, FILE *stream);

int uart_recieve(FILE *stream);

#endif