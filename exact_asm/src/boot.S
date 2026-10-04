/* Recovered boot code; objdump addresses are bytes. No blob embedding. */
.section .boot,"ax",@progbits
.org 0x1a0, 0xff
.global boot_0201a0
boot_0201a0:
    rjmp	boot_0202a0
.org 0x1dc, 0xff
.global boot_0201dc
boot_0201dc:
    rjmp	boot_02023a
.org 0x1e0, 0xff
.global boot_0201e0
boot_0201e0:
    rjmp	boot_0201e2
.org 0x1e2, 0xff
.global boot_0201e2
boot_0201e2:
    push	r31
.org 0x1e4, 0xff
.global boot_0201e4
boot_0201e4:
    in	r31, 0x3f
.org 0x1e6, 0xff
.global boot_0201e6
boot_0201e6:
    push	r31
.org 0x1e8, 0xff
.global boot_0201e8
boot_0201e8:
    push	r30
.org 0x1ea, 0xff
.global boot_0201ea
boot_0201ea:
    push	r19
.org 0x1ec, 0xff
.global boot_0201ec
boot_0201ec:
    push	r18
.org 0x1ee, 0xff
.global boot_0201ee
boot_0201ee:
    push	r17
.org 0x1f0, 0xff
.global boot_0201f0
boot_0201f0:
    push	r16
.org 0x1f2, 0xff
.global boot_0201f2
boot_0201f2:
    ldi	r30, 0x02
.org 0x1f4, 0xff
.global boot_0201f4
boot_0201f4:
    ldi	r31, 0x20
.org 0x1f6, 0xff
.global boot_0201f6
boot_0201f6:
    ld	r16, Z+
.org 0x1f8, 0xff
.global boot_0201f8
boot_0201f8:
    ld	r17, Z+
.org 0x1fa, 0xff
.global boot_0201fa
boot_0201fa:
    ld	r18, Z
.org 0x1fc, 0xff
.global boot_0201fc
boot_0201fc:
    and	r18, r18
.org 0x1fe, 0xff
.global boot_0201fe
boot_0201fe:
    breq	boot_020204
.org 0x200, 0xff
.global boot_020200
boot_020200:
    cp	r16, r17
.org 0x202, 0xff
.global boot_020202
boot_020202:
    brne	boot_020210
.org 0x204, 0xff
.global boot_020204
boot_020204:
    lds	r19, 0x0BA3
.org 0x208, 0xff
.global boot_020208
boot_020208:
    andi	r19, 0xFC
.org 0x20a, 0xff
.global boot_02020a
boot_02020a:
    sts	0x0BA3, r19
.org 0x20e, 0xff
.global boot_02020e
boot_02020e:
    rjmp	boot_020228
.org 0x210, 0xff
.global boot_020210
boot_020210:
    inc	r16
.org 0x212, 0xff
.global boot_020212
boot_020212:
    eor	r17, r17
.org 0x214, 0xff
.global boot_020214
boot_020214:
    ldi	r30, 0x47
.org 0x216, 0xff
.global boot_020216
boot_020216:
    ldi	r31, 0x20
.org 0x218, 0xff
.global boot_020218
boot_020218:
    add	r30, r16
.org 0x21a, 0xff
.global boot_02021a
boot_02021a:
    adc	r31, r17
.org 0x21c, 0xff
.global boot_02021c
boot_02021c:
    ld	r17, Z
.org 0x21e, 0xff
.global boot_02021e
boot_02021e:
    sts	0x0BA0, r17
.org 0x222, 0xff
.global boot_020222
boot_020222:
    ldi	r30, 0x02
.org 0x224, 0xff
.global boot_020224
boot_020224:
    ldi	r31, 0x20
.org 0x226, 0xff
.global boot_020226
boot_020226:
    st	Z, r16
.org 0x228, 0xff
.global boot_020228
boot_020228:
    pop	r16
.org 0x22a, 0xff
.global boot_02022a
boot_02022a:
    pop	r17
.org 0x22c, 0xff
.global boot_02022c
boot_02022c:
    pop	r18
.org 0x22e, 0xff
.global boot_02022e
boot_02022e:
    pop	r19
.org 0x230, 0xff
.global boot_020230
boot_020230:
    pop	r30
.org 0x232, 0xff
.global boot_020232
boot_020232:
    pop	r31
.org 0x234, 0xff
.global boot_020234
boot_020234:
    out	0x3f, r31
