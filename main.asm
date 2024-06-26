;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined BOOT(void)
    rjmp        main                                    
OFF_code_000001:              
    align       align(26)                               
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined4 DMA_CH1(undefined2 param_1, undefined2 param_2)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
    rjmp        FUN_code_0000f1                         
OFF_code_00000f:              
    align       align(26)                               
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;ulonglong TCC0_OVF(undefined2 param_1, undefined2 param_2, uint param_3, undefined2 param_4, undefined2 param_5)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
          ;param_3       uint            R21R20                      
          ;param_4       undefined2      R19R18                      
          ;param_5       undefined2      R17R16                      
    rjmp        FUN_code_00024f                         
OFF_code_00001d:              
    align       align(78)                               
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined PORTB_INT0(void)
    rjmp        FUN_code_00055e                         
OFF_code_000045:              
    align       align(34)                               
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined PORTE_INT0(void)
    rjmp        FUN_code_0005cb                         
    align       align(2)                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined PORTE_INT1(undefined2 param_1, undefined2 param_2, undefined2 param_3, undefined2 param_4, uint param_5)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
          ;param_3       undefined2      R21R20                      
          ;param_4       undefined2      R19R18                      
          ;param_5       uint            R17R16                      
    rjmp        FUN_code_000615                         
OFF_code_000059:              
    align       align(78)                               
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined PORTD_INT0(void)
    rjmp        FUN_code_000526                         
OFF_code_000081:              
    align       align(158)                              
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined PORTF_INT0(void)
    rjmp        FUN_code_000710                         
    align       align(2)                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined PORTF_INT1(void)
    rjmp        FUN_code_000570                         
OFF_code_0000d3:              
    align       align(54)                               
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined USARTF0_RXC(void)
    rjmp        FUN_code_000770                         
    align       align(2)                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined USARTF0_DRE(void)
    rjmp        FUN_code_000744                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined4 FUN_code_0000f1(undefined2 param_1, undefined2 param_2)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
    push        Zhi                                     
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined4 USARTF0_TXC(undefined2 param_1, undefined2 param_2)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
    in          Zhi,SREG                                
    push        R16                                     
    push        R17                                     
    push        R18                                     
    push        Ylo                                     
    push        Yhi                                     
    push        Zlo                                     
    push        Zhi                                     
    ldi         R17,0xa                                 
LAB_code_0000fb:              
    lds         R16,iospace:PORTD_IN                    ;= ??
    sbrc        R16,0x4                                 
    rjmp        LAB_code_000101                         
    dec         R17                                     
    brbc        LAB_code_0000fb,Zflg                    
LAB_code_000101:              
    lds         R17,iospace:DMA_CH1_TRFCNT              ;= ??
    ldi         R16,0x11                                
    sts         iospace:DMA_CH1_CTRLB,R16               ;= ??
    ldi         Zlo,0x76                                
    ldi         Zhi,0x2b                                
    ldi         Ylo,0x39                                
    ldi         Yhi,0x24                                
    ldi         R16,0x8                                 
    sub         R16,R17                                 
    eor         R17,R17                                 
    add         Ylo,R16                                 
    adc         Yhi,R17                                 
    ld          R16,Y+                                  
    cpi         R16,0xaa                                
    brbs        LAB_code_000116,Zflg                    
LAB_code_000112:              
    lpm         R17,Z+                                  
    cp          R16,R17                                 
    brbs        LAB_code_000130,Zflg                    
    rjmp        LAB_code_00022c                         
LAB_code_000116:              
    cpi         R17,0x7                                 
    brbs        LAB_code_000120,Zflg                    
    inc         R17                                     
    cpi         Ylo,0x41                                
    brbc        LAB_code_00011c,Zflg                    
    sbiw        Y,0x8                                   
LAB_code_00011c:              
    ld          R16,Y+                                  
    cpi         R16,0xaa                                
    brbs        LAB_code_000116,Zflg                    
    rjmp        LAB_code_00022c                         
LAB_code_000120:              
    cli                                                 
    rcall       FUN_code_0005b4                         ;undefined FUN_code_0005b4(void)
    bset        Iflg                                    
    ldi         R16,0x3                                 
    sts         sram:DAT_mem_2441,R16                   ;= ??
    pop         Zhi                                     
    pop         Zlo                                     
    pop         Yhi                                     
    pop         Ylo                                     
    pop         R18                                     
    pop         R17                                     
    pop         R16                                     
    out         SREG,Zhi                                
    pop         Zhi                                     
    rjmp        FUN_code_000570                         ;undefined FUN_code_000570(void)
LAB_code_000130:              
    cpi         Ylo,0x41                                
    brbc        LAB_code_000133,Zflg                    
    sbiw        Y,0x8                                   
LAB_code_000133:              
    ld          R16,Y+                                  
    cpi         Zlo,0x7e                                
    brbc        LAB_code_000112,Zflg                    
    eor         R16,R16                                 
    sts         iospace:DMA_CH1_CTRLA,R16               ;= ??
    bset        Iflg                                    
    push        R0                                      
    push        R1                                      
    push        R2                                      
    push        R10                                     
    push        R19                                     
    push        R20                                     
    push        R21                                     
    push        param_2                                 
    push        param_2                                 
    push        param_1                                 
    push        param_1                                 
    push        Xlo                                     
    push        Xhi                                     
    call        FUN_code_000b32                         ;undefined FUN_code_000b32(void)
    call        FUN_code_000c20                         ;undefined FUN_code_000c20(void)
    ldi         Ylo,0x39                                
    ldi         Yhi,0x24                                
    sts         iospace:DMA_CH0_SRCADDR0,Ylo            ;= ??
    sts         iospace:DMA_CH0_SRCADDR1,Yhi            ;= ??
    eor         R20,R20                                 
    sts         iospace:DMA_CH0_SRCADDR2,R20            ;= ??
    st          Y+,R18                                  
    st          Y+,R19                                  
    st          Y+,R16                                  
    st          Y,R20                                   
    ldi         R16,0x4                                 
    sts         iospace:DMA_CH0_TRFCNT,R16              ;= ??
    eor         R16,R16                                 
    sts         iospace:DAT_mem_0115,R16                ;= ??
    ldi         R16,0x94                                
    sts         iospace:DMA_CH0_CTRLA,R16               ;= ??
LAB_code_000161:              
    lds         R16,iospace:DMA_CH0_CTRLB               ;= ??
    sbrs        R16,0x4                                 
    rjmp        LAB_code_000161                         
    ldi         R16,0x10                                
    sts         iospace:DMA_CH0_CTRLB,R16               ;= ??
    ldi         Xlo,0x39                                
    ldi         Xhi,0x24                                
    sts         iospace:DMA_CH1_DESTADDR0,Xlo           ;= ??
    sts         iospace:DMA_CH1_DESTADDR1,Xhi           ;= ??
    eor         R20,R20                                 
    sts         iospace:DMA_CH1_DESTADDR2,R20           ;= ??
    ldi         R16,0x8                                 
    sts         iospace:DMA_CH1_TRFCNT,R16              ;= ??
    eor         R16,R16                                 
    sts         iospace:DAT_mem_0125,R16                ;= ??
LAB_code_000177:              
    lds         R16,iospace:PORTD_IN                    ;= ??
    sbrs        R16,0x4                                 
    rjmp        LAB_code_000177                         
    lds         R16,iospace:SPID_STATUS                 ;= ??
    lds         R16,iospace:SPID_DATA                   ;= ??
    ldi         R16,0x84                                
    sts         iospace:DMA_CH1_CTRLA,R16               ;= ??
LAB_code_000182:              
    lds         R16,iospace:DMA_CH1_CTRLB               ;= ??
    sbrs        R16,0x4                                 
    rjmp        LAB_code_000182                         
    ldi         R16,0x10                                
    sts         iospace:DMA_CH1_CTRLB,R16               ;= ??
    ld          Ylo,X+                                  
    ld          Yhi,X+                                  
    ld          Zlo,X+                                  
    adiw        X,0x1                                   
    ld          R0,X+                                   
    ld          R1,X+                                   
    ld          R2,X                                    
    movw        R21R20,Y                                
    mov         param_2,Zlo                             
    ldi         Xlo,0x39                                
    ldi         Xhi,0x24                                
    sts         iospace:DMA_CH0_SRCADDR0,Xlo            ;= ??
    sts         iospace:DMA_CH0_SRCADDR1,Xhi            ;= ??
    eor         R16,R16                                 
    st          X+,R16                                  
    st          X+,R16                                  
    st          X+,R16                                  
    st          X,R16                                   
    sts         iospace:DMA_CH0_SRCADDR2,R16            ;= ??
    ldi         R16,0x4                                 
    sts         iospace:DMA_CH0_TRFCNT,R16              ;= ??
    eor         R16,R16                                 
    sts         iospace:DAT_mem_0115,R16                ;= ??
    sts         iospace:DMA_CH0_REPCNT,R16              ;= ??
LAB_code_0001a7:              
    lds         R16,iospace:PORTD_IN                    ;= ??
    sbrs        R16,0x4                                 
    rjmp        LAB_code_0001a7                         
    ldi         R16,0xb4                                
    sts         iospace:DMA_CH0_CTRLA,R16               ;= ??
    eor         R16,R16                                 
    ldi         Xlo,0x35                                
    ldi         Xhi,0x24                                
    st          X+,R16                                  
    st          X+,R16                                  
    st          X+,R16                                  
    st          X,R16                                   
LAB_code_0001b5:              
    call        FUN_code_000b88                         ;undefined FUN_code_000b88(void)
LAB_code_0001b7:              
    movw        R17R16,R1R0                             
    mov         R18,R2                                  
    sub         R16,R20                                 
    sbc         R17,R21                                 
    sbc         R18,param_2                             
    subi        R16,0xff                                
    sbci        R17,0xff                                12
    sbci        R18,0xff                                
    and         R18,R18                                 
    brbc        LAB_code_0001c7,Zflg                    
    ldi         R18,0x1                                 
    cp          R16,R18                                 
    cpc         R17,R18                                 
    brbc        LAB_code_0001c7,Cflg                    
    ldi         Zhi,0x1                                 
    rjmp        LAB_code_0001ca                         
LAB_code_0001c7:              
    eor         R16,R16                                 
    ldi         R17,0x1                                 
    eor         Zhi,Zhi                                 
LAB_code_0001ca:              
    ldi         Xlo,0x35                                
    ldi         Xhi,0x22                                
    lds         R18,sram:DAT_mem_2435                   ;= ??
    add         Xhi,R18                                 
    ldi         R19,0x1                                 
    eor         R18,R19                                 
    sts         sram:DAT_mem_2435,R18                   ;= ??
    sts         iospace:DMA_CH1_DESTADDR0,Xlo           ;= ??
    sts         iospace:DMA_CH1_DESTADDR1,Xhi           ;= ??
    eor         R20,R20                                 
    sts         iospace:DMA_CH1_DESTADDR2,R20           ;= ??
    sts         iospace:DMA_CH1_TRFCNT,R16              ;= ??
    sts         iospace:DAT_mem_0125,R17                ;= ??
    rcall       FUN_code_000236                         ;undefined FUN_code_000236(void)
    ldi         R16,0x84                                
    sts         iospace:DMA_CH1_CTRLA,R16               ;= ??
LAB_code_0001e2:              
    lds         R16,iospace:DMA_CH1_CTRLB               ;= ??
    sbrs        R16,0x4                                 
    rjmp        LAB_code_0001e2                         
    ldi         R16,0x10                                
    sts         iospace:DMA_CH1_CTRLB,R16               ;= ??
    and         Zhi,Zhi                                 
    brbs        LAB_code_000204,Zflg                    
LAB_code_0001eb:              
    lds         R18,iospace:PORTD_IN                    ;= ??
    sbrs        R18,0x4                                 
    rjmp        LAB_code_0001eb                         
    eor         R16,R16                                 
    sts         iospace:DMA_CH0_CTRLA,R16               ;= ??
    ldi         Xlo,0x39                                
    ldi         Xhi,0x24                                
    sts         iospace:DMA_CH0_SRCADDR0,Xlo            ;= ??
    sts         iospace:DMA_CH0_SRCADDR1,Xhi            ;= ??
    eor         R16,R16                                 
    sts         iospace:DMA_CH0_SRCADDR2,R16            ;= ??
    ldi         R16,0x8                                 
    sts         iospace:DMA_CH0_TRFCNT,R16              ;= ??
    eor         R16,R16                                 
    sts         iospace:DAT_mem_0115,R16                ;= ??
    ldi         R16,0xb4                                
    sts         iospace:DMA_CH0_CTRLA,R16               ;= ??
LAB_code_000204:              
    call        FUN_code_000b9d                         ;undefined FUN_code_000b9d(void)
    call        FUN_code_000b59                         ;undefined FUN_code_000b59(void)
    brbs        LAB_code_00020f,Cflg                    
    and         R21,R21                                 
    brbs        LAB_code_00020c,Zflg                    
    rjmp        LAB_code_0001b7                         
LAB_code_00020c:              
    call        FUN_code_000b9d                         ;undefined FUN_code_000b9d(void)
    rjmp        LAB_code_0001b5                         
