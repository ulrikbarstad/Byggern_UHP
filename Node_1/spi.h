#include <stdint.h>

typedef enum {
    SPI_SLAVE_DISPLAY,
    SPI_SLAVE_IO,
    SPI_SLAVE_CAN
} SPI_Slave;

void spi_init();
uint8_t spi_transfer(uint8_t data);
void spi_write(uint8_t data);
uint8_t spi_read();
void spi_deselct_all();
void spi_select_slave(SPI_Slave slave);
void spi_write_nbits(uint8_t *data, int n);
uint8_t spi_read_nbits(int n);