.org 0x236, 0xff
.global boot_020236
boot_020236:
    pop	r31
.org 0x238, 0xff
.global boot_020238
boot_020238:
    reti
.org 0x23a, 0xff
.global boot_02023a
boot_02023a:
    push	r31
.org 0x23c, 0xff
.global boot_02023c
boot_02023c:
    in	r31, 0x3f
.org 0x23e, 0xff
.global boot_02023e
boot_02023e:
    push	r31
.org 0x240, 0xff
.global boot_020240
boot_020240:
    push	r30
.org 0x242, 0xff
.global boot_020242
boot_020242:
    push	r19
.org 0x244, 0xff
.global boot_020244
boot_020244:
    push	r18
.org 0x246, 0xff
.global boot_020246
boot_020246:
    push	r17
.org 0x248, 0xff
.global boot_020248
boot_020248:
    push	r16
.org 0x24a, 0xff
.global boot_02024a
boot_02024a:
    ldi	r30, 0x00
.org 0x24c, 0xff
.global boot_02024c
boot_02024c:
    ldi	r31, 0x20
.org 0x24e, 0xff
.global boot_02024e
boot_02024e:
    ld	r16, Z+
.org 0x250, 0xff
.global boot_020250
boot_020250:
    ld	r17, Z
.org 0x252, 0xff
.global boot_020252
boot_020252:
    inc	r17
.org 0x254, 0xff
.global boot_020254
boot_020254:
    andi	r17, 0x3F
.org 0x256, 0xff
.global boot_020256
boot_020256:
    mov	r18, r17
.org 0x258, 0xff
.global boot_020258
boot_020258:
    subi	r18, 0xEC
.org 0x25a, 0xff
.global boot_02025a
boot_02025a:
    andi	r18, 0x3F
.org 0x25c, 0xff
.global boot_02025c
boot_02025c:
    cp	r18, r16
.org 0x25e, 0xff
.global boot_02025e
boot_02025e:
    brne	boot_020270
.org 0x260, 0xff
.global boot_020260
boot_020260:
    ldi	r18, 0x01
.org 0x262, 0xff
.global boot_020262
boot_020262:
    sts	0x06A5, r18
.org 0x266, 0xff
.global boot_020266
boot_020266:
    lds	r18, 0x2005
.org 0x26a, 0xff
.global boot_02026a
boot_02026a:
    ori	r18, 0x80
.org 0x26c, 0xff
.global boot_02026c
boot_02026c:
    sts	0x2005, r18
.org 0x270, 0xff
.global boot_020270
boot_020270:
    lds	r19, 0x0BA1
.org 0x274, 0xff
.global boot_020274
boot_020274:
    lds	r18, 0x0BA0
.org 0x278, 0xff
.global boot_020278
boot_020278:
    sbrc	r19, 4
.org 0x27a, 0xff
.global boot_02027a
boot_02027a:
    rjmp	boot_02028e
.org 0x27c, 0xff
.global boot_02027c
boot_02027c:
    cp	r16, r17
.org 0x27e, 0xff
.global boot_02027e
boot_02027e:
    breq	boot_02028e
.org 0x280, 0xff
.global boot_020280
boot_020280:
    st	Z, r17
.org 0x282, 0xff
.global boot_020282
boot_020282:
    ldi	r30, 0x07
.org 0x284, 0xff
.global boot_020284
boot_020284:
    ldi	r31, 0x20
.org 0x286, 0xff
.global boot_020286
boot_020286:
    eor	r16, r16
.org 0x288, 0xff
.global boot_020288
boot_020288:
    add	r30, r17
.org 0x28a, 0xff
.global boot_02028a
boot_02028a:
    adc	r31, r16
.org 0x28c, 0xff
.global boot_02028c
boot_02028c:
    st	Z, r18
.org 0x28e, 0xff
.global boot_02028e
boot_02028e:
    pop	r16
.org 0x290, 0xff
.global boot_020290
boot_020290:
    pop	r17
.org 0x292, 0xff
.global boot_020292
boot_020292:
    pop	r18
.org 0x294, 0xff
.global boot_020294
boot_020294:
    pop	r19
.org 0x296, 0xff
.global boot_020296
boot_020296:
    pop	r30
.org 0x298, 0xff
.global boot_020298
boot_020298:
    pop	r31
.org 0x29a, 0xff
.global boot_02029a
boot_02029a:
    out	0x3f, r31
