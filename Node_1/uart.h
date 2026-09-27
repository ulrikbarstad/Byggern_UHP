#ifndef UART_H
#define UART_H

void uart_init();

int uart_transmit(char data, FILE *stream);

int uart_recieve(FILE *stream);

#endif