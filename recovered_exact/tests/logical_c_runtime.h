/* Functional register-state C model. No cycle or asynchronous IRQ model. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
typedef struct {
    uint8_t r[32], sreg, memory[65536], stack[256];
    uint16_t depth, call_depth;
    uint32_t calls[256], pc;
    uint32_t trace[32];
    unsigned trace_count;
} PMLogical;
enum { CARRY, ZERO, NEGATIVE, OVERFLOW, SIGNED, HALF, TRANSFER, INTERRUPT };
static void pm_flag(PMLogical *s, unsigned bit, bool value) {
    s->sreg = (s->sreg & ~(1u << bit)) | ((unsigned)value << bit);
}
static bool pm_getflag(PMLogical *s, unsigned bit) { return (s->sreg >> bit) & 1u; }
static void pm_nzv(PMLogical *s, uint8_t result, bool overflow) {
    pm_flag(s, ZERO, result == 0); pm_flag(s, NEGATIVE, result & 128);
    pm_flag(s, OVERFLOW, overflow); pm_flag(s, SIGNED, !!(result & 128) ^ overflow);
}
static uint8_t pm_add(PMLogical *s, uint8_t a, uint8_t b, unsigned carry) {
    uint8_t result = a + b + carry;
    pm_flag(s, CARRY, ((a & b) | (b & ~result) | (~result & a)) & 128);
    pm_flag(s, HALF, ((a & b) | (b & ~result) | (~result & a)) & 8);
    pm_nzv(s, result, ((a & b & ~result) | (~a & ~b & result)) & 128);
    return result;
}
static uint8_t pm_sub(PMLogical *s, uint8_t a, uint8_t b, unsigned carry, bool chained_zero) {
    bool previous_zero = pm_getflag(s, ZERO);
    uint8_t result = a - b - carry;
    pm_flag(s, CARRY, ((~a & b) | (b & result) | (result & ~a)) & 128);
    pm_flag(s, HALF, ((~a & b) | (b & result) | (result & ~a)) & 8);
    pm_nzv(s, result, ((a & ~b & ~result) | (~a & b & result)) & 128);
    if (chained_zero) pm_flag(s, ZERO, previous_zero && result == 0);
    return result;
}
static uint16_t pm_pointer(PMLogical *s, unsigned low) { return s->r[low] | ((uint16_t)s->r[low+1] << 8); }
static void pm_setpointer(PMLogical *s, unsigned low, uint16_t value) { s->r[low] = value; s->r[low+1] = value >> 8; }
static void pm_event(PMLogical *s, unsigned kind, unsigned address, unsigned value) {
    s->trace[s->trace_count++] = (kind << 24) | (address << 8) | value;
}
static uint8_t pm_read(PMLogical *s, uint16_t address) {
    uint8_t value = s->memory[address]; pm_event(s, 1, address, value); return value;
}
static void pm_write(PMLogical *s, uint16_t address, uint8_t value) {
    pm_event(s, 2, address, value); s->memory[address] = value;
}
static uint8_t pm_io_read(PMLogical *s, uint16_t address) {
    return address == 0x3f ? s->sreg : pm_read(s, address);
}
static void pm_io_write(PMLogical *s, uint16_t address, uint8_t value) {
    if (address == 0x3f) s->sreg = value; else pm_write(s, address, value);
}
static void pm_irq(PMLogical *s, bool enabled) {
    pm_flag(s, INTERRUPT, enabled); pm_event(s, 3, 0, enabled);
}
