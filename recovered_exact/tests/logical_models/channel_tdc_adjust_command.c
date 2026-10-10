uint32_t pm_logical_channel_tdc_adjust_command(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x1ee0: { // rcall .+1874
        s->calls[s->call_depth++] = 7906;
        return 9780;
    }
    case 0x1ee2: { // brcs .+84
        return (pm_getflag(s, 0) == 1) ? 7992 : 7908;
    }
    case 0x1ee4: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 7910;
    }
    case 0x1ee6: { // brne .+80
        return (pm_getflag(s, 1) == 0) ? 7992 : 7912;
    }
    case 0x1ee8: { // cpi r20, 0x0C
        pm_sub(s, s->r[20], 12, 0, false);
        return 7914;
    }
    case 0x1eea: { // brge .+76
        return (pm_getflag(s, 4) == 0) ? 7992 : 7916;
    }
    case 0x1eec: { // mov r19, r20
        s->r[19] = s->r[20];
        return 7918;
    }
    case 0x1eee: { // cpi r16, 0x20
        pm_sub(s, s->r[16], 32, 0, false);
        return 7920;
    }
    case 0x1ef0: { // brne .+70
        return (pm_getflag(s, 1) == 0) ? 7992 : 7922;
    }
    case 0x1ef2: { // rcall .+1856
        s->calls[s->call_depth++] = 7924;
        return 9780;
    }
    case 0x1ef4: { // brcs .+66
        return (pm_getflag(s, 0) == 1) ? 7992 : 7926;
    }
    case 0x1ef6: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 7928;
    }
    case 0x1ef8: { // brne .+62
        return (pm_getflag(s, 1) == 0) ? 7992 : 7930;
    }
    case 0x1efa: { // ldi r24, 0x00
        s->r[24] = 0;
        return 7932;
    }
    case 0x1efc: { // cpi r20, 0x40
        pm_sub(s, s->r[20], 64, 0, false);
        return 7934;
    }
    case 0x1efe: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 7936;
    }
    case 0x1f00: { // brge .+54
        return (pm_getflag(s, 4) == 0) ? 7992 : 7938;
    }
    case 0x1f02: { // ldi r24, 0xFF
        s->r[24] = 255;
        return 7940;
    }
    case 0x1f04: { // cpi r20, 0xC0
        pm_sub(s, s->r[20], 192, 0, false);
        return 7942;
    }
    case 0x1f06: { // cpc r21, r24
        pm_sub(s, s->r[21], s->r[24], pm_getflag(s, CARRY), true);
        return 7944;
    }
    case 0x1f08: { // brlt .+46
        return (pm_getflag(s, 4) == 1) ? 7992 : 7946;
    }
    case 0x1f0a: { // rcall .+528
        s->calls[s->call_depth++] = 7948;
        return 8476;
    }
    case 0x1f0c: { // brcc .+2
        return (pm_getflag(s, 0) == 0) ? 7952 : 7950;
    }
    case 0x1f0e: { // ret
        return s->calls[--s->call_depth];
    }
    case 0x1f10: { // ldi r28, 0x7B
        s->r[28] = 123;
        return 7954;
    }
    case 0x1f12: { // ldi r29, 0x21
        s->r[29] = 33;
        return 7956;
    }
    case 0x1f14: { // eor r11, r11
        s->r[11] ^= s->r[11];
        pm_nzv(s, s->r[11], false);
        return 7958;
    }
    case 0x1f16: { // add r28, r19
        s->r[28] = pm_add(s, s->r[28], s->r[19], 0);
        return 7960;
    }
    case 0x1f18: { // adc r29, r11
        s->r[29] = pm_add(s, s->r[29], s->r[11], pm_getflag(s, CARRY));
        return 7962;
    }
    case 0x1f1a: { // st Y, r20
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[20]);
        return 7964;
    }
    case 0x1f1c: { // mov r21, r19
        s->r[21] = s->r[19];
        return 7966;
    }
    case 0x1f1e: { // andi r21, 0x03
        s->r[21] &= 3;
        pm_nzv(s, s->r[21], false);
        return 7968;
    }
    case 0x1f20: { // ldi r22, 0x20
        s->r[22] = 32;
        return 7970;
    }
    case 0x1f22: { // mul r21, r22
        uint16_t product = (s->r[21]) * (int)s->r[22];
        pm_setpointer(s, 0, product);
        pm_flag(s, ZERO, product == 0);
        pm_flag(s, CARRY, product & 0x8000);
        return 7972;
    }
    case 0x1f24: { // ldi r16, 0x0C
        s->r[16] = 12;
        return 7974;
    }
    case 0x1f26: { // add r16, r0
        s->r[16] = pm_add(s, s->r[16], s->r[0], 0);
        return 7976;
    }
    case 0x1f28: { // lsr r19
        bool carry = s->r[19] & 1;
        s->r[19] = (s->r[19] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[19], !!(s->r[19] & 128) ^ carry);
        return 7978;
    }
    case 0x1f2a: { // lsr r19
        bool carry = s->r[19] & 1;
        s->r[19] = (s->r[19] >> 1) | 0;
        pm_flag(s, CARRY, carry);
        pm_nzv(s, s->r[19], !!(s->r[19] & 128) ^ carry);
        return 7980;
    }
    case 0x1f2c: { // mov r17, r20
        s->r[17] = s->r[20];
        return 7982;
    }
    case 0x1f2e: { // eor r18, r18
        s->r[18] ^= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 7984;
    }
    case 0x1f30: { // cli
        pm_irq(s, false);
        return 7986;
    }
    case 0x1f32: { // rcall .+576
        s->calls[s->call_depth++] = 7988;
        return 8564;
    }
    case 0x1f34: { // sei
        pm_irq(s, true);
        return 7990;
    }
    case 0x1f36: { // rjmp .+170
        return 8162;
    }
    case 0x1f38: { // rjmp .-2930
        return 5064;
    }
    default: return UINT32_MAX;
    }
}
