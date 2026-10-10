uint32_t pm_logical_console_character_send(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x28ac: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 10414;
    }
    case 0x28ae: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 10416;
    }
    case 0x28b0: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 10418;
    }
    case 0x28b2: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 10420;
    }
    case 0x28b4: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 10422;
    }
    case 0x28b6: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 10424;
    }
    case 0x28b8: { // ldi r30, 0x02
        s->r[30] = 2;
        return 10426;
    }
    case 0x28ba: { // ldi r31, 0x20
        s->r[31] = 32;
        return 10428;
    }
    case 0x28bc: { // cli
        pm_irq(s, false);
        return 10430;
    }
    case 0x28be: { // ld r17, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[17] = pm_read(s, address);
        return 10432;
    }
    case 0x28c0: { // ld r18, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[18] = pm_read(s, address);
        return 10434;
    }
    case 0x28c2: { // cp r17, r18
        pm_sub(s, s->r[17], s->r[18], 0, false);
        return 10436;
    }
    case 0x28c4: { // breq .+34
        return (pm_getflag(s, 1) == 1) ? 10472 : 10438;
    }
    case 0x28c6: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 10440;
    }
    case 0x28c8: { // cp r17, r18
        pm_sub(s, s->r[17], s->r[18], 0, false);
        return 10442;
    }
    case 0x28ca: { // brne .+6
        return (pm_getflag(s, 1) == 0) ? 10450 : 10444;
    }
    case 0x28cc: { // sei
        pm_irq(s, true);
        return 10446;
    }
    case 0x28ce: { // sbiw r30, 0x02
        uint16_t old = pm_pointer(s, 30);
        uint16_t value = old - 2;
        pm_setpointer(s, 30, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, !old_negative && negative);
        pm_flag(s, OVERFLOW, old_negative && !negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 10448;
    }
    case 0x28d0: { // rjmp .-22
        return 10428;
    }
    case 0x28d2: { // ldi r30, 0x47
        s->r[30] = 71;
        return 10452;
    }
    case 0x28d4: { // ldi r31, 0x20
        s->r[31] = 32;
        return 10454;
    }
    case 0x28d6: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 10456;
    }
    case 0x28d8: { // add r30, r18
        s->r[30] = pm_add(s, s->r[30], s->r[18], 0);
        return 10458;
    }
    case 0x28da: { // adc r31, r17
        s->r[31] = pm_add(s, s->r[31], s->r[17], pm_getflag(s, CARRY));
        return 10460;
    }
    case 0x28dc: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 10462;
    }
    case 0x28de: { // ldi r30, 0x03
        s->r[30] = 3;
        return 10464;
    }
    case 0x28e0: { // ldi r31, 0x20
        s->r[31] = 32;
        return 10466;
    }
    case 0x28e2: { // st Z, r18
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[18]);
        return 10468;
    }
    case 0x28e4: { // sei
        pm_irq(s, true);
        return 10470;
    }
    case 0x28e6: { // rjmp .+32
        return 10504;
    }
    case 0x28e8: { // inc r18
        s->r[18]++;
        pm_nzv(s, s->r[18], s->r[18] == 128);
        return 10474;
    }
    case 0x28ea: { // ld r19, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[19] = pm_read(s, address);
        return 10476;
    }
    case 0x28ec: { // and r19, r19
        s->r[19] &= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 10478;
    }
    case 0x28ee: { // breq .-30
        return (pm_getflag(s, 1) == 1) ? 10450 : 10480;
    }
    case 0x28f0: { // lds r20, 0x0BA1
        uint16_t address = 2977;
        s->r[20] = pm_read(s, address);
        return 10484;
    }
    case 0x28f4: { // sbrs r20, 5
        return (!!(s->r[20] & (1u << 5)) == 1) ? 10488 : 10486;
    }
    case 0x28f6: { // rjmp .-38
        return 10450;
    }
    case 0x28f8: { // sts 0x0BA0, r16
        uint16_t address = 2976;
        pm_write(s, address, s->r[16]);
        return 10492;
    }
    case 0x28fc: { // lds r20, 0x0BA3
        uint16_t address = 2979;
        s->r[20] = pm_read(s, address);
        return 10496;
    }
    case 0x2900: { // ori r20, 0x02
        s->r[20] |= 2;
        pm_nzv(s, s->r[20], false);
        return 10498;
    }
    case 0x2902: { // sts 0x0BA3, r20
        uint16_t address = 2979;
        pm_write(s, address, s->r[20]);
        return 10502;
    }
    case 0x2906: { // sei
        pm_irq(s, true);
        return 10504;
    }
    case 0x2908: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 10506;
    }
    case 0x290a: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 10508;
    }
    case 0x290c: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 10510;
    }
    case 0x290e: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 10512;
    }
    case 0x2910: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 10514;
    }
    case 0x2912: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 10516;
    }
    case 0x2914: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
