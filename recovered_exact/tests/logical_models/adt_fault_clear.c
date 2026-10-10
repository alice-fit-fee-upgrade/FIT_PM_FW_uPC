uint32_t pm_logical_adt_fault_clear(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x25ea: { // ldi r16, 0x10
        s->r[16] = 16;
        return 9708;
    }
    case 0x25ec: { // sts 0x0606, r16
        uint16_t address = 1542;
        pm_write(s, address, s->r[16]);
        return 9712;
    }
    case 0x25f0: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 9714;
    }
    case 0x25f2: { // rcall .+20
        s->calls[s->call_depth++] = 9716;
        return 9736;
    }
    case 0x25f4: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 9718;
    }
    case 0x25f6: { // rcall .+16
        s->calls[s->call_depth++] = 9720;
        return 9736;
    }
    case 0x25f8: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 9722;
    }
    case 0x25fa: { // rcall .+12
        s->calls[s->call_depth++] = 9724;
        return 9736;
    }
    case 0x25fc: { // ldi r16, 0xFF
        s->r[16] = 255;
        return 9726;
    }
    case 0x25fe: { // rcall .+8
        s->calls[s->call_depth++] = 9728;
        return 9736;
    }
    case 0x2600: { // ldi r16, 0x10
        s->r[16] = 16;
        return 9730;
    }
    case 0x2602: { // sts 0x0605, r16
        uint16_t address = 1541;
        pm_write(s, address, s->r[16]);
        return 9734;
    }
    case 0x2606: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