.org 0x29c, 0xff
.global boot_02029c
boot_02029c:
    pop	r31
.org 0x29e, 0xff
.global boot_02029e
boot_02029e:
    reti
.org 0x2a0, 0xff
.global boot_0202a0
boot_0202a0:
    push	r31
.org 0x2a2, 0xff
.global boot_0202a2
boot_0202a2:
    in	r31, 0x3f
.org 0x2a4, 0xff
.global boot_0202a4
boot_0202a4:
    push	r31
.org 0x2a6, 0xff
.global boot_0202a6
boot_0202a6:
    push	r30
.org 0x2a8, 0xff
.global boot_0202a8
boot_0202a8:
    push	r29
.org 0x2aa, 0xff
.global boot_0202aa
boot_0202aa:
    push	r18
.org 0x2ac, 0xff
.global boot_0202ac
boot_0202ac:
    push	r17
.org 0x2ae, 0xff
.global boot_0202ae
boot_0202ae:
    push	r16
.org 0x2b0, 0xff
.global boot_0202b0
boot_0202b0:
    ldi	r30, 0x04
.org 0x2b2, 0xff
.global boot_0202b2
boot_0202b2:
    ldi	r31, 0x20
.org 0x2b4, 0xff
.global boot_0202b4
boot_0202b4:
    eor	r29, r29
.org 0x2b6, 0xff
.global boot_0202b6
boot_0202b6:
    lds	r16, 0x06A8
.org 0x2ba, 0xff
.global boot_0202ba
boot_0202ba:
    sbrc	r16, 1
.org 0x2bc, 0xff
.global boot_0202bc
boot_0202bc:
    rjmp	boot_0202f4
.org 0x2be, 0xff
.global boot_0202be
boot_0202be:
    inc	r29
.org 0x2c0, 0xff
.global boot_0202c0
boot_0202c0:
    ld	r17, -Z
.org 0x2c2, 0xff
.global boot_0202c2
boot_0202c2:
    ld	r16, -Z
.org 0x2c4, 0xff
.global boot_0202c4
boot_0202c4:
    cp	r16, r17
.org 0x2c6, 0xff
.global boot_0202c6
boot_0202c6:
    breq	boot_0202f2
.org 0x2c8, 0xff
.global boot_0202c8
boot_0202c8:
    lds	r18, 0x0BA1
.org 0x2cc, 0xff
.global boot_0202cc
boot_0202cc:
    sbrs	r18, 5
.org 0x2ce, 0xff
.global boot_0202ce
boot_0202ce:
    rjmp	boot_0202e8
.org 0x2d0, 0xff
.global boot_0202d0
boot_0202d0:
    eor	r17, r17
.org 0x2d2, 0xff
.global boot_0202d2
boot_0202d2:
    inc	r16
.org 0x2d4, 0xff
.global boot_0202d4
boot_0202d4:
    ldi	r30, 0x47
.org 0x2d6, 0xff
.global boot_0202d6
boot_0202d6:
    ldi	r31, 0x20
.org 0x2d8, 0xff
.global boot_0202d8
boot_0202d8:
    add	r30, r16
.org 0x2da, 0xff
.global boot_0202da
boot_0202da:
    adc	r31, r17
.org 0x2dc, 0xff
.global boot_0202dc
boot_0202dc:
    ld	r17, Z
.org 0x2de, 0xff
.global boot_0202de
boot_0202de:
    sts	0x0BA0, r17
.org 0x2e2, 0xff
.global boot_0202e2
boot_0202e2:
    ldi	r30, 0x02
.org 0x2e4, 0xff
.global boot_0202e4
boot_0202e4:
    ldi	r31, 0x20
.org 0x2e6, 0xff
.global boot_0202e6
boot_0202e6:
    st	Z, r16
.org 0x2e8, 0xff
.global boot_0202e8
boot_0202e8:
    lds	r18, 0x0BA3
.org 0x2ec, 0xff
.global boot_0202ec
boot_0202ec:
    ori	r18, 0x02
.org 0x2ee, 0xff
.global boot_0202ee
boot_0202ee:
    sts	0x0BA3, r18
.org 0x2f2, 0xff
.global boot_0202f2
boot_0202f2:
    adiw	r30, 0x02
.org 0x2f4, 0xff
.global boot_0202f4
boot_0202f4:
    st	Z, r29