LAB_code_00020f:              
    call        FUN_code_000b9d                         ;undefined FUN_code_000b9d(void)
    call        FUN_code_000bb9                         ;undefined FUN_code_000bb9(undefined2...
    ldi         Xlo,0x3d                                
    ldi         Xhi,0x24                                
    st          X+,R18                                  
    st          X+,R19                                  
    st          X+,R16                                  
    st          X+,R17                                  
    rcall       FUN_code_000236                         ;undefined FUN_code_000236(void)
    eor         R16,R16                                 
    sts         iospace:DMA,R16                         ;= ??
    call        FUN_code_000b3f                         ;undefined FUN_code_000b3f(void)
    pop         Xhi                                     
    pop         Xlo                                     
    pop         param_1                                 
    pop         param_1                                 
    pop         param_2                                 
    pop         param_2                                 
    pop         R21                                     
    pop         R20                                     
    pop         R19                                     
    pop         R10                                     
    pop         R2                                      
    pop         R1                                      
    pop         R0                                      
LAB_code_00022c:              
    pop         Zhi                                     
    pop         Zlo                                     
    pop         Yhi                                     
    pop         Ylo                                     
    pop         R18                                     
    pop         R17                                     
    pop         R16                                     
    out         SREG,Zhi                                
    pop         Zhi                                     
    reti                                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000236(void)
    ldi         R16,0x80                                
    ldi         R17,0x10                                
LAB_code_000238:              
    bset        Iflg                                    
    cli                                                 
    lds         R18,iospace:PORTD_IN                    ;= ??
    sbrs        R18,0x4                                 
    rjmp        LAB_code_000238                         
    sts         sram:DAT_mem_243c,R16                   ;= ??
    sts         iospace:DMA_CH0_CTRLB,R17               ;= ??
    bset        Iflg                                    
LAB_code_000243:              
    lds         R16,iospace:DMA_CH0_CTRLB               ;= ??
    sbrs        R16,0x4                                 
    rjmp        LAB_code_000243                         
    eor         R16,R16                                 
    sts         sram:DAT_mem_243c,R16                   ;= ??
LAB_code_00024a:              
    lds         R18,iospace:PORTD_IN                    ;= ??
    sbrs        R18,0x4                                 
    rjmp        LAB_code_00024a                         
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;ulonglong FUN_code_00024f(undefined2 param_1, undefined2 param_2, uint param_3, undefined2 param_4, undefined2 param_5)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
          ;param_3       uint            R21R20                      
          ;param_4       undefined2      R19R18                      
          ;param_5       undefined2      R17R16                      
    push        Zhi                                     
    in          Zhi,SREG                                
    push        Zhi                                     
    push        Zlo                                     
    push        Yhi                                     
    push        Ylo                                     
    push        param_1                                 
    push        param_1                                 
    push        param_2                                 
    push        param_2                                 
    push        param_3                                 
    push        param_3                                 
    push        param_4                                 
    push        param_4                                 
    push        param_5                                 
    push        param_5                                 
    push        R1                                      
    push        R0                                      
    ldi         Zlo,0x59                                
    ldi         Zhi,0x21                                
    ld          param_5,Z                               
    and         param_5,param_5                         
    brbc        LAB_code_000267,Zflg                    
    rjmp        LAB_code_00031b                         
LAB_code_000267:              
    dec         param_5                                 
    brbc        LAB_code_00026a,Zflg                    
    rjmp        LAB_code_0002d5                         
LAB_code_00026a:              
    dec         param_5                                 
    brbs        LAB_code_0002ab,Zflg                    
    dec         param_5                                 
    brbs        LAB_code_000298,Zflg                    
    dec         param_5                                 
    brbs        LAB_code_00028c,Zflg                    
    ldi         param_4,0x7f                            
    cli                                                 
    call        FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    andi        param_5,0xf                             
    cpi         param_5,0xf                             
    brbs        LAB_code_00027c,Zflg                    
    ldd         param_5,Z+0x1                           
    dec         param_5                                 
    brbs        LAB_code_0002b8,Zflg                    
    rjmp        LAB_code_00031a                         
LAB_code_00027c:              
    rcall       FUN_code_0004c5                         ;undefined FUN_code_0004c5(void)
    ldi         param_5,0x2                             
    sts         iospace:PORTE_INTFLAGS,param_5          ;= ??
    ldi         param_5,0x9                             
    sts         iospace:PORTE_INTCTRL,param_5           ;= ??
    ldi         param_4,0x7f                            
    ldi         param_5,0x0                             
    ldi         param_5,0x8                             
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    ldi         param_5,0x80                            
    rjmp        LAB_code_0002c0                         
LAB_code_00028c:              
    lds         param_5,iospace:PORTD_IN                ;= ??
    sbrs        param_5,0x3                             
    rjmp        LAB_code_00031b                         
    ldi         param_5,0x80                            
    sts         iospace:PORTB_OUTSET,param_5            ;= ??
    ldi         param_5,0x5                             
    st          Z,param_5                               
    ldi         param_5,0xfa                            
    std         Z+0x1,param_5                           
    rjmp        LAB_code_00031b                         
LAB_code_000298:              
    eor         param_3,param_3                         
    eor         param_4,param_4                         
    cli                                                 
LAB_code_00029b:              
    ldi         param_5,0x4                             
LAB_code_00029c:              
    call        FUN_code_001104                         ;undefined5 FUN_code_001104(undefined...
    or          param_3,param_5                         
    subi        param_5,0xe0                            
    cpi         param_5,0xc4                            
    brbc        LAB_code_00029c,Zflg                    
    inc         param_4                                 
    cpi         param_4,0x3                             
    brbc        LAB_code_00029b,Zflg                    
    bset        Iflg                                    
    andi        param_3,0x1                             
    brbc        LAB_code_0002b8,Zflg                    
    ldi         param_5,0x4                             
    st          Z,param_5                               
    rjmp        LAB_code_00031b                         
LAB_code_0002ab:              
    ldi         param_5,0x82                            
    eor         param_4,param_4                         
LAB_code_0002ad:              
    cli                                                 
    call        FUN_code_001104                         ;undefined5 FUN_code_001104(undefined...
    bset        Iflg                                    
    andi        param_5,0xf                             
    cpi         param_5,0x1                             
    brbc        LAB_code_0002b8,Zflg                    
    inc         param_4                                 
    cpi         param_4,0x3                             
    brbc        LAB_code_0002ad,Zflg                    
    rjmp        LAB_code_0002c5                         
LAB_code_0002b8:              
    cli                                                 
    lds         param_5,sram:DAT_mem_2157               ;= ??
    ori         param_5,0x80                            
    sts         sram:DAT_mem_2157,param_5               ;= ??
    bset        Iflg                                    
    ldi         param_5,0x40                            
LAB_code_0002c0:              
    sts         iospace:PORTA_OUTCLR,param_5            ;= ??
    eor         param_5,param_5                         
    st          Z,param_5                               
    rjmp        LAB_code_00031b                         
LAB_code_0002c5:              
    eor         param_5,param_5                         
    ldi         param_5,0x3                             
    eor         param_4,param_4                         
    ldi         param_4,0x7                             
    cli                                                 
    call        FUN_code_0010ba                         ;undefined5 FUN_code_0010ba(undefined...
    ldi         param_5,0x1                             
    eor         param_5,param_5                         
    eor         param_4,param_4                         
    call        FUN_code_0010ba                         ;undefined5 FUN_code_0010ba(undefined...
    bset        Iflg                                    
    ldi         param_5,0x3                             
    st          Z,param_5                               
    rjmp        LAB_code_00031b                         
LAB_code_0002d5:              
    ldd         param_5,Z+0x1                           
    dec         param_5                                 
    brbs        LAB_code_0002d9,Zflg                    
    rjmp        LAB_code_00031a                         
LAB_code_0002d9:              
    ldi         param_5,0x2                             
    st          Z,param_5                               
    ldi         param_5,0x20                            
    sts         iospace:PORTF_OUTCLR,param_5            ;= ??
    sts         iospace:PORTF_OUTSET,param_5            ;= ??
    sts         iospace:PORTB_OUTSET,param_5            ;= ??
    ldi         param_5,0x8                             
    sts         iospace:PORTC_OUTSET,param_5            ;= ??
    lds         param_5,iospace:PORTB_IN                ;= ??
    sbrc        param_5,0x6                             
    rjmp        LAB_code_0002f7                         
    cli                                                 
    lds         param_5,sram:DAT_mem_2157               ;= ??
    ori         param_5,0x40                            
    sts         sram:DAT_mem_2157,param_5               ;= ??
    ldi         param_5,0x40                            
    sts         iospace:PORTA_OUTCLR,param_5            ;= ??
    ldi         param_5,0x4                             
    sts         iospace:PORTE_OUTCLR,param_5            ;= ??
    bset        Iflg                                    
    rjmp        LAB_code_00031b                         
LAB_code_0002f7:              
    ldi         param_5,0x1                             
    sts         iospace:PORTB_INTFLAGS,param_5          ;= ??
    ldi         param_5,0x2                             
    sts         iospace:PORTB_INTCTRL,param_5           ;= ??
    ldi         param_5,0x80                            
    ldi         param_5,0x0                             
    ldi         param_4,0xa6                            
    ldi         param_4,0x3                             
    cli                                                 
    call        FUN_code_0010ba                         ;undefined5 FUN_code_0010ba(undefined...
    ldi         param_5,0x81                            
    ldi         param_5,0x2                             
    eor         param_4,param_4                         
    call        FUN_code_0010ba                         ;undefined5 FUN_code_0010ba(undefined...
    ldi         param_5,0xc                             
    ldi         Ylo,0x7b                                
    ldi         Yhi,0x21                                
    eor         param_4,param_4                         
LAB_code_00030d:              
    ld          param_5,Y+                              
    eor         param_4,param_4                         
    call        FUN_code_0010ba                         ;undefined5 FUN_code_0010ba(undefined...
    subi        param_5,0xe0                            
    cpi         param_5,0x8c                            
    brbc        LAB_code_00030d,Zflg                    
    ldi         param_5,0xc                             
    inc         param_4                                 
    cpi         param_4,0x3                             
    brbc        LAB_code_00030d,Zflg                    
    bset        Iflg                                    
    rjmp        LAB_code_00031b                         
LAB_code_00031a:              
    std         Z+0x1,param_5                           
LAB_code_00031b:              
    adiw        Z,0x2                                   
    ld          param_5,Z                               
    ldd         param_1,Z+0x1                           
    ldd         param_1,Z+0x2                           
    and         param_5,param_5                         
    brbc        LAB_code_000322,Zflg                    
    rjmp        LAB_code_000398                         
LAB_code_000322:              
    cpi         param_5,0x1                             
    brbc        LAB_code_00033d,Zflg                    
    sbiw        param_1,0x1                             
    brbc        LAB_code_000357,Zflg                    
    lds         param_5,sram:DAT_mem_2234               ;= ??
    and         param_5,param_5                         
    brbs        LAB_code_000359,Zflg                    
    cli                                                 
    lds         param_5,sram:DAT_mem_2157               ;= ??
    andi        param_5,0x7                             
    cpi         param_5,0x1                             
    brbc        LAB_code_000339,Zflg                    
    ldi         param_5,0x4                             
    sts         iospace:PORTE_OUTSET,param_5            ;= ??
    bset        Iflg                                    
    ldi         param_1,0xe8                            
    ldi         param_1,0x3                             
    ldi         param_5,0x2                             
    st          Z,param_5                               
    rjmp        LAB_code_000357                         
LAB_code_000339:              
    bset        Iflg                                    
    eor         param_5,param_5                         
    st          Z,param_5                               
    rjmp        LAB_code_000357                         
LAB_code_00033d:              
    cpi         param_5,0x2                             
    brbc        LAB_code_00035a,Zflg                    
    sbiw        param_1,0x1                             
    brbc        LAB_code_000357,Zflg                    
    lds         param_5,sram:DAT_mem_2157               ;= ??
    sbrc        param_5,0x3                             
    rjmp        LAB_code_00034f                         
    ldi         param_5,0x4                             
    cli                                                 
    sts         iospace:PORTE_OUTCLR,param_5            ;= ??
LAB_code_000349:              
    sbi         iospace:GPIO_GPIOR0,0x1                 
    rcall       FUN_code_00063f                         ;undefined FUN_code_00063f(void)
    bset        Iflg                                    
    eor         param_5,param_5                         
    st          Z,param_5                               
    rjmp        LAB_code_000357                         
LAB_code_00034f:              
    ldi         param_1,0xe8                            
    ldi         param_1,0x3                             
    ldi         param_5,0x3                             
    sts         sram:DAT_mem_2442,param_5               ;= ??
    rcall       FUN_code_000688                         ;undefined FUN_code_000688(void)
    ldi         param_5,0x3                             
    st          Z,param_5                               
LAB_code_000357:              
    std         Z+0x1,param_1                           
    std         Z+0x2,param_1                           
LAB_code_000359:              
    rjmp        LAB_code_000398                         
LAB_code_00035a:              
    cpi         param_5,0x3                             
    brbc        LAB_code_00036c,Zflg                    
    sbiw        param_1,0x1                             
    brbc        LAB_code_000357,Zflg                    
    rcall       FUN_code_0006ee                         ;undefined FUN_code_0006ee(void)
    ldi         param_5,0x4                             
    sts         iospace:PORTD_OUTSET,param_5            ;= ??
    ldi         param_5,0x2                             
    sts         iospace:PORTD_OUTCLR,param_5            ;= ??
    sts         iospace:PORTD_OUTSET,param_5            ;= ??
    ldi         param_5,0x4                             
    st          Z,param_5                               
    ldi         param_1,0x88                            
    ldi         param_1,0x13                            
    rjmp        LAB_code_000357                         
LAB_code_00036c:              
    cpi         param_5,0x4                             
    brbc        LAB_code_000381,Zflg                    
    lds         param_5,iospace:PORTD_IN                ;= ??
    sbrs        param_5,0x3                             
    rjmp        LAB_code_00037d                         
    ldi         param_5,0x5                             
    st          Z,param_5                               
    cli                                                 
    lds         param_5,sram:DAT_mem_2157               ;= ??
    ori         param_5,0x10                            
    sts         sram:DAT_mem_2157,param_5               ;= ??
    bset        Iflg                                    
    rcall       FUN_code_0004ef                         ;uint6 FUN_code_0004ef(undefined2 par...
    rjmp        LAB_code_000383                         
LAB_code_00037d:              
    sbiw        param_1,0x1                             
    brbc        LAB_code_000357,Zflg                    
    cli                                                 
    rjmp        LAB_code_000349                         
LAB_code_000381:              
    cpi         param_5,0x5                             
    brbc        LAB_code_000398,Zflg                    
LAB_code_000383:              
    lds         param_5,iospace:PORTB_OUT               ;= ??
    sbrs        param_5,0x7                             
    rjmp        LAB_code_000398                         
    ldi         param_5,0x1                             
    sts         iospace:PORTD_INTFLAGS,param_5          ;= ??
    ldi         param_5,0x2                             
    sts         iospace:PORTD_INTCTRL,param_5           ;= ??
    lds         param_5,sram:DAT_mem_2159               ;= ??
    and         param_5,param_5                         
    brbc        LAB_code_000398,Zflg                    
    eor         param_5,param_5                         
    st          Z,param_5                               
    cli                                                 
    call        FUN_code_001298                         ;undefined FUN_code_001298(void)
    bset        Iflg                                    
    rcall       FUN_code_000472                         ;undefined FUN_code_000472(void)
LAB_code_000398:              
    ldd         param_1,Z+0x3                           
    ldd         param_1,Z+0x4                           
    ldi         param_5,0x1                             
    cpi         param_1,0xf4                            
    cpc         param_1,param_5                         
    brbc        LAB_code_0003aa,Zflg                    
    cli                                                 
    sbis        iospace:GPIO_GPIOR0,0x0                 
    rjmp        LAB_code_0003a4                         
    ldi         param_5,0x1                             
    sts         iospace:PORTA_OUTTGL,param_5            ;= ??
LAB_code_0003a4:              
    sbis        iospace:GPIO_GPIOR0,0x2                 
    rjmp        LAB_code_0003a9                         
    ldi         param_5,0x20                            
    sts         iospace:PORTA_OUTTGL,param_5            ;= ??
LAB_code_0003a9:              
    bset        Iflg                                    
LAB_code_0003aa:              
    sbiw        param_1,0x1                             
    brbs        LAB_code_0003af,Zflg                    
    std         Z+0x3,param_1                           
    std         Z+0x4,param_1                           
    rjmp        LAB_code_00045f                         
LAB_code_0003af:              
    cli                                                 
    sbis        iospace:GPIO_GPIOR0,0x0                 
    rjmp        LAB_code_0003b5                         
    ldi         param_5,0x1                             
    sts         iospace:PORTA_OUTTGL,param_5            ;= ??
LAB_code_0003b5:              
    sbis        iospace:GPIO_GPIOR0,0x2                 
    rjmp        LAB_code_0003ba                         
    ldi         param_5,0x20                            
    sts         iospace:PORTA_OUTTGL,param_5            ;= ??
LAB_code_0003ba:              
    bset        Iflg                                    
    ldi         param_1,0xe8                            
    ldi         param_1,0x3                             
    std         Z+0x3,param_1                           
    std         Z+0x4,param_1                           
    ldi         param_5,0x50                            
    ser         param_5                                 
    ser         param_4                                 
    call        FUN_code_0012df                         ;undefined FUN_code_0012df(void)
    eor         param_5,param_5                         
    sbrs        param_3,0x2                             
    rjmp        LAB_code_0003c9                         
    ldi         param_5,0x6                             
    rjmp        LAB_code_0003d0                         
LAB_code_0003c9:              
    sbrs        param_3,0x1                             
    rjmp        LAB_code_0003cd                         
    ldi         param_5,0x4                             
    rjmp        LAB_code_0003d0                         
LAB_code_0003cd:              
    sbrs        param_3,0x0                             
    rjmp        LAB_code_0003d0                         
    ldi         param_5,0x2                             
LAB_code_0003d0:              
    cli                                                 
    lds         param_5,sram:DAT_mem_2157               ;= ??
    mov         param_4,param_5                         
    andi        param_4,0x6                             
    eor         param_4,param_5                         
    brbs        LAB_code_0003e7,Zflg                    
    andi        param_5,0xf9                            
    or          param_5,param_5                         
    sts         sram:DAT_mem_2157,param_5               ;= ??
    sbic        iospace:GPIO_GPIOR0,0x1                 
    rjmp        LAB_code_0003e6                         
    cpi         param_5,0x6                             
    brbc        LAB_code_0003e1,Zflg                    
    sbi         iospace:GPIO_GPIOR0,0x1                 
    rjmp        LAB_code_0003e5                         
LAB_code_0003e1:              
    and         param_5,param_5                         
    brbs        LAB_code_0003e5,Zflg                    
    sbi         iospace:GPIO_GPIOR0,0x0                 
    rjmp        LAB_code_0003e6                         
LAB_code_0003e5:              
    rcall       FUN_code_00063f                         ;undefined FUN_code_00063f(void)
LAB_code_0003e6:              
    bset        Iflg                                    
LAB_code_0003e7:              
    andi        param_3,0xf8                            
    ldi         param_4,0x14                            
    mul         param_3,param_4                         
    mov         param_5,R1                              
    eor         param_5,param_5                         
    mulsu       param_3,param_4                         
    add         param_5,R0                              
    adc         param_5,R1                              
    ldd         param_4,Z+0x5                           
    ldd         param_4,Z+0x6                           
    std         Z+0x5,param_5                           
    std         Z+0x6,param_5                           
    lds         param_3,iospace:PORTE_INTCTRL           ;= ??
    sbrs        param_3,0x3                             
    rjmp        LAB_code_00045f                         
    cp          param_5,param_4                         
    cpc         param_5,param_4                         
    brbs        LAB_code_0003ff,Zflg                    
    ldi         param_4,0xbc                            
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
LAB_code_0003ff:              
    sbic        iospace:GPIO_GPIOR0,0x3                 
    rjmp        LAB_code_00045f                         
    ldi         param_4,0x7d                            
    cli                                                 
    call        FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    movw        Z,param_5                               
    or          param_5,param_5                         
    brbc        LAB_code_00040a,Zflg                    
    rjmp        LAB_code_00045f                         
LAB_code_00040a:              
    eor         param_2,param_2                         
LAB_code_00040b:              
    lsr         Zhi                                     
    ror         Zlo                                     
    brbs        LAB_code_00040f,Cflg                    
    rjmp        LAB_code_00045b                         
LAB_code_00040f:              
    mov         param_4,param_2                         
    add         param_4,param_4                         
    subi        param_4,0xf3                            
    cli                                                 
    call        FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    movw        param_3,param_5                         
    inc         param_4                                 
    cli                                                 
    call        FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    cp          param_5,param_3                         
    cpc         param_5,param_3                         
    brbc        LAB_code_000422,Cflg                    
    movw        param_1,param_5                         
    movw        param_5,param_3                         
    movw        param_3,param_1                         
LAB_code_000422:              
    and         param_5,param_5                         
    brbc        LAB_code_00042b,Zflg                    
    cpi         param_5,0x97                            
    brbc        LAB_code_00042b,Cflg                    
    cpi         param_3,0x1e                            
    brbc        LAB_code_00045b,Cflg                    
    ldi         param_5,0x1e                            
    sub         param_5,param_3                         
    rjmp        LAB_code_000434                         
LAB_code_00042b:              
    and         param_3,param_3                         
    brbs        LAB_code_00042f,Zflg                    
    cpi         param_3,0x1e                            
    brbs        LAB_code_00045b,Cflg                    
LAB_code_00042f:              
    movw        param_3,param_5                         
    ldi         param_5,0x96                            
    eor         param_5,param_5                         
    sub         param_5,param_3                         
    sbc         param_5,param_3                         
LAB_code_000434:              
    mov         param_3,param_2                         
    ldi         Ylo,0xcf                                
    ldi         Yhi,0x21                                
    add         param_3,param_3                         
    add         param_3,param_3                         
    mov         param_4,param_3                         
    add         param_3,param_3                         
    subi        param_3,0xfc                            
    eor         param_3,param_3                         
    add         Ylo,param_3                             
    adc         Yhi,param_3                             
    ld          param_3,Y                               
    ldd         param_3,Y+0x1                           
    add         param_3,param_5                         
    adc         param_3,param_5                         
    ldi         param_1,0x1                             
    cpi         param_3,0xf5                            
    cpc         param_3,param_1                         
    brbc        LAB_code_00044e,Sflg                    
    ldi         param_1,0xfe                            
    cpi         param_3,0xc                             
    cpc         param_3,param_1                         
    brbc        LAB_code_000450,Sflg                    
    ldi         param_3,0xc                             
    ldi         param_3,0xfe                            
    rjmp        LAB_code_000450                         
LAB_code_00044e:              
    ldi         param_3,0xf4                            
    ldi         param_3,0x1                             
LAB_code_000450:              
    st          Y,param_3                               
    std         Y+0x1,param_3                           
    movw        param_5,param_3                         
    subi        param_4,0x7e                            
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    mov         param_2,param_2                         
    call        FUN_code_001082                         ;undefined3 FUN_code_001082(void)
LAB_code_00045b:              
    inc         param_2                                 
    cpi         param_2,0xc                             
    brbs        LAB_code_00045f,Zflg                    
    rjmp        LAB_code_00040b                         
LAB_code_00045f:              
    pop         R0                                      
    pop         R1                                      
    pop         param_5                                 
    pop         param_5                                 
    pop         param_4                                 
    pop         param_4                                 
    pop         param_3                                 
    pop         param_3                                 
    pop         param_2                                 
    pop         param_2                                 
    pop         param_1                                 
    pop         param_1                                 
    pop         Ylo                                     
    pop         Yhi                                     
    pop         Zlo                                     
    pop         Zhi                                     
    out         SREG,Zhi                                
    pop         Zhi                                     
    reti                                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000472(void)
    push        R18                                     
    push        R17                                     
    push        R16                                     
    push        Yhi                                     
    push        Zlo                                     
    ldi         R18,0x7c                                
    eor         R16,R16                                 
    eor         R17,R17                                 
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    lds         R16,sram:DAT_mem_222f                   ;= ??
    eor         R17,R17                                 
    eor         R18,R18                                 
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    ldi         Ylo,0xb7                                
    ldi         Yhi,0x21                                
LAB_code_000488:              
    ld          R16,Y+                                  
    ld          R17,Y+                                  
    inc         R18                                     
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    cpi         R18,0xc                                 
    brbc        LAB_code_000488,Zflg                    
    ldi         R18,0x24                                
    ldi         Ylo,0x87                                
    ldi         Yhi,0x21                                
LAB_code_000494:              
    ld          R16,Y+                                  
    ld          R17,Y+                                  
    inc         R18                                     
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    cpi         R18,0x3c                                
    brbc        LAB_code_000494,Zflg                    
    inc         R18                                     
    lds         R16,sram:DAT_mem_2230                   ;= ??
    lds         R17,sram:DAT_mem_2231                   ;= ??
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    lds         R16,sram:DAT_mem_2160                   ;= ??
    lds         R17,sram:DAT_mem_2161                   ;= ??
    ldi         R18,0xbc                                
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    lds         R16,sram:DAT_mem_2232                   ;= ??
    lds         R17,sram:DAT_mem_2233                   ;= ??
    ldi         R18,0xbd                                
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    ldi         R18,0x7c                                
    ser         R16                                     
    ldi         R17,0xf                                 
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    pop         Zlo                                     
    pop         Yhi                                     
    pop         R16                                     
    pop         R17                                     
    pop         R18                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0004c5(void)
    push        R16                                     
    push        R17                                     
    push        R18                                     
    push        Ylo                                     
    push        Yhi                                     
    ldi         R18,0x80                                
    ldi         Ylo,0xcf                                
    ldi         Yhi,0x21                                
LAB_code_0004cd:              
    ld          R16,Y+                                  
    ld          R17,Y+                                  
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    inc         R18                                     
    cpi         R18,0xb0                                
    brbc        LAB_code_0004cd,Zflg                    
    ldi         Ylo,0x63                                
    ldi         Yhi,0x21                                
LAB_code_0004d8:              
    ld          R16,Y+                                  
    ld          R17,Y+                                  
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    inc         R18                                     
    cpi         R18,0xbc                                
    brbc        LAB_code_0004d8,Zflg                    
    lds         R16,sram:DAT_mem_2441                   ;= ??
    eor         R17,R17                                 
    ldi         R18,0xbe                                
    cli                                                 
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    pop         Yhi                                     
    pop         Ylo                                     
    pop         R18                                     
    pop         R17                                     
    pop         R16                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;uint6 FUN_code_0004ef(undefined2 param_1, undefined param_2, undefined2 param_3, undefined2 param_4, undefined2 param_5)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined          R22                      
          ;param_3       undefined2      R21R20                      
          ;param_4       undefined2      R19R18                      
          ;param_5       undefined2      R17R16                      
    push        Zhi                                     
    push        Zlo                                     
    push        Yhi                                     
    push        Ylo                                     
    push        R23                                     
    push        param_2                                 
    push        param_3                                 
    push        param_3                                 
    push        param_4                                 
    push        param_4                                 
    push        param_5                                 
    push        param_5                                 
    push        R1                                      
    push        R0                                      
    eor         R23,R23                                 
    ldi         Ylo,0xcf                                
    ldi         Yhi,0x21                                
LAB_code_000500:              
    ld          param_3,Y+                              
    ld          param_3,Y+                              
    mov         param_2,R23                             
    call        FUN_code_001053                         ;undefined4 FUN_code_001053(undefined...
    ld          param_3,Y+                              
    ld          param_3,Y+                              
    mov         param_2,R23                             
    call        FUN_code_001076                         ;undefined3 FUN_code_001076(void)
    ld          param_3,Y+                              
    ld          param_3,Y+                              
    mov         param_2,R23                             
    call        FUN_code_001082                         ;undefined3 FUN_code_001082(void)
    ld          param_3,Y+                              
    ld          param_3,Y+                              
    mov         param_2,R23                             
    call        FUN_code_001068                         ;undefined4 FUN_code_001068(undefined...
    inc         R23                                     
    cpi         R23,0xc                                 
    brbc        LAB_code_000500,Zflg                    
    pop         R0                                      
    pop         R1                                      
    pop         param_5                                 
    pop         param_5                                 
    pop         param_4                                 
    pop         param_4                                 
    pop         param_3                                 
    pop         param_3                                 
    pop         param_2                                 
    pop         R23                                     
    pop         Ylo                                     
    pop         Yhi                                     
    pop         Zlo                                     
    pop         Zhi                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000526(void)
    push        Zhi                                     
    in          Zhi,SREG                                
    push        R16                                     
    lds         R16,iospace:PORTD_IN                    ;= ??
    bst         R16,0x3                                 
    cli                                                 
    lds         R16,sram:DAT_mem_2157                   ;= ??
    bld         R16,0x4                                 
    sts         sram:DAT_mem_2157,R16                   ;= ??
    bset        Iflg                                    
    brbc        LAB_code_000539,Tflg                    
    cli                                                 
    ldi         R16,0x5                                 
    rcall       FUN_code_00054e                         ;undefined FUN_code_00054e(void)
    bset        Iflg                                    
    rjmp        LAB_code_00054a                         
LAB_code_000539:              
    ldi         R16,0x1                                 
    sts         iospace:PORTE_INTCTRL,R16               ;= ??
    eor         R16,R16                                 
    sts         sram:DAT_mem_2158,R16                   ;= ??
    lds         R16,sram:DAT_mem_2157                   ;= ??
    andi        R16,0x7f                                
    sts         sram:DAT_mem_2157,R16                   ;= ??
    ldi         R16,0xc0                                
    sts         iospace:PORTA_OUTSET,R16                ;= ??
    ldi         R16,0x1                                 
    sts         sram:DAT_mem_2441,R16                   ;= ??
LAB_code_00054a:              
    pop         R16                                     
    out         SREG,Zhi                                
    pop         Zhi                                     
    reti                                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_00054e(void)
    sts         sram:DAT_mem_215b,R16                   ;= ??
    ldi         R16,0x80                                
    sts         iospace:PORTB_OUTCLR,R16                ;= ??
    lds         R16,sram:DAT_mem_2159                   ;= ??
    and         R16,R16                                 
    brbs        LAB_code_00055a,Zflg                    
    cpi         R16,0x5                                 
    brbs        LAB_code_00055a,Zflg                    
    ret                                                 
LAB_code_00055a:              
    ldi         R16,0x4                                 
    sts         sram:DAT_mem_2159,R16                   ;= ??
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_00055e(void)
    push        Zhi                                     
    in          Zhi,SREG                                
    push        R16                                     
    lds         R16,sram:DAT_mem_2157                   ;= ??
    ori         R16,0x60                                
    sts         sram:DAT_mem_2157,R16                   ;= ??
    ldi         R16,0x40                                
    sts         iospace:PORTA_OUTCLR,R16                ;= ??
    ldi         R16,0x4                                 
    sts         iospace:PORTE_OUTCLR,R16                ;= ??
    pop         R16                                     
    out         SREG,Zhi                                
    pop         Zhi                                     
    reti                                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000570(void)
    push        Zhi                                     
    in          Zhi,SREG                                
    push        R16                                     
    push        R17                                     
    push        R18                                     
    push        R19                                     
    lds         R16,iospace:PORTF_IN                    ;= ??
    bst         R16,0x6                                 
    brbc        LAB_code_000585,Tflg                    
    sbrc        R16,0x5                                 
    rjmp        LAB_code_000585                         
    ldi         R16,0xf5                                
    ldi         R17,0xf                                 
    ldi         R18,0x4                                 
    ldi         R19,0x40                                
    call        FUN_code_001243                         ;undefined FUN_code_001243(void)
    ldi         R16,0x20                                
    sts         iospace:PORTF_OUTSET,R16                ;= ??
LAB_code_000585:              
    call        FUN_code_001267                         ;undefined FUN_code_001267(void)
    swap        R19                                     
    andi        R19,0xe                                 
    bld         R19,0x0                                 
    lds         R16,sram:DAT_mem_2162                   ;= ??
    andi        R16,0xe                                 
    sts         sram:DAT_mem_2162,R19                   ;= ??
    eor         R16,R19                                 
    brbs        LAB_code_0005a1,Zflg                    
    ldi         R16,0x20                                
    sbrs        R19,0x1                                 
    rjmp        LAB_code_000598                         
    sts         iospace:PORTA_OUTCLR,R16                ;= ??
    cbi         iospace:GPIO_GPIOR0,0x2                 
    rjmp        LAB_code_0005a1                         
LAB_code_000598:              
    sbrs        R19,0x2                                 
    rjmp        LAB_code_00059b                         
    sbi         iospace:GPIO_GPIOR0,0x2                 
LAB_code_00059b:              
    sbrs        R19,0x3                                 
    rjmp        LAB_code_0005a1                         
    ldi         R16,0x20                                
    sts         iospace:PORTA_OUTSET,R16                ;= ??
    cbi         iospace:GPIO_GPIOR0,0x2                 
LAB_code_0005a1:              
    brbc        LAB_code_0005a9,Tflg                    
    ldi         R16,0xa                                 
    sts         sram:DAT_mem_215a,R16                   ;= ??
    ldi         R16,0x1                                 
    sts         sram:DAT_mem_2159,R16                   ;= ??
    rjmp        LAB_code_0005ad                         
LAB_code_0005a9:              
    rcall       FUN_code_0005b4                         ;undefined FUN_code_0005b4(void)
    ldi         R16,0x2                                 
    sts         sram:DAT_mem_2441,R16                   ;= ??
LAB_code_0005ad:              
    pop         R19                                     
    pop         R18                                     
    pop         R17                                     
    pop         R16                                     
    out         SREG,Zhi                                
    pop         Zhi                                     
    reti                                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0005b4(void)
    ldi         R16,0x1                                 
    sts         iospace:PORTE_INTCTRL,R16               ;= ??
    ldi         R16,0x8                                 
    sts         iospace:PORTC_OUTCLR,R16                ;= ??
    ldi         R16,0xa0                                
    sts         iospace:PORTB_OUTCLR,R16                ;= ??
    ldi         R16,0xc0                                
    sts         iospace:PORTA_OUTSET,R16                ;= ??
    eor         R16,R16                                 
    sts         sram:DAT_mem_2158,R16                   ;= ??
    sts         sram:DAT_mem_2159,R16                   ;= ??
    lds         R16,sram:DAT_mem_2157                   ;= ??
    andi        R16,0x7f                                
    sts         sram:DAT_mem_2157,R16                   ;= ??
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0005cb(void)
    push        Zhi                                     
    in          Zhi,SREG                                
    cli                                                 
    push        R16                                     
    push        R17                                     
    push        R18                                     
    eor         R17,R17                                 
    lds         R16,iospace:PORTE_IN                    ;= ??
    bst         R16,0x1                                 
    bld         R17,0x0                                 
    bst         R16,0x3                                 
    bld         R17,0x3                                 
    bst         R16,0x2                                 
    lds         R16,sram:DAT_mem_2157                   ;= ??
    mov         R18,R16                                 
    andi        R16,0xf6                                
    or          R16,R17                                 
    sts         sram:DAT_mem_2157,R16                   ;= ??
    eor         R18,R17                                 
    sbrs        R18,0x0                                 
    rjmp        LAB_code_0005f6                         
    sbrc        R17,0x0                                 
    rjmp        LAB_code_0005eb                         
    ldi         R16,0x4                                 
    sts         iospace:PORTE_OUTCLR,R16                ;= ??
    sbi         iospace:GPIO_GPIOR0,0x1                 
    rcall       FUN_code_00064b                         ;undefined FUN_code_00064b(void)
    rjmp        LAB_code_00060e                         
LAB_code_0005eb:              
    ldi         R17,0xd0                                
    ldi         R18,0x7                                 
LAB_code_0005ed:              
    ldi         R16,0x1                                 
    sts         sram:DAT_mem_215b,R16                   ;= ??
    sts         sram:DAT_mem_215c,R17                   ;= ??
    sts         sram:DAT_mem_215d,R18                   ;= ??
    cbi         iospace:GPIO_GPIOR0,0x1                 
    rjmp        LAB_code_00060e                         
LAB_code_0005f6:              
    sbrs        R18,0x3                                 
    rjmp        LAB_code_00060f                         
    sbrc        R17,0x3                                 
    rjmp        LAB_code_00060f                         
    lds         R16,sram:DAT_mem_215b                   ;= ??
    cpi         R16,0x2                                 
    brbc        LAB_code_00060b,Zflg                    
    lds         R16,sram:DAT_mem_2442                   ;= ??
    and         R16,R16                                 
    brbs        LAB_code_00060b,Zflg                    
    dec         R16                                     
    sts         sram:DAT_mem_2442,R16                   ;= ??
    ldi         R16,0x4                                 
    sts         iospace:PORTE_OUTCLR,R16                ;= ??
    ldi         R17,0x88                                
    ldi         R18,0x13                                
    rjmp        LAB_code_0005ed                         
LAB_code_00060b:              
    rcall       FUN_code_00064b                         ;undefined FUN_code_00064b(void)
    brbc        LAB_code_00060f,Tflg                    
    sbi         iospace:GPIO_GPIOR0,0x1                 
LAB_code_00060e:              
    rcall       FUN_code_00063f                         ;undefined FUN_code_00063f(void)
LAB_code_00060f:              
    pop         R18                                     
    pop         R17                                     
    pop         R16                                     
    out         SREG,Zhi                                
    pop         Zhi                                     
    reti                                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000615(undefined2 param_1, undefined2 param_2, undefined2 param_3, undefined2 param_4, uint param_5)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
          ;param_3       undefined2      R21R20                      
          ;param_4       undefined2      R19R18                      
          ;param_5       uint            R17R16                      
    push        Zhi                                     
    in          Zhi,SREG                                
    push        param_5                                 
    push        param_5                                 
    push        param_4                                 
    ldi         param_4,0x7f                            
    call        FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    sbrs        param_5,0x1                             
    rjmp        LAB_code_000624                         
    lds         param_4,sram:DAT_mem_2006               ;= ??
    ori         param_4,0x1                             
    sts         sram:DAT_mem_2006,param_4               ;= ??
LAB_code_000624:              
    swap        param_5                                 
    andi        param_5,0xf                             
    swap        param_5                                 
    andi        param_5,0x10                            
    or          param_5,param_5                         
    lds         param_5,sram:DAT_mem_2158               ;= ??
    andi        param_5,0xfe                            
    or          param_5,param_5                         
    sts         sram:DAT_mem_2158,param_5               ;= ??
    andi        param_5,0x1c                            
    brbs        LAB_code_000639,Zflg                    
    lds         param_5,sram:DAT_mem_2157               ;= ??
    ori         param_5,0x80                            
    sts         sram:DAT_mem_2157,param_5               ;= ??
    ldi         param_5,0x40                            
    sts         iospace:PORTA_OUTCLR,param_5            ;= ??
LAB_code_000639:              
    pop         param_4                                 
    pop         param_5                                 
    pop         param_5                                 
    out         SREG,Zhi                                
    pop         Zhi                                     
    reti                                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_00063f(void)
    sbic        iospace:GPIO_GPIOR0,0x1                 
    rjmp        LAB_code_000646                         
    cbi         iospace:GPIO_GPIOR0,0x0                 
    ldi         R16,0x1                                 
    sts         iospace:PORTA_OUTSET,R16                ;= ??
    ret                                                 
LAB_code_000646:              
    ldi         R16,0x1                                 
    sts         iospace:PORTA_OUTCLR,R16                ;= ??
    cbi         iospace:GPIO_GPIOR0,0x0                 
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_00064b(void)
    eor         R16,R16                                 
    sts         sram:DAT_mem_215c,R16                   ;= ??
    sts         sram:DAT_mem_215d,R16                   ;= ??
    sts         sram:DAT_mem_215b,R16                   ;= ??
    sts         sram:DAT_mem_2159,R16                   ;= ??
    sts         sram:DAT_mem_2441,R16                   ;= ??
    sts         sram:DAT_mem_2006,R16                   ;= ??
    sts         sram:DAT_mem_2162,R16                   ;= ??
    ldi         R16,0x1                                 
    sts         iospace:PORTE_INTCTRL,R16               ;= ??
    ldi         R16,0x2                                 
    sts         iospace:PORTF_INTCTRL,R16               ;= ??
    ldi         R16,0x0                                 
    sts         iospace:PORTB_INTCTRL,R16               ;= ??
    sts         iospace:PORTD_INTCTRL,R16               ;= ??
    ldi         R16,0xbf                                
    sts         iospace:PORTB_DIRCLR,R16                ;= ??
    eor         R16,R16                                 
    sts         iospace:SPIC,R16                        ;= ??
    ser         R16                                     
    sts         iospace:PORTC_DIRCLR,R16                ;= ??
    ldi         R16,0x41                                
    sts         iospace:PORTD_DIRCLR,R16                ;= ??
    ldi         R16,0x4                                 
    sts         iospace:PORTD_OUTCLR,R16                ;= ??
    ldi         R16,0x20                                
    sts         iospace:PORTF_OUTCLR,R16                ;= ??
    ldi         R16,0x20                                
    sts         iospace:PORTF_DIRCLR,R16                ;= ??
    lds         R16,sram:DAT_mem_2157                   ;= ??
    andi        R16,0xef                                
    sts         sram:DAT_mem_2157,R16                   ;= ??
    ldi         R16,0xa0                                
    sts         iospace:PORTA_OUTSET,R16                ;= ??
    eor         R16,R16                                 
    sts         iospace:SPID,R16                        ;= ??
    sts         iospace:DMA,R16                         ;= ??
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000688(void)
    ldi         R16,0xbc                                
    sts         iospace:PORTB_OUT,R16                   ;= ??
    ldi         R16,0xbf                                
    sts         iospace:PORTB_DIRSET,R16                ;= ??
    ldi         R16,0x7                                 
    sts         iospace:PORTC_OUT,R16                   ;= ??
    ldi         R16,0xbf                                
    sts         iospace:PORTC_DIRSET,R16                ;= ??
    ldi         R16,0x1                                 
    sts         iospace:PORTD_OUTSET,R16                ;= ??
    ldi         R16,0x41                                
    sts         iospace:PORTD_DIRSET,R16                ;= ??
    ldi         R16,0x10                                
    sts         iospace:PORTF_OUTSET,R16                ;= ??
    ldi         R16,0x30                                
    sts         iospace:PORTF_DIRSET,R16                ;= ??
    ldi         R16,0xd1                                
    sts         iospace:SPIC,R16                        ;= ??
    ldi         R16,0x40                                
    sts         iospace:DMA,R16                         ;= ??
LAB_code_0006a6:              
    lds         R16,iospace:DMA                         ;= ??
    sbrc        R16,0x6                                 
    rjmp        LAB_code_0006a6                         
    ldi         R16,0x44                                
    sts         iospace:SPID,R16                        ;= ??
    ldi         R16,0x83                                
    sts         iospace:DMA,R16                         ;= ??
    ldi         R16,0x6a                                
    sts         iospace:DMA_CH0_TRIGSRC,R16             ;= ??
    ldi         R16,0xc3                                
    sts         iospace:DMA_CH0_DESTADDR0,R16           ;= ??
    ldi         R16,0x9                                 
    sts         iospace:DMA_CH0_DESTADDR1,R16           ;= ??
    eor         R16,R16                                 
    sts         iospace:DMA_CH0_DESTADDR2,R16           ;= ??
    ldi         R16,0x50                                
    sts         iospace:DMA_CH0_ADDRCTRL,R16            ;= ??
    ldi         R16,0x6a                                
    sts         iospace:DMA_CH1_TRIGSRC,R16             ;= ??
    ldi         R16,0x39                                
    sts         iospace:DMA_CH1_DESTADDR0,R16           ;= ??
    ldi         R16,0x24                                
    sts         iospace:DMA_CH1_DESTADDR1,R16           ;= ??
    eor         R16,R16                                 
    sts         iospace:DMA_CH1_DESTADDR2,R16           ;= ??
    ldi         R16,0xc3                                
    sts         iospace:DMA_CH1_SRCADDR0,R16            ;= ??
    ldi         R16,0x9                                 
    sts         iospace:DMA_CH1_SRCADDR1,R16            ;= ??
    eor         R16,R16                                 
    sts         iospace:DMA_CH1_SRCADDR2,R16            ;= ??
    sts         iospace:DMA_CH1_REPCNT,R16              ;= ??
    ldi         R16,0x8                                 
    sts         iospace:DMA_CH1_TRFCNT,R16              ;= ??
    eor         R16,R16                                 
    sts         iospace:DAT_mem_0125,R16                ;= ??
    ldi         R16,0x5                                 
    sts         iospace:DMA_CH1_ADDRCTRL,R16            ;= ??
    ldi         R16,0xa4                                
    sts         iospace:DMA_CH1_CTRLA,R16               ;= ??
    ldi         R16,0x1                                 
    sts         iospace:DMA_CH1_CTRLB,R16               ;= ??
    ldi         R16,0x8                                 
    ldi         R17,0x10                                
    eor         R18,R18                                 
    eor         R19,R19                                 
    cli                                                 
    call        FUN_code_001243                         ;undefined FUN_code_001243(void)
    bset        Iflg                                    
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0006ee(void)
    push        R16                                     
    push        R17                                     
    push        R18                                     
    push        R19                                     
    push        R20                                     
    push        Zlo                                     
    push        Zhi                                     
    ldi         Zlo,0x16                                
    ldi         Zhi,0x29                                
    ldi         R20,0xa                                 
LAB_code_0006f8:              
    lpm         R16,Z+                                  
    lpm         R17,Z+                                  
    lpm         R18,Z+                                  
    lpm         R19,Z+                                  
    cli                                                 
    call        FUN_code_001243                         ;undefined FUN_code_001243(void)
    bset        Iflg                                    
    dec         R20                                     
    brbc        LAB_code_0006f8,Zflg                    
    ldi         R16,0xa                                 
    sts         iospace:PORTF_INTCTRL,R16               ;= ??
    ldi         R16,0x20                                
    sts         iospace:PORTB_OUTCLR,R16                ;= ??
    pop         Zhi                                     
    pop         Zlo                                     
    pop         R20                                     
    pop         R19                                     
    pop         R18                                     
    pop         R17                                     
    pop         R16                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000710(void)
    push        Zhi                                     
    in          Zhi,SREG                                
    push        Zhi                                     
    push        Zlo                                     
    push        Yhi                                     
    push        R18                                     
    push        R17                                     
    push        R16                                     
    ldi         Zlo,0x4                                 
    ldi         Zhi,0x20                                
    eor         Yhi,Yhi                                 
    lds         R16,iospace:PORTF_IN                    ;= ??
    sbrc        R16,0x1                                 
    rjmp        LAB_code_00073a                         
    inc         Yhi                                     
    ld          R17,-Z                                  
    ld          R16,-Z                                  
    cp          R16,R17                                 
    brbs        LAB_code_000739,Zflg                    
    lds         R18,iospace:USARTF0_STATUS              ;= ??
    sbrs        R18,0x5                                 
    rjmp        LAB_code_000734                         
    eor         R17,R17                                 
    inc         R16                                     
    ldi         Zlo,0x47                                
    ldi         Zhi,0x20                                
    add         Zlo,R16                                 
    adc         Zhi,R17                                 
    ld          R17,Z                                   
    sts         iospace:USARTF0,R17                     ;= ??
    ldi         Zlo,0x2                                 
    ldi         Zhi,0x20                                
    st          Z,R16                                   
LAB_code_000734:              
    lds         R18,iospace:USARTF0_CTRLA               ;= ??
    ori         R18,0x2                                 
    sts         iospace:USARTF0_CTRLA,R18               ;= ??
LAB_code_000739:              
    adiw        Z,0x2                                   
LAB_code_00073a:              
    st          Z,Yhi                                   
    pop         R16                                     
    pop         R17                                     
    pop         R18                                     
    pop         Yhi                                     
    pop         Zlo                                     
    pop         Zhi                                     
    out         SREG,Zhi                                
    pop         Zhi                                     
    reti                                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000744(void)
    push        Zhi                                     
    in          Zhi,SREG                                
    push        Zhi                                     
    push        Zlo                                     
    push        R19                                     
    push        R18                                     
    push        R17                                     
    push        R16                                     
    ldi         Zlo,0x2                                 
    ldi         Zhi,0x20                                
    ld          R16,Z+                                  
    ld          R17,Z+                                  
    ld          R18,Z                                   
    and         R18,R18                                 
    brbs        LAB_code_000755,Zflg                    
    cp          R16,R17                                 
    brbc        LAB_code_00075b,Zflg                    
LAB_code_000755:              
    lds         R19,iospace:USARTF0_CTRLA               ;= ??
    andi        R19,0xfc                                
    sts         iospace:USARTF0_CTRLA,R19               ;= ??
    rjmp        LAB_code_000767                         
LAB_code_00075b:              
    inc         R16                                     
    eor         R17,R17                                 
    ldi         Zlo,0x47                                
    ldi         Zhi,0x20                                
    add         Zlo,R16                                 
    adc         Zhi,R17                                 
    ld          R17,Z                                   
    sts         iospace:USARTF0,R17                     ;= ??
    ldi         Zlo,0x2                                 
    ldi         Zhi,0x20                                
    st          Z,R16                                   
LAB_code_000767:              
    pop         R16                                     
    pop         R17                                     
    pop         R18                                     
    pop         R19                                     
    pop         Zlo                                     
    pop         Zhi                                     
    out         SREG,Zhi                                
    pop         Zhi                                     
    reti                                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000770(void)
    push        Zhi                                     
    in          Zhi,SREG                                
    push        Zhi                                     
    push        Zlo                                     
    push        R19                                     
    push        R18                                     
    push        R17                                     
    push        R16                                     
    ldi         Zlo,0x0                                 
    ldi         Zhi,0x20                                
    ld          R16,Z+                                  
    ld          R17,Z                                   
    inc         R17                                     
    andi        R17,0x3f                                
    mov         R18,R17                                 
    subi        R18,0xec                                
    andi        R18,0x3f                                
    cp          R18,R16                                 
    brbc        LAB_code_00078b,Zflg                    
    ldi         R18,0x1                                 
    sts         iospace:PORTF_OUTSET,R18                ;= ??
    lds         R18,sram:DAT_mem_2005                   ;= ??
    ori         R18,0x80                                
    sts         sram:DAT_mem_2005,R18                   ;= ??
LAB_code_00078b:              
    lds         R19,iospace:USARTF0_STATUS              ;= ??
    lds         R18,iospace:USARTF0                     ;= ??
    sbrc        R19,0x4                                 
    rjmp        LAB_code_0007a1                         
    cp          R16,R17                                 
    brbs        LAB_code_0007a1,Zflg                    
    st          Z,R17                                   
    ldi         Zlo,0x7                                 
    ldi         Zhi,0x20                                
    eor         R16,R16                                 
    add         Zlo,R17                                 
    adc         Zhi,R16                                 
    st          Z,R18                                   
    cpi         R18,0xd                                 
    brbc        LAB_code_0007a1,Zflg                    
    lds         R16,sram:DAT_mem_2005                   ;= ??
    inc         R16                                     
    sts         sram:DAT_mem_2005,R16                   ;= ??
LAB_code_0007a1:              
    pop         R16                                     
    pop         R17                                     
    pop         R18                                     
    pop         R19                                     
    pop         Zlo                                     
    pop         Zhi                                     
    out         SREG,Zhi                                
    pop         Zhi                                     
    reti                                                
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined main(void)
    ldi         R16,0xcb                                
    sts         iospace:OSC_XOSCCTRL,R16                ;= ??
    ldi         R16,0x9                                 
    sts         iospace:OSC,R16                         ;= ??
LAB_code_0007b0:              
    lds         R17,iospace:OSC_STATUS                  ;= ??
    sbrs        R17,0x3                                 
    rjmp        LAB_code_0007b0                         
    ldi         R16,0xc2                                
    sts         iospace:OSC_PLLCTRL,R16                 ;= ??
    ldi         R16,0x19                                
    sts         iospace:OSC,R16                         ;= ??
LAB_code_0007ba:              
    lds         R17,iospace:OSC_STATUS                  ;= ??
    sbrs        R17,0x4                                 
    rjmp        LAB_code_0007ba                         
    ldi         R17,0xd8                                
    ldi         R16,0x4                                 
    sts         iospace:CPU_CCP,R17                     ;= ??
    sts         iospace:CLK,R16                         ;= ??
    ldi         R16,0x18                                
    sts         iospace:OSC,R16                         ;= ??
    ser         R25                                     
    out         SPL,R25                                 
    ldi         R25,0x3f                                
    out         SPH,R25                                 
    ldi         R16,0xfb                                
    sts         iospace:PORTA_DIRSET,R16                ;= ??
    ldi         R16,0xf3                                
    sts         iospace:PORTA_OUTSET,R16                ;= ??
    ldi         R16,0x18                                
    sts         iospace:PORTA_PIN2CTRL,R16              ;= ??
    ldi         R16,0x80                                
    sts         iospace:PORTA_PIN1CTRL,R16              ;= ??
    sts         iospace:PORTA_PIN3CTRL,R16              ;= ??
    sts         iospace:PORTA_PIN4CTRL,R16              ;= ??
    ldi         R16,0x2                                 
    sts         iospace:PORTB_PIN6CTRL,R16              ;= ??
    ldi         R16,0x80                                
    sts         iospace:PORTC_PIN3CTRL,R16              ;= ??
    ldi         R16,0x30                                
    sts         iospace:PORTC_PIN4CTRL,R16              ;= ??
    ldi         R16,0x28                                
    sts         iospace:PORTD_PIN1CTRL,R16              ;= ??
    sts         iospace:PORTD_PIN2CTRL,R16              ;= ??
    ldi         R16,0x4                                 
    sts         iospace:PORTD_OUTCLR,R16                ;= ??
    ldi         R16,0x2                                 
    sts         iospace:PORTD_OUTSET,R16                ;= ??
    ldi         R16,0x6                                 
    sts         iospace:PORTD_DIRSET,R16                ;= ??
    ldi         R16,0x0                                 
    sts         iospace:PORTD_PIN3CTRL,R16              ;= ??
    ldi         R16,0x4                                 
    sts         iospace:PORTE_DIRSET,R16                ;= ??
    ldi         R16,0x1                                 
    sts         iospace:PORTE_PIN0CTRL,R16              ;= ??
    ldi         R16,0x0                                 
    sts         iospace:PORTE_PIN1CTRL,R16              ;= ??
    ldi         R16,0x80                                
    sts         iospace:PORTE_PIN2CTRL,R16              ;= ??
    ldi         R16,0x0                                 
    sts         iospace:PORTE_PIN3CTRL,R16              ;= ??
    ldi         R16,0xa                                 
    sts         iospace:PORTE_INT0MASK,R16              ;= ??
    ldi         R16,0x1                                 
    sts         iospace:PORTE_INT1MASK,R16              ;= ??
    ldi         R16,0x9                                 
    sts         iospace:PORTF_DIRSET,R16                ;= ??
    ldi         R16,0x0                                 
    sts         iospace:PORTF_PIN1CTRL,R16              ;= ??
    ldi         R16,0x0                                 
    sts         iospace:PORTF_PIN6CTRL,R16              ;= ??
    ldi         R16,0x10                                
    sts         iospace:PORTF_PIN7CTRL,R16              ;= ??
    ldi         R16,0x40                                
    sts         iospace:PORTB_INT0MASK,R16              ;= ??
    ldi         R16,0x8                                 
    sts         iospace:PORTD_INT0MASK,R16              ;= ??
    ldi         R16,0x2                                 
    sts         iospace:PORTF_INT0MASK,R16              ;= ??
    ldi         R16,0x40                                
    sts         iospace:PORTF_INT1MASK,R16              ;= ??
    ldi         R16,0x1                                 
    sts         iospace:PORTF_OUTSET,R16                ;= ??
    ldi         R16,0xd0                                
    sts         iospace:USARTF0_BAUDCTRLB,R16           ;= ??
    ldi         R16,0x83                                
    sts         iospace:USARTF0_BAUDCTRLA,R16           ;= ??
    ldi         R16,0x3                                 
    sts         iospace:USARTF0_CTRLC,R16               ;= ??
    ldi         R16,0x18                                
    sts         iospace:USARTF0_CTRLB,R16               ;= ??
    ldi         R16,0x20                                
    sts         iospace:USARTF0_CTRLA,R16               ;= ??
    ldi         R16,0x6                                 
    sts         iospace:TCC0,R16                        ;= ??
    ldi         R16,0x7d                                
    sts         iospace:TCC0_PER,R16                    ;= ??
    ldi         R16,0x0                                 
    sts         iospace:DAT_mem_0827,R16                ;= ??
    ldi         R16,0x0                                 
    sts         iospace:TCC0_CTRLB,R16                  ;= ??
    ldi         R16,0x0                                 
    sts         iospace:TCC0_CTRLE,R16                  ;= ??
    ldi         R16,0x1                                 
    sts         iospace:TCC0_INTCTRLA,R16               ;= ??
    ldi         R16,0x8                                 
    sts         iospace:NVM_CTRLB,R16                   ;= ??
    eor         R25,R25                                 
    ldi         Zlo,0x0                                 
    ldi         Zhi,0x20                                
    ldi         Ylo,0x43                                
    ldi         Yhi,0x24                                
LAB_code_00084e:              
    st          Z+,R25                                  
    cp          Zlo,Ylo                                 
    cpc         Zhi,Yhi                                 
    brbc        LAB_code_00084e,Zflg                    
    ldi         R16,0x3                                 
    sts         sram:DAT_mem_2442,R16                   ;= ??
    lds         R16,iospace:PORTF_IN                    ;= ??
    sbrs        R16,0x1                                 
    inc         R25                                     
    sts         sram:DAT_mem_2004,R25                   ;= ??
    lds         R16,iospace:PORTE_IN                    ;= ??
    bst         R16,0x1                                 
    eor         R16,R16                                 
    bld         R16,0x0                                 
    sts         sram:DAT_mem_2157,R16                   ;= ??
    brbs        LAB_code_000868,Tflg                    
    sbi         iospace:GPIO_GPIOR0,0x1                 
    ldi         R16,0x1                                 
    sts         iospace:PORTA_OUTCLR,R16                ;= ??
    rjmp        LAB_code_000871                         
LAB_code_000868:              
    ldi         R16,0x1                                 
    sts         sram:DAT_mem_215b,R16                   ;= ??
    ldi         R16,0xd0                                
    sts         sram:DAT_mem_215c,R16                   ;= ??
    ldi         R16,0x7                                 
    sts         sram:DAT_mem_215d,R16                   ;= ??
LAB_code_000871:              
    ldi         R16,0xe8                                
    sts         sram:DAT_mem_215e,R16                   ;= ??
    ldi         R16,0x3                                 
    sts         sram:DAT_mem_215f,R16                   ;= ??
    ldi         R16,0x1                                 
    sts         iospace:PORTE_INTCTRL,R16               ;= ??
    ldi         R16,0x2                                 
    sts         iospace:PORTF_INTCTRL,R16               ;= ??
    ldi         R16,0x3                                 
    sts         iospace:PMIC_CTRL,R16                   ;= ??
    call        FUN_code_0012f5                         ;undefined FUN_code_0012f5(void)
    ldi         R16,0x8                                 
    ldi         R17,0x50                                
    call        FUN_code_0012cc                         ;undefined FUN_code_0012cc(void)
    ldi         R16,0x20                                
    ldi         R17,0x0                                 
    ldi         R18,0x23                                
    call        FUN_code_0012df                         ;undefined FUN_code_0012df(void)
    ldi         R16,0x30                                
    ldi         R17,0x0                                 
    ldi         R18,0x1e                                
    call        FUN_code_0012df                         ;undefined FUN_code_0012df(void)
    ldi         Zlo,0x63                                
    ldi         Zhi,0x21                                
    ldi         Ylo,0x0                                 
    ldi         Yhi,0x10                                
    ldi         R17,0x22                                
LAB_code_000895:              
    ld          R16,Y+                                  
    st          Z+,R16                                  
    cpi         Zlo,0x35                                
    cpc         Zhi,R17                                 
    brbc        LAB_code_000895,Zflg                    
    cbi         iospace:GPIO_GPIOR0,0x3                 
    bset        Iflg                                    
    ldi         Zlo,0x62                                
    ldi         Zhi,0x29                                
    call        FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         R16,0x1                                 
    sts         iospace:PORTF_OUTCLR,R16                ;= ??
LAB_code_0008a3:              
    lds         R16,sram:DAT_mem_2006                   ;= ??
    and         R16,R16                                 
    brbs        LAB_code_0008a8,Zflg                    
    rcall       FUN_code_0008ae                         ;undefined FUN_code_0008ae(void)
LAB_code_0008a8:              
    lds         R16,sram:DAT_mem_2005                   ;= ??
    and         R16,R16                                 
    brbs        LAB_code_0008a3,Zflg                    
    rcall       FUN_code_000975                         ;undefined FUN_code_000975(void)
    rjmp        LAB_code_0008a3                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0008ae(void)
    eor         R16,R16                                 
    cli                                                 
    sts         sram:DAT_mem_2006,R16                   ;= ??
    lds         R16,iospace:PORTB_OUT                   ;= ??
    sbrc        R16,0x7                                 
    rjmp        LAB_code_0008b8                         
LAB_code_0008b6:              
    bset        Iflg                                    
    ret                                                 
LAB_code_0008b8:              
    call        FUN_code_0011e5                         ;undefined FUN_code_0011e5(void)
    bset        Iflg                                    
    ldi         R18,0x80                                
    ldi         Ylo,0xcf                                
    ldi         Yhi,0x21                                
LAB_code_0008be:              
    lsr         R15                                     
    ror         R14                                     
    ror         R13                                     
    ror         R12                                     
    ror         R11                                     
    ror         R10                                     
    ror         R9                                      
    ror         R8                                      
    brbs        LAB_code_0008c9,Cflg                    
    adiw        Y,0x2                                   
    rjmp        LAB_code_00096b                         
LAB_code_0008c9:              
    cli                                                 
    lds         R20,iospace:PORTE_INTCTRL               ;= ??
    sbrs        R20,0x3                                 
    rjmp        LAB_code_0008b6                         
    call        FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    cpi         R18,0xb0                                
    brbs        LAB_code_0008d4,Cflg                    
    rjmp        LAB_code_00094c                         
LAB_code_0008d4:              
    mov         R19,R18                                 
    andi        R19,0x3                                 
    brbc        LAB_code_0008f5,Zflg                    
    ldi         R24,0x75                                
    cpi         R16,0x31                                
    cpc         R17,R24                                 
    brbc        LAB_code_0008e2,Cflg                    
    ldi         R24,0x1                                 
    cpi         R16,0x2c                                
    cpc         R17,R24                                 
    brbc        LAB_code_0008ec,Cflg                    
    ldi         R16,0x2c                                
    ldi         R17,0x1                                 
    rjmp        LAB_code_0008e4                         
LAB_code_0008e2:              
    ldi         R16,0x30                                
    ldi         R17,0x75                                
LAB_code_0008e4:              
    cli                                                 
    lds         R20,iospace:PORTE_INTCTRL               ;= ??
    sbrs        R20,0x3                                 
    rjmp        LAB_code_0008b6                         
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
LAB_code_0008ec:              
    st          Y+,R16                                  
    st          Y+,R17                                  
    movw        R21R20,R17R16                           
    mov         R22,R18                                 
    andi        R22,0x3f                                
    lsr         R22                                     
    lsr         R22                                     
    rcall       FUN_code_001053                         ;undefined4 FUN_code_001053(undefined...
    rjmp        LAB_code_00096b                         
LAB_code_0008f5:              
    cpi         R19,0x1                                 
    brbc        LAB_code_000915,Zflg                    
    ldi         R24,0x1                                 
    cpi         R16,0xf5                                
    cpc         R17,R24                                 
    brbc        LAB_code_000902,Sflg                    
    ldi         R24,0xfe                                
    cpi         R16,0xc                                 
    cpc         R17,R24                                 
    brbc        LAB_code_00090c,Sflg                    
    ldi         R16,0xc                                 
    ldi         R17,0xfe                                
    rjmp        LAB_code_000904                         
LAB_code_000902:              
    ldi         R16,0xf4                                
    ldi         R17,0x1                                 
LAB_code_000904:              
    cli                                                 
    lds         R20,iospace:PORTE_INTCTRL               ;= ??
    sbrs        R20,0x3                                 
    rjmp        LAB_code_0008b6                         
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
LAB_code_00090c:              
    st          Y+,R16                                  
    st          Y+,R17                                  
    movw        R21R20,R17R16                           
    mov         R22,R18                                 
    andi        R22,0x3f                                
    lsr         R22                                     
    lsr         R22                                     
    rcall       FUN_code_001076                         ;undefined3 FUN_code_001076(void)
    rjmp        LAB_code_00096b                         
LAB_code_000915:              
    cpi         R19,0x2                                 
    brbc        LAB_code_000935,Zflg                    
    ldi         R24,0x1                                 
    cpi         R16,0xf5                                
    cpc         R17,R24                                 
    brbc        LAB_code_000922,Sflg                    
    ldi         R24,0xfe                                
    cpi         R16,0xc                                 
    cpc         R17,R24                                 
    brbc        LAB_code_00092c,Sflg                    
    ldi         R16,0xc                                 
    ldi         R17,0xfe                                
    rjmp        LAB_code_000924                         
LAB_code_000922:              
    ldi         R16,0xf4                                
    ldi         R17,0x1                                 
LAB_code_000924:              
    cli                                                 
    lds         R20,iospace:PORTE_INTCTRL               ;= ??
    sbrs        R20,0x3                                 
    rjmp        LAB_code_0008b6                         
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
LAB_code_00092c:              
    st          Y+,R16                                  
    st          Y+,R17                                  
    movw        R21R20,R17R16                           
    mov         R22,R18                                 
    andi        R22,0x3f                                
    lsr         R22                                     
    lsr         R22                                     
    rcall       FUN_code_001082                         ;undefined3 FUN_code_001082(void)
    rjmp        LAB_code_00096b                         
LAB_code_000935:              
    ldi         R24,0x4e                                
    cpi         R16,0x21                                
    cpc         R17,R24                                 
    brbs        LAB_code_00093b,Cflg                    
    ldi         R16,0x20                                
    ldi         R17,0x4e                                
LAB_code_00093b:              
    cli                                                 
    lds         R20,iospace:PORTE_INTCTRL               ;= ??
    sbrs        R20,0x3                                 
    rjmp        LAB_code_0008b6                         
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    st          Y+,R16                                  
    st          Y+,R17                                  
    movw        R21R20,R17R16                           
    mov         R22,R18                                 
    andi        R22,0x3f                                
    lsr         R22                                     
    lsr         R22                                     
    rcall       FUN_code_001068                         ;undefined4 FUN_code_001068(undefined...
    rjmp        LAB_code_00096b                         
LAB_code_00094c:              
    ldi         R24,0xf                                 
    cpi         R16,0xa1                                
    cpc         R17,R24                                 
    brbs        LAB_code_00095a,Cflg                    
    ldi         R16,0xa0                                
    ldi         R17,0xf                                 
    cli                                                 
    lds         R20,iospace:PORTE_INTCTRL               ;= ??
    sbrs        R20,0x3                                 
    rjmp        LAB_code_0008b6                         
    call        FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
LAB_code_00095a:              
    st          Y+,R16                                  
    st          Y+,R17                                  
    mov         R22,R18                                 
    andi        R22,0xf                                 
    mov         R23,R22                                 
    ldi         Zlo,0xcf                                
    ldi         Zhi,0x21                                
    add         R23,R23                                 
    add         R23,R23                                 
    add         R23,R23                                 
    eor         R24,R24                                 
    add         Zlo,R23                                 
    adc         Zhi,R24                                 
    ld          R20,Z+                                  
    ld          R21,Z                                   
    rcall       FUN_code_001053                         ;undefined4 FUN_code_001053(undefined...
    rjmp        LAB_code_00096b                         
LAB_code_00096b:              
    inc         R18                                     
    cpi         R18,0xb0                                
    brbc        LAB_code_000971,Zflg                    
    ldi         Ylo,0x63                                
    ldi         Yhi,0x21                                
    rjmp        LAB_code_0008be                         
LAB_code_000971:              
    cpi         R18,0xbc                                
    brbs        LAB_code_000974,Zflg                    
    rjmp        LAB_code_0008be                         
LAB_code_000974:              
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000975(void)
    andi        R16,0x7f                                
    sts         sram:DAT_mem_2005,R16                   ;= ??
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0x43                                
    brbc        LAB_code_000984,Zflg                    
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0x41                                
    brbc        LAB_code_000981,Zflg                    
    rjmp        FUN_code_000df2                         ;undefined FUN_code_000df2(void)
LAB_code_000981:              
    cpi         R16,0x50                                
    brbc        LAB_code_0009b1,Zflg                    
    rjmp        LAB_code_000a3c                         
LAB_code_000984:              
    cpi         R16,0x4f                                
    brbc        LAB_code_00098e,Zflg                    
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0x4e                                
    brbc        LAB_code_00098b,Zflg                    
    rjmp        LAB_code_0009eb                         
LAB_code_00098b:              
    cpi         R16,0x46                                
    brbc        LAB_code_0009b1,Zflg                    
    rjmp        LAB_code_000a39                         
LAB_code_00098e:              
    cpi         R16,0x50                                
    brbc        LAB_code_00099b,Zflg                    
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,'A'                                 
    brbc        LAB_code_000995,Zflg                    
    rjmp        LAB_code_000a82                         
LAB_code_000995:              
    cpi         R16,'C'                                 
    brbc        LAB_code_000998,Zflg                    
    rjmp        FUN_code_000d1f                         ;undefined FUN_code_000d1f(void)
LAB_code_000998:              
    cpi         R16,'F'                                 
    brbc        LAB_code_0009b1,Zflg                    
    rjmp        FUN_code_000ad3                         ;undefined FUN_code_000ad3(void)
LAB_code_00099b:              
    cpi         R16,0x52                                
    brbc        LAB_code_0009b2,Zflg                    
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0x41                                
    brbc        LAB_code_0009a2,Zflg                    
    rjmp        FUN_code_000c47                         ;undefined FUN_code_000c47(void)
LAB_code_0009a2:              
    cpi         R16,0x43                                
    brbc        LAB_code_0009a5,Zflg                    
    rjmp        FUN_code_000e31                         ;undefined FUN_code_000e31(void)
LAB_code_0009a5:              
    cpi         R16,0x53                                
    brbc        LAB_code_0009a8,Zflg                    
    rjmp        FUN_code_000d32                         ;undefined FUN_code_000d32(void)
LAB_code_0009a8:              
    cpi         R16,0x46                                
    brbc        LAB_code_0009ab,Zflg                    
    rjmp        FUN_code_000e79                         ;undefined FUN_code_000e79(void)
LAB_code_0009ab:              
    cpi         R16,0x54                                
    brbc        LAB_code_0009ae,Zflg                    
    rjmp        FUN_code_000c88                         ;undefined FUN_code_000c88(void)
LAB_code_0009ae:              
    cpi         R16,0x5a                                
    brbc        LAB_code_0009b1,Zflg                    
    rjmp        FUN_code_000c5f                         ;undefined FUN_code_000c5f(void)
LAB_code_0009b1:              
    rjmp        LAB_code_0009e4                         
LAB_code_0009b2:              
    cpi         R16,0x53                                
    brbc        LAB_code_0009db,Zflg                    
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0x43                                
    brbc        LAB_code_0009c6,Zflg                    
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0x4c                                
    brbc        LAB_code_0009bd,Zflg                    
    rjmp        FUN_code_000f3e                         ;undefined FUN_code_000f3e(void)
LAB_code_0009bd:              
    cpi         R16,0x53                                
    brbc        LAB_code_0009c0,Zflg                    
    rjmp        FUN_code_000f70                         ;undefined FUN_code_000f70(void)
LAB_code_0009c0:              
    cpi         R16,0x54                                
    brbc        LAB_code_0009c3,Zflg                    
    rjmp        FUN_code_000f17                         ;undefined FUN_code_000f17(void)
LAB_code_0009c3:              
    cpi         R16,0x52                                
    brbc        LAB_code_0009e4,Zflg                    
    rjmp        FUN_code_000edb                         ;undefined FUN_code_000edb(void)
LAB_code_0009c6:              
    cpi         R16,0x44                                
    brbc        LAB_code_0009c9,Zflg                    
    rjmp        FUN_code_000f9d                         ;undefined FUN_code_000f9d(void)
LAB_code_0009c9:              
    cpi         R16,0x4c                                
    brbc        LAB_code_0009cc,Zflg                    
    rjmp        FUN_code_000fc7                         ;undefined FUN_code_000fc7(void)
LAB_code_0009cc:              
    cpi         R16,0x4f                                
    brbc        LAB_code_0009cf,Zflg                    
    rjmp        FUN_code_000ff6                         ;undefined FUN_code_000ff6(void)
LAB_code_0009cf:              
    cpi         R16,0x53                                
    brbc        LAB_code_0009d2,Zflg                    
    rjmp        FUN_code_000eb1                         ;undefined FUN_code_000eb1(void)
LAB_code_0009d2:              
    cpi         R16,0x54                                
    brbc        LAB_code_0009d5,Zflg                    
    rjmp        FUN_code_000ec9                         ;undefined FUN_code_000ec9(void)
LAB_code_0009d5:              
    cpi         R16,0x56                                
    brbc        LAB_code_0009d8,Zflg                    
    rjmp        LAB_code_000a6c                         
LAB_code_0009d8:              
    cpi         R16,0x5a                                
    brbc        LAB_code_0009e4,Zflg                    
    rjmp        FUN_code_001027                         ;undefined FUN_code_001027(void)
LAB_code_0009db:              
    cpi         R16,0x57                                
    brbc        LAB_code_0009e4,Zflg                    
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0x52                                
    brbc        LAB_code_0009e4,Zflg                    
    rjmp        FUN_code_000cd7                         ;undefined FUN_code_000cd7(void)
LAB_code_0009e2:              
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
LAB_code_0009e4:              
    cpi         R16,0xd                                 
    brbc        LAB_code_0009e2,Zflg                    
    ldi         Zlo,0x88                                
    ldi         Zhi,0x29                                
    call        FUN_code_001413                         ;undefined FUN_code_001413(void)
    ret                                                 
LAB_code_0009eb:              
    ldi         R20,0x1                                 
LAB_code_0009ec:              
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_0009e4,Zflg                    
    lds         R16,sram:DAT_mem_2234                   ;= ??
    cp          R16,R20                                 
    brbc        LAB_code_0009f5,Zflg                    
    rjmp        LAB_code_000ff1                         
LAB_code_0009f5:              
    sts         sram:DAT_mem_2234,R20                   ;= ??
    lds         R16,iospace:NVM_STATUS                  ;= ??
    sbrc        R16,0x7                                 
    rjmp        LAB_code_0009f5                         
    ldi         R16,0x36                                
    sts         iospace:NVM_CMD,R16                     ;= ??
    ldi         R16,0x1                                 
    ldi         R17,0xd8                                
    cli                                                 
    sts         iospace:CPU_CCP,R17                     ;= ??
    sts         iospace:NVM_CTRLA,R16                   ;= ??
    bset        Iflg                                    
LAB_code_000a06:              
    lds         R16,iospace:NVM_STATUS                  ;= ??
    sbrc        R16,0x7                                 
    rjmp        LAB_code_000a06                         
    ldi         Ylo,0xd1                                
    ldi         Yhi,0x10                                
    st          Y,R20                                   
    ldi         R16,0x35                                
    sts         iospace:NVM_CMD,R16                     ;= ??
    sts         iospace:NVM,Ylo                         ;= ??
    sts         iospace:NVM_ADDR1,Yhi                   ;= ??
    ldi         R16,0x1                                 
    ldi         R17,0xd8                                
    cli                                                 
    sts         iospace:CPU_CCP,R17                     ;= ??
    sts         iospace:NVM_CTRLA,R16                   ;= ??
    bset        Iflg                                    
LAB_code_000a1c:              
    lds         R16,iospace:NVM_STATUS                  ;= ??
    sbrc        R16,0x7                                 
    rjmp        LAB_code_000a1c                         
    and         R20,R20                                 
    brbc        LAB_code_000a38,Zflg                    
    ldi         R16,0x4                                 
    sts         iospace:PORTE_OUTCLR,R16                ;= ??
LAB_code_000a25:              
    lds         R16,sram:DAT_mem_2157                   ;= ??
    sbrc        R16,0x3                                 
    rjmp        LAB_code_000a25                         
    cli                                                 
    ldi         R16,0x1                                 
    sts         sram:DAT_mem_215b,R16                   ;= ??
    ldi         R16,0xd0                                
    sts         sram:DAT_mem_215c,R16                   ;= ??
    ldi         R16,0x7                                 
    sts         sram:DAT_mem_215d,R16                   ;= ??
    cbi         iospace:GPIO_GPIOR0,0x1                 
    ldi         R16,0x41                                
    sts         iospace:PORTA_OUTSET,R16                ;= ??
    bset        Iflg                                    
LAB_code_000a38:              
    rjmp        LAB_code_000ff1                         
LAB_code_000a39:              
    eor         R20,R20                                 
    rjmp        LAB_code_0009ec                         
LAB_code_000a3b:              
    rjmp        LAB_code_0009e4                         
LAB_code_000a3c:              
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_000a3b,Zflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000a43,Cflg                    
    ret                                                 
LAB_code_000a43:              
    eor         R18,R18                                 
    cli                                                 
    call        FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    sts         sram:DAT_mem_222f,R16                   ;= ??
    ldi         Ylo,0xb7                                
    ldi         Yhi,0x21                                
LAB_code_000a4c:              
    inc         R18                                     
    cli                                                 
    call        FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    st          Y+,R16                                  
    st          Y+,R17                                  
    cpi         R18,0xc                                 
    brbc        LAB_code_000a4c,Zflg                    
    ldi         R18,0x24                                
    ldi         Ylo,0x87                                
    ldi         Yhi,0x21                                
LAB_code_000a58:              
    inc         R18                                     
    cli                                                 
    call        FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    st          Y+,R16                                  
    st          Y+,R17                                  
    cpi         R18,0x3c                                
    brbc        LAB_code_000a58,Zflg                    
    inc         R18                                     
    cli                                                 
    call        FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    sts         sram:DAT_mem_2230,R16                   ;= ??
    sts         sram:DAT_mem_2231,R17                   ;= ??
    rjmp        LAB_code_000ff1                         
LAB_code_000a6b:              
    rjmp        LAB_code_0009e4                         
LAB_code_000a6c:              
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0x20                                
    brbc        LAB_code_000a6b,Zflg                    
    call        FUN_code_001356                         ;undefined FUN_code_001356(void)
    brbs        LAB_code_000a6b,Cflg                    
    cpi         R16,0xd                                 
    brbc        LAB_code_000a6b,Zflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000a78,Cflg                    
    ret                                                 
LAB_code_000a78:              
    sts         sram:DAT_mem_2232,R20                   ;= ??
    sts         sram:DAT_mem_2233,R21                   ;= ??
    ldi         R18,0xbd                                
    movw        R17R16,R21R20                           
    cli                                                 
    rcall       FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    rjmp        LAB_code_000ff1                         
LAB_code_000a82:              
    rcall       FUN_code_000abd                         ;undefined FUN_code_000abd(void)
    brbs        LAB_code_000a6b,Cflg                    
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         Zlo,R16                                 
    mov         R20,R16                                 
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         Zhi,R16                                 
    mov         R21,R16                                 
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         R22,R16                                 
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         R3,R16                                  
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         R4,R16                                  
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         R5,R16                                  
    cpi         R22,0x2                                 
    brbc        LAB_code_000a6b,Sflg                    
    cp          R3,Zlo                                  
    cpc         R4,Zhi                                  
    cpc         R5,R22                                  
    brbs        LAB_code_000a6b,Cflg                    
    cli                                                 
    sts         RAMPZ,R22                               
    rcall       FUN_code_00064b                         ;undefined FUN_code_00064b(void)
    eor         R16,R16                                 
    sts         iospace:PORTE_INTCTRL,R16               ;= ??
    sts         iospace:TCC0_INTCTRLA,R16               ;= ??
    ldi         R16,0x4                                 
    sts         iospace:PORTE_OUTCLR,R16                ;= ??
    ldi         R16,0x43                                
    ldi         R17,0xd8                                
    sts         iospace:CPU_CCP,R17                     ;= ??
    sts         iospace:PMIC_CTRL,R16                   ;= ??
    ldi         R16,0x26                                
    sts         iospace:NVM_CMD,R16                     ;= ??
    ldi         R16,0x1                                 
    ldi         R17,0xd8                                
    cli                                                 
    sts         iospace:CPU_CCP,R17                     ;= ??
    sts         iospace:NVM_CTRLA,R16                   ;= ??
    bset        Iflg                                    
    jmp         code:FUN_code_010184                    ;undefined FUN_code_010184(void)
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000abd(void)
    call        FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_000ad0,Zflg                    
    ldi         Zlo,0x76                                
    ldi         Zhi,0x2b                                
LAB_code_000ac3:              
    lpm         R17,Z+                                  
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    cp          R16,R17                                 
    brbc        LAB_code_000ad0,Zflg                    
    cpi         Zlo,0x7e                                
    brbc        LAB_code_000ac3,Zflg                    
    ldi         Zlo,0x98                                
    ldi         Zhi,0x29                                
    call        FUN_code_001413                         ;undefined FUN_code_001413(void)
    clc                                                 
    ret                                                 
LAB_code_000ad0:              
    bset        Cflg                                    
    ret                                                 
LAB_code_000ad2:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000ad3(void)
    rcall       FUN_code_000abd                         ;undefined FUN_code_000abd(void)
    brbs        LAB_code_000ad2,Cflg                    
    eor         R16,R16                                 
    sts         iospace:DMA,R16                         ;= ??
    rcall       FUN_code_000b32                         ;undefined FUN_code_000b32(void)
    eor         R16,R16                                 
    ldi         Xlo,0x35                                
    ldi         Xhi,0x24                                
    st          X+,R16                                  
    st          X+,R16                                  
    st          X+,R16                                  
    st          X,R16                                   
    rcall       FUN_code_000c20                         ;undefined FUN_code_000c20(void)
    eor         R17,R17                                 
    call        FUN_code_00137c                         ;undefined FUN_code_00137c(void)
    movw        R17R16,R19R18                           
    call        FUN_code_00137c                         ;undefined FUN_code_00137c(void)
    call        FUN_code_00140f                         ;undefined FUN_code_00140f(void)
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         Ylo,R16                                 
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         Yhi,R16                                 
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         Zlo,R16                                 
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         R0,R16                                  
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         R1,R16                                  
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    mov         R2,R16                                  
    movw        R21R20,Y                                
    mov         R22,Zlo                                 
LAB_code_000afd:              
    rcall       FUN_code_000b88                         ;undefined FUN_code_000b88(void)
LAB_code_000afe:              
    mov         R17,R0                                  
    mov         R18,R1                                  
    mov         R19,R2                                  
    ldi         R16,0x1                                 
    call        FUN_code_001456                         ;undefined FUN_code_001456(void)
    sub         R17,R20                                 
    sbc         R18,R21                                 
    sbc         R19,R22                                 
    subi        R17,0xff                                
    sbci        R18,0xff                                
    sbci        R19,0xff                                
    or          R18,R19                                 
    brbs        LAB_code_000b0d,Zflg                    
    eor         R17,R17                                 
LAB_code_000b0d:              
    lds         R24,sram:DAT_mem_2435                   ;= ??
    lds         R25,sram:DAT_mem_2436                   ;= ??
LAB_code_000b11:              
    ldi         Xlo,0x35                                
    ldi         Xhi,0x22                                
    add         Xlo,R24                                 
    adc         Xhi,R25                                 
    call        FUN_code_00141b                         ;undefined FUN_code_00141b(void)
    st          X,R16                                   
    adiw        R25R24,0x1                              
    andi        R25,0x1                                 
    dec         R17                                     
    brbc        LAB_code_000b11,Zflg                    
    sts         sram:DAT_mem_2435,R24                   ;= ??
    sts         sram:DAT_mem_2436,R25                   ;= ??
    rcall       FUN_code_000b9d                         ;undefined FUN_code_000b9d(void)
    rcall       FUN_code_000b59                         ;undefined FUN_code_000b59(void)
    brbs        LAB_code_000b27,Cflg                    
    and         R21,R21                                 
    brbc        LAB_code_000afe,Zflg                    
    rcall       FUN_code_000b9d                         ;undefined FUN_code_000b9d(void)
    rjmp        LAB_code_000afd                         
LAB_code_000b27:              
    rcall       FUN_code_000b9d                         ;undefined FUN_code_000b9d(void)
    rcall       FUN_code_000bb9                         ;undefined FUN_code_000bb9(undefined2...
    call        FUN_code_00137c                         ;undefined FUN_code_00137c(void)
    movw        R17R16,R19R18                           
    call        FUN_code_00137c                         ;undefined FUN_code_00137c(void)
    call        FUN_code_00140f                         ;undefined FUN_code_00140f(void)
    rcall       FUN_code_000b3f                         ;undefined FUN_code_000b3f(void)
    rjmp        LAB_code_000ff1                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000b32(void)
    ldi         R16,0x2                                 
    sts         iospace:PORTD_OUTCLR,R16                ;= ??
    ldi         R16,0x10                                
    sts         iospace:PORTE_OUTSET,R16                ;= ??
    ldi         R16,0xb0                                
    sts         iospace:PORTE_DIRSET,R16                ;= ??
    ldi         R16,0x50                                
    sts         iospace:SPIE,R16                        ;= ??
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000b3f(void)
    cli                                                 
    eor         R16,R16                                 
    sts         iospace:PORTD_INTCTRL,R16               ;= ??
    sts         iospace:SPIE,R16                        ;= ??
    ldi         R16,0xb0                                
    sts         iospace:PORTE_DIRCLR,R16                ;= ??
    cbi         iospace:GPIO_GPIOR0,0x1                 
    ldi         R16,0x1                                 
    sts         iospace:PORTA_OUTSET,R16                ;= ??
    ldi         R16,0x2                                 
    sts         iospace:PORTD_OUTSET,R16                ;= ??
    ldi         R16,0x88                                
    sts         sram:DAT_mem_215c,R16                   ;= ??
    ldi         R16,0x13                                
    sts         sram:DAT_mem_215d,R16                   ;= ??
    ldi         R16,0x4                                 
    rcall       FUN_code_00054e                         ;undefined FUN_code_00054e(void)
    bset        Iflg                                    
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000b59(void)
    rcall       FUN_code_000b8f                         ;undefined FUN_code_000b8f(void)
    ldi         R16,0x2                                 
    rcall       FUN_code_000c04                         ;undefined FUN_code_000c04(undefined2...
    lds         R24,sram:DAT_mem_2437                   ;= ??
    lds         R25,sram:DAT_mem_2438                   ;= ??
LAB_code_000b60:              
    ldi         Xlo,0x35                                
    ldi         Xhi,0x22                                
    add         Xlo,R24                                 
    adc         Xhi,R25                                 
    ld          R16,X                                   
    adiw        R25R24,0x1                              
    andi        R25,0x1                                 
    sts         iospace:SPIE_DATA,R16                   ;= ??
LAB_code_000b69:              
    lds         R19,iospace:SPIE_STATUS                 ;= ??
    sbrs        R19,0x7                                 
    rjmp        LAB_code_000b69                         
    cp          R0,R20                                  
    cpc         R1,R21                                  
    cpc         R2,R22                                  
    brbc        LAB_code_000b7a,Zflg                    
    sts         sram:DAT_mem_2437,R24                   ;= ??
    sts         sram:DAT_mem_2438,R25                   ;= ??
    ldi         R16,0x10                                
    sts         iospace:PORTE_OUTSET,R16                ;= ??
    bset        Cflg                                    
    ret                                                 
LAB_code_000b7a:              
    subi        R20,0xff                                
    sbci        R21,0xff                                
    sbci        R22,0xff                                
    and         R20,R20                                 
    brbc        LAB_code_000b60,Zflg                    
    sts         sram:DAT_mem_2437,R24                   ;= ??
    sts         sram:DAT_mem_2438,R25                   ;= ??
    ldi         R16,0x10                                
    sts         iospace:PORTE_OUTSET,R16                ;= ??
    clc                                                 
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000b88(void)
    rcall       FUN_code_000b8f                         ;undefined FUN_code_000b8f(void)
    ldi         R16,0xd8                                
    rcall       FUN_code_000c04                         ;undefined FUN_code_000c04(undefined2...
    ldi         R16,0x10                                
    sts         iospace:PORTE_OUTSET,R16                ;= ??
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000b8f(void)
    ldi         R16,0x10                                
    sts         iospace:PORTE_OUTCLR,R16                ;= ??
    ldi         R16,0x6                                 
    sts         iospace:SPIE_DATA,R16                   ;= ??
LAB_code_000b95:              
    lds         R19,iospace:SPIE_STATUS                 ;= ??
    sbrs        R19,0x7                                 
    rjmp        LAB_code_000b95                         
    ldi         R16,0x10                                
    sts         iospace:PORTE_OUTSET,R16                ;= ??
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000b9d(void)
    ldi         R24,0x40                                
    ldi         R25,0x6                                 
LAB_code_000b9f:              
    sbiw        R25R24,0x1                              
    brbc        LAB_code_000b9f,Zflg                    
    ldi         R16,0x10                                
    sts         iospace:PORTE_OUTCLR,R16                ;= ??
    ldi         R16,0x5                                 
    sts         iospace:SPIE_DATA,R16                   ;= ??
LAB_code_000ba7:              
    lds         R19,iospace:SPIE_STATUS                 ;= ??
    sbrs        R19,0x7                                 
    rjmp        LAB_code_000ba7                         
    sts         iospace:SPIE_DATA,R24                   ;= ??
LAB_code_000bad:              
    lds         R19,iospace:SPIE_STATUS                 ;= ??
    sbrs        R19,0x7                                 
    rjmp        LAB_code_000bad                         
    lds         R24,iospace:SPIE_DATA                   ;= ??
    ldi         R16,0x10                                
    sts         iospace:PORTE_OUTSET,R16                ;= ??
    sbrc        R24,0x0                                 
    rjmp        FUN_code_000b9d                         
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000bb9(undefined2 param_1, undefined2 param_2, undefined2 param_3)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
          ;param_3       undefined2      R21R20                      
    ldi         R16,0x10                                
    sts         iospace:PORTE_OUTCLR,R16                ;= ??
    ldi         R16,0x3                                 
    sts         iospace:SPIE_DATA,R16                   ;= ??
    eor         R10,R10                                 
LAB_code_000bc0:              
    lds         R19,iospace:SPIE_STATUS                 ;= ??
    sbrs        R19,0x7                                 
    rjmp        LAB_code_000bc0                         
    sts         iospace:SPIE_DATA,Zlo                   ;= ??
LAB_code_000bc6:              
    lds         R19,iospace:SPIE_STATUS                 ;= ??
    sbrs        R19,0x7                                 
    rjmp        LAB_code_000bc6                         
    sts         iospace:SPIE_DATA,Yhi                   ;= ??
LAB_code_000bcc:              
    lds         R19,iospace:SPIE_STATUS                 ;= ??
    sbrs        R19,0x7                                 
    rjmp        LAB_code_000bcc                         
    sts         iospace:SPIE_DATA,Ylo                   ;= ??
    ser         R16                                     
    ser         R17                                     
    ser         R18                                     
    ser         R19                                     
    ldi         param_2,0xb7                            
    ldi         param_1,0x1d                            
    ldi         param_1,0xc1                            
    ldi         Xlo,0x4                                 
LAB_code_000bda:              
    lds         param_2,iospace:SPIE_STATUS             ;= ??
    sbrs        param_2,0x7                             
    rjmp        LAB_code_000bda                         
    sts         iospace:SPIE_DATA,R10                   ;= ??
LAB_code_000be0:              
    lds         param_2,iospace:SPIE_STATUS             ;= ??
    sbrs        param_2,0x7                             
    rjmp        LAB_code_000be0                         
    lds         param_2,iospace:SPIE_DATA               ;= ??
    cp          R0,Ylo                                  
    cpc         R1,Yhi                                  
    cpc         R2,Zlo                                  
    brbs        LAB_code_000bf0,Zflg                    
    sts         iospace:SPIE_DATA,R10                   ;= ??
    rcall       FUN_code_000bf2                         ;undefined FUN_code_000bf2(undefined2...
    adiw        Y,0x1                                   
    adc         Zlo,R10                                 
    rjmp        LAB_code_000be0                         
LAB_code_000bf0:              
    rcall       FUN_code_000bf2                         ;undefined FUN_code_000bf2(undefined2...
    rjmp        LAB_code_000c43                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000bf2(undefined2 param_1, undefined2 param_2, undefined2 param_3, undefined2 param_4, undefined2 param_5)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
          ;param_3       undefined2      R21R20                      
          ;param_4       undefined2      R19R18                      
          ;param_5       undefined2      R17R16                      
    ldi         param_3,0x8                             
LAB_code_000bf3:              
    add         param_4,param_4                         
    adc         param_4,param_4                         
    adc         param_5,param_5                         
    adc         param_5,param_5                         
    brbc        LAB_code_000bfb,Cflg                    
    lsr         param_2                                 
    brbs        LAB_code_000c01,Cflg                    
    rjmp        LAB_code_000bfd                         
LAB_code_000bfb:              
    lsr         param_2                                 
    brbc        LAB_code_000c01,Cflg                    
LAB_code_000bfd:              
    eor         param_4,param_2                         
    eor         param_4,param_1                         
    eor         param_5,param_1                         
    eor         param_5,Xlo                             
LAB_code_000c01:              
    dec         param_3                                 
    brbc        LAB_code_000bf3,Zflg                    
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000c04(undefined2 param_1, undefined param_2, undefined2 param_3, undefined2 param_4, undefined unaff_R16)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined          R22                      
          ;param_3       undefined2      R21R20                      
          ;param_4       undefined2      R19R18                      
          ;unaff_R16     undefined          R16                      
    ldi         R17,0x10                                
    sts         iospace:PORTE_OUTCLR,R17                ;= ??
    sts         iospace:SPIE_DATA,unaff_R16             ;= ??
LAB_code_000c09:              
    lds         param_4,iospace:SPIE_STATUS             ;= ??
    sbrs        param_4,0x7                             
    rjmp        LAB_code_000c09                         
    sts         iospace:SPIE_DATA,param_2               ;= ??
LAB_code_000c0f:              
    lds         param_4,iospace:SPIE_STATUS             ;= ??
    sbrs        param_4,0x7                             
    rjmp        LAB_code_000c0f                         
    sts         iospace:SPIE_DATA,param_3               ;= ??
LAB_code_000c15:              
    lds         param_4,iospace:SPIE_STATUS             ;= ??
    sbrs        param_4,0x7                             
    rjmp        LAB_code_000c15                         
    sts         iospace:SPIE_DATA,param_3               ;= ??
LAB_code_000c1b:              
    lds         param_4,iospace:SPIE_STATUS             ;= ??
    sbrs        param_4,0x7                             
    rjmp        LAB_code_000c1b                         
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000c20(void)
    ldi         R16,0x10                                
    sts         iospace:PORTE_OUTCLR,R16                ;= ??
    ldi         R16,0x9f                                
    sts         iospace:SPIE_DATA,R16                   ;= ??
    eor         R20,R20                                 
LAB_code_000c27:              
    lds         R19,iospace:SPIE_STATUS                 ;= ??
    sbrs        R19,0x7                                 
    rjmp        LAB_code_000c27                         
    sts         iospace:SPIE_DATA,R20                   ;= ??
LAB_code_000c2d:              
    lds         R19,iospace:SPIE_STATUS                 ;= ??
    sbrs        R19,0x7                                 
    rjmp        LAB_code_000c2d                         
    lds         R18,iospace:SPIE_DATA                   ;= ??
    sts         iospace:SPIE_DATA,R20                   ;= ??
LAB_code_000c35:              
    lds         R19,iospace:SPIE_STATUS                 ;= ??
    sbrs        R19,0x7                                 
    rjmp        LAB_code_000c35                         
    lds         R19,iospace:SPIE_DATA                   ;= ??
    sts         iospace:SPIE_DATA,R20                   ;= ??
LAB_code_000c3d:              
    lds         R20,iospace:SPIE_STATUS                 ;= ??
    sbrs        R20,0x7                                 
    rjmp        LAB_code_000c3d                         
    lds         R16,iospace:SPIE_DATA                   ;= ??
LAB_code_000c43:              
    ldi         R20,0x10                                
    sts         iospace:PORTE_OUTSET,R20                ;= ??
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000c47(void)
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_000c87,Zflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000c4d,Cflg                    
    ret                                                 
LAB_code_000c4d:              
    ldi         R18,0x64                                
LAB_code_000c4e:              
    cli                                                 
    rcall       FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    ldi         R16,0x20                                
    call        FUN_code_001456                         ;undefined FUN_code_001456(void)
    inc         R18                                     
    cli                                                 
    rcall       FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    rcall       FUN_code_00140f                         ;undefined FUN_code_00140f(void)
    inc         R18                                     
    cpi         R18,0x7c                                
    brbs        LAB_code_000c4e,Cflg                    
    rjmp        LAB_code_000ff1                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000c5f(void)
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_000c87,Zflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000c65,Cflg                    
    ret                                                 
LAB_code_000c65:              
    eor         R19,R19                                 
LAB_code_000c66:              
    ldi         R18,0xd                                 
    add         R18,R19                                 
    cli                                                 
    rcall       FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    ldi         R16,0x20                                
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    inc         R18                                     
    cli                                                 
    rcall       FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    ldi         R16,0x20                                
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    ldi         R18,0x4c                                
    add         R18,R19                                 
    cli                                                 
    rcall       FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    ldi         R16,0x20                                
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    inc         R18                                     
    cli                                                 
    rcall       FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    rcall       FUN_code_00140f                         ;undefined FUN_code_00140f(void)
    subi        R19,0xfe                                
    cpi         R19,0x18                                
    brbs        LAB_code_000c66,Cflg                    
    rjmp        LAB_code_000ff1                         
LAB_code_000c87:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000c88(void)
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_000c87,Zflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000c8e,Cflg                    
    ret                                                 
LAB_code_000c8e:              
    ldi         R18,0x3e                                
    cli                                                 
    rcall       FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    mov         R20,R17                                 
    eor         R17,R17                                 
    sbrc        R16,0x7                                 
    com         R17                                     
    rcall       FUN_code_0013a3                         ;undefined FUN_code_0013a3(void)
    ldi         R16,0x20                                
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    mov         R16,R20                                 
    eor         R17,R17                                 
    sbrc        R16,0x7                                 
    com         R17                                     
    rcall       FUN_code_0013a3                         ;undefined FUN_code_0013a3(void)
    ldi         R16,0x20                                
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    ldi         R18,0x3f                                
    cli                                                 
    rcall       FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    eor         R17,R17                                 
    sbrc        R16,0x7                                 
    com         R17                                     
    rcall       FUN_code_0013a3                         ;undefined FUN_code_0013a3(void)
    rcall       FUN_code_00140f                         ;undefined FUN_code_00140f(void)
    ldi         Ylo,0x7b                                
    ldi         Yhi,0x21                                
    eor         R23,R23                                 
LAB_code_000cac:              
    ldi         R18,0x40                                
    add         R18,R23                                 
    cli                                                 
    rcall       FUN_code_0011b4                         ;undefined FUN_code_0011b4(undefined2...
    bset        Iflg                                    
    rcall       FUN_code_00137c                         ;undefined FUN_code_00137c(void)
    cpi         R16,0x60                                
    brbs        LAB_code_000cb7,Sflg                    
    sbrs        R17,0x0                                 
    subi        R17,0x2                                 
    rjmp        LAB_code_000cbb                         
LAB_code_000cb7:              
    cpi         R16,0x10                                
    brbc        LAB_code_000cbb,Cflg                    
    sbrc        R17,0x0                                 
    subi        R17,0xfe                                
LAB_code_000cbb:              
    asr         R17                                     
    asr         R17                                     
    brbc        LAB_code_000cbf,Cflg                    
    ori         R16,0x80                                
LAB_code_000cbf:              
    push        R16                                     
    ldi         R16,0x20                                
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    pop         R16                                     
    rcall       FUN_code_0013a3                         ;undefined FUN_code_0013a3(void)
    ldi         R16,0x20                                
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    eor         R24,R24                                 
    ld          R16,Y+                                  
    add         R16,R16                                 
    asr         R16                                     
    eor         R17,R17                                 
    sbrc        R16,0x7                                 
    com         R17                                     
    rcall       FUN_code_0013a3                         ;undefined FUN_code_0013a3(void)
    ldi         R16,0x20                                
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    rcall       FUN_code_00140f                         ;undefined FUN_code_00140f(void)
    inc         R23                                     
    cpi         R23,0xc                                 
    brbs        LAB_code_000cac,Sflg                    
    rcall       FUN_code_00140f                         ;undefined FUN_code_00140f(void)
    rjmp        LAB_code_000ff1                         
LAB_code_000cd6:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000cd7(void)
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_000cd6,Zflg                    
LAB_code_000cda:              
    lds         R16,iospace:NVM_STATUS                  ;= ??
    sbrc        R16,0x7                                 
    rjmp        LAB_code_000cda                         
    ldi         R16,0x36                                
    sts         iospace:NVM_CMD,R16                     ;= ??
    ldi         R16,0x1                                 
    ldi         R17,0xd8                                
    cli                                                 
    sts         iospace:CPU_CCP,R17                     ;= ??
    sts         iospace:NVM_CTRLA,R16                   ;= ??
    bset        Iflg                                    
    ldi         Zlo,0x63                                
    ldi         Zhi,0x21                                
    ser         Ylo                                     
    ldi         Yhi,0xf                                 
    ldi         R20,0x22                                
LAB_code_000cee:              
    lds         R16,iospace:NVM_STATUS                  ;= ??
    sbrc        R16,0x7                                 
    rjmp        LAB_code_000cee                         
    sbi         iospace:GPIO_GPIOR0,0x3                 
    eor         R19,R19                                 
LAB_code_000cf4:              
    adiw        Y,0x1                                   
    ld          R16,Z+                                  
    ld          R17,Y                                   
    eor         R17,R16                                 
    brbs        LAB_code_000cfb,Zflg                    
    st          Y,R16                                   
    ldi         R19,0x1                                 
LAB_code_000cfb:              
    cpi         Zlo,0x35                                
    cpc         Zhi,R20                                 
    brbs        LAB_code_000d04,Zflg                    
    mov         R18,Ylo                                 
    andi        R18,0x1f                                
    cpi         R18,0x1f                                
    brbc        LAB_code_000cf4,Zflg                    
    rcall       FUN_code_000d07                         ;undefined FUN_code_000d07(void)
    rjmp        LAB_code_000cf4                         
LAB_code_000d04:              
    rcall       FUN_code_000d07                         ;undefined FUN_code_000d07(void)
    cbi         iospace:GPIO_GPIOR0,0x3                 
    rjmp        LAB_code_000ff1                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000d07(void)
    and         R19,R19                                 
    brbs        LAB_code_000d1d,Zflg                    
    ldi         R16,0x35                                
    sts         iospace:NVM_CMD,R16                     ;= ??
    sts         iospace:NVM,Ylo                         ;= ??
    sts         iospace:NVM_ADDR1,Yhi                   ;= ??
    ldi         R16,0x1                                 
    ldi         R17,0xd8                                
    cli                                                 
    sts         iospace:CPU_CCP,R17                     ;= ??
    sts         iospace:NVM_CTRLA,R16                   ;= ??
    bset        Iflg                                    
LAB_code_000d18:              
    lds         R16,iospace:NVM_STATUS                  ;= ??
    sbrc        R16,0x7                                 
    rjmp        LAB_code_000d18                         
    eor         R19,R19                                 
LAB_code_000d1d:              
    ret                                                 
LAB_code_000d1e:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000d1f(void)
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_000d1e,Zflg                    
    ldi         Zlo,0x3e                                
    ldi         Zhi,0x29                                
    ldi         R20,0x9                                 
    cli                                                 
LAB_code_000d26:              
    lpm         R16,Z+                                  
    lpm         R17,Z+                                  
    lpm         R18,Z+                                  
    lpm         R19,Z+                                  
    rcall       FUN_code_001243                         ;undefined FUN_code_001243(void)
    dec         R20                                     
    brbc        LAB_code_000d26,Zflg                    
    bset        Iflg                                    
    ldi         Zlo,0x98                                
    ldi         Zhi,0x29                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000d32(void)
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_000d1e,Zflg                    
    ldi         Zlo,0x6a                                
    ldi         Zhi,0x2b                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    lds         R16,sram:DAT_mem_2232                   ;= ??
    lds         R17,sram:DAT_mem_2233                   ;= ??
    rcall       FUN_code_00137c                         ;undefined FUN_code_00137c(void)
    ldi         Zlo,0x7e                                
    ldi         Zhi,0x2b                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x92                                
    ldi         Zhi,0x2b                                
    lpm         R18,Z+                                  
    lpm         R19,Z+                                  
    lpm         R16,Z+                                  
    lpm         R17,Z+                                  
    rcall       FUN_code_00137c                         ;undefined FUN_code_00137c(void)
    movw        R17R16,R19R18                           
    rcall       FUN_code_00137c                         ;undefined FUN_code_00137c(void)
    ldi         Zlo,0xa6                                
    ldi         Zhi,0x29                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    cli                                                 
    lds         R18,sram:DAT_mem_2157                   ;= ??
    lds         R16,sram:DAT_mem_2160                   ;= ??
    lds         R17,sram:DAT_mem_2161                   ;= ??
    lds         R20,iospace:PORTE_IN                    ;= ??
    bset        Iflg                                    
    ldi         Zlo,0x98                                
    ldi         Zhi,0x29                                
    sbrc        R18,0x0                                 
    rjmp        LAB_code_000d5c                         
    ldi         Zlo,0x9e                                
    ldi         Zhi,0x29                                
LAB_code_000d5c:              
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    mov         R19,R18                                 
    ldi         Zlo,0xc0                                
    ldi         Zhi,0x29                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    rcall       FUN_code_00139b                         ;undefined FUN_code_00139b(void)
    ldi         Zlo,0xce                                
    ldi         Zhi,0x29                                
    andi        R19,0x6                                 
    brbs        LAB_code_000d70,Zflg                    
    ldi         Zlo,0xe2                                
    ldi         Zhi,0x29                                
    cpi         R19,0x6                                 
    brbs        LAB_code_000d70,Zflg                    
    ldi         Zlo,0xdc                                
    ldi         Zhi,0x29                                
    andi        R19,0x4                                 
    brbc        LAB_code_000d70,Zflg                    
    ldi         Zlo,0xd6                                
    ldi         Zhi,0x29                                
LAB_code_000d70:              
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0xec                                
    ldi         Zhi,0x29                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    sbrc        R20,0x2                                 
    rjmp        LAB_code_000d7a                         
    ldi         Zlo,0x2                                 
    ldi         Zhi,0x2a                                
LAB_code_000d78:              
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ret                                                 
LAB_code_000d7a:              
    ldi         Zlo,0xfc                                
    ldi         Zhi,0x29                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x98                                
    ldi         Zhi,0x29                                
    sbrc        R18,0x3                                 
    rjmp        LAB_code_000d89                         
    ldi         Zlo,0x9e                                
    ldi         Zhi,0x29                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    sbrs        R18,0x6                                 
    ret                                                 
    ldi         Zlo,0xf2                                
    ldi         Zhi,0x2a                                
    rjmp        LAB_code_000d78                         
LAB_code_000d89:              
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x28                                
    ldi         Zhi,0x2a                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    lds         R19,sram:DAT_mem_2162                   ;= ??
    ldi         Zlo,0x36                                
    ldi         Zhi,0x2a                                
    sbrc        R19,0x1                                 
    rjmp        LAB_code_000d99                         
    ldi         Zlo,0x3e                                
    ldi         Zhi,0x2a                                
    sbrc        R19,0x2                                 
    rjmp        LAB_code_000d99                         
    ldi         Zlo,0x48                                
    ldi         Zhi,0x2a                                
LAB_code_000d99:              
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x52                                
    ldi         Zhi,0x2a                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x5a                                
    ldi         Zhi,0x2a                                
    sbrs        R19,0x0                                 
    rjmp        LAB_code_000da3                         
    ldi         Zlo,0x5e                                
    ldi         Zhi,0x2a                                
LAB_code_000da3:              
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x4                                 
    ldi         Zhi,0x2b                                
    sbrc        R18,0x4                                 
    rjmp        LAB_code_000dab                         
    ldi         Zlo,0xb4                                
    ldi         Zhi,0x2a                                
    rjmp        LAB_code_000d78                         
LAB_code_000dab:              
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x12                                
    ldi         Zhi,0x2b                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x98                                
    ldi         Zhi,0x29                                
    sbrs        R18,0x7                                 
    rjmp        LAB_code_000dc9                         
    ldi         Zlo,0x20                                
    ldi         Zhi,0x2b                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    lds         R20,sram:DAT_mem_2158                   ;= ??
    eor         R17,R17                                 
LAB_code_000db9:              
    sbrs        R20,0x2                                 
    rjmp        LAB_code_000dc4                         
    ldi         Zlo,0x28                                
    ldi         Zhi,0x2b                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         R16,0x30                                
    add         R16,R17                                 
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    ldi         Zlo,0x2e                                
    ldi         Zhi,0x2b                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
LAB_code_000dc4:              
    cpi         R17,0x2                                 
    brbc        LAB_code_000df1,Cflg                    
    inc         R17                                     
    lsr         R20                                     
    rjmp        LAB_code_000db9                         
LAB_code_000dc9:              
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x96                                
    ldi         Zhi,0x2b                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    lds         R20,sram:DAT_mem_2441                   ;= ??
    add         R20,R20                                 
    ldi         Zlo,0xa8                                
    ldi         Zhi,0x2b                                
    eor         R21,R21                                 
    add         Zlo,R20                                 
    adc         Zhi,R21                                 
    lpm         R20,Z+                                  
    lpm         R21,Z                                   
    movw        Z,R21R20                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x84                                
    ldi         Zhi,0x29                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    lds         R20,sram:DAT_mem_2158                   ;= ??
    ldi         Zlo,0x44                                
    ldi         Zhi,0x2b                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x2                                 
    ldi         Zhi,0x2a                                
    sbrc        R20,0x0                                 
    rjmp        LAB_code_000de7                         
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ret                                                 
LAB_code_000de7:              
    ldi         Zlo,0xfc                                
    ldi         Zhi,0x29                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Zlo,0x4e                                
    ldi         Zhi,0x2b                                
    sbrs        R20,0x1                                 
    rjmp        LAB_code_000df0                         
    ldi         Zlo,0x5c                                
    ldi         Zhi,0x2b                                
LAB_code_000df0:              
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
LAB_code_000df1:              
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000df2(void)
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_000e30,Zflg                    
    cli                                                 
    lds         R16,sram:DAT_mem_2157                   ;= ??
    in          R17,iospace:GPIO_GPIOR0                 
    andi        R17,0x3                                 
    brbc        LAB_code_000e0c,Zflg                    
    lds         R16,sram:DAT_mem_2157                   ;= ??
    andi        R16,0x7f                                
    sts         sram:DAT_mem_2157,R16                   ;= ??
    lds         R16,sram:DAT_mem_2158                   ;= ??
    andi        R16,0xe1                                
    sts         sram:DAT_mem_2158,R16                   ;= ??
    bset        Iflg                                    
    ldi         R16,0x40                                
    sts         iospace:PORTA_OUTSET,R16                ;= ??
    ldi         Zlo,0x8                                 
    ldi         Zhi,0x2a                                
    rjmp        LAB_code_000e2e                         
LAB_code_000e0c:              
    andi        R16,0x7                                 
    cpi         R16,0x1                                 
    brbs        LAB_code_000e13,Zflg                    
    bset        Iflg                                    
    ldi         Zlo,0x9e                                
    ldi         Zhi,0x29                                
    rjmp        LAB_code_000e2e                         
LAB_code_000e13:              
    lds         R16,sram:DAT_mem_2157                   ;= ??
    andi        R16,0x1f                                
    sts         sram:DAT_mem_2157,R16                   ;= ??
    eor         R16,R16                                 
    sts         sram:DAT_mem_2158,R16                   ;= ??
    ldi         R16,0x1                                 
    sts         sram:DAT_mem_215b,R16                   ;= ??
    ldi         R16,0x4                                 
    sts         iospace:PORTE_OUTCLR,R16                ;= ??
    ldi         R16,0xd0                                
    sts         sram:DAT_mem_215c,R16                   ;= ??
    ldi         R16,0x7                                 
    sts         sram:DAT_mem_215d,R16                   ;= ??
    cbi         iospace:GPIO_GPIOR0,0x1                 
    ldi         R16,0x41                                
    sts         iospace:PORTA_OUTSET,R16                ;= ??
    bset        Iflg                                    
    ldi         Zlo,0x98                                
    ldi         Zhi,0x29                                
LAB_code_000e2e:              
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ret                                                 
LAB_code_000e30:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000e31(void)
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_000e30,Zflg                    
    eor         R20,R20                                 
    eor         R21,R21                                 
LAB_code_000e36:              
    ldi         Zlo,0xc6                                
    ldi         Zhi,0x2a                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    mov         R16,R20                                 
    eor         R17,R17                                 
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Ylo,0x63                                
    ldi         Yhi,0x21                                
    mov         R22,R20                                 
    add         R22,R22                                 
    add         Ylo,R22                                 
    adc         Yhi,R21                                 
    ld          R16,Y+                                  
    ld          R17,Y                                   
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    mov         R23,R20                                 
    andi        R23,0x3                                 
    ldi         R24,0x20                                
    mul         R23,R24                                 
    ldi         R16,0xc                                 
    add         R16,R0                                  
    mov         R19,R20                                 
    lsr         R19                                     
    lsr         R19                                     
    cli                                                 
    rcall       FUN_code_001104                         ;undefined5 FUN_code_001104(undefined...
    bset        Iflg                                    
    mov         R16,R17                                 
    eor         R17,R17                                 
    sbrc        R16,0x7                                 
    ser         R17                                     
    rcall       FUN_code_0013a3                         ;undefined FUN_code_0013a3(void)
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Ylo,0xb7                                
    ldi         Yhi,0x21                                
    mov         R22,R20                                 
    add         R22,R22                                 
    add         Ylo,R22                                 
    adc         Yhi,R21                                 
    ld          R16,Y+                                  
    ld          R17,Y+                                  
    rcall       FUN_code_0013a3                         ;undefined FUN_code_0013a3(void)
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ldi         Ylo,0x87                                
    ldi         Yhi,0x21                                
    mov         R22,R20                                 
    add         R22,R22                                 
    add         R22,R22                                 
    add         Ylo,R22                                 
    adc         Yhi,R21                                 
    ld          R16,Y+                                  
    ld          R17,Y+                                  
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    ldi         R16,0x20                                
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    ld          R16,Y+                                  
    ld          R17,Y                                   
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    rcall       FUN_code_00140f                         ;undefined FUN_code_00140f(void)
    inc         R20                                     
    cpi         R20,0xc                                 
    brbs        LAB_code_000e77,Zflg                    
    rjmp        LAB_code_000e36                         
LAB_code_000e77:              
    ret                                                 
LAB_code_000e78:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000e79(void)
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0xd                                 
    brbc        LAB_code_000e78,Zflg                    
    ldi         Ylo,0xcf                                
    ldi         Yhi,0x21                                
    eor         R20,R20                                 
LAB_code_000e7f:              
    ldi         Zlo,0x68                                
    ldi         Zhi,0x2a                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    mov         R16,R20                                 
    eor         R17,R17                                 
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    cli                                                 
    ld          R16,Y+                                  
    ld          R17,Y+                                  
    bset        Iflg                                    
    rcall       FUN_code_001397                         ;undefined FUN_code_001397(void)
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    cli                                                 
    ld          R16,Y+                                  
    ld          R17,Y+                                  
    bset        Iflg                                    
    rcall       FUN_code_001397                         ;undefined FUN_code_001397(void)
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    cli                                                 
    ld          R16,Y+                                  
    ld          R17,Y+                                  
    bset        Iflg                                    
    rcall       FUN_code_001397                         ;undefined FUN_code_001397(void)
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    cli                                                 
    ld          R16,Y+                                  
    ld          R17,Y+                                  
    bset        Iflg                                    
    rcall       FUN_code_00139f                         ;undefined FUN_code_00139f(void)
    rcall       FUN_code_00140f                         ;undefined FUN_code_00140f(void)
    inc         R20                                     
    cpi         R20,0xc                                 
    brbc        LAB_code_000e7f,Zflg                    
    ldi         Zlo,0x92                                
    ldi         Zhi,0x2a                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    lds         R16,sram:DAT_mem_222f                   ;= ??
    eor         R17,R17                                 
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    lds         R16,sram:DAT_mem_2230                   ;= ??
    lds         R17,sram:DAT_mem_2231                   ;= ??
    rcall       FUN_code_0013a7                         ;undefined FUN_code_0013a7(void)
    rcall       FUN_code_00140f                         ;undefined FUN_code_00140f(void)
    ret                                                 
LAB_code_000eb0:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000eb1(void)
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0x20                                
    brbc        LAB_code_000eb0,Zflg                    
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000e78,Cflg                    
    and         R21,R21                                 
    brbs        LAB_code_000e78,Nflg                    
    ldi         Xlo,0x10                                
    cpi         R20,0x0                                 
    cpc         R21,Xlo                                 
    brbc        LAB_code_000eb0,Sflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000ebf,Cflg                    
    ret                                                 
LAB_code_000ebf:              
    sts         sram:DAT_mem_2230,R20                   ;= ??
    sts         sram:DAT_mem_2231,R21                   ;= ??
    ldi         R18,0x3d                                
    movw        R17R16,R21R20                           
    cli                                                 
    rcall       FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    rjmp        LAB_code_000ff1                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000ec9(void)
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    cpi         R16,0x20                                
    brbc        LAB_code_000eb0,Zflg                    
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000eb0,Cflg                    
    and         R21,R21                                 
    brbc        LAB_code_000eb0,Zflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000ed3,Cflg                    
    ret                                                 
LAB_code_000ed3:              
    sts         sram:DAT_mem_222f,R20                   ;= ??
    ldi         R18,0x0                                 
    movw        R17R16,R21R20                           
    cli                                                 
    rcall       FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    rjmp        LAB_code_000ff1                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000edb(void)
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f16,Cflg                    
    and         R21,R21                                 
    brbc        LAB_code_000f16,Zflg                    
    cpi         R20,0xc                                 
    brbc        LAB_code_000f16,Sflg                    
    mov         R22,R20                                 
    cpi         R16,0x20                                
    brbc        LAB_code_000f16,Zflg                    
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f16,Cflg                    
    cpi         R16,0x2c                                
    brbc        LAB_code_000f16,Zflg                    
    movw        R25R24,R21R20                           
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f16,Cflg                    
    cpi         R16,0xd                                 
    brbc        LAB_code_000f16,Zflg                    
    ldi         Xlo,0xc                                 
    cpi         R20,0x0                                 
    cpc         R21,Xlo                                 
    brbc        LAB_code_000f16,Sflg                    
    cpi         R24,0x0                                 
    cpc         R25,Xlo                                 
    brbc        LAB_code_000f16,Sflg                    
    ldi         Xlo,0x5                                 
    cpi         R20,0x55                                
    cpc         R21,Xlo                                 
    brbs        LAB_code_000f16,Sflg                    
    cpi         R24,0x55                                
    cpc         R25,Xlo                                 
    brbs        LAB_code_000f16,Sflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000efe,Cflg                    
    ret                                                 
LAB_code_000efe:              
    ldi         Ylo,0x87                                
    ldi         Yhi,0x21                                
    add         R22,R22                                 
    mov         R23,R22                                 
    add         R23,R23                                 
    eor         R11,R11                                 
    add         Ylo,R23                                 
    adc         Yhi,R11                                 
    st          Y+,R24                                  
    st          Y+,R25                                  
    st          Y+,R20                                  
    st          Y,R21                                   
    ldi         R18,0x25                                
    add         R18,R22                                 
    movw        R17R16,R25R24                           
    cli                                                 
    rcall       FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    inc         R18                                     
    movw        R17R16,R21R20                           
    cli                                                 
    rcall       FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    rjmp        LAB_code_000ff1                         
LAB_code_000f16:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000f17(void)
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f3d,Cflg                    
    and         R21,R21                                 
    brbc        LAB_code_000f3d,Zflg                    
    cpi         R20,0xc                                 
    brbc        LAB_code_000f3d,Sflg                    
    mov         R22,R20                                 
    cpi         R16,0x20                                
    brbc        LAB_code_000f3d,Zflg                    
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f3d,Cflg                    
    cpi         R16,0xd                                 
    brbc        LAB_code_000f3d,Zflg                    
    and         R21,R21                                 
    brbs        LAB_code_000f3d,Nflg                    
    ldi         R24,0x10                                
    cpi         R20,0x0                                 
    cpc         R21,R24                                 
    brbc        LAB_code_000f3d,Sflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000f2d,Cflg                    
    ret                                                 
LAB_code_000f2d:              
    ldi         Ylo,0xb7                                
    ldi         Yhi,0x21                                
    mov         R23,R22                                 
    add         R23,R23                                 
    eor         R11,R11                                 
    add         Ylo,R23                                 
    adc         Yhi,R11                                 
    st          Y+,R20                                  
    st          Y,R21                                   
    ldi         R18,0x1                                 
    add         R18,R22                                 
    movw        R17R16,R21R20                           
    cli                                                 
    rcall       FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    rjmp        LAB_code_000ff1                         
LAB_code_000f3d:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000f3e(void)
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f3d,Cflg                    
    and         R21,R21                                 
    brbc        LAB_code_000f3d,Zflg                    
    cpi         R20,0xc                                 
    brbc        LAB_code_000f3d,Sflg                    
    mov         R22,R20                                 
    cpi         R16,0x20                                
    brbc        LAB_code_000f3d,Zflg                    
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f3d,Cflg                    
    cpi         R16,0xd                                 
    brbc        LAB_code_000f3d,Zflg                    
    and         R21,R21                                 
    brbs        LAB_code_000f3d,Nflg                    
    ldi         R24,0xf                                 
    cpi         R20,0xa1                                
    cpc         R21,R24                                 
    brbc        LAB_code_000f3d,Sflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000f54,Cflg                    
    ret                                                 
LAB_code_000f54:              
    movw        R17R16,R21R20                           
    ldi         R18,0xb0                                
    add         R18,R22                                 
    cli                                                 
    rcall       FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    ldi         Ylo,0x63                                
    ldi         Yhi,0x21                                
    mov         R23,R22                                 
    add         R23,R23                                 
    eor         R11,R11                                 
    add         Ylo,R23                                 
    adc         Yhi,R11                                 
    st          Y+,R20                                  
    st          Y,R21                                   
    ldi         Ylo,0xcf                                
    ldi         Yhi,0x21                                
    mov         R10,R22                                 
    add         R10,R10                                 
    add         R10,R10                                 
    add         R10,R10                                 
    eor         R11,R11                                 
    add         Ylo,R10                                 
    adc         Yhi,R11                                 
    ld          R20,Y+                                  
    ld          R21,Y                                   
    rcall       FUN_code_001053                         ;undefined4 FUN_code_001053(undefined...
    rjmp        LAB_code_000ff1                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000f70(void)
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f9c,Cflg                    
    and         R21,R21                                 
    brbc        LAB_code_000f9c,Zflg                    
    cpi         R20,0xc                                 
    brbc        LAB_code_000f9c,Sflg                    
    mov         R19,R20                                 
    cpi         R16,0x20                                
    brbc        LAB_code_000f9c,Zflg                    
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f9c,Cflg                    
    cpi         R16,0xd                                 
    brbc        LAB_code_000f9c,Zflg                    
    ldi         R24,0x0                                 
    cpi         R20,0x40                                
    cpc         R21,R24                                 
    brbc        LAB_code_000f9c,Sflg                    
    ser         R24                                     
    cpi         R20,0xc0                                
    cpc         R21,R24                                 
    brbs        LAB_code_000f9c,Sflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000f88,Cflg                    
    ret                                                 
LAB_code_000f88:              
    ldi         Ylo,0x7b                                
    ldi         Yhi,0x21                                
    eor         R11,R11                                 
    add         Ylo,R19                                 
    adc         Yhi,R11                                 
    st          Y,R20                                   
    mov         R21,R19                                 
    andi        R21,0x3                                 
    ldi         R22,0x20                                
    mul         R21,R22                                 
    ldi         R16,0xc                                 
    add         R16,R0                                  
    lsr         R19                                     
    lsr         R19                                     
    mov         R17,R20                                 
    eor         R18,R18                                 
    cli                                                 
    rcall       FUN_code_0010ba                         ;undefined5 FUN_code_0010ba(undefined...
    bset        Iflg                                    
    rjmp        LAB_code_000ff1                         
LAB_code_000f9c:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000f9d(void)
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f9c,Cflg                    
    and         R21,R21                                 
    brbc        LAB_code_000f9c,Zflg                    
    cpi         R20,0xc                                 
    brbc        LAB_code_000f9c,Sflg                    
    mov         R22,R20                                 
    cpi         R16,0x20                                
    brbc        LAB_code_000f9c,Zflg                    
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f9c,Cflg                    
    cpi         R16,0xd                                 
    brbc        LAB_code_000f9c,Zflg                    
    and         R21,R21                                 
    brbs        LAB_code_000f9c,Nflg                    
    ldi         R24,0x4e                                
    cpi         R20,0x21                                
    cpc         R21,R24                                 
    brbc        LAB_code_000f9c,Sflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000fb3,Cflg                    
    ret                                                 
LAB_code_000fb3:              
    movw        R17R16,R21R20                           
    mov         R23,R22                                 
    add         R23,R23                                 
    add         R23,R23                                 
    ldi         R18,0x83                                
    add         R18,R23                                 
    cli                                                 
    rcall       FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    ldi         Ylo,0xcf                                
    ldi         Yhi,0x21                                
    add         R23,R23                                 
    subi        R23,0xfa                                
    eor         R11,R11                                 
    add         Ylo,R23                                 
    adc         Yhi,R11                                 
    st          Y+,R20                                  
    st          Y,R21                                   
    rcall       FUN_code_001068                         ;undefined4 FUN_code_001068(undefined...
    rjmp        LAB_code_000ff1                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000fc7(void)
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f9c,Cflg                    
    and         R21,R21                                 
    brbc        LAB_code_000f9c,Zflg                    
    cpi         R20,0xc                                 
    brbc        LAB_code_000f9c,Sflg                    
    mov         R22,R20                                 
    cpi         R16,0x20                                
    brbc        LAB_code_000f9c,Zflg                    
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000f9c,Cflg                    
    cpi         R16,0xd                                 
    brbc        LAB_code_000f9c,Zflg                    
    ldi         R24,0x75                                
    cpi         R20,0x31                                
    cpc         R21,R24                                 
    brbc        LAB_code_000f9c,Sflg                    
    ldi         R24,0x1                                 
    cpi         R20,0x2c                                
    cpc         R21,R24                                 
    brbs        LAB_code_000f9c,Sflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_000fdf,Cflg                    
    ret                                                 
LAB_code_000fdf:              
    movw        R17R16,R21R20                           
    mov         R10,R22                                 
    add         R10,R10                                 
    add         R10,R10                                 
    ldi         R18,0x80                                
    add         R18,R10                                 
    cli                                                 
    rcall       FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    ldi         Ylo,0xcf                                
    ldi         Yhi,0x21                                
    add         R10,R10                                 
    eor         R11,R11                                 
    add         Ylo,R10                                 
    adc         Yhi,R11                                 
    st          Y+,R20                                  
    st          Y,R21                                   
    rcall       FUN_code_001053                         ;undefined4 FUN_code_001053(undefined...
LAB_code_000ff1:              
    ldi         Zlo,0x98                                
    ldi         Zhi,0x29                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ret                                                 
LAB_code_000ff5:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_000ff6(void)
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000ff5,Cflg                    
    and         R21,R21                                 
    brbc        LAB_code_000ff5,Zflg                    
    cpi         R20,0xc                                 
    brbc        LAB_code_000ff5,Sflg                    
    mov         R22,R20                                 
    cpi         R16,0x20                                
    brbc        LAB_code_000ff5,Zflg                    
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000ff5,Cflg                    
    cpi         R16,0xd                                 
    brbc        LAB_code_000ff5,Zflg                    
    ldi         R24,0x1                                 
    cpi         R20,0xf5                                
    cpc         R21,R24                                 
    brbc        LAB_code_000ff5,Sflg                    
    ldi         R24,0xfe                                
    cpi         R20,0xc                                 
    cpc         R21,R24                                 
    brbs        LAB_code_000ff5,Sflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_00100e,Cflg                    
    ret                                                 
LAB_code_00100e:              
    sbi         iospace:GPIO_GPIOR0,0x3                 
    movw        R17R16,R21R20                           
    mov         R23,R22                                 
    add         R23,R23                                 
    add         R23,R23                                 
    ldi         R18,0x82                                
    add         R18,R23                                 
    cli                                                 
    rcall       FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    ldi         Ylo,0xcf                                
    ldi         Yhi,0x21                                
    add         R23,R23                                 
    subi        R23,0xfc                                
    eor         R11,R11                                 
    add         Ylo,R23                                 
    adc         Yhi,R11                                 
    cli                                                 
    st          Y+,R20                                  
    st          Y,R21                                   
    bset        Iflg                                    
    rcall       FUN_code_001082                         ;undefined3 FUN_code_001082(void)
    cbi         iospace:GPIO_GPIOR0,0x3                 
    rjmp        LAB_code_000ff1                         
LAB_code_001026:              
    rjmp        LAB_code_0009e4                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001027(void)
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000ff5,Cflg                    
    and         R21,R21                                 
    brbc        LAB_code_000ff5,Zflg                    
    cpi         R20,0xc                                 
    brbc        LAB_code_000ff5,Sflg                    
    mov         R22,R20                                 
    cpi         R16,0x20                                
    brbc        LAB_code_000ff5,Zflg                    
    rcall       FUN_code_00131a                         ;undefined5 FUN_code_00131a(undefined...
    brbs        LAB_code_000ff5,Cflg                    
    cpi         R16,0xd                                 
    brbc        LAB_code_000ff5,Zflg                    
    ldi         R24,0x1                                 
    cpi         R20,0xf5                                
    cpc         R21,R24                                 
    brbc        LAB_code_001026,Sflg                    
    ldi         R24,0xfe                                
    cpi         R20,0xc                                 
    cpc         R21,R24                                 
    brbs        LAB_code_001026,Sflg                    
    rcall       FUN_code_00108e                         ;undefined FUN_code_00108e(void)
    brbc        LAB_code_00103f,Cflg                    
    ret                                                 
LAB_code_00103f:              
    movw        R17R16,R21R20                           
    mov         R23,R22                                 
    add         R23,R23                                 
    add         R23,R23                                 
    ldi         R18,0x81                                
    add         R18,R23                                 
    cli                                                 
    rcall       FUN_code_001187                         ;undefined FUN_code_001187(void)
    bset        Iflg                                    
    ldi         Ylo,0xcf                                
    ldi         Yhi,0x21                                
    add         R23,R23                                 
    subi        R23,0xfe                                
    eor         R11,R11                                 
    add         Ylo,R23                                 
    adc         Yhi,R11                                 
    st          Y+,R20                                  
    st          Y,R21                                   
    rcall       FUN_code_001076                         ;undefined3 FUN_code_001076(void)
    rjmp        LAB_code_000ff1                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined4 FUN_code_001053(undefined2 param_1, undefined2 param_2, uint param_3, undefined2 param_4, undefined2 param_5)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
          ;param_3       uint            R21R20                      
          ;param_4       undefined2      R19R18                      
          ;param_5       undefined2      R17R16                      
    push        param_4                                 
    ldi         param_4,0xc                             
    ldi         param_4,0x2                             
    rcall       FUN_code_0010a6                         ;undefined FUN_code_0010a6(undefined2...
    ldi         Zlo,0x63                                
    ldi         Zhi,0x21                                
    eor         param_3,param_3                         
    add         param_2,param_2                         
    add         Zlo,param_2                             
    adc         Zhi,param_3                             
    add         param_2,param_2                         
    ori         param_2,0x3                             
    ld          param_3,Z+                              
    ld          param_3,Z                               
    add         param_5,param_3                         
    adc         param_5,param_3                         
    com         param_5                                 
    com         param_5                                 
    rcall       FUN_code_001155                         ;undefined3 FUN_code_001155(undefined...
    pop         param_4                                 
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined4 FUN_code_001068(undefined2 param_1, undefined2 param_2, undefined2 param_3, undefined2 param_4, undefined2 param_5)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
          ;param_3       undefined2      R21R20                      
          ;param_4       undefined2      R19R18                      
          ;param_5       undefined2      R17R16                      
    push        param_4                                 
    ldi         param_4,0x20                            
    ldi         param_4,0x4e                            
    sub         param_4,param_3                         
    sbc         param_4,param_3                         
    movw        param_3,param_4                         
    ldi         param_4,0x72                            
    ldi         param_4,0x2                             
    rcall       FUN_code_0010a6                         ;undefined FUN_code_0010a6(undefined2...
    add         param_2,param_2                         
    add         param_2,param_2                         
    rcall       FUN_code_001155                         ;undefined3 FUN_code_001155(undefined...
    pop         param_4                                 
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined3 FUN_code_001076(void)
    push        R18                                     
    ldi         R18,0x88                                
    ldi         R19,0x41                                
    rcall       FUN_code_001098                         ;undefined FUN_code_001098(void)
    ldi         R18,0x80                                
    eor         R17,R18                                 
    add         R22,R22                                 
    add         R22,R22                                 
    ori         R22,0x2                                 
    rcall       FUN_code_001155                         ;undefined3 FUN_code_001155(undefined...
    pop         R18                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined3 FUN_code_001082(void)
    push        R18                                     
    ldi         R18,0x88                                
    ldi         R19,0x41                                
    rcall       FUN_code_001098                         ;undefined FUN_code_001098(void)
    ldi         R18,0x80                                
    eor         R17,R18                                 
    add         R22,R22                                 
    add         R22,R22                                 
    ori         R22,0x1                                 
    rcall       FUN_code_001155                         ;undefined3 FUN_code_001155(undefined...
    pop         R18                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_00108e(void)
    clc                                                 
    lds         R16,sram:DAT_mem_2157                   ;= ??
    sbrc        R16,0x4                                 
    ret                                                 
    ldi         Zlo,0xb4                                
    ldi         Zhi,0x2a                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    bset        Cflg                                    
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001098(void)
    eor         R17,R17                                 
    mul         R18,R20                                 
    sbrc        R0,0x7                                  
    inc         R1                                      
    mov         R16,R1                                  
    mul         R19,R20                                 
    add         R16,R0                                  
    adc         R17,R1                                  
    mulsu       R21,R18                                 
    add         R16,R0                                  
    adc         R17,R1                                  
    mulsu       R21,R19                                 
    add         R17,R0                                  
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0010a6(undefined2 param_1, undefined2 param_2, uint param_3, uint param_4)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
          ;param_3       uint            R21R20                      
          ;param_4       uint            R19R18                      
    eor         R17,R17                                 
    mul         param_4,param_3                         
    sbrc        R0,0x7                                  
    inc         R1                                      
    mov         R16,R1                                  
    mul         param_4,param_3                         
    add         R16,R0                                  
    adc         R17,R1                                  
    mul         param_4,param_3                         
    add         R16,R0                                  
    adc         R17,R1                                  
    mul         param_4,param_3                         
    and         R1,R1                                   
    brbc        LAB_code_0010b7,Zflg                    
    add         R17,R0                                  
    brbs        LAB_code_0010b7,Cflg                    
    ret                                                 
LAB_code_0010b7:              
    ser         R16                                     
    ser         R17                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined5 FUN_code_0010ba(undefined2 param_1, undefined param_2, undefined2 param_3, uint param_4, undefined2 param_5)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined          R22                      
          ;param_3       undefined2      R21R20                      
          ;param_4       uint            R19R18                      
          ;param_5       undefined2      R17R16                      
    push        param_5                                 
    push        param_5                                 
    push        param_4                                 
    push        param_4                                 
    push        param_3                                 
    push        param_3                                 
    push        param_2                                 
    mov         param_3,param_4                         
    andi        param_4,0x3                             
    cpi         param_4,0x3                             
    brbs        LAB_code_0010c7,Cflg                    
    ldi         param_2,0x1c                            
    rjmp        LAB_code_0010cc                         
LAB_code_0010c7:              
    ldi         param_2,0x4                             
LAB_code_0010c8:              
    dec         param_4                                 
    brbs        LAB_code_0010cc,Nflg                    
    add         param_2,param_2                         
    rjmp        LAB_code_0010c8                         
LAB_code_0010cc:              
    ori         param_2,0x2                             
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    ldi         param_2,0x1                             
    ldi         param_3,0x2                             
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    andi        param_3,0x4                             
    brbs        LAB_code_0010d9,Zflg                    
    sts         iospace:PORTB_OUTSET,param_3            ;= ??
LAB_code_0010d9:              
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    ldi         param_3,0x18                            
LAB_code_0010de:              
    sbrs        param_5,0x7                             
    sts         iospace:PORTB_OUTCLR,param_3            ;= ??
    sbrc        param_5,0x7                             
    sts         iospace:PORTB_OUTSET,param_3            ;= ??
    adc         param_5,param_5                         
    adc         param_4,param_4                         
    adc         param_5,param_5                         
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    dec         param_3                                 
    brbc        LAB_code_0010de,Zflg                    
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    ldi         param_3,0x1c                            
    sts         iospace:PORTB_OUTSET,param_3            ;= ??
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    pop         param_2                                 
    pop         param_3                                 
    pop         param_3                                 
    pop         param_4                                 
    pop         param_4                                 
    pop         param_5                                 
    pop         param_5                                 
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined5 FUN_code_001104(undefined2 param_1, undefined param_2, undefined2 param_3, undefined2 param_4, undefined2 param_5)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined          R22                      
          ;param_3       undefined2      R21R20                      
          ;param_4       undefined2      R19R18                      
          ;param_5       undefined2      R17R16                      
    push        param_5                                 
    push        param_4                                 
    push        param_3                                 
    push        param_3                                 
    push        param_2                                 
    cpi         param_4,0x3                             
    brbs        LAB_code_00110c,Cflg                    
    rjmp        LAB_code_00114f                         
LAB_code_00110c:              
    ldi         param_2,0x4                             
LAB_code_00110d:              
    dec         param_4                                 
    brbs        LAB_code_001111,Nflg                    
    add         param_2,param_2                         
    rjmp        LAB_code_00110d                         
LAB_code_001111:              
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    ldi         param_2,0x1                             
    ldi         param_3,0x2                             
    sts         iospace:PORTB_OUTSET,param_3            ;= ??
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_3            ;= ??
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    ldi         param_3,0x8                             
LAB_code_001122:              
    sbrs        param_5,0x7                             
    sts         iospace:PORTB_OUTCLR,param_3            ;= ??
    sbrc        param_5,0x7                             
    sts         iospace:PORTB_OUTSET,param_3            ;= ??
    adc         param_5,param_5                         
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    dec         param_3                                 
    brbc        LAB_code_001122,Zflg                    
    sts         iospace:PORTB_DIRCLR,param_3            ;= ??
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    ldi         param_3,0x10                            
LAB_code_001136:              
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    add         param_5,param_5                         
    adc         param_4,param_4                         
    lds         param_5,iospace:PORTB_IN                ;= ??
    sbrc        param_5,0x1                             
    ori         param_5,0x1                             
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    dec         param_3                                 
    brbc        LAB_code_001136,Zflg                    
    ldi         param_3,0x1c                            
    sts         iospace:PORTB_OUTSET,param_3            ;= ??
    sts         iospace:PORTB_DIRSET,param_3            ;= ??
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
    sts         iospace:PORTB_OUTSET,param_2            ;= ??
    sts         iospace:PORTB_OUTCLR,param_2            ;= ??
LAB_code_00114f:              
    pop         param_2                                 
    pop         param_3                                 
    pop         param_3                                 
    pop         param_4                                 
    pop         param_5                                 
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined3 FUN_code_001155(undefined param_1, uint param_2, undefined2 param_3, undefined2 param_4, undefined2 param_5)
          ;param_1       undefined          R24                      
          ;param_2       uint            R23R22                      
          ;param_3       undefined2      R21R20                      
          ;param_4       undefined2      R19R18                      
          ;param_5       undefined2      R17R16                      
    push        param_2                                 
    push        param_2                                 
    push        param_1                                 
    cli                                                 
    ldi         param_2,0xd5                            
    sts         iospace:SPIC,param_2                    ;= ??
    mov         param_2,param_2                         
    ldi         param_1,0x1                             
    andi        param_2,0x30                            
    brbs        LAB_code_001164,Zflg                    
    add         param_1,param_1                         
    subi        param_2,0x10                            
    brbs        LAB_code_001164,Zflg                    
    add         param_1,param_1                         
LAB_code_001164:              
    mov         param_2,param_2                         
    swap        param_2                                 
    andi        param_2,0xc0                            
    add         param_2,param_2                         
    andi        param_2,0x6                             
    or          param_2,param_2                         
    ori         param_2,0x10                            
    sts         iospace:PORTC_OUTCLR,param_1            ;= ??
    sts         iospace:SPIC_DATA,param_2               ;= ??
LAB_code_00116f:              
    lds         param_2,iospace:SPIC_STATUS             ;= ??
    sbrs        param_2,0x7                             
    rjmp        LAB_code_00116f                         
    sts         iospace:SPIC_DATA,param_5               ;= ??
LAB_code_001175:              
    lds         param_2,iospace:SPIC_STATUS             ;= ??
    sbrs        param_2,0x7                             
    rjmp        LAB_code_001175                         
    sts         iospace:SPIC_DATA,param_5               ;= ??
LAB_code_00117b:              
    lds         param_2,iospace:SPIC_STATUS             ;= ??
    sbrs        param_2,0x7                             
    rjmp        LAB_code_00117b                         
    ldi         param_2,0x7                             
    sts         iospace:PORTC_OUTSET,param_2            ;= ??
    bset        Iflg                                    
    pop         param_1                                 
    pop         param_2                                 
    pop         param_2                                 
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001187(void)
    push        R18                                     
    push        R21                                     
    push        R22                                     
    ldi         R22,0xd1                                
    sts         iospace:SPIC,R22                        ;= ??
    eor         R21,R21                                 
    lsr         R18                                     
    ror         R21                                     
    lsr         R18                                     
    ror         R21                                     
    ldi         R22,0x1                                 
    sts         iospace:PORTD_OUTCLR,R22                ;= ??
    sts         iospace:SPIC_DATA,R18                   ;= ??
LAB_code_001197:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_001197                         
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_00119d:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_00119d                         
    sts         iospace:SPIC_DATA,R17                   ;= ??
LAB_code_0011a3:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_0011a3                         
    sts         iospace:SPIC_DATA,R16                   ;= ??
LAB_code_0011a9:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_0011a9                         
    ldi         R22,0x1                                 
    sts         iospace:PORTD_OUTSET,R22                ;= ??
    pop         R22                                     
    pop         R21                                     
    pop         R18                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0011b4(undefined2 param_1, undefined2 param_2, undefined2 param_3, byte param_4, undefined2 param_5)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined2      R23R22                      
          ;param_3       undefined2      R21R20                      
          ;param_4       byte               R18                      
          ;param_5       undefined2      R17R16                      
    push        param_4                                 
    push        param_3                                 
    push        param_2                                 
    ldi         param_2,0xd1                            
    sts         iospace:SPIC,param_2                    ;= ??
    ldi         param_2,0x1                             
    lsr         param_4                                 
    ror         param_3                                 
    lsr         param_4                                 
    ror         param_3                                 
    ori         param_4,0x80                            
    sts         iospace:PORTD_OUTCLR,param_2            ;= ??
    sts         iospace:SPIC_DATA,param_4               ;= ??
LAB_code_0011c4:              
    lds         param_2,iospace:SPIC_STATUS             ;= ??
    sbrs        param_2,0x7                             
    rjmp        LAB_code_0011c4                         
    sts         iospace:SPIC_DATA,param_3               ;= ??
LAB_code_0011ca:              
    lds         param_2,iospace:SPIC_STATUS             ;= ??
    sbrs        param_2,0x7                             
    rjmp        LAB_code_0011ca                         
    sts         iospace:SPIC_DATA,param_5               ;= ??
LAB_code_0011d0:              
    lds         param_2,iospace:SPIC_STATUS             ;= ??
    sbrs        param_2,0x7                             
    rjmp        LAB_code_0011d0                         
    lds         param_5,iospace:SPIC_DATA               ;= ??
    sts         iospace:SPIC_DATA,param_5               ;= ??
LAB_code_0011d8:              
    lds         param_2,iospace:SPIC_STATUS             ;= ??
    sbrs        param_2,0x7                             
    rjmp        LAB_code_0011d8                         
    lds         param_5,iospace:SPIC_DATA               ;= ??
    ldi         param_2,0x1                             
    sts         iospace:PORTD_OUTSET,param_2            ;= ??
    pop         param_2                                 
    pop         param_3                                 
    pop         param_4                                 
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0011e5(void)
    push        R18                                     
    push        R21                                     
    push        R22                                     
    ldi         R22,0xd1                                
    sts         iospace:SPIC,R22                        ;= ??
    ldi         R18,0xbc                                
    eor         R21,R21                                 
    ldi         R22,0x1                                 
    sts         iospace:PORTD_OUTCLR,R22                ;= ??
    sts         iospace:SPIC_DATA,R18                   ;= ??
LAB_code_0011f2:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_0011f2                         
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_0011f8:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_0011f8                         
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_0011fe:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_0011fe                         
    lds         R9,iospace:SPIC_DATA                    ;= ??
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_001206:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_001206                         
    lds         R8,iospace:SPIC_DATA                    ;= ??
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_00120e:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_00120e                         
    lds         R11,iospace:SPIC_DATA                   ;= ??
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_001216:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_001216                         
    lds         R10,iospace:SPIC_DATA                   ;= ??
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_00121e:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_00121e                         
    lds         R13,iospace:SPIC_DATA                   ;= ??
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_001226:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_001226                         
    lds         R12,iospace:SPIC_DATA                   ;= ??
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_00122e:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_00122e                         
    lds         R15,iospace:SPIC_DATA                   ;= ??
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_001236:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_001236                         
    lds         R14,iospace:SPIC_DATA                   ;= ??
    ldi         R22,0x1                                 
    sts         iospace:PORTD_OUTSET,R22                ;= ??
    pop         R22                                     
    pop         R21                                     
    pop         R18                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001243(void)
    push        R22                                     
    ldi         R22,0xf1                                
    sts         iospace:SPIC,R22                        ;= ??
    ldi         R22,0x10                                
    sts         iospace:PORTF_OUTCLR,R22                ;= ??
    sts         iospace:SPIC_DATA,R16                   ;= ??
LAB_code_00124c:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_00124c                         
    sts         iospace:SPIC_DATA,R17                   ;= ??
LAB_code_001252:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_001252                         
    sts         iospace:SPIC_DATA,R18                   ;= ??
LAB_code_001258:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_001258                         
    sts         iospace:SPIC_DATA,R19                   ;= ??
LAB_code_00125e:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_00125e                         
    ldi         R22,0x10                                
    sts         iospace:PORTF_OUTSET,R22                ;= ??
    pop         R22                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001267(void)
    push        R22                                     
    push        R23                                     
    ldi         R16,0x8e                                
    eor         R17,R17                                 
    eor         R18,R18                                 
    eor         R19,R19                                 
    rcall       FUN_code_001243                         ;undefined FUN_code_001243(void)
    ldi         R22,0x10                                
    sts         iospace:PORTF_OUTCLR,R22                ;= ??
    eor         R23,R23                                 
    sts         iospace:SPIC_DATA,R23                   ;= ??
LAB_code_001274:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_001274                         
    lds         R16,iospace:SPIC_DATA                   ;= ??
    sts         iospace:SPIC_DATA,R23                   ;= ??
LAB_code_00127c:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_00127c                         
    lds         R17,iospace:SPIC_DATA                   ;= ??
    sts         iospace:SPIC_DATA,R23                   ;= ??
LAB_code_001284:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_001284                         
    lds         R18,iospace:SPIC_DATA                   ;= ??
    sts         iospace:SPIC_DATA,R23                   ;= ??
LAB_code_00128c:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_00128c                         
    lds         R19,iospace:SPIC_DATA                   ;= ??
    ldi         R22,0x10                                
    sts         iospace:PORTF_OUTSET,R22                ;= ??
    pop         R23                                     
    pop         R22                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001298(void)
    push        R18                                     
    push        R21                                     
    push        R22                                     
    push        Zlo                                     
    push        Zhi                                     
    ldi         Zlo,0x92                                
    ldi         Zhi,0x2b                                
    ldi         R22,0xd1                                
    sts         iospace:SPIC,R22                        ;= ??
    ldi         R18,0x3d                                
    ldi         R21,0x40                                
    ldi         R22,0x1                                 
    sts         iospace:PORTD_OUTCLR,R22                ;= ??
    sts         iospace:SPIC_DATA,R18                   ;= ??
LAB_code_0012a9:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_0012a9                         
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_0012af:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_0012af                         
LAB_code_0012b3:              
    lpm         R18,Z+                                  
    lpm         R21,Z+                                  
    sts         iospace:SPIC_DATA,R21                   ;= ??
LAB_code_0012b7:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_0012b7                         
    sts         iospace:SPIC_DATA,R18                   ;= ??
LAB_code_0012bd:              
    lds         R22,iospace:SPIC_STATUS                 ;= ??
    sbrs        R22,0x7                                 
    rjmp        LAB_code_0012bd                         
    cpi         Zlo,0x96                                
    brbc        LAB_code_0012b3,Zflg                    
    ldi         R22,0x1                                 
    sts         iospace:PORTD_OUTSET,R22                ;= ??
    pop         Zhi                                     
    pop         Zlo                                     
    pop         R22                                     
    pop         R21                                     
    pop         R18                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0012cc(void)
    push        R16                                     
    push        R17                                     
    push        R18                                     
    push        R19                                     
    ldi         R18,0x10                                
    sts         iospace:PORTA_OUTCLR,R18                ;= ??
    mov         R19,R17                                 
    rcall       FUN_code_001304                         ;undefined FUN_code_001304(void)
    mov         R16,R19                                 
    rcall       FUN_code_001304                         ;undefined FUN_code_001304(void)
    ldi         R18,0x10                                
    sts         iospace:PORTA_OUTSET,R18                ;= ??
    pop         R19                                     
    pop         R18                                     
    pop         R17                                     
    pop         R16                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0012df(void)
    push        R16                                     
    push        R17                                     
    push        R18                                     
    ldi         R20,0x10                                
    sts         iospace:PORTA_OUTCLR,R20                ;= ??
    mov         R20,R17                                 
    mov         R21,R18                                 
    rcall       FUN_code_001304                         ;undefined FUN_code_001304(void)
    mov         R16,R21                                 
    rcall       FUN_code_001304                         ;undefined FUN_code_001304(void)
    mov         R21,R16                                 
    mov         R16,R20                                 
    rcall       FUN_code_001304                         ;undefined FUN_code_001304(void)
    mov         R20,R16                                 
    ldi         R18,0x10                                
    sts         iospace:PORTA_OUTSET,R18                ;= ??
    pop         R18                                     
    pop         R17                                     
    pop         R16                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0012f5(void)
    ldi         R16,0x10                                
    sts         iospace:PORTA_OUTCLR,R16                ;= ??
    ser         R16                                     
    rcall       FUN_code_001304                         ;undefined FUN_code_001304(void)
    ser         R16                                     
    rcall       FUN_code_001304                         ;undefined FUN_code_001304(void)
    ser         R16                                     
    rcall       FUN_code_001304                         ;undefined FUN_code_001304(void)
    ser         R16                                     
    rcall       FUN_code_001304                         ;undefined FUN_code_001304(void)
    ldi         R16,0x10                                
    sts         iospace:PORTA_OUTSET,R16                ;= ??
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001304(void)
    ldi         R18,0x8                                 
LAB_code_001305:              
    ldi         R17,0x8                                 
    sbrc        R16,0x7                                 
    sts         iospace:PORTA_OUTSET,R17                ;= ??
    sbrs        R16,0x7                                 
    sts         iospace:PORTA_OUTCLR,R17                ;= ??
    ldi         R17,0x2                                 
    sts         iospace:PORTA_OUTCLR,R17                ;= ??
    add         R16,R16                                 
    nop                                                 
    sts         iospace:PORTA_OUTSET,R17                ;= ??
    lds         R17,iospace:PORTA_IN                    ;= ??
    bst         R17,0x2                                 
    bld         R16,0x0                                 
    dec         R18                                     
    brbc        LAB_code_001305,Zflg                    
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined5 FUN_code_00131a(undefined2 param_1, undefined param_2, undefined2 param_3, undefined2 param_4, char unaff_R16)
          ;param_1       undefined2      R25R24                      
          ;param_2       undefined          R22                      
          ;param_3       undefined2      R21R20                      
          ;param_4       undefined2      R19R18                      
          ;unaff_R16     char               R16                      
    push        param_2                                 
    push        R17                                     
    push        param_4                                 
    push        param_4                                 
    push        R1                                      
    push        R0                                      
    eor         param_4,param_4                         
    ldi         R17,0xa                                 
    eor         param_4,param_4                         
    eor         param_3,param_3                         
    eor         param_3,param_3                         
LAB_code_001325:              
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    and         param_4,param_4                         
    brbc        LAB_code_00132d,Zflg                    
    cpi         unaff_R16,0x2d                          
    brbc        LAB_code_00132d,Zflg                    
    inc         param_4                                 
    inc         param_4                                 
    rjmp        LAB_code_001325                         
LAB_code_00132d:              
    inc         param_4                                 
    cpi         unaff_R16,0x30                          
    brbs        LAB_code_001341,Sflg                    
    cpi         unaff_R16,0x3a                          
    brbc        LAB_code_001341,Sflg                    
    mov         param_2,unaff_R16                       
    subi        param_2,0x30                            
    mul         param_3,R17                             
    add         R0,param_2                              
    eor         param_2,param_2                         
    adc         R1,param_2                              
    mov         param_2,param_3                         
    mov         param_3,R1                              
    mov         param_3,R0                              
    mul         R17,param_2                             
    and         R1,R1                                   
    brbc        LAB_code_00134e,Zflg                    
    add         param_3,R0                              
    brbc        LAB_code_001325,Cflg                    
    rjmp        LAB_code_00134e                         
LAB_code_001341:              
    cpi         param_4,0x1                             
    brbs        LAB_code_00134e,Zflg                    
    and         param_4,param_4                         
    brbs        LAB_code_00134c,Zflg                    
    cpi         param_4,0x2                             
    brbs        LAB_code_00134e,Zflg                    
    eor         param_4,param_4                         
    neg         param_3                                 
    adc         param_3,param_4                         
    neg         param_3                                 
    brbc        LAB_code_00134e,Nflg                    
LAB_code_00134c:              
    clc                                                 
    rjmp        LAB_code_00134f                         
LAB_code_00134e:              
    bset        Cflg                                    
LAB_code_00134f:              
    pop         R0                                      
    pop         R1                                      
    pop         param_4                                 
    pop         param_4                                 
    pop         R17                                     
    pop         param_2                                 
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001356(void)
    push        R22                                     
    push        R18                                     
    eor         R18,R18                                 
    eor         R20,R20                                 
    eor         R21,R21                                 
LAB_code_00135b:              
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
    mov         R22,R16                                 
    cpi         R16,0x30                                
    brbs        LAB_code_001374,Cflg                    
    cpi         R16,0x3a                                
    brbs        LAB_code_001366,Cflg                    
    cpi         R16,0x41                                
    brbs        LAB_code_001374,Cflg                    
    cpi         R16,0x47                                
    brbc        LAB_code_001374,Cflg                    
    subi        R22,0x7                                 
LAB_code_001366:              
    subi        R22,0x30                                
    add         R20,R20                                 
    adc         R21,R21                                 
    add         R20,R20                                 
    adc         R21,R21                                 
    add         R20,R20                                 
    adc         R21,R21                                 
    add         R20,R20                                 
    adc         R21,R21                                 
    or          R20,R22                                 
    inc         R18                                     
    cpi         R18,0x4                                 
    brbc        LAB_code_00135b,Zflg                    
    rcall       FUN_code_00141e                         ;undefined FUN_code_00141e(void)
LAB_code_001374:              
    and         R18,R18                                 
    brbs        LAB_code_001378,Zflg                    
    clc                                                 
    rjmp        LAB_code_001379                         
LAB_code_001378:              
    bset        Cflg                                    
LAB_code_001379:              
    pop         R18                                     
    pop         R22                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_00137c(void)
    push        R16                                     
    push        R17                                     
    push        R18                                     
    mov         R18,R16                                 
    swap        R17                                     
    mov         R16,R17                                 
    rcall       FUN_code_001390                         ;undefined FUN_code_001390(void)
    swap        R17                                     
    mov         R16,R17                                 
    rcall       FUN_code_001390                         ;undefined FUN_code_001390(void)
    swap        R18                                     
    mov         R16,R18                                 
    rcall       FUN_code_001390                         ;undefined FUN_code_001390(void)
    swap        R18                                     
    mov         R16,R18                                 
    rcall       FUN_code_001390                         ;undefined FUN_code_001390(void)
    pop         R18                                     
    pop         R17                                     
    pop         R16                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001390(void)
    andi        R16,0xf                                 
    subi        R16,0xd0                                
    cpi         R16,0x3a                                
    brbs        LAB_code_001395,Cflg                    
    subi        R16,0xf9                                
LAB_code_001395:              
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001397(void)
    push        R20                                     
    ldi         R20,0x2                                 
    bset        Cflg                                    
    rjmp        LAB_code_0013aa                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_00139b(void)
    push        R20                                     
    ldi         R20,0x1                                 
    bset        Cflg                                    
    rjmp        LAB_code_0013aa                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_00139f(void)
    push        R20                                     
    ldi         R20,0x3                                 
    bset        Cflg                                    
    rjmp        LAB_code_0013aa                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0013a3(void)
    push        R20                                     
    eor         R20,R20                                 
    bset        Cflg                                    
    rjmp        LAB_code_0013aa                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_0013a7(void)
    push        R20                                     
    eor         R20,R20                                 
    clc                                                 
LAB_code_0013aa:              
    push        R14                                     
    push        R15                                     
    push        R16                                     
    push        R17                                     
    push        R18                                     
    push        R19                                     
    push        R25                                     
    push        Zlo                                     
    push        Zhi                                     
    eor         R15,R15                                 
    brbc        LAB_code_0013bd,Cflg                    
    inc         R15                                     
    and         R17,R17                                 
    brbc        LAB_code_0013bd,Nflg                    
    neg         R15                                     
    neg         R16                                     
    brbc        LAB_code_0013bc,Cflg                    
    inc         R17                                     
LAB_code_0013bc:              
    neg         R17                                     
LAB_code_0013bd:              
    ldi         Zlo,0x56                                
    ldi         Zhi,0x21                                
    eor         R19,R19                                 
    st          Z,R19                                   
LAB_code_0013c1:              
    ldi         R18,0xd                                 
    eor         R14,R14                                 
LAB_code_0013c3:              
    subi        R17,0xa0                                
    sbrs        R14,0x0                                 
    brbs        LAB_code_0013c8,Cflg                    
    bset        Cflg                                    
    rjmp        LAB_code_0013ca                         
LAB_code_0013c8:              
    subi        R17,0x60                                
    clc                                                 
LAB_code_0013ca:              
    adc         R16,R16                                 
    adc         R17,R17                                 
    adc         R14,R14                                 
    dec         R18                                     
    brbc        LAB_code_0013c3,Zflg                    
    mov         R18,R17                                 
    ror         R14                                     
    ror         R18                                     
    andi        R17,0x1f                                
    swap        R18                                     
    andi        R18,0xf                                 
    subi        R18,0xd0                                
    st          -Z,R18                                  
    inc         R19                                     
    cp          R20,R19                                 
    brbc        LAB_code_0013dc,Zflg                    
    ldi         R18,0x2e                                
    st          -Z,R18                                  
LAB_code_0013dc:              
    and         R17,R17                                 
    brbc        LAB_code_0013c1,Zflg                    
    and         R16,R16                                 
    brbc        LAB_code_0013c1,Zflg                    
    cp          R20,R19                                 
    brbs        LAB_code_0013ee,Sflg                    
    ldi         R18,0x30                                
    st          -Z,R18                                  
    brbs        LAB_code_0013ee,Zflg                    
LAB_code_0013e5:              
    inc         R19                                     
    cp          R19,R20                                 
    brbs        LAB_code_0013ea,Zflg                    
    st          -Z,R18                                  
    rjmp        LAB_code_0013e5                         
LAB_code_0013ea:              
    ldi         R18,0x2e                                
    st          -Z,R18                                  
    ldi         R18,0x30                                
    st          -Z,R18                                  
LAB_code_0013ee:              
    ldi         R17,0x50                                
    and         R20,R20                                 
    brbs        LAB_code_0013f2,Zflg                    
    dec         R17                                     
LAB_code_0013f2:              
    ldi         R16,0x20                                
    and         R15,R15                                 
    brbs        LAB_code_0013fa,Zflg                    
    brbc        LAB_code_0013f7,Nflg                    
    ldi         R16,0x2d                                
LAB_code_0013f7:              
    st          -Z,R16                                  
    ldi         R16,0x20                                
    dec         R17                                     
LAB_code_0013fa:              
    cp          Zlo,R17                                 
    brbs        LAB_code_0013fe,Zflg                    
    st          -Z,R16                                  
    rjmp        LAB_code_0013fa                         
LAB_code_0013fe:              
    adiw        Z,0x1                                   
LAB_code_0013ff:              
    ld          R16,Z+                                  
    and         R16,R16                                 
    brbs        LAB_code_001404,Zflg                    
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    rjmp        LAB_code_0013ff                         
LAB_code_001404:              
    pop         Zhi                                     
    pop         Zlo                                     
    pop         R25                                     
    pop         R19                                     
    pop         R18                                     
    pop         R17                                     
    pop         R16                                     
    pop         R15                                     
    pop         R14                                     
    pop         R20                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_00140f(void)
    ldi         Zlo,0x84                                
    ldi         Zhi,0x29                                
    rcall       FUN_code_001413                         ;undefined FUN_code_001413(void)
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001413(void)
    push        R16                                     
LAB_code_001414:              
    lpm         R16,Z+                                  
    and         R16,R16                                 
    brbs        LAB_code_001419,Zflg                    
    rcall       FUN_code_001456                         ;undefined FUN_code_001456(void)
    rjmp        LAB_code_001414                         
LAB_code_001419:              
    pop         R16                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_00141b(void)
    push        R19                                     
    ldi         R19,0x1                                 
    rjmp        LAB_code_001420                         
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_00141e(void)
    push        R19                                     
    eor         R19,R19                                 
LAB_code_001420:              
    push        R17                                     
    push        R18                                     
    push        R20                                     
    push        Zlo                                     
    push        Zhi                                     
    ldi         Zlo,0x0                                 
    ldi         Zhi,0x20                                
LAB_code_001427:              
    cli                                                 
    ld          R17,Z                                   
    ldd         R16,Z+0x1                               
    bset        Iflg                                    
    cp          R16,R17                                 
    brbs        LAB_code_001427,Zflg                    
    inc         R17                                     
    andi        R17,0x3f                                
    lds         R20,iospace:PORTF_OUT                   ;= ??
    sbrs        R20,0x0                                 
    rjmp        LAB_code_00143a                         
    mov         R18,R17                                 
    subi        R18,0xec                                
    andi        R18,0x3f                                
    cpse        R16,R18                                 
    ldi         R20,0x1                                 
    sts         iospace:PORTF_OUTCLR,R20                ;= ??
LAB_code_00143a:              
    eor         R18,R18                                 
    ldi         Zlo,0x7                                 
    ldi         Zhi,0x20                                
    add         Zlo,R17                                 
    adc         Zhi,R18                                 
    ld          R18,Z                                   
    sts         sram:DAT_mem_2000,R17                   ;= ??
    cpi         R18,0xd                                 
    brbc        LAB_code_001449,Zflg                    
    lds         R17,sram:DAT_mem_2005                   ;= ??
    dec         R17                                     
    sts         sram:DAT_mem_2005,R17                   ;= ??
LAB_code_001449:              
    cpi         R18,0x60                                
    brbs        LAB_code_00144e,Sflg                    
    brbs        LAB_code_00144e,Zflg                    
    sbrs        R19,0x0                                 
    andi        R18,0x5f                                
LAB_code_00144e:              
    mov         R16,R18                                 
    pop         Zhi                                     
    pop         Zlo                                     
    pop         R20                                     
    pop         R18                                     
    pop         R17                                     
    pop         R19                                     
    ret                                                 
;************************************************************************************************
;*                                           FUNCTION                                           *
;************************************************************************************************
;undefined FUN_code_001456(void)
    push        Zhi                                     
    push        Zlo                                     
    push        R20                                     
    push        R19                                     
    push        R18                                     
    push        R17                                     
    ldi         Zlo,0x2                                 
    ldi         Zhi,0x20                                
LAB_code_00145e:              
    cli                                                 
    ld          R17,Z+                                  
    ld          R18,Z+                                  
    cp          R17,R18                                 
    brbs        LAB_code_001474,Zflg                    
    inc         R18                                     
    cp          R17,R18                                 
    brbc        LAB_code_001469,Zflg                    
    bset        Iflg                                    
    sbiw        Z,0x2                                   
    rjmp        LAB_code_00145e                         
LAB_code_001469:              
    ldi         Zlo,0x47                                
    ldi         Zhi,0x20                                
    eor         R17,R17                                 
    add         Zlo,R18                                 
    adc         Zhi,R17                                 
    st          Z,R16                                   
    ldi         Zlo,0x3                                 
    ldi         Zhi,0x20                                
    st          Z,R18                                   
    bset        Iflg                                    
    rjmp        LAB_code_001484                         
LAB_code_001474:              
    inc         R18                                     
    ld          R19,Z                                   
    and         R19,R19                                 
    brbs        LAB_code_001469,Zflg                    
    lds         R20,iospace:USARTF0_STATUS              ;= ??
    sbrs        R20,0x5                                 
    rjmp        LAB_code_001469                         
    sts         iospace:USARTF0,R16                     ;= ??
    lds         R20,iospace:USARTF0_CTRLA               ;= ??
    ori         R20,0x2                                 
    sts         iospace:USARTF0_CTRLA,R20               ;= ??
    bset        Iflg                                    
LAB_code_001484:              
    pop         R17                                     
    pop         R18                                     
    pop         R19                                     
    pop         R20                                     
    pop         Zlo                                     
    pop         Zhi                                     
    ret  
