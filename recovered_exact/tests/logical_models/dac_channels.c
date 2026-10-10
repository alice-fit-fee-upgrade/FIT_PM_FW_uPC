uint32_t pm_logical_dac_channels(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x20ec: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 8430;
    }
    case 0x20ee: { // ldi r18, 0x88
        s->r[18] = 136;
        return 8432;
    }
    case 0x20f0: { // ldi r19, 0x41
        s->r[19] = 65;
        return 8434;
    }
    case 0x20f2: { // rcall .+60
        s->calls[s->call_depth++] = 8436;
        return 8496;
    }
    case 0x20f4: { // ldi r18, 0x80
        s->r[18] = 128;
        return 8438;
    }
    case 0x20f6: { // eor r17, r18
        s->r[17] ^= s->r[18];
        pm_nzv(s, s->r[17], false);
        return 8440;
    }
    case 0x20f8: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8442;
    }
    case 0x20fa: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8444;
    }
    case 0x20fc: { // ori r22, 0x02
        s->r[22] |= 2;
        pm_nzv(s, s->r[22], false);
        return 8446;
    }
    case 0x20fe: { // rcall .+426
        s->calls[s->call_depth++] = 8448;
        return 8874;
    }
    case 0x2100: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 8450;
    }
    case 0x2102: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x2104: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 8454;
    }
    case 0x2106: { // ldi r18, 0x88
        s->r[18] = 136;
        return 8456;
    }
    case 0x2108: { // ldi r19, 0x41
        s->r[19] = 65;
        return 8458;
    }
    case 0x210a: { // rcall .+36
        s->calls[s->call_depth++] = 8460;
        return 8496;
    }
    case 0x210c: { // ldi r18, 0x80
        s->r[18] = 128;
        return 8462;
    }
    case 0x210e: { // eor r17, r18
        s->r[17] ^= s->r[18];
        pm_nzv(s, s->r[17], false);
        return 8464;
    }
    case 0x2110: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8466;
    }
    case 0x2112: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8468;
    }
    case 0x2114: { // ori r22, 0x01
        s->r[22] |= 1;
        pm_nzv(s, s->r[22], false);
        return 8470;
    }
    case 0x2116: { // rcall .+402
        s->calls[s->call_depth++] = 8472;
        return 8874;
    }
    case 0x2118: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 8474;
    }
    case 0x211a: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