.org 0x2f6, 0xff
.global boot_0202f6
boot_0202f6:
    pop	r16
.org 0x2f8, 0xff
.global boot_0202f8
boot_0202f8:
    pop	r17
.org 0x2fa, 0xff
.global boot_0202fa
boot_0202fa:
    pop	r18
.org 0x2fc, 0xff
.global boot_0202fc
boot_0202fc:
    pop	r29
.org 0x2fe, 0xff
.global boot_0202fe
boot_0202fe:
    pop	r30
.org 0x300, 0xff
.global boot_020300
boot_020300:
    pop	r31
.org 0x302, 0xff
.global boot_020302
boot_020302:
    out	0x3f, r31
.org 0x304, 0xff
.global boot_020304
boot_020304:
    pop	r31
.org 0x306, 0xff
.global boot_020306
boot_020306:
    reti
.org 0x308, 0xff
.global boot_020308
boot_020308:
    sei
.org 0x30a, 0xff
.global boot_02030a
boot_02030a:
    mov	r26, r3
.org 0x30c, 0xff
.global boot_02030c
boot_02030c:
    mov	r27, r4
.org 0x30e, 0xff
.global boot_02030e
boot_02030e:
    mov	r28, r5
.org 0x310, 0xff
.global boot_020310
boot_020310:
    ldi	r16, 0x01
.org 0x312, 0xff
.global boot_020312
boot_020312:
    rcall	boot_02047c
.org 0x314, 0xff
.global boot_020314
boot_020314:
    lds	r6, 0x003B
.org 0x318, 0xff
.global boot_020318
boot_020318:
    sub	r26, r30
.org 0x31a, 0xff
.global boot_02031a
boot_02031a:
    sbc	r27, r31
.org 0x31c, 0xff
.global boot_02031c
boot_02031c:
    sbc	r28, r6
.org 0x31e, 0xff
.global boot_02031e
boot_02031e:
    subi	r26, 0xFF
.org 0x320, 0xff
.global boot_020320
boot_020320:
    sbci	r27, 0xFF
.org 0x322, 0xff
.global boot_020322
boot_020322:
    sbci	r28, 0xFF
.org 0x324, 0xff
.global boot_020324
boot_020324:
    and	r28, r28
.org 0x326, 0xff
.global boot_020326
boot_020326:
    brne	boot_02032c
.org 0x328, 0xff
.global boot_020328
boot_020328:
    cpi	r27, 0x02
.org 0x32a, 0xff
.global boot_02032a
boot_02032a:
    brcs	boot_020330
.org 0x32c, 0xff
.global boot_02032c
boot_02032c:
    eor	r26, r26
.org 0x32e, 0xff
.global boot_02032e
boot_02032e:
    ldi	r27, 0x02
.org 0x330, 0xff
.global boot_020330
boot_020330:
    ldi	r28, 0x35
.org 0x332, 0xff
.global boot_020332
boot_020332:
    ldi	r29, 0x22
.org 0x334, 0xff
.global boot_020334
boot_020334:
    rcall	boot_02042a
.org 0x336, 0xff
.global boot_020336
boot_020336:
    st	Y+, r16
.org 0x338, 0xff
.global boot_020338
boot_020338:
    sbiw	r26, 0x01
.org 0x33a, 0xff
.global boot_02033a
boot_02033a:
    brne	boot_020334
.org 0x33c, 0xff
.global boot_02033c
boot_02033c:
    ldi	r28, 0x35
.org 0x33e, 0xff
.global boot_02033e
boot_02033e:
    ldi	r29, 0x22
.org 0x340, 0xff
.global boot_020340
boot_020340:
    rcall	boot_0203d6
.org 0x342, 0xff
.global boot_020342
boot_020342:
    ldi	r16, 0x23
.org 0x344, 0xff
.global boot_020344
boot_020344:
    sts	0x01CA, r16
.org 0x348, 0xff
.global boot_020348
boot_020348:
    rjmp	boot_020358
.org 0x34a, 0xff
.global boot_02034a
boot_02034a:
    spm	Z+
.org 0x34c, 0xff
.global boot_02034c
boot_02034c:
    lds	r6, 0x003B
.org 0x350, 0xff
.global boot_020350
boot_020350:
    cp	r30, r3
.org 0x352, 0xff
.global boot_020352
boot_020352:
    cpc	r31, r4
.org 0x354, 0xff
.global boot_020354
boot_020354:
    cpc	r6, r5
