#ifndef SPI_H
#define SPI_H

#include <stdint.h>

typedef enum {
    SPI_SLAVE_DISPLAY,
    SPI_SLAVE_IO,
    SPI_SLAVE_CAN
} SPI_Slave;

void spi_init();
uint8_t spi_transfer(uint8_t data);
void spi_write(uint8_t data);
uint8_t spi_read(void);
void spi_deselect_all(void);
void spi_select_slave(SPI_Slave slave);
void spi_write_nbytes(const uint8_t *data, int n);
void spi_read_nbytes(uint8_t *data, int n);

#endif