#ifndef PM_DAC_CANDIDATE_H
#define PM_DAC_CANDIDATE_H
#include <stdint.h>
/* SPI exclusively owned, IRQs masked. Old bridge supplies original IRQ policy.
 * No argument validation: control has the original private packed encoding. */
void pm_dev_dac_send_locked(uint8_t control,uint16_t value);
/* Reapply48 current RAM settings; does not persist or replace them.
 * Each legacy send enables IRQs on completion, preserving the old policy. */
void pm_dev_dac_apply_stored(void);
#endif
