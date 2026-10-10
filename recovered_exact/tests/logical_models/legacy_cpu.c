uint32_t pm_logical_legacy_cpu(PMLogical *s, uint32_t pc)
{
    switch (pc) {
    case 0x0a56: { // bst r16, 3
        pm_flag(s, TRANSFER, s->r[16] & (1u << 3));
        return 2648;
    }
    case 0x0a5e: { // bld r16, 4
        s->r[16] = (s->r[16] & ~(1u << 4)) | (pm_getflag(s, TRANSFER) << 4);
        return 2656;
    }
    case 0x0af0: { // bst r16, 6
        pm_flag(s, TRANSFER, s->r[16] & (1u << 6));
        return 2802;
    }
    case 0x0b12: { // bld r19, 0
        s->r[19] = (s->r[19] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 2836;
    }
    case 0x0ba8: { // bst r16, 1
        pm_flag(s, TRANSFER, s->r[16] & (1u << 1));
        return 2986;
    }
    case 0x0baa: { // bld r17, 0
        s->r[17] = (s->r[17] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 2988;
    }
    case 0x0bae: { // bld r17, 3
        s->r[17] = (s->r[17] & ~(1u << 3)) | (pm_getflag(s, TRANSFER) << 3);
        return 2992;
    }
    case 0x0bb0: { // bst r16, 2
        pm_flag(s, TRANSFER, s->r[16] & (1u << 2));
        return 2994;
    }
    case 0x167e: { // cli
        pm_irq(s, false);
        return 5760;
    }
    case 0x16ae: { // sei
        pm_irq(s, true);
        return 5808;
    }
    case 0x2620: { // nop
        return 9762;
    }
    case 0x262a: { // bst r17, 2
        pm_flag(s, TRANSFER, s->r[17] & (1u << 2));
        return 9772;
    }
    case 0x262c: { // bld r16, 0
        s->r[16] = (s->r[16] & ~(1u << 0)) | (pm_getflag(s, TRANSFER) << 0);
        return 9774;
    }
    default: return UINT32_MAX;
    }
}
