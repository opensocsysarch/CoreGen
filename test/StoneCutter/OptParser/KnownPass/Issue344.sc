#-- StoneCutter source file for ISA=BasicRV32I.ISA


# Instruction Formats
instformat R.if(enc opc:7,reg[GPR] rd:5,enc funct3:3,reg[GPR] rs1:5,reg[GPR] rs2:5,enc funct7:7)
instformat I.if(enc opc:7,reg[GPR] rd:5,enc funct3:3,reg[GPR] rs1:5,imm imm:12)
instformat S.if(enc opc:7,imm imm5:5,enc funct3:3,reg[GPR] rs1:5,reg[GPR] rs2:5,imm imm7:7)
instformat U.if(enc opc:7,reg[GPR] rd:5,imm imm:20)
instformat J.if(enc opc:7,reg[GPR] rd:5,imm imm12:8,imm imm11:2,imm imm1:9,imm imm20:1)

# Register Class Definitions
regclass GPR( u32 x0[RO], u32 x1[RW,CSR], u32 x2[RW,CSR], u32 x3[RW,CSR], u32 x4[RW,CSR], u32 x5[RW,CSR], u32 x6[RW,CSR], u32 x7[RW,CSR], u32 x8[RW,CSR], u32 x9[RW,CSR], u32 x10[RW,CSR], u32 x11[RW,CSR], u32 x12[RW,CSR], u32 x13[RW,CSR], u32 x14[RW,CSR], u32 x15[RW,CSR], u32 x16[RW,CSR], u32 x17[RW,CSR], u32 x18[RW,CSR], u32 x19[RW,CSR], u32 x20[RW,CSR], u32 x21[RW,CSR], u32 x22[RW,CSR], u32 x23[RW,CSR], u32 x24[RW,CSR], u32 x25[RW,CSR], u32 x26[RW,CSR], u32 x27[RW,CSR], u32 x28[RW,CSR], u32 x29[RW,CSR], u32 x30[RW,CSR], u32 x31[RW,CSR], u32 pc[RO,PC], u32 exc[RO], u32 ne[RO], u32 eq[RO], u32 gt[RO], u32 lt[RO], u32 gte[RO], u32 lte[RO] )


# Instruction Definitions
# add
def add:R.if( rd rs1 rs2 )
{
rd = rs1 + rs2
}

# addi
def addi:I.if( rd rs1 imm )
{
rd = rs1 + imm
}

# sub
def sub:R.if( rd rs1 rs2 )
{
rd = rs1 - rs2
}

# sll
def sll:R.if( rd rs1 rs2 )
{
rd = rs1 << rs2
}

# srl
def srl:R.if( rd rs1 rs2 )
{
rd = rs1 >> rs2
}

# sra
def sra:R.if( rd rs1 rs2 )
{
rd = rs1 >> rs2
}

# slli
def slli:I.if( rd rs1 imm )
{
rd = rs1 << (imm & 17)
}

# srli
def srli:I.if( rd rs1 imm )
{
rd = rs1 >> (imm & 17)
}

# srai
def srai:I.if( rd rs1 imm )
{
rd = rs1 >> (imm & 17)
}

# and
def and:R.if( rd rs1 rs2 )
{
rd = rs1 & rs2
}

# andi
def andi:I.if( rd rs1 imm )
{
rd = rs1 & imm
}

# or
def or:R.if( rd rs1 rs2 )
{
rd = rs1 | rs2
}

# ori
def ori:I.if( rd rs1 imm )
{
rd = rs1 | imm
}

# xor
def xor:R.if( rd rs1 rs2 )
{
rd = rs1 ^ rs2
}

# xori
def xori:I.if( rd rs1 imm )
{
rd = rs1 ^ imm
}

# slt
def slt:R.if( rd rs1 rs2 )
{
if( rs1 < rs2){ rd = 1 }else{ rd = 0 }
}

# slti
def slti:I.if( rd rs1 imm )
{
if( rs1 < imm){ rd = 1 }else{ rd = 0 }
}

# sltu
def sltu:R.if( rd rs1 rs2 )
{
if( rs1 < rs2){ rd = 1 }else{ rd = 0 }
}

# sltiu
def sltiu:I.if( rd rs1 imm )
{
if( rs1 < imm){ rd = 1 }else{ rd = 0 }
}

