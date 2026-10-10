uint32_t pm_logical_console_crlf(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x281e: { // ldi r30, 0x84
        s->r[30] = 132;
        return 10272;
    }
    case 0x2820: { // ldi r31, 0x29
        s->r[31] = 41;
        return 10274;
    }
    case 0x2822: { // rcall .+2
        s->calls[s->call_depth++] = 10276;
        return 10278;
    }
    case 0x2824: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
