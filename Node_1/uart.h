void uart_init();

int uart_transmit(char data, FILE *stream);

int uart_recieve(FILE *stream);