# lb
def lb:I.if( rd rs1 imm )
{
rd = SEXT(LOADELEM(rs1+imm,8),7)
}

# lh
def lh:I.if( rd rs1 imm )
{
rd = SEXT(LOADELEM(rs1+imm,16),15)
}

# lw
def lw:I.if( rd rs1 imm )
{
rd = SEXT(LOADELEM(rs1+imm,32),31)
}

# sb
def sb:S.if( imm5 rs1 rs2 imm7 )
{
STOREELEM(rs1,rd+((imm5 | (imm7 << 5))),8)
}

# sh
def sh:S.if( imm5 rs1 rs2 imm7 )
{
STOREELEM(rs1,rd+((imm5 | (imm7 << 5))),16)
}

# sw
def sw:S.if( imm5 rs1 rs2 imm7 )
{
STOREELEM(rs1,rd+((imm5 | (imm7 << 5))),32)
}

# lbu
def lbu:I.if( rd rs1 imm )
{
rd = ZEXT(LOADELEM(rs1+imm,8), 7)
}

# lhu
def lhu:I.if( rd rs1 imm )
{
rd = ZEXT(LOADELEM(rs1+imm,16), 15)
}

# lui
def lui:U.if( rd imm )
{
rd = (imm << 11) & 4294963200
}

# beq
def beq:S.if( imm5 rs1 rs2 imm7 )
{
u32 offset
if( rs1 == rs2 ){ 
                offset = ((imm5 & 15) | ( (imm7 & 63) << 5) | ( (imm5 & 128) << 11) | (( imm7 & 64) << 12)) << 1
              }else{ 
                offset = pc + 4 
              }
              pc = pc + SEXT(offset,13)

}

# auipc
def auipc:U.if( rd imm )
{
rd = ((imm << 11) & 4294963200)+pc
}

# bne
def bne:S.if( imm5 rs1 rs2 imm7 )
{
u32 offset
if( rs1 != rs2 ){ 
                offset =  ((imm5 & 15) | ( (imm7 & 63) << 5) | ( (imm5 & 128) << 11) | (( imm7 & 64) << 12)) << 1
              }else{ 
                offset  = pc + 4 
              }
              pc = pc + SEXT(offset,13)

}

# blt
def blt:S.if( imm5 rs1 rs2 imm7 )
{
u32 offset
if( rs1 < rs2 ){ 
                offset = ((imm5 & 15) | ( (imm7 & 63) << 5) | ( (imm5 & 128) << 11) | (( imm7 & 64) << 12)) << 1
              }else{ 
                offset = pc + 4 
              }
              pc = pc + SEXT(offset,13)

}

# bge
def bge:S.if( imm5 rs1 rs2 imm7 )
{
u32 offset
if( rs1 >= rs2 ){ 
                offset = ((imm5 & 15) | ( (imm7 & 63) << 5) | ( (imm5 & 128) << 11) | (( imm7 & 64) << 12)) << 1
              }else{ 
                offset = pc + 4 
              }
              pc = pc + SEXT(offset,13)

}

# bltu
def bltu:S.if( imm5 rs1 rs2 imm7 )
{
u32 offset
if( rs1 < rs2 ){ 
                offset = ((imm5 & 15) | ( (imm7 & 63) << 5) | ( (imm5 & 128) << 11) | (( imm7 & 64) << 12)) << 1
              }else{ 
                offset = pc + 4 
              }
              pc = pc + SEXT(offset,13)

}

# bgeu
def bgeu:S.if( imm5 rs1 rs2 imm7 )
{
u32 offset
if( rs1 >= rs2 ){ 
                offset = ((imm5 & 15) | ( (imm7 & 63) << 5) | ( (imm5 & 128) << 11) | (( imm7 & 64) << 12)) << 1
              }else{ 
                offset = pc + 4 
              }
              pc = pc + SEXT(offset,14)

}

# jal
def jal:J.if( rd imm12 imm11 imm1 imm20 )
{
rd = pc + 4
pc = SEXT( (((imm20 << 20) | (imm12 << 12) | (imm11 << 11) | (imm1 << 10)) << 1),19 )

}

# jalr
def jalr:I.if( rd rs1 imm )
{
rd = pc + 4
pc = (SEXT(imm, 12) + rs1) & 4294967294

}

