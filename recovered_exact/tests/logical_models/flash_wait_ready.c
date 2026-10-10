uint32_t pm_logical_flash_wait_ready(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x173a: { // ldi r24, 0x40
        s->r[24] = 64;
        return 5948;
    }
    case 0x173c: { // ldi r25, 0x06
        s->r[25] = 6;
        return 5950;
    }
    case 0x173e: { // sbiw r24, 0x01
        uint16_t old = pm_pointer(s, 24);
        uint16_t value = old - 1;
        pm_setpointer(s, 24, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, !old_negative && negative);
        pm_flag(s, OVERFLOW, old_negative && !negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 5952;
    }
    case 0x1740: { // brne .-4
        return (pm_getflag(s, 1) == 0) ? 5950 : 5954;
    }
    case 0x1742: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5956;
    }
    case 0x1744: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 5960;
    }
    case 0x1748: { // ldi r16, 0x05
        s->r[16] = 5;
        return 5962;
    }
    case 0x174a: { // sts 0x0AC3, r16
        uint16_t address = 2755;
        pm_write(s, address, s->r[16]);
        return 5966;
    }
    case 0x174e: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 5970;
    }
    case 0x1752: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 5974 : 5972;
    }
    case 0x1754: { // rjmp .-8
        return 5966;
    }
    case 0x1756: { // sts 0x0AC3, r24
        uint16_t address = 2755;
        pm_write(s, address, s->r[24]);
        return 5978;
    }
    case 0x175a: { // lds r19, 0x0AC2
        uint16_t address = 2754;
        s->r[19] = pm_read(s, address);
        return 5982;
    }
    case 0x175e: { // sbrs r19, 7
        return (!!(s->r[19] & (1u << 7)) == 1) ? 5986 : 5984;
    }
    case 0x1760: { // rjmp .-8
        return 5978;
    }
    case 0x1762: { // lds r24, 0x0AC3
        uint16_t address = 2755;
        s->r[24] = pm_read(s, address);
        return 5990;
    }
    case 0x1766: { // ldi r16, 0x10
        s->r[16] = 16;
        return 5992;
    }
    case 0x1768: { // sts 0x0685, r16
        uint16_t address = 1669;
        pm_write(s, address, s->r[16]);
        return 5996;
    }
    case 0x176c: { // sbrc r24, 0
        return (!!(s->r[24] & (1u << 0)) == 0) ? 6000 : 5998;
    }
    case 0x176e: { // rjmp .-54
        return 5946;
    }
    case 0x1770: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
