/**
 * @file    spie_norflash.h
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Declares SPI NOR flash driver API for PM.
 * Defines operations for reading, programming and erasing flash memory.
 * Exposes device constants, commands and helper data structures.
 * Used by FPGA configuration and firmware update command handlers.
 */

#ifndef SPIE_NORFLASH_H
#define SPIE_NORFLASH_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

/* Bit masks confirmed from disassembly */
#define SPIE_CS_bm     PIN4_bm  /* PE4 */
#define SPIE_MOSI_bm   PIN5_bm  /* PE5 */
#define SPIE_MISO_bm   PIN6_bm  /* PE6 */
#define SPIE_SCK_bm    PIN7_bm  /* PE7 */

/* SPI port base */
#ifndef SPIE_PORT
#  define SPIE_PORT PORTE
#endif

#ifdef __cplusplus
extern "C" {
#endif

void spie_port_init(void);
void spie_port_shutdown(void);
void spie_chip_select(bool active);

void spie_write_enable(void);
void spie_send_command(uint8_t cmd, uint16_t addr);
void spie_read_jedec_id(void);

#ifdef __cplusplus
}
#endif

#endif /* SPIE_NORFLASH_H */