.org 0x356, 0xff
.global boot_020356
boot_020356:
    brcc	boot_02036a
.org 0x358, 0xff
.global boot_020358
boot_020358:
    ld	r0, Y+
.org 0x35a, 0xff
.global boot_02035a
boot_02035a:
    ld	r1, Y+
.org 0x35c, 0xff
.global boot_02035c
boot_02035c:
    sbrs	r31, 0
.org 0x35e, 0xff
.global boot_02035e
boot_02035e:
    rjmp	boot_02034a
.org 0x360, 0xff
.global boot_020360
boot_020360:
    cpi	r30, 0xFE
.org 0x362, 0xff
.global boot_020362
boot_020362:
    brne	boot_02034a
.org 0x364, 0xff
.global boot_020364
boot_020364:
    spm
.org 0x366, 0xff
.global boot_020366
boot_020366:
    rcall	boot_0203e0
.org 0x368, 0xff
.global boot_020368
boot_020368:
    rjmp	boot_02030a
.org 0x36a, 0xff
.global boot_02036a
boot_02036a:
    rcall	boot_0203e0
.org 0x36c, 0xff
.global boot_02036c
boot_02036c:
    rcall	boot_0203d6
.org 0x36e, 0xff
.global boot_02036e
boot_02036e:
    sts	0x01C0, r20
.org 0x372, 0xff
.global boot_020372
boot_020372:
    sts	0x01C1, r21
.org 0x376, 0xff
.global boot_020376
boot_020376:
    sts	0x01C2, r22
.org 0x37a, 0xff
.global boot_02037a
boot_02037a:
    sts	0x01C4, r3
.org 0x37e, 0xff
.global boot_02037e
boot_02037e:
    sts	0x01C5, r4
.org 0x382, 0xff
.global boot_020382
boot_020382:
    sts	0x01C6, r5
.org 0x386, 0xff
.global boot_020386
boot_020386:
    ldi	r16, 0x3A
.org 0x388, 0xff
.global boot_020388
boot_020388:
    sts	0x01CA, r16
.org 0x38c, 0xff
.global boot_02038c
boot_02038c:
    ldi	r16, 0x01
.org 0x38e, 0xff
.global boot_02038e
boot_02038e:
    ldi	r17, 0xD8
.org 0x390, 0xff
.global boot_020390
boot_020390:
    cli
.org 0x392, 0xff
.global boot_020392
boot_020392:
    sts	0x0034, r17
.org 0x396, 0xff
.global boot_020396
boot_020396:
    sts	0x01CB, r16
.org 0x39a, 0xff
.global boot_02039a
boot_02039a:
    sei
.org 0x39c, 0xff
.global boot_02039c
boot_02039c:
    rcall	boot_0203d6
.org 0x39e, 0xff
.global boot_02039e
boot_02039e:
    lds	r16, 0x01C6
.org 0x3a2, 0xff
.global boot_0203a2
boot_0203a2:
    eor	r17, r17
.org 0x3a4, 0xff
.global boot_0203a4
boot_0203a4:
    rcall	boot_0203f4
.org 0x3a6, 0xff
.global boot_0203a6
boot_0203a6:
    lds	r16, 0x01C4
.org 0x3aa, 0xff
.global boot_0203aa
boot_0203aa:
    lds	r17, 0x01C5
.org 0x3ae, 0xff
.global boot_0203ae
boot_0203ae:
    rcall	boot_0203f4
.org 0x3b0, 0xff
.global boot_0203b0
boot_0203b0:
    ldi	r16, 0x0D
.org 0x3b2, 0xff
.global boot_0203b2
boot_0203b2:
    rcall	boot_02047c
.org 0x3b4, 0xff
.global boot_0203b4
boot_0203b4:
    ldi	r16, 0x0A
.org 0x3b6, 0xff
.global boot_0203b6
boot_0203b6:
    rcall	boot_02047c
.org 0x3b8, 0xff
.global boot_0203b8
boot_0203b8:
    lds	r19, 0x0BA3
.org 0x3bc, 0xff
.global boot_0203bc
boot_0203bc:
    andi	r19, 0x03
.org 0x3be, 0xff
.global boot_0203be
boot_0203be:
    brne	boot_0203b8
.org 0x3c0, 0xff
.global boot_0203c0
boot_0203c0:
    cli
.org 0x3c2, 0xff
.global boot_0203c2
boot_0203c2:
    ldi	r16, 0x01
