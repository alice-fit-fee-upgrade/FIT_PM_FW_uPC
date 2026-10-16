/* Host-only adapter model. No claim to validate AVR ABI here. */
#include "hw_diag.h"
uint32_t pm_pll_read_status_c(void);
uint32_t pm_pll_read_legacy_native(void) { return pm_pll_read_status_c(); }
