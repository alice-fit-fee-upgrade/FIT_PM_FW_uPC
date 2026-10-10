uint32_t pm_logical_eeprom_settings_store(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x19ae: { // rcall .+3724
        s->calls[s->call_depth++] = 6576;
        return 10300;
    }
    case 0x19b0: { // cpi r16, 0x0D
        pm_sub(s, s->r[16], 13, 0, false);
        return 6578;
    }
    case 0x19b2: { // brne .-8
        return (pm_getflag(s, 1) == 0) ? 6572 : 6580;
    }
    case 0x19b4: { // lds r16, 0x01CF
        uint16_t address = 463;
        s->r[16] = pm_read(s, address);
        return 6584;
    }
    case 0x19b8: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 6588 : 6586;
    }
    case 0x19ba: { // rjmp .-8
        return 6580;
    }
    case 0x19bc: { // ldi r16, 0x36
        s->r[16] = 54;
        return 6590;
    }
    case 0x19be: { // sts 0x01CA, r16
        uint16_t address = 458;
        pm_write(s, address, s->r[16]);
        return 6594;
    }
    case 0x19c2: { // ldi r16, 0x01
        s->r[16] = 1;
        return 6596;
    }
    case 0x19c4: { // ldi r17, 0xD8
        s->r[17] = 216;
        return 6598;
    }
    case 0x19c6: { // cli
        pm_irq(s, false);
        return 6600;
    }
    case 0x19c8: { // sts 0x0034, r17
        uint16_t address = 52;
        pm_write(s, address, s->r[17]);
        return 6604;
    }
    case 0x19cc: { // sts 0x01CB, r16
        uint16_t address = 459;
        pm_write(s, address, s->r[16]);
        return 6608;
    }
    case 0x19d0: { // sei
        pm_irq(s, true);
        return 6610;
    }
    case 0x19d2: { // ldi r30, 0x63
        s->r[30] = 99;
        return 6612;
    }
    case 0x19d4: { // ldi r31, 0x21
        s->r[31] = 33;
        return 6614;
    }
    case 0x19d6: { // ldi r28, 0xFF
        s->r[28] = 255;
        return 6616;
    }
    case 0x19d8: { // ldi r29, 0x0F
        s->r[29] = 15;
        return 6618;
    }
    case 0x19da: { // ldi r20, 0x22
        s->r[20] = 34;
        return 6620;
    }
    case 0x19dc: { // lds r16, 0x01CF
        uint16_t address = 463;
        s->r[16] = pm_read(s, address);
        return 6624;
    }
    case 0x19e0: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 6628 : 6626;
    }
    case 0x19e2: { // rjmp .-8
        return 6620;
    }
    case 0x19e4: { // sbi 0x00, 3
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value | (1u << 3));
        return 6630;
    }
    case 0x19e6: { // eor r19, r19
        s->r[19] ^= s->r[19];
        pm_nzv(s, s->r[19], false);
        return 6632;
    }
    case 0x19e8: { // adiw r28, 0x01
        uint16_t old = pm_pointer(s, 28);
        uint16_t value = old + 1;
        pm_setpointer(s, 28, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, old_negative && !negative);
        pm_flag(s, OVERFLOW, !old_negative && negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 6634;
    }
    case 0x19ea: { // ld r16, Z+
        uint16_t address = pm_pointer(s, 30) + 0;
        pm_setpointer(s, 30, address + 1);
        s->r[16] = pm_read(s, address);
        return 6636;
    }
    case 0x19ec: { // ld r17, Y
        uint16_t address = pm_pointer(s, 28) + 0;
        s->r[17] = pm_read(s, address);
        return 6638;
    }
    case 0x19ee: { // eor r17, r16
        s->r[17] ^= s->r[16];
        pm_nzv(s, s->r[17], false);
        return 6640;
    }
    case 0x19f0: { // breq .+4
        return (pm_getflag(s, 1) == 1) ? 6646 : 6642;
    }
    case 0x19f2: { // st Y, r16
        uint16_t address = pm_pointer(s, 28) + 0;
        pm_write(s, address, s->r[16]);
        return 6644;
    }
    case 0x19f4: { // ldi r19, 0x01
        s->r[19] = 1;
        return 6646;
    }
    case 0x19f6: { // cpi r30, 0x35
        pm_sub(s, s->r[30], 53, 0, false);
        return 6648;
    }
    case 0x19f8: { // cpc r31, r20
        pm_sub(s, s->r[31], s->r[20], pm_getflag(s, CARRY), true);
        return 6650;
    }
    case 0x19fa: { // breq .+12
        return (pm_getflag(s, 1) == 1) ? 6664 : 6652;
    }
    case 0x19fc: { // mov r18, r28
        s->r[18] = s->r[28];
        return 6654;
    }
    case 0x19fe: { // andi r18, 0x1F
        s->r[18] &= 31;
        pm_nzv(s, s->r[18], false);
        return 6656;
    }
    case 0x1a00: { // cpi r18, 0x1F
        pm_sub(s, s->r[18], 31, 0, false);
        return 6658;
    }
    case 0x1a02: { // brne .-28
        return (pm_getflag(s, 1) == 0) ? 6632 : 6660;
    }
    case 0x1a04: { // rcall .+8
        s->calls[s->call_depth++] = 6662;
        return 6670;
    }
    case 0x1a06: { // rjmp .-32
        return 6632;
    }
    case 0x1a08: { // rcall .+4
        s->calls[s->call_depth++] = 6666;
        return 6670;
    }
    case 0x1a0a: { // cbi 0x00, 3
        uint8_t value = pm_io_read(s, 0);
        pm_io_write(s, 0, value & ~ (1u << 3));
        return 6668;
    }
    case 0x1a0c: { // rjmp .+1492
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
