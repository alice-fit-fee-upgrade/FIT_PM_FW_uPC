uint32_t pm_logical_tdc_host_read(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x2208: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 8714;
    }
    case 0x220a: { // push r19
        s->stack[s->depth++] = s->r[19];
        return 8716;
    }
    case 0x220c: { // push r20
        s->stack[s->depth++] = s->r[20];
        return 8718;
    }
    case 0x220e: { // push r21
        s->stack[s->depth++] = s->r[21];
        return 8720;
    }
    case 0x2210: { // push r22
        s->stack[s->depth++] = s->r[22];
        return 8722;
    }
    case 0x2212: { // cpi r19, 0x03
        pm_sub(s, s->r[19], 3, 0, false);
        return 8724;
    }
    case 0x2214: { // brcs .+2
        return (pm_getflag(s, 0) == 1) ? 8728 : 8726;
    }
    case 0x2216: { // rjmp .+134
        return 8862;
    }
    case 0x2218: { // ldi r22, 0x04
        s->r[22] = 4;
        return 8730;
    }
    case 0x221a: { // dec r19
        s->r[19]--;
        pm_nzv(s, s->r[19], s->r[19] == 127);
        return 8732;
    }
    case 0x221c: { // brmi .+4
        return (pm_getflag(s, 2) == 1) ? 8738 : 8734;
    }
    case 0x221e: { // add r22, r22
        s->r[22] = pm_add(s, s->r[22], s->r[22], 0);
        return 8736;
    }
    case 0x2220: { // rjmp .-8
        return 8730;
    }
    case 0x2222: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8742;
    }
    case 0x2226: { // ldi r22, 0x01
        s->r[22] = 1;
        return 8744;
    }
    case 0x2228: { // ldi r20, 0x02
        s->r[20] = 2;
        return 8746;
    }
    case 0x222a: { // sts 0x0625, r20
        uint16_t address = 1573;
        pm_write(s, address, s->r[20]);
        return 8750;
    }
    case 0x222e: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8754;
    }
    case 0x2232: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8758;
    }
    case 0x2236: { // sts 0x0626, r20
        uint16_t address = 1574;
        pm_write(s, address, s->r[20]);
        return 8762;
    }
    case 0x223a: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8766;
    }
    case 0x223e: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8770;
    }
    case 0x2242: { // ldi r21, 0x08
        s->r[21] = 8;
        return 8772;
    }
    case 0x2244: { // sbrs r16, 7
        return (!!(s->r[16] & (1u << 7)) == 1) ? 8778 : 8774;
    }
    case 0x2246: { // sts 0x0626, r20
        uint16_t address = 1574;
        pm_write(s, address, s->r[20]);
        return 8778;
    }
    case 0x224a: { // sbrc r16, 7
        return (!!(s->r[16] & (1u << 7)) == 0) ? 8784 : 8780;
    }
    case 0x224c: { // sts 0x0625, r20
        uint16_t address = 1573;
        pm_write(s, address, s->r[20]);
        return 8784;
    }
    case 0x2250: { // adc r16, r16
        s->r[16] = pm_add(s, s->r[16], s->r[16], pm_getflag(s, CARRY));
        return 8786;
    }
    case 0x2252: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8790;
    }
    case 0x2256: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8794;
    }
    case 0x225a: { // dec r21
        s->r[21]--;
        pm_nzv(s, s->r[21], s->r[21] == 127);
        return 8796;
    }
    case 0x225c: { // brne .-26
        return (pm_getflag(s, 1) == 0) ? 8772 : 8798;
    }
    case 0x225e: { // sts 0x0622, r20
        uint16_t address = 1570;
        pm_write(s, address, s->r[20]);
        return 8802;
    }
    case 0x2262: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8806;
    }
    case 0x2266: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8810;
    }
    case 0x226a: { // ldi r21, 0x10
        s->r[21] = 16;
        return 8812;
    }
    case 0x226c: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8816;
    }
    case 0x2270: { // add r17, r17
        s->r[17] = pm_add(s, s->r[17], s->r[17], 0);
        return 8818;
    }
    case 0x2272: { // adc r18, r18
        s->r[18] = pm_add(s, s->r[18], s->r[18], pm_getflag(s, CARRY));
        return 8820;
    }
    case 0x2274: { // lds r16, 0x0628
        uint16_t address = 1576;
        s->r[16] = pm_read(s, address);
        return 8824;
    }
    case 0x2278: { // sbrc r16, 1
        return (!!(s->r[16] & (1u << 1)) == 0) ? 8828 : 8826;
    }
    case 0x227a: { // ori r17, 0x01
        s->r[17] |= 1;
        pm_nzv(s, s->r[17], false);
        return 8828;
    }
    case 0x227c: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8832;
    }
    case 0x2280: { // dec r21
        s->r[21]--;
        pm_nzv(s, s->r[21], s->r[21] == 127);
        return 8834;
    }
    case 0x2282: { // brne .-24
        return (pm_getflag(s, 1) == 0) ? 8812 : 8836;
    }
    case 0x2284: { // ldi r21, 0x1C
        s->r[21] = 28;
        return 8838;
    }
    case 0x2286: { // sts 0x0625, r21
        uint16_t address = 1573;
        pm_write(s, address, s->r[21]);
        return 8842;
    }
    case 0x228a: { // sts 0x0621, r20
        uint16_t address = 1569;
        pm_write(s, address, s->r[20]);
        return 8846;
    }
    case 0x228e: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8850;
    }
    case 0x2292: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8854;
    }
    case 0x2296: { // sts 0x0625, r22
        uint16_t address = 1573;
        pm_write(s, address, s->r[22]);
        return 8858;
    }
    case 0x229a: { // sts 0x0626, r22
        uint16_t address = 1574;
        pm_write(s, address, s->r[22]);
        return 8862;
    }
    case 0x229e: { // pop r22
        s->r[22] = s->stack[--s->depth];
        return 8864;
    }
    case 0x22a0: { // pop r21
        s->r[21] = s->stack[--s->depth];
        return 8866;
    }
    case 0x22a2: { // pop r20
        s->r[20] = s->stack[--s->depth];
        return 8868;
    }
    case 0x22a4: { // pop r19
        s->r[19] = s->stack[--s->depth];
        return 8870;
    }
    case 0x22a6: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 8872;
    }
    case 0x22a8: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
