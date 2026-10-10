uint32_t pm_logical_console_hex_word(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x26f8: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 9978;
    }
    case 0x26fa: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 9980;
    }
    case 0x26fc: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 9982;
    }
    case 0x26fe: { // mov r18, r16
        s->r[18] = s->r[16];
        return 9984;
    }
    case 0x2700: { // swap r17
        s->r[17] = (s->r[17] >> 4) | (s->r[17] << 4);
        return 9986;
    }
    case 0x2702: { // mov r16, r17
        s->r[16] = s->r[17];
        return 9988;
    }
    case 0x2704: { // rcall .+26
        s->calls[s->call_depth++] = 9990;
        return 10016;
    }
    case 0x2706: { // swap r17
        s->r[17] = (s->r[17] >> 4) | (s->r[17] << 4);
        return 9992;
    }
    case 0x2708: { // mov r16, r17
        s->r[16] = s->r[17];
        return 9994;
    }
    case 0x270a: { // rcall .+20
        s->calls[s->call_depth++] = 9996;
        return 10016;
    }
    case 0x270c: { // swap r18
        s->r[18] = (s->r[18] >> 4) | (s->r[18] << 4);
        return 9998;
    }
    case 0x270e: { // mov r16, r18
        s->r[16] = s->r[18];
        return 10000;
    }
    case 0x2710: { // rcall .+14
        s->calls[s->call_depth++] = 10002;
        return 10016;
    }
    case 0x2712: { // swap r18
        s->r[18] = (s->r[18] >> 4) | (s->r[18] << 4);
        return 10004;
    }
    case 0x2714: { // mov r16, r18
        s->r[16] = s->r[18];
        return 10006;
    }
    case 0x2716: { // rcall .+8
        s->calls[s->call_depth++] = 10008;
        return 10016;
    }
    case 0x2718: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 10010;
    }
    case 0x271a: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 10012;
    }
    case 0x271c: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 10014;
    }
    case 0x271e: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
