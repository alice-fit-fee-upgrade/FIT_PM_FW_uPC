#include "ths788.h"


/*! CPU speed 32MHz, BAUDRATE 400kHz and Baudrate Register Settings */
#define CPU_SPEED           32000000
#define BAUDRATE            400000
#define TWI_BAUDSETTING     TWI_BAUD(CPU_SPEED, BAUDRATE)


void ths788_init()
{
    return;
}

void ths788_data_write(uint8_t addr, uint16_t *p_data)
{
    return;
}
