uint32_t pm_logical_programming_unlock(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x157a: { // call 0x283c
        s->calls[s->call_depth++] = 5502;
        return 10300;
    }
    case 0x157e: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 5504;
    }
    case 0x1580: { // brne .+30
        return (pm_getflag(s, 1) == 0) ? 5536 : 5506;
    }
    case 0x1582: { // ldi r30, 0x76
        s->r[30] = 118;
        return 5508;
    }
    case 0x1584: { // ldi r31, 0x2B
        s->r[31] = 43;
        return 5510;
    }
    case 0x1586: { // lpm r17, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[17] = pm_golden_flash[address];
        return 5512;
    }
    case 0x1588: { // call 0x2836
        s->calls[s->call_depth++] = 5516;
        return 10294;
    }
    case 0x158c: { // cp r16, r17
        pm_sub(s, s->r[16], s->r[17], 0, false);
        return 5518;
    }
    case 0x158e: { // brne .+16
        return (pm_getflag(s, 1) == 0) ? 5536 : 5520;
    }
    case 0x1590: { // cpi r30, 0x7E
        pm_sub(s, s->r[30], 126, 0, false);
        return 5522;
    }
    case 0x1592: { // brne .-14
        return (pm_getflag(s, 1) == 0) ? 5510 : 5524;
    }
    case 0x1594: { // ldi r30, 0x98
        s->r[30] = 152;
        return 5526;
    }
    case 0x1596: { // ldi r31, 0x29
        s->r[31] = 41;
        return 5528;
    }
    case 0x1598: { // call 0x2826
        s->calls[s->call_depth++] = 5532;
        return 10278;
    }
    case 0x159c: { // clc
        pm_flag(s, CARRY, false);
        return 5534;
    }
    case 0x159e: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x15a0: { // sec
        pm_flag(s, CARRY, true);
        return 5538;
    }
    case 0x15a2: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x15a4: { // rjmp .-478
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
