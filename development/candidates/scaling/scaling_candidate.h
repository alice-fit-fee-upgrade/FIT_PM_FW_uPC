#ifndef PM_SCALING_CANDIDATE_H
#define PM_SCALING_CANDIDATE_H
#include <stdint.h>
/* Normal GNU ABI. Signed input is supplied as its16-bit two's-complement bits. */
uint16_t pm_dev_scale_signed(uint16_t coefficient, uint16_t input);
/* Preserve the original staged saturation, not a new mathematical clamp. */
uint16_t pm_dev_scale_unsigned(uint16_t coefficient, uint16_t input);
/* Legacy adapter packet: result bits0..15, outgoing SREG bits16..23. */
uint32_t pm_dev_scale_unsigned_abi(uint16_t coefficient, uint16_t input,
                                 uint8_t entry_sreg);
#endif
