uint32_t pm_logical_dac_gain_scale(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x20d0: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 8402;
    }
    case 0x20d2: { // ldi r18, 0x20
        s->r[18] = 32;
        return 8404;
    }
    case 0x20d4: { // ldi r19, 0x4E
        s->r[19] = 78;
        return 8406;
    }
    case 0x20d6: { // sub r18, r20
        s->r[18] = pm_sub(s, s->r[18], s->r[20], 0, false);
        return 8408;
    }
    case 0x20d8: { // sbc r19, r21
        s->r[19] = pm_sub(s, s->r[19], s->r[21], pm_getflag(s, CARRY), true);
        return 8410;
    }
    case 0x20da: { // movw r20, r18
        uint16_t pair = pm_pointer(s, 18);
        pm_setpointer(s, 20, pair);
        return 8412;
    }
    case 0x20dc: { // ldi r18, 0x72
        s->r[18] = 114;
        return 8414;
    }
    case 0x20de: { // ldi r19, 0x02
        s->r[19] = 2;
        return 8416;
    }
    case 0x20e0: { // rcall .+106
        s->calls[s->call_depth++] = 8418;
        return 8524;
    }
    case 0x20e2: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8420;
    }
    case 0x20e4: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8422;
    }
    case 0x20e6: { // rcall .+450
        s->calls[s->call_depth++] = 8424;
        return 8874;
    }
    case 0x20e8: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 8426;
    }
    case 0x20ea: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
