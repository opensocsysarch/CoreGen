instformat Arith(reg[GPR] ra, reg[GPR] rb,
		reg[GPR] rt, enc opc, enc func,
		imm imm)
instformat ReadCtrl(reg[GPR] ra, reg[CTRL] rb,
                 reg[GPR] rt, enc opc, enc func,
                 imm imm)
regclass GPR( u64 r0, u64 r1, u64 r2,  u64 r3,
           u64 r4, u64 r5, u64 r6, u64 r7, u64 r8 )
regclass CTRL( u64 pc[PC] )

def add:Arith( ra rb rt imm ) {
  rt = ra + rb
}
def lp:Arith( ra rb rt imm ){
  for( i=ra; i < imm; i+1 ){
    rt = rb << 5
  }
}
def brc:ReadCtrl( ra rb rt imm ){
  if( ra == rb ){ pc = pc + rt }
  else{ pc = pc + 4 }
}