.org 0x3c4, 0xff
.global boot_0203c4
boot_0203c4:
    sts	0x0048, r16
.org 0x3c8, 0xff
.global boot_0203c8
boot_0203c8:
    ldi	r16, 0x01
.org 0x3ca, 0xff
.global boot_0203ca
boot_0203ca:
    ldi	r17, 0xD8
.org 0x3cc, 0xff
.global boot_0203cc
boot_0203cc:
    sts	0x0034, r17
.org 0x3d0, 0xff
.global boot_0203d0
boot_0203d0:
    sts	0x0079, r16
.org 0x3d4, 0xff
.global boot_0203d4
boot_0203d4:
    sleep
.org 0x3d6, 0xff
.global boot_0203d6
boot_0203d6:
    lds	r16, 0x01CF
.org 0x3da, 0xff
.global boot_0203da
boot_0203da:
    sbrc	r16, 7
.org 0x3dc, 0xff
.global boot_0203dc
boot_0203dc:
    rjmp	boot_0203d6
.org 0x3de, 0xff
.global boot_0203de
boot_0203de:
    ret
.org 0x3e0, 0xff
.global boot_0203e0
boot_0203e0:
    ldi	r16, 0x25
.org 0x3e2, 0xff
.global boot_0203e2
boot_0203e2:
    sts	0x01CA, r16
.org 0x3e6, 0xff
.global boot_0203e6
boot_0203e6:
    ldi	r16, 0x9D
.org 0x3e8, 0xff
.global boot_0203e8
boot_0203e8:
    cli
.org 0x3ea, 0xff
.global boot_0203ea
boot_0203ea:
    sts	0x0034, r16
.org 0x3ee, 0xff
.global boot_0203ee
boot_0203ee:
    spm	Z+
.org 0x3f0, 0xff
.global boot_0203f0
boot_0203f0:
    sei
.org 0x3f2, 0xff
.global boot_0203f2
boot_0203f2:
    ret
.org 0x3f4, 0xff
.global boot_0203f4
boot_0203f4:
    push	r16
.org 0x3f6, 0xff
.global boot_0203f6
boot_0203f6:
    push	r17
.org 0x3f8, 0xff
.global boot_0203f8
boot_0203f8:
    push	r18
.org 0x3fa, 0xff
.global boot_0203fa
boot_0203fa:
    mov	r18, r16
.org 0x3fc, 0xff
.global boot_0203fc
boot_0203fc:
    swap	r17
.org 0x3fe, 0xff
.global boot_0203fe
boot_0203fe:
    mov	r16, r17
.org 0x400, 0xff
.global boot_020400
boot_020400:
    rcall	boot_02041c
.org 0x402, 0xff
.global boot_020402
boot_020402:
    swap	r17
.org 0x404, 0xff
.global boot_020404
boot_020404:
    mov	r16, r17
.org 0x406, 0xff
.global boot_020406
boot_020406:
    rcall	boot_02041c
.org 0x408, 0xff
.global boot_020408
boot_020408:
    swap	r18
.org 0x40a, 0xff
.global boot_02040a
boot_02040a:
    mov	r16, r18
.org 0x40c, 0xff
.global boot_02040c
boot_02040c:
    rcall	boot_02041c
.org 0x40e, 0xff
.global boot_02040e
boot_02040e:
    swap	r18
.org 0x410, 0xff
.global boot_020410
boot_020410:
    mov	r16, r18
.org 0x412, 0xff
.global boot_020412
boot_020412:
    rcall	boot_02041c
.org 0x414, 0xff
.global boot_020414
boot_020414:
    pop	r18
.org 0x416, 0xff
.global boot_020416
boot_020416:
    pop	r17
.org 0x418, 0xff
.global boot_020418
boot_020418:
    pop	r16
.org 0x41a, 0xff
.global boot_02041a
boot_02041a:
    ret
.org 0x41c, 0xff
.global boot_02041c
boot_02041c:
    andi	r16, 0x0F
.org 0x41e, 0xff
.global boot_02041e
boot_02041e:
    subi	r16, 0xD0
.org 0x420, 0xff
.global boot_020420
boot_020420:
    cpi	r16, 0x3A
.org 0x422, 0xff
.global boot_020422
boot_020422:
    brcs	boot_020426
.org 0x424, 0xff
.global boot_020424
boot_020424:
    subi	r16, 0xF9
