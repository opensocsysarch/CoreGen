#-- sc_parser_test150.sc

# Register Class Definitions
regclass GPR( u64 r0<a, g, d>, u64 r1, u64 r2, u64 r3, u64 r4, u64 r5<1234>, u64 r6, u64 r7, u64 r8, u64 r9, u64 r10, u64 r11, u64 r12, u64 r13, u64 r14, u64 r15, u64 r16, u64 r17, u64 r18, u64 r19, u64 r20, u64 r21, u64 r22, u64 r23, u64 r24, u64 r25, u64 r26, u64 r27, u64 r28, u64 r29, u64 r30, u64 r31 )
regclass CTRL( u64 pc[PC], u64 exc, u64 ne, u64 eq, u64 gt, u64 lt, u64 gte, u64 lte, u64 sp, u64 fp, u64 rp )