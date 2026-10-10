uint32_t pm_logical_dac_offset_scale(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x20a6: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 8360;
    }
    case 0x20a8: { // ldi r18, 0x0C
        s->r[18] = 12;
        return 8362;
    }
    case 0x20aa: { // ldi r19, 0x02
        s->r[19] = 2;
        return 8364;
    }
    case 0x20ac: { // rcall .+158
        s->calls[s->call_depth++] = 8366;
        return 8524;
    }
    case 0x20ae: { // ldi r30, 0x63
        s->r[30] = 99;
        return 8368;
    }
    case 0x20b0: { // ldi r31, 0x21
        s->r[31] = 33;
        return 8370;
    }
    case 0x20b2: { // eor r20, r20
        s->r[20] ^= s->r[20];
        pm_nzv(s, s->r[20], false);
        return 8372;
    }
    case 0x20b4: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8374;
    }
    case 0x20b6: { // add r30, r22
        s->r[30] = pm_add(s, s->r[30], s->r[22], 0);
        return 8376;
    }
    case 0x20b8: { // adc r31, r20
        s->r[31] = pm_add(s, s->r[31], s->r[20], pm_getflag(s, CARRY));
        return 8378;
    }
    case 0x20ba: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8380;
    }
    case 0x20bc: { // ori r22, 0x03
        s->r[22] |= 3;
        pm_nzv(s, s->r[22], false);
        return 8382;
    }
    case 0x20be: { // ld r20, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[20] = pm_read(s, address);
        return 8384;
    }
    case 0x20c0: { // ld r21, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[21] = pm_read(s, address);
        return 8386;
    }
    case 0x20c2: { // add r16, r20
        s->r[16] = pm_add(s, s->r[16], s->r[20], 0);
        return 8388;
    }
    case 0x20c4: { // adc r17, r21
        s->r[17] = pm_add(s, s->r[17], s->r[21], pm_getflag(s, CARRY));
        return 8390;
    }
    case 0x20c6: { // com r16
        s->r[16] = ~s->r[16];
        pm_nzv(s, s->r[16], false);
        pm_flag(s, CARRY, true);
        return 8392;
    }
    case 0x20c8: { // com r17
        s->r[17] = ~s->r[17];
        pm_nzv(s, s->r[17], false);
        pm_flag(s, CARRY, true);
        return 8394;
    }
    case 0x20ca: { // rcall .+478
        s->calls[s->call_depth++] = 8396;
        return 8874;
    }
    case 0x20cc: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 8398;
    }
    case 0x20ce: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