.org 0x426, 0xff
.global boot_020426
boot_020426:
    rcall	boot_02047c
.org 0x428, 0xff
.global boot_020428
boot_020428:
    ret
.org 0x42a, 0xff
.global boot_02042a
boot_02042a:
    push	r17
.org 0x42c, 0xff
.global boot_02042c
boot_02042c:
    push	r18
.org 0x42e, 0xff
.global boot_02042e
boot_02042e:
    push	r20
.org 0x430, 0xff
.global boot_020430
boot_020430:
    push	r30
.org 0x432, 0xff
.global boot_020432
boot_020432:
    push	r31
.org 0x434, 0xff
.global boot_020434
boot_020434:
    ldi	r30, 0x00
.org 0x436, 0xff
.global boot_020436
boot_020436:
    ldi	r31, 0x20
.org 0x438, 0xff
.global boot_020438
boot_020438:
    cli
.org 0x43a, 0xff
.global boot_02043a
boot_02043a:
    ld	r17, Z
.org 0x43c, 0xff
.global boot_02043c
boot_02043c:
    ldd	r16, Z+1
.org 0x43e, 0xff
.global boot_02043e
boot_02043e:
    sei
.org 0x440, 0xff
.global boot_020440
boot_020440:
    cp	r16, r17
.org 0x442, 0xff
.global boot_020442
boot_020442:
    breq	boot_020438
.org 0x444, 0xff
.global boot_020444
boot_020444:
    inc	r17
.org 0x446, 0xff
.global boot_020446
boot_020446:
    andi	r17, 0x3F
.org 0x448, 0xff
.global boot_020448
boot_020448:
    lds	r20, 0x06A4
.org 0x44c, 0xff
.global boot_02044c
boot_02044c:
    sbrs	r20, 0
.org 0x44e, 0xff
.global boot_02044e
boot_02044e:
    rjmp	boot_02045e
.org 0x450, 0xff
.global boot_020450
boot_020450:
    mov	r18, r17
.org 0x452, 0xff
.global boot_020452
boot_020452:
    subi	r18, 0xEC
.org 0x454, 0xff
.global boot_020454
boot_020454:
    andi	r18, 0x3F
.org 0x456, 0xff
.global boot_020456
boot_020456:
    cpse	r16, r18
.org 0x458, 0xff
.global boot_020458
boot_020458:
    ldi	r20, 0x01
.org 0x45a, 0xff
.global boot_02045a
boot_02045a:
    sts	0x06A6, r20
.org 0x45e, 0xff
.global boot_02045e
boot_02045e:
    eor	r18, r18
.org 0x460, 0xff
.global boot_020460
boot_020460:
    ldi	r30, 0x07
.org 0x462, 0xff
.global boot_020462
boot_020462:
    ldi	r31, 0x20
.org 0x464, 0xff
.global boot_020464
boot_020464:
    add	r30, r17
.org 0x466, 0xff
.global boot_020466
boot_020466:
    adc	r31, r18
.org 0x468, 0xff
.global boot_020468
boot_020468:
    ld	r18, Z
.org 0x46a, 0xff
.global boot_02046a
boot_02046a:
    sts	0x2000, r17
.org 0x46e, 0xff
.global boot_02046e
boot_02046e:
    mov	r16, r18
.org 0x470, 0xff
.global boot_020470
boot_020470:
    pop	r31
.org 0x472, 0xff
.global boot_020472
boot_020472:
    pop	r30
.org 0x474, 0xff
.global boot_020474
boot_020474:
    pop	r20
.org 0x476, 0xff
.global boot_020476
boot_020476:
    pop	r18
.org 0x478, 0xff
.global boot_020478
boot_020478:
    pop	r17
.org 0x47a, 0xff
.global boot_02047a
boot_02047a:
    ret
.org 0x47c, 0xff
.global boot_02047c
boot_02047c:
    push	r31
.org 0x47e, 0xff
.global boot_02047e
boot_02047e:
    push	r30
.org 0x480, 0xff
.global boot_020480
boot_020480:
    push	r20
.org 0x482, 0xff
.global boot_020482
boot_020482:
    push	r19
.org 0x484, 0xff
.global boot_020484
boot_020484:
    push	r18
.org 0x486, 0xff
.global boot_020486
boot_020486:
    push	r17
.org 0x488, 0xff
.global boot_020488
boot_020488:
    ldi	r30, 0x02
