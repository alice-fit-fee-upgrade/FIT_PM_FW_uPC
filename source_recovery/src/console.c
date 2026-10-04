#include "pm_recovered.h"

/* 0x2634..0x26AB. Deliberately retain 8-bit count wrap and the original
 * two-stage multiply/overflow order, including partial values on error. */
pm_number pm_parse_integer(const pm_stream *s)
{
    pm_number out = {0, 0, true};
    uint8_t count = 0, negative = 0;
    for (;;) {
        uint8_t ch = s->get(s->context);
        out.terminator = ch;
        if (count == 0 && ch == '-') {
            ++negative;
            ++count;
            continue;
        }
        ++count;
        if (ch < '0' || ch >= ':') {
            if (count == 1) return out;
            if (negative != 0) {
                if (count == 2) return out;
                out.value = (uint16_t)(0u - out.value);
                if (out.value < 0x8000u) return out;
            }
            out.error = false;
            return out;
        }
        uint16_t low = (uint16_t)((out.value & 255u) * 10u + (ch - '0'));
        uint16_t high = (uint16_t)((out.value >> 8) * 10u);
        out.value = low;
        if (high > 255u) return out;
        uint16_t sum = (uint16_t)((low >> 8) + high);
        out.value = (uint16_t)((low & 255u) | (sum << 8));
        if (sum > 255u) return out;
    }
}

/* 0x26AC..0x26F7: uppercase only; after four digits consume ONE more char. */
pm_number pm_parse_hex(const pm_stream *s)
{
    pm_number out = {0, 0, true};
    uint8_t count = 0;
    for (;;) {
        uint8_t ch = s->get(s->context), digit;
        out.terminator = ch;
        if (ch >= '0' && ch <= '9') digit = (uint8_t)(ch - '0');
        else if (ch >= 'A' && ch <= 'F') digit = (uint8_t)(ch - 'A' + 10u);
        else break;
        out.value = (uint16_t)((out.value << 4) | digit);
        if (++count == 4) {
            out.terminator = s->get(s->context);
            break;
        }
    }
    out.error = count == 0;
    return out;
}
void pm_send_hex_digit(const pm_stream *s, uint8_t value)
{
    s->put(s->context, pm_hex_digit(value));
}
void pm_send_hex16(const pm_stream *s, uint16_t value)
{
    pm_send_hex_digit(s, (uint8_t)(value >> 12));
    pm_send_hex_digit(s, (uint8_t)(value >> 8));
    pm_send_hex_digit(s, (uint8_t)(value >> 4));
    pm_send_hex_digit(s, (uint8_t)value);
}
void pm_send_crlf(const pm_stream *s)
{
    s->put(s->context, 13);
    s->put(s->context, 10);
}
void pm_send_flash_string(const pm_bus *b, const pm_stream *s, uint16_t address)
{
    uint8_t ch;
    while ((ch = b->flash8(b->context, address++)) != 0)
        s->put(s->context, ch);
}
/* Five original entries share 0x2754..0x281D. Width is 5 + signed-mode + nonzero-decimal-places;
 * signed mode reserves a sign cell, even for positive numbers. No printf. */
void pm_send_decimal(const pm_stream *s, uint16_t value, bool signed_value, uint8_t places)
{
    uint8_t text[9], count = 0, digits = 0;
    bool negative = signed_value && (value & 0x8000u) != 0;
    if (negative) value = (uint16_t)(0u - value);
    do {
        text[count++] = (uint8_t)('0' + value % 10u);
        value /= 10u;
        if (++digits == places) text[count++] = '.';
    } while (value != 0);
    if (digits <= places) {
        text[count++] = '0';
        if (digits < places) {
            while (++digits != places) text[count++] = '0';
            text[count++] = '.';
            text[count++] = '0';
        }
    }
    if (signed_value) text[count++] = negative ? '-' : ' ';
    uint8_t width = (uint8_t)(5u + signed_value + (places != 0));
    while (count < width) text[count++] = ' ';
    while (count != 0) s->put(s->context, text[--count]);
}
