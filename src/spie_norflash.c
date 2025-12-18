/**
 * @file    spie_norflash.c
 * @brief   ALICE FIT XMEGA firmware – Processing Module (PM)
 *
 * @project ALICE Fast Interaction Trigger (FIT)
 * @module  Processing Module (PM)
 * @target  ATxmega128A3 @ 32 MHz
 *
 * @details
 * Implements SPI NOR flash access used by PM for configuration storage.
 * Provides read/program/erase primitives and basic device operations.
 * Used for FPGA bitstream/firmware images and parameter persistence.
 * Designed for small footprint and deterministic embedded behavior.
 */

#include "spie_norflash.h"
#include <util/delay.h> /* for _delay_us() replacement of _spin_delay_like_ghidra() */

/* Ensure F_CPU is known here (should also be passed via -DF_CPU=... from CMake) */
#ifndef F_CPU
# define F_CPU 32000000UL
#endif

/* 1 us delay using _delay_loop_2:
 * Each iteration of _delay_loop_2 burns 4 CPU cycles.
 * For 1 us: count = F_CPU / 4_000_000.
 * For N us: call it N times (runtime variable OK). */
static inline void spie_delay_us(uint16_t us)
{
    const uint16_t iter_per_us = (uint16_t)(F_CPU / 4000000UL);
    while (us--) {
        _delay_loop_2(iter_per_us);
    }
}

/* --- Initialization ------------------------------------------------------ */
void spie_port_init(void)
{
  /* Set PE4–PE7 as outputs (CS, MOSI, MISO, CLK) */
  SPIE_PORT.DIRSET = 0xF0; /* 1111 0000b */

  /* Default state:
   * CS high (inactive), CLK low, MOSI low.
   * This matches spie_port_enable_and_configure() startup.
   */
  SPIE_PORT.OUTSET = SPIE_CS_bm;   /* CS high = 1 */
  SPIE_PORT.OUTCLR = (SPIE_MOSI_bm | SPIE_SCK_bm); /* drive low */
}

/* Shutdown back to safe GPIO defaults */
void spie_port_shutdown(void)
{
  /* Same as spie_iface_shutdown_and_gpio_preset_then_post():
   *   - CS high, MOSI low, CLK low
   *   - direction unchanged
   */
  SPIE_PORT.OUTSET = SPIE_CS_bm;
  SPIE_PORT.OUTCLR = (SPIE_MOSI_bm | SPIE_SCK_bm);
}

/* --- Chip Select --------------------------------------------------------- */
void spie_chip_select(bool active)
{
  if (active)
    SPIE_PORT.OUTCLR = SPIE_CS_bm;  /* CS = 0 → active */
  else
    SPIE_PORT.OUTSET = SPIE_CS_bm;  /* CS = 1 → inactive */
}

/* --- Flash Command Helpers ----------------------------------------------- */

/* Write Enable (0x06 command) */
void spie_write_enable(void)
{
  spie_chip_select(true);
  /* Send 0x06 on SPIC (abstracted here) */
  SPIC.DATA = 0x06;
  while (!(SPIC.STATUS & SPI_IF_bm));
  (void)SPIC.DATA; /* dummy read */
  spie_chip_select(false);

  /* Original used _spin_delay_like_ghidra(); */
  spie_delay_us(10);
}

/* Send arbitrary command + 16-bit address (hold CS) */
void spie_send_command(uint8_t cmd, uint16_t addr)
{
  spie_chip_select(true);

  SPIC.DATA = cmd;
  while (!(SPIC.STATUS & SPI_IF_bm));
  (void)SPIC.DATA;

  SPIC.DATA = (uint8_t)(addr >> 8);
  while (!(SPIC.STATUS & SPI_IF_bm));
  (void)SPIC.DATA;

  SPIC.DATA = (uint8_t)(addr & 0xFF);
  while (!(SPIC.STATUS & SPI_IF_bm));
  (void)SPIC.DATA;
}

/* Read and discard JEDEC ID (0x9F) */
void spie_read_jedec_id(void)
{
  spie_chip_select(true);

  SPIC.DATA = 0x9F;
  while (!(SPIC.STATUS & SPI_IF_bm));
  (void)SPIC.DATA;

  for (uint8_t i = 0; i < 3; ++i) {
    SPIC.DATA = 0x00;
    while (!(SPIC.STATUS & SPI_IF_bm));
    (void)SPIC.DATA;
  }

  spie_chip_select(false);
}