.org 0x48a, 0xff
.global boot_02048a
boot_02048a:
    ldi	r31, 0x20
.org 0x48c, 0xff
.global boot_02048c
boot_02048c:
    cli
.org 0x48e, 0xff
.global boot_02048e
boot_02048e:
    ld	r17, Z+
.org 0x490, 0xff
.global boot_020490
boot_020490:
    ld	r18, Z+
.org 0x492, 0xff
.global boot_020492
boot_020492:
    cp	r17, r18
.org 0x494, 0xff
.global boot_020494
boot_020494:
    breq	boot_0204b8
.org 0x496, 0xff
.global boot_020496
boot_020496:
    inc	r18
.org 0x498, 0xff
.global boot_020498
boot_020498:
    cp	r17, r18
.org 0x49a, 0xff
.global boot_02049a
boot_02049a:
    brne	boot_0204a2
.org 0x49c, 0xff
.global boot_02049c
boot_02049c:
    sei
.org 0x49e, 0xff
.global boot_02049e
boot_02049e:
    sbiw	r30, 0x02
.org 0x4a0, 0xff
.global boot_0204a0
boot_0204a0:
    rjmp	boot_02048c
.org 0x4a2, 0xff
.global boot_0204a2
boot_0204a2:
    ldi	r30, 0x47
.org 0x4a4, 0xff
.global boot_0204a4
boot_0204a4:
    ldi	r31, 0x20
.org 0x4a6, 0xff
.global boot_0204a6
boot_0204a6:
    eor	r17, r17
.org 0x4a8, 0xff
.global boot_0204a8
boot_0204a8:
    add	r30, r18
.org 0x4aa, 0xff
.global boot_0204aa
boot_0204aa:
    adc	r31, r17
.org 0x4ac, 0xff
.global boot_0204ac
boot_0204ac:
    st	Z, r16
.org 0x4ae, 0xff
.global boot_0204ae
boot_0204ae:
    ldi	r30, 0x03
.org 0x4b0, 0xff
.global boot_0204b0
boot_0204b0:
    ldi	r31, 0x20
.org 0x4b2, 0xff
.global boot_0204b2
boot_0204b2:
    st	Z, r18
.org 0x4b4, 0xff
.global boot_0204b4
boot_0204b4:
    sei
.org 0x4b6, 0xff
.global boot_0204b6
boot_0204b6:
    rjmp	boot_0204d8
.org 0x4b8, 0xff
.global boot_0204b8
boot_0204b8:
    inc	r18
.org 0x4ba, 0xff
.global boot_0204ba
boot_0204ba:
    ld	r19, Z
.org 0x4bc, 0xff
.global boot_0204bc
boot_0204bc:
    and	r19, r19
.org 0x4be, 0xff
.global boot_0204be
boot_0204be:
    breq	boot_0204a2
.org 0x4c0, 0xff
.global boot_0204c0
boot_0204c0:
    lds	r20, 0x0BA1
.org 0x4c4, 0xff
.global boot_0204c4
boot_0204c4:
    sbrs	r20, 5
.org 0x4c6, 0xff
.global boot_0204c6
boot_0204c6:
    rjmp	boot_0204a2
.org 0x4c8, 0xff
.global boot_0204c8
boot_0204c8:
    sts	0x0BA0, r16
.org 0x4cc, 0xff
.global boot_0204cc
boot_0204cc:
    lds	r20, 0x0BA3
.org 0x4d0, 0xff
.global boot_0204d0
boot_0204d0:
    ori	r20, 0x02
.org 0x4d2, 0xff
.global boot_0204d2
boot_0204d2:
    sts	0x0BA3, r20
.org 0x4d6, 0xff
.global boot_0204d6
boot_0204d6:
    sei
.org 0x4d8, 0xff
.global boot_0204d8
boot_0204d8:
    pop	r17
.org 0x4da, 0xff
.global boot_0204da
boot_0204da:
    pop	r18
.org 0x4dc, 0xff
.global boot_0204dc
boot_0204dc:
    pop	r19
.org 0x4de, 0xff
.global boot_0204de
boot_0204de:
    pop	r20
.org 0x4e0, 0xff
.global boot_0204e0
boot_0204e0:
    pop	r30
.org 0x4e2, 0xff
.global boot_0204e2
boot_0204e2:
    pop	r31
.org 0x4e4, 0xff
.global boot_0204e4
boot_0204e4:
    ret
