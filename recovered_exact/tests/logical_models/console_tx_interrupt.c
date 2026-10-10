uint32_t pm_logical_console_tx_interrupt(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0e88: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 3722;
    }
    case 0x0e8a: { // in r31, 0x3f
        s->r[31] = pm_io_read(s, 63);
        return 3724;
    }
    case 0x0e8c: { // push r31
        s->stack[s->depth++] = s->r[31];
        return 3726;
    }
    case 0x0e8e: { // push r30
        s->stack[s->depth++] = s->r[30];
        return 3728;
    }
    case 0x0e90: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 3730;
    }
    case 0x0e92: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 3732;
    }
    case 0x0e94: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 3734;
    }
    case 0x0e96: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 3736;
    }
    case 0x0e98: { // ldi r30, 0x02
        s->r[30] = 2;
        return 3738;
    }
    case 0x0e9a: { // ldi r31, 0x20
        s->r[31] = 32;
        return 3740;
    }
    case 0x0e9c: { // ld r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_read(s, address);
        return 3742;
    }
    case 0x0e9e: { // ld r17, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[17] = pm_read(s, address);
        return 3744;
    }
    case 0x0ea0: { // ld r18, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[18] = pm_read(s, address);
        return 3746;
    }
    case 0x0ea2: { // and r18, r18
        s->r[18] &= s->r[18];
        pm_nzv(s, s->r[18], false);
        return 3748;
    }
    case 0x0ea4: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 3754 : 3750;
    }
    case 0x0ea6: { // cp r16, r17
        pm_sub(s, s->r[16], s->r[17], 0, false);
        return 3752;
    }
    case 0x0ea8: { // brne .+12
        return (pm_getflag(s, 1) == 0) ? 3766 : 3754;
    }
    case 0x0eaa: { // lds r19, 0x0BA3
        uint16_t address = 2979;
        s->r[19] = pm_read(s, address);
        return 3758;
    }
    case 0x0eae: { // andi r19, 0xFC
        s->r[19] &= 252;
        pm_nzv(s, s->r[19], false);
        return 3760;
    }
    case 0x0eb0: { // sts 0x0BA3, r19
        uint16_t address = 2979;
        pm_write(s, address, s->r[19]);
        return 3764;
    }
    case 0x0eb4: { // rjmp .+24
        return 3790;
    }
    case 0x0eb6: { // inc r16
        s->r[16]++;
        pm_nzv(s, s->r[16], s->r[16] == 128);
        return 3768;
    }
    case 0x0eb8: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 3770;
    }
    case 0x0eba: { // ldi r30, 0x47
        s->r[30] = 71;
        return 3772;
    }
    case 0x0ebc: { // ldi r31, 0x20
        s->r[31] = 32;
        return 3774;
    }
    case 0x0ebe: { // add r30, r16
        s->r[30] = pm_add(s, s->r[30], s->r[16], 0);
        return 3776;
    }
    case 0x0ec0: { // adc r31, r17
        s->r[31] = pm_add(s, s->r[31], s->r[17], pm_getflag(s, CARRY));
        return 3778;
    }
    case 0x0ec2: { // ld r17, Z
        uint16_t address = pm_pointer(s, 30) + 0;
        s->r[17] = pm_read(s, address);
        return 3780;
    }
    case 0x0ec4: { // sts 0x0BA0, r17
        uint16_t address = 2976;
        pm_write(s, address, s->r[17]);
        return 3784;
    }
    case 0x0ec8: { // ldi r30, 0x02
        s->r[30] = 2;
        return 3786;
    }
    case 0x0eca: { // ldi r31, 0x20
        s->r[31] = 32;
        return 3788;
    }
    case 0x0ecc: { // st Z, r16
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_write(s, address, s->r[16]);
        return 3790;
    }
    case 0x0ece: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 3792;
    }
    case 0x0ed0: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 3794;
    }
    case 0x0ed2: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 3796;
    }
    case 0x0ed4: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 3798;
    }
    case 0x0ed6: { // pop r30
        s->r[30] = s->stack[--s->depth];
        return 3800;
    }
    case 0x0ed8: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 3802;
    }
    case 0x0eda: { // out 0x3f, r31
        pm_io_write(s, 63, s->r[31]);
        return 3804;
    }
    case 0x0edc: { // pop r31
        s->r[31] = s->stack[--s->depth];
        return 3806;
    }
    case 0x0ede: { // reti
        pm_flag(s, INTERRUPT, true);
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
