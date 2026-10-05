/* Private register entries; value/channel barriers preserve the original ABI. */
extern void FUN_code_001053(void);
extern void dac_set_value_2(void);
extern void dac_set_value(void);
extern void FUN_code_001068(void);
#include <stdint.h>
#define LOAD_SETTING(value, cursor) asm volatile("ld r20, Y+\n\tld r21, Y+" : "=r" (value), "+y" (cursor) : : "memory")
#define DAC_CALL(name, value, channel) do { \
 asm volatile("" : "+r" (value), "+r" (channel) : : "memory"); \
 name(); \
 asm volatile("" : "=r" (value), "=r" (channel) : : "memory"); \
} while (0)

/* Apply the four stored words for each channel. The exact save frame is
 * required because callers expect all fourteen original registers restored. */
void FUN_code_0004ef(void)
{
    asm volatile("push r31\n\tpush r30\n\tpush r29\n\tpush r28\n\tpush r23\n\tpush r22\n\tpush r21\n\tpush r20\n\tpush r19\n\tpush r18\n\tpush r17\n\tpush r16\n\tpush r1\n\tpush r0" : : : "memory");
    register uint8_t channel asm("r23");
    asm volatile("clr %0" : "=r" (channel) : : "cc");
    register uint8_t *settings asm("r28") = (uint8_t *)0x21cf;
    asm volatile("" : "+y" (settings));
next_channel:;
    register uint16_t word asm("r20");
    register uint8_t selected asm("r22");
    LOAD_SETTING(word, settings);
    selected = channel; asm volatile("" : "+r" (selected), "+r" (channel));
    DAC_CALL(FUN_code_001053, word, selected);
    LOAD_SETTING(word, settings);
    selected = channel; asm volatile("" : "+r" (selected), "+r" (channel));
    DAC_CALL(dac_set_value_2, word, selected);
    LOAD_SETTING(word, settings);
    selected = channel; asm volatile("" : "+r" (selected), "+r" (channel));
    DAC_CALL(dac_set_value, word, selected);
    LOAD_SETTING(word, settings);
    selected = channel; asm volatile("" : "+r" (selected), "+r" (channel));
    DAC_CALL(FUN_code_001068, word, selected);
    asm volatile("inc %0" : "+r" (channel) : : "cc");
    if (channel != 12) goto next_channel;
    asm volatile("pop r0\n\tpop r1\n\tpop r16\n\tpop r17\n\tpop r18\n\tpop r19\n\tpop r20\n\tpop r21\n\tpop r22\n\tpop r23\n\tpop r28\n\tpop r29\n\tpop r30\n\tpop r31\n\tret" : : : "memory");
    __builtin_unreachable();
}
