#include "pm_recovered.h"
#include "pm_addresses.h"
#define READ(a) b->read8(b->context, (a))
#define WRITE(a,v) b->write8(b->context, (a), (v))
#define IRQ(v) b->irq(b->context, (v))
/* 0x2836/0x283C..0x28AB: 64-byte receive ring, unusual RTS write retained. */
uint8_t pm_console_next(const pm_bus *b, bool raw)
{
    uint8_t head, tail;
    do {
        IRQ(false);
        head = READ(PM_RX_HEAD);
        tail = READ(PM_RX_TAIL);
        IRQ(true);
    } while (head == tail);
    head = (uint8_t)((head + 1u) & 63u);
    uint8_t port = READ(PM_PORTF_OUT);
    if (port & 1u) {
        if (tail != ((head + 20u) & 63u)) port = 1;
        WRITE(PM_PORTF_OUTCLR, port);
    }
    uint8_t ch = READ((uint16_t)(PM_RX_BASE + head));
    WRITE(PM_RX_HEAD, head);
    if (ch == 13) WRITE(PM_RX_LINES_AND_RTS, (uint8_t)(READ(PM_RX_LINES_AND_RTS) - 1u));
    /* Original BRLT comparison is SIGNED: bytes >=0x80 are not folded. */
    if (!raw && ch > 0x60 && ch < 0x80) ch &= 0x5f;
    return ch;
}
/* 0x28AC..0x2915: TX indices are 8-bit, NOT a 64-byte ring. */
void pm_console_send(const pm_bus *b, uint8_t byte)
{
    uint8_t head, tail;
    for (;;) {
        IRQ(false);
        head = READ(PM_TX_HEAD);
        tail = READ(PM_TX_TAIL);
        if (head == tail) {
            ++tail;
            uint8_t ready = READ(PM_TX_READY);
            if (ready != 0 && (READ(PM_USARTF0_STATUS) & 0x20u)) {
                WRITE(PM_USARTF0_DATA, byte);
                WRITE(PM_USARTF0_CTRLA, (uint8_t)(READ(PM_USARTF0_CTRLA) | 2u));
                IRQ(true);
                return;
            }
            break;
        }
        ++tail;
        if (head != tail) break;
        IRQ(true);
    }
    WRITE((uint16_t)(PM_TX_BASE + tail), byte);
    WRITE(PM_TX_TAIL, tail);
    IRQ(true);
}
/* C bodies of 0x0E88 / 0x0EE0; ISR prologue/epilogue stay integration work. */
void pm_console_dre(const pm_bus *b)
{
    uint8_t head = READ(PM_TX_HEAD), tail = READ(PM_TX_TAIL), ready = READ(PM_TX_READY);
    if (ready == 0 || head == tail) {
        WRITE(PM_USARTF0_CTRLA, (uint8_t)(READ(PM_USARTF0_CTRLA) & 0xfcu));
    } else {
        ++head;
        uint8_t byte = READ((uint16_t)(PM_TX_BASE + head));
        WRITE(PM_USARTF0_DATA, byte);
        WRITE(PM_TX_HEAD, head);
    }
}
void pm_console_rxc(const pm_bus *b)
{
    uint8_t head = READ(PM_RX_HEAD);
    uint8_t tail = (uint8_t)((READ(PM_RX_TAIL) + 1u) & 63u);
    if (((tail + 20u) & 63u) == head) {
        WRITE(PM_PORTF_OUTSET, 1);
        WRITE(PM_RX_LINES_AND_RTS, (uint8_t)(READ(PM_RX_LINES_AND_RTS) | 128u));
    }
    uint8_t status = READ(PM_USARTF0_STATUS);
    uint8_t byte = READ(PM_USARTF0_DATA);
    if ((status & 16u) != 0 || head == tail) return;
    WRITE(PM_RX_TAIL, tail);
    WRITE((uint16_t)(PM_RX_BASE + tail), byte);
    if (byte == 13) WRITE(PM_RX_LINES_AND_RTS, (uint8_t)(READ(PM_RX_LINES_AND_RTS) + 1u));
}
