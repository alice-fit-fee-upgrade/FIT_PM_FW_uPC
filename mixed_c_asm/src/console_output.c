#include <stdint.h>
/* Original 0x2720 masks to a nibble. Preserve its final CPI/SUBI flags. */
uint16_t pm_hex_digit_c(uint8_t value,uint8_t incoming_sreg)
{
    uint8_t nibble=value&15u;
    uint8_t ascii=(uint8_t)(nibble+(nibble<10u?'0':('A'-10)));
    uint8_t flags=(incoming_sreg&0xc0u)|(nibble<10u?0x35u:1u);
    return (uint16_t)ascii|((uint16_t)flags<<8);
}
extern uint8_t pm_ascii_emit_flags(uint8_t ascii,uint8_t before_sreg);
static uint8_t emit_digit(uint8_t value,uint8_t sreg)
{
    uint16_t packet=pm_hex_digit_c(value,sreg);
    return pm_ascii_emit_flags((uint8_t)packet,(uint8_t)(packet>>8));
}
/* Original misleading name cli_send_32bit_hex: four digits of R17:R16. */
uint8_t pm_hex16_c(uint16_t value,uint8_t incoming_sreg)
{
    uint8_t high=(uint8_t)(value>>8),low=(uint8_t)value;
    uint8_t flags=emit_digit(high>>4,incoming_sreg);
    flags=emit_digit(high,flags);
    flags=emit_digit(low>>4,flags);
    return emit_digit(low,flags);
}

/* The original CRLF entry delegates to the unchanged FLASH-string sender. */
extern uint32_t pm_flash_message_call(uint16_t address);
uint32_t pm_crlf_c(void)
{
    return pm_flash_message_call(0x2984u);
}
