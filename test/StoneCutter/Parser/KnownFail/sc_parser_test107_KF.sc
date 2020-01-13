#-- StoneCutter source file for ISA=BasicRISC.ISA


# Instruction Formats
#instformat Arith.if(reg[GPR] ra:5,reg[GPR] rb:5,reg[GPR] rt:5,enc opc:5,enc func:5,imm imm:7)
instformat Arith.if(reg[GPR] ra,reg[GPR] rb,reg[GPR] rt,enc opc,enc func,imm imm:7)
instformat ReadCtrl.if(reg[GPR] ra:5,reg[CTRL] rb:5,reg[GPR] rt:5,enc opc:5,enc func:5,imm imm:7)
instformat WriteCtrl.if(reg[GPR] ra:5,reg[GPR] rb:5,reg[CTRL] rt:5,enc opc:5,enc func:5,imm imm:7)

# Register Class Definitions
regclass GPR( u64 r0[RW,], u64 r1[RO], u64 r2[CSR], u64 r3[AMS], u64 r4[TUS], u64 r5[Shared], u64 r6[RW,CSR], u64 r7, u64 r8, u64 r9, u64 r10, u64 r11, u64 r12, u64 r13, u64 r14, u64 r15, u64 r16, u64 r17, u64 r18, u64 r19, u64 r20, u64 r21, u64 r22, u64 r23, u64 r24, u64 r25, u64 r26, u64 r27, u64 r28, u64 r29, u64 r30, u64 r31 )
regclass CTRL( u64 pc[PC], u64 exc, u64 ne, u64 eq, u64 gt, u64 lt, u64 gte, u64 lte, u64 sp, u64 fp, u64 rp )


# Instruction Definitions
# add
def add:Arith.if( ra rb rt imm )
{
rt = ra + rb
}

# sub
def sub:Arith.if( ra rb rt imm )
{
rt = ra - rb
}

# mul
def mul:Arith.if( ra rb rt imm )
{
rt = ra * rb
}

# div
def div:Arith.if( ra rb rt imm )
{
rt = ra / rb
}

# divu
def divu:Arith.if( ra rb rt imm )
{
rt = ra / rb
}

# sll
def sll:Arith.if( ra rb rt imm )
{
rt = ra << rb
}

# srl
def srl:Arith.if( ra rb rt imm )
{
rt = ra >> rb
}

# sra
def sra:Arith.if( ra rb rt imm )
{
rt = ra >> rb
}

# and
def and:Arith.if( ra rb rt imm )
{
rt = ra & rb
}

# or
def or:Arith.if( ra rb rt imm )
{
rt = ra | rb
}

# nand
def nand:Arith.if( ra rb rt imm )
{
rt = NOT(ra & rb)
}

# nor
def nor:Arith.if( ra rb rt imm )
{
rt = NOT(ra | rb)
}

# xor
def xor:Arith.if( ra rb rt imm )
{
rt = ra ^ rb
}

# cmp.ne
def cmp.ne:Arith.if( ra rb rt imm )
{
if( ra != rb ){ rt = 2 }else{ rt = 0 }
}

# cmp.eq
def cmp.eq:Arith.if( ra rb rt imm )
{
if( ra == rb ){ rt = 3 }else{ rt = 0 }
}

# cmp.gt
def cmp.gt:Arith.if( ra rb rt imm )
{
if( ra > rb ){ rt = 4 }else{ rt = 0 }
}

# cmp.lt
def cmp.lt:Arith.if( ra rb rt imm )
{
if( ra < rb ){ rt = 5 }else{ rt = 0 }
}

# cmp.gte
def cmp.gte:Arith.if( ra rb rt imm )
{
if( ra >= rb ){ rt = 6 }else{ rt = 0 }
}

# cmp.lte
def cmp.lte:Arith.if( ra rb rt imm )
{
if( ra <= rb ){ rt = 7 }else{ rt = 0 }
}

# lb
def lb:Arith.if( ra rb rt imm )
{
rt = SEXT(LOADELEM(ra+imm,8),7)
}

# lh
def lh:Arith.if( ra rb rt imm )
{
rt = SEXT(LOADELEM(ra+imm,16),15)
}

# lw
def lw:Arith.if( ra rb rt imm )
{
rt = SEXT(LOADELEM(ra+imm,32),31)
}

# ld
def ld:Arith.if( ra rb rt imm )
{
rt = LOADELEM(ra+imm,64)
}

# sb
def sb:Arith.if( ra rb rt imm )
{
STOREELEM(ra,rt+imm,8)
}

# sh
def sh:Arith.if( ra rb rt imm )
{
STOREELEM(SEXT(ra,15),rt+imm,16)
}

# sw
def sw:Arith.if( ra rb rt imm )
{
STOREELEM(SEXT(ra,31),rt+imm,32)
}

# sd
def sd:Arith.if( ra rb rt imm )
{
STOREELEM(ra,rt+imm,64)
}

# lbu
def lbu:Arith.if( ra rb rt imm )
{
rt = ZEXT(LOADELEM(ra+imm,8),7)
}

# lhu
def lhu:Arith.if( ra rb rt imm )
{
rt = ZEXT(LOADELEM(ra+imm,16),15)
}

# lwu
def lwu:Arith.if( ra rb rt imm )
{
rt = ZEXT(LOADELEM(ra+imm,32),31)
}

# sbu
def sbu:Arith.if( ra rb rt imm )
{
STOREELEM(ZEXT(ra,7),rt+imm,8)
}

# shu
def shu:Arith.if( ra rb rt imm )
{
STOREELEM(ZEXT(ra,15),rt+imm,16)
}

# swu
def swu:Arith.if( ra rb rt imm )
{
STOREELEM(ZEXT(ra,31),rt+imm,32)
}

# not
def not:Arith.if( ra rb rt imm )
{
rt = NOT(ra)
}

# bra
def bra:Arith.if( ra rb rt imm )
{
pc = rt
}

# br
def br:Arith.if( ra rb rt imm )
{
pc = pc + rt
}

# cadd
def cadd:ReadCtrl.if( ra rb rt imm )
{
rt = ra + rb
}

# brac
def brac:ReadCtrl.if( ra rb rt imm )
{
if( ra == rb ){ pc = rt }else{ pc = pc + 4 }
}

# brc
def brc:ReadCtrl.if( ra rb rt imm )
{
if( ra == rb ){ pc = pc + rt }else{ pc = pc + 4 }
}

# ladd
def ladd:WriteCtrl.if( ra rb rt imm )
{
rt = ra + rb
}

