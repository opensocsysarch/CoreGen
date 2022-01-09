instformat R(enc opc:7,reg[GPR] rd:5,enc funct3:3,reg[GPR] rs1:5,reg[GPR] rs2:5,enc funct7:7)

regclass GPR( u64 x0[RW], u64 x1[RW], u64 x2[RW], u64 x3[RW], u64 x4[RW],
              u64 x5[RW], u64 x6[RW], u64 x7[RW], u64 x8[RW], u64 x9[RW],
              u64 x10[RW], u64 x11[RW], u64 x12[RW], u64 x13[RW], u64 x14[RW],
              u64 x15[RW], u64 x16[RW], u64 x17[RW], u64 x18[RW], u64 x19[RW],
              u64 x20[RW], u64 x21[RW], u64 x22[RW], u64 x23[RW], u64 x24[RW],
              u64 x25[RW], u64 x26[RW], u64 x27[RW], u64 x28[RW], u64 x29[RW],
              u64 x30[RW], u64 x31[RW] )

#-- Stage0 is implied: fetch+crack

def STAGE_1:VLIW(){
  u64 TmpRS1 = LOADELEM( rs1, 64 )
  u64 TmpRS2 = LOADELEM( rs2, 64 )

  OUT(TmpRS1)
  OUT(TmpRS2)
}

def STAGE_2:VLIW(TmpRS1 ){
  IN(TmpRS1)
  IN(TmpRS2)
}
