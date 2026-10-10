uint32_t pm_logical_fpga_binary_programming_stream(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x15a6: { // rcall .-46
        s->calls[s->call_depth++] = 5544;
        return 5498;
    }
    case 0x15a8: { // brcs .-6
        return (pm_getflag(s, 0) == 1) ? 5540 : 5546;
    }
    case 0x15aa: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 5548;
    }
    case 0x15ac: { // sts 0x0100, r16
        uint16_t address = 256;
        pm_write(s, address, s->r[16]);
        return 5552;
    }
    case 0x15b0: { // rcall .+178
        s->calls[s->call_depth++] = 5554;
        return 5732;
    }
    case 0x15b2: { // eor r16, r16
        s->r[16] ^= s->r[16];
        pm_nzv(s, s->r[16], false);
        return 5556;
    }
    case 0x15b4: { // ldi r26, 0x35
        s->r[26] = 53;
        return 5558;
    }
    case 0x15b6: { // ldi r27, 0x24
        s->r[27] = 36;
        return 5560;
    }
    case 0x15b8: { // st X+, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[16]);
        return 5562;
    }
    case 0x15ba: { // st X+, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[16]);
        return 5564;
    }
    case 0x15bc: { // st X+, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_setpointer(s, 26, address + 1);
        pm_write(s, address, s->r[16]);
        return 5566;
    }
    case 0x15be: { // st X, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_write(s, address, s->r[16]);
        return 5568;
    }
    case 0x15c0: { // rcall .+638
        s->calls[s->call_depth++] = 5570;
        return 6208;
    }
    case 0x15c2: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 5572;
    }
    case 0x15c4: { // call 0x26f8
        s->calls[s->call_depth++] = 5576;
        return 9976;
    }
    case 0x15c8: { // movw r16, r18
        uint16_t pair = pm_pointer(s, 18);
        pm_setpointer(s, 16, pair);
        return 5578;
    }
    case 0x15ca: { // call 0x26f8
        s->calls[s->call_depth++] = 5582;
        return 9976;
    }
    case 0x15ce: { // call 0x281e
        s->calls[s->call_depth++] = 5586;
        return 10270;
    }
    case 0x15d2: { // call 0x2836
        s->calls[s->call_depth++] = 5590;
        return 10294;
    }
    case 0x15d6: { // mov r28, r16
        s->r[28] = s->r[16];
        return 5592;
    }
    case 0x15d8: { // call 0x2836
        s->calls[s->call_depth++] = 5596;
        return 10294;
    }
    case 0x15dc: { // mov r29, r16
        s->r[29] = s->r[16];
        return 5598;
    }
    case 0x15de: { // call 0x2836
        s->calls[s->call_depth++] = 5602;
        return 10294;
    }
    case 0x15e2: { // mov r30, r16
        s->r[30] = s->r[16];
        return 5604;
    }
    case 0x15e4: { // call 0x2836
        s->calls[s->call_depth++] = 5608;
        return 10294;
    }
    case 0x15e8: { // mov r0, r16
        s->r[0] = s->r[16];
        return 5610;
    }
    case 0x15ea: { // call 0x2836
        s->calls[s->call_depth++] = 5614;
        return 10294;
    }
    case 0x15ee: { // mov r1, r16
        s->r[1] = s->r[16];
        return 5616;
    }
    case 0x15f0: { // call 0x2836
        s->calls[s->call_depth++] = 5620;
        return 10294;
    }
    case 0x15f4: { // mov r2, r16
        s->r[2] = s->r[16];
        return 5622;
    }
    case 0x15f6: { // movw r20, r28
        uint16_t pair = pm_pointer(s, 28);
        pm_setpointer(s, 20, pair);
        return 5624;
    }
    case 0x15f8: { // mov r22, r30
        s->r[22] = s->r[30];
        return 5626;
    }
    case 0x15fa: { // rcall .+276
        s->calls[s->call_depth++] = 5628;
        return 5904;
    }
    case 0x15fc: { // mov r17, r0
        s->r[17] = s->r[0];
        return 5630;
    }
    case 0x15fe: { // mov r18, r1
        s->r[18] = s->r[1];
        return 5632;
    }
    case 0x1600: { // mov r19, r2
        s->r[19] = s->r[2];
        return 5634;
    }
    case 0x1602: { // ldi r16, 0x01
        s->r[16] = 1;
        return 5636;
    }
    case 0x1604: { // call 0x28ac
        s->calls[s->call_depth++] = 5640;
        return 10412;
    }
    case 0x1608: { // sub r17, r20
        s->r[17] = pm_sub(s, s->r[17], s->r[20], 0, false);
        return 5642;
    }
    case 0x160a: { // sbc r18, r21
        s->r[18] = pm_sub(s, s->r[18], s->r[21], pm_getflag(s, CARRY), true);
        return 5644;
    }
    case 0x160c: { // sbc r19, r22
        s->r[19] = pm_sub(s, s->r[19], s->r[22], pm_getflag(s, CARRY), true);
        return 5646;
    }
    case 0x160e: { // subi r17, 0xFF
        s->r[17] = pm_sub(s, s->r[17], 255, 0, false);
        return 5648;
    }
    case 0x1610: { // sbci r18, 0xFF
        s->r[18] = pm_sub(s, s->r[18], 255, pm_getflag(s, CARRY), true);
        return 5650;
    }
    case 0x1612: { // sbci r19, 0xFF
        s->r[19] = pm_sub(s, s->r[19], 255, pm_getflag(s, CARRY), true);
        return 5652;
    }
    case 0x1614: { // or r18, r19
        s->r[18] |= s->r[19];
        pm_nzv(s, s->r[18], false);
        return 5654;
    }
    case 0x1616: { // breq .+2
        return (pm_getflag(s, 1) == 1) ? 5658 : 5656;
    }
    case 0x1618: { // eor r17, r17
        s->r[17] ^= s->r[17];
        pm_nzv(s, s->r[17], false);
        return 5658;
    }
    case 0x161a: { // lds r24, 0x2435
        uint16_t address = 9269;
        s->r[24] = pm_read(s, address);
        return 5662;
    }
    case 0x161e: { // lds r25, 0x2436
        uint16_t address = 9270;
        s->r[25] = pm_read(s, address);
        return 5666;
    }
    case 0x1622: { // ldi r26, 0x35
        s->r[26] = 53;
        return 5668;
    }
    case 0x1624: { // ldi r27, 0x22
        s->r[27] = 34;
        return 5670;
    }
    case 0x1626: { // add r26, r24
        s->r[26] = pm_add(s, s->r[26], s->r[24], 0);
        return 5672;
    }
    case 0x1628: { // adc r27, r25
        s->r[27] = pm_add(s, s->r[27], s->r[25], pm_getflag(s, CARRY));
        return 5674;
    }
    case 0x162a: { // call 0x2836
        s->calls[s->call_depth++] = 5678;
        return 10294;
    }
    case 0x162e: { // st X, r16
        uint16_t address = pm_pointer(s, 26) + 0;
        pm_write(s, address, s->r[16]);
        return 5680;
    }
    case 0x1630: { // adiw r24, 0x01
        uint16_t old = pm_pointer(s, 24);
        uint16_t value = old + 1;
        pm_setpointer(s, 24, value);
        bool old_negative = old & 0x8000, negative = value & 0x8000;
        pm_flag(s, CARRY, old_negative && !negative);
        pm_flag(s, OVERFLOW, !old_negative && negative);
        pm_flag(s, NEGATIVE, negative);
        pm_flag(s, ZERO, value == 0);
        pm_flag(s, SIGNED, negative ^ pm_getflag(s, OVERFLOW));
        return 5682;
    }
    case 0x1632: { // andi r25, 0x01
        s->r[25] &= 1;
        pm_nzv(s, s->r[25], false);
        return 5684;
    }
    case 0x1634: { // dec r17
        s->r[17]--;
        pm_nzv(s, s->r[17], s->r[17] == 127);
        return 5686;
    }
    case 0x1636: { // brne .-22
        return (pm_getflag(s, 1) == 0) ? 5666 : 5688;
    }
    case 0x1638: { // sts 0x2435, r24
        uint16_t address = 9269;
        pm_write(s, address, s->r[24]);
        return 5692;
    }
    case 0x163c: { // sts 0x2436, r25
        uint16_t address = 9270;
        pm_write(s, address, s->r[25]);
        return 5696;
    }
    case 0x1640: { // rcall .+248
        s->calls[s->call_depth++] = 5698;
        return 5946;
    }
    case 0x1642: { // rcall .+110
        s->calls[s->call_depth++] = 5700;
        return 5810;
    }
    case 0x1644: { // brcs .+8
        return (pm_getflag(s, 0) == 1) ? 5710 : 5702;
    }
    case 0x1646: { // and r21, r21
        s->r[21] &= s->r[21];
        pm_nzv(s, s->r[21], false);
        return 5704;
    }
    case 0x1648: { // brne .-78
        return (pm_getflag(s, 1) == 0) ? 5628 : 5706;
    }
    case 0x164a: { // rcall .+238
        s->calls[s->call_depth++] = 5708;
        return 5946;
    }
    case 0x164c: { // rjmp .-84
        return 5626;
    }
    case 0x164e: { // rcall .+234
        s->calls[s->call_depth++] = 5712;
        return 5946;
    }
    case 0x1650: { // rcall .+288
        s->calls[s->call_depth++] = 5714;
        return 6002;
    }
    case 0x1652: { // call 0x26f8
        s->calls[s->call_depth++] = 5718;
        return 9976;
    }
    case 0x1656: { // movw r16, r18
        uint16_t pair = pm_pointer(s, 18);
        pm_setpointer(s, 16, pair);
        return 5720;
    }
    case 0x1658: { // call 0x26f8
        s->calls[s->call_depth++] = 5724;
        return 9976;
    }
    case 0x165c: { // call 0x281e
        s->calls[s->call_depth++] = 5728;
        return 10270;
    }
    case 0x1660: { // rcall .+28
        s->calls[s->call_depth++] = 5730;
        return 5758;
    }
    case 0x1662: { // rjmp .+2430
        return 8162;
    }
    default: return UINT32_MAX;
    }
}
