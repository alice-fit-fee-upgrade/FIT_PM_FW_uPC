uint32_t pm_logical_power_fault_state(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0b68: { // ldi r16, 0x01
        s->r[16] = 1;
        return 2922;
    }
    case 0x0b6a: { // sts 0x0689, r16
        uint16_t address = 1673;
        pm_write(s, address, s->r[16]);
        return 2926;
    }
    case 0x0b6e: { // ldi r16, 0x08
        s->r[16] = 8;
        return 2928;
    }
    case 0x0b70: { // sts 0x0646, r16
        uint16_t address = 1606;
        pm_write(s, address, s->r[16]);
        return 2932;
    }
    case 0x0b74: { // ldi r16, 0xA0
        s->r[16] = 160;
        return 2934;
    }
    case 0x0b76: { // sts 0x0626, r16
        uint16_t address = 1574;
        pm_write(s, address, s->r[16]);
        return 2938;
    }
    case 0x0b7a: { // ldi r16, 0xC0
        s->r[16] = 192;
        return 2940;
    }
    case 0x0b7c: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 2944;
    }
    case 0x0b80: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 2946;
    }
    case 0x0b82: { // sts 0x2158, r16
        uint16_t address = 8536;
        pm_write(s, address, s->r[16]);
        return 2950;
    }
    case 0x0b86: { // sts 0x2159, r16
        uint16_t address = 8537;
        pm_write(s, address, s->r[16]);
        return 2954;
    }
    case 0x0b8a: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 2958;
    }
    case 0x0b8e: { // andi r16, 0x7F
        s->r[16] &= 127;
        pm_nzv(s, s->r[16], false);
        return 2960;
    }
    case 0x0b90: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 2964;
    }
    case 0x0b94: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
