uint32_t pm_logical_power_state_request(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0a9c: { // sts 0x215B, r16
        uint16_t address = 8539;
        pm_write(s, address, s->r[16]);
        return 2720;
    }
    case 0x0aa0: { // ldi r16, 0x80
        s->r[16] = 128;
        return 2722;
    }
    case 0x0aa2: { // sts 0x0626, r16
        uint16_t address = 1574;
        pm_write(s, address, s->r[16]);
        return 2726;
    }
    case 0x0aa6: { // lds r16, 0x2159
        uint16_t address = 8537;
        s->r[16] = pm_read(s, address);
        return 2730;
    }
    case 0x0aaa: { // and r16, r16
        s->r[16] &= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 2732;
    }
    case 0x0aac: { // breq .+6
        return (pm_getflag(s, 1) == 1) ? 2740 : 2734;
    }
    case 0x0aae: { // cpi r16, 0x05
        pm_sub(s, s->r[16], 5, 0, false);
        return 2736;
    }
    case 0x0ab0: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 2740 : 2738;
    }
    case 0x0ab2: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x0ab4: { // ldi r16, 0x04
        s->r[16] = 4;
        return 2742;
    }
    case 0x0ab6: { // sts 0x2159, r16
        uint16_t address = 8537;
        pm_write(s, address, s->r[16]);
        return 2746;
    }
    case 0x0aba: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
