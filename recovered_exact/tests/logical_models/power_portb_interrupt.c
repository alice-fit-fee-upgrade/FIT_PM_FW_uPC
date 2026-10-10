uint32_t pm_logical_power_portb_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0abc: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 2750;
    }
    case 0x0abe: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 2752;
    }
    case 0x0ac0: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 2754;
    }
    case 0x0ac2: { // lds r16, 0x2157
        uint16_t address = 8535;
        s->r[16] = pm_read(s, address);
        return 2758;
    }
    case 0x0ac6: { // ori r16, 0x60
        s->r[16] |= 96;
        pm_nzv(s, s->r[16], false);
        return 2760;
    }
    case 0x0ac8: { // sts 0x2157, r16
        uint16_t address = 8535;
        pm_write(s, address, s->r[16]);
        return 2764;
    }
    case 0x0acc: { // ldi r16, 0x40
        s->r[16] = 64;
        return 2766;
    }
    case 0x0ace: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 2770;
    }
    case 0x0ad2: { // ldi r16, 0x04
        s->r[16] = 4;
        return 2772;
    }
    case 0x0ad4: { // sts 0x0686, r16
        uint16_t address = 1670;
        pm_write(s, address, s->r[16]);
        return 2776;
    }
    case 0x0ad8: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 2778;
    }
    case 0x0ada: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 2780;
    }
    case 0x0adc: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 2782;
    }
    case 0x0ade: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
