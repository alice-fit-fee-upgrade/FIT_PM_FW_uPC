uint32_t pm_logical_adt16(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x25be: { // push r16
        s->stack[s->depth++] = s->r[16];
        return 9664;
    }
    case 0x25c0: { // push r17
        s->stack[s->depth++] = s->r[17];
        return 9666;
    }
    case 0x25c2: { // push r18
        s->stack[s->depth++] = s->r[18];
        return 9668;
    }
    case 0x25c4: { // ldi r20, 0x10
        s->r[20] = 16;
        return 9670;
    }
    case 0x25c6: { // sts 0x0606, r20
        uint16_t address = 1542;
        pm_write(s, address, s->r[20]);
        return 9674;
    }
    case 0x25ca: { // mov r20, r17
        s->r[20] = s->r[17];
        return 9676;
    }
    case 0x25cc: { // mov r21, r18
        s->r[21] = s->r[18];
        return 9678;
    }
    case 0x25ce: { // rcall .+56
        s->calls[s->call_depth++] = 9680;
        return 9736;
    }
    case 0x25d0: { // mov r16, r21
        s->r[16] = s->r[21];
        return 9682;
    }
    case 0x25d2: { // rcall .+52
        s->calls[s->call_depth++] = 9684;
        return 9736;
    }
    case 0x25d4: { // mov r21, r16
        s->r[21] = s->r[16];
        return 9686;
    }
    case 0x25d6: { // mov r16, r20
        s->r[16] = s->r[20];
        return 9688;
    }
    case 0x25d8: { // rcall .+46
        s->calls[s->call_depth++] = 9690;
        return 9736;
    }
    case 0x25da: { // mov r20, r16
        s->r[20] = s->r[16];
        return 9692;
    }
    case 0x25dc: { // ldi r18, 0x10
        s->r[18] = 16;
        return 9694;
    }
    case 0x25de: { // sts 0x0605, r18
        uint16_t address = 1541;
        pm_write(s, address, s->r[18]);
        return 9698;
    }
    case 0x25e2: { // pop r18
        s->r[18] = s->stack[--s->depth];
        return 9700;
    }
    case 0x25e4: { // pop r17
        s->r[17] = s->stack[--s->depth];
        return 9702;
    }
    case 0x25e6: { // pop r16
        s->r[16] = s->stack[--s->depth];
        return 9704;
    }
    case 0x25e8: { // ret
        return s->calls[--s->call_depth];
    }
    default: return UINT32_MAX;
    }
}
