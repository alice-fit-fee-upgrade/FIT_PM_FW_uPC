uint32_t pm_logical_adt8(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2598: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 9626;
    }
    case 0x259a: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 9628;
    }
    case 0x259c: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 9630;
    }
    case 0x259e: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 9632;
    }
    case 0x25a0: { // ldi r18, 0x10
        s->r[18] = 16;
        return 9634;
    }
    case 0x25a2: { // sts 0x0606, r18
        uint16_t address = 1542;
        pm_write(s, address, s->r[18]);
        return 9638;
    }
    case 0x25a6: { // mov r19, r17
        s->r[19] = s->r[17];
        return 9640;
    }
    case 0x25a8: { // rcall .+94
        s->calls[s->call_depth++] = 9642;
        return 9736;
    }
    case 0x25aa: { // mov r16, r19
        s->r[16] = s->r[19];
        return 9644;
    }
    case 0x25ac: { // rcall .+90
        s->calls[s->call_depth++] = 9646;
        return 9736;
    }
    case 0x25ae: { // ldi r18, 0x10
        s->r[18] = 16;
        return 9648;
    }
    case 0x25b0: { // sts 0x0605, r18
        uint16_t address = 1541;
        pm_write(s, address, s->r[18]);
        return 9652;
    }
    case 0x25b4: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 9654;
    }
    case 0x25b6: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 9656;
    }
    case 0x25b8: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 9658;
    }
    case 0x25ba: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 9660;
    }
    case 0x25bc: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
