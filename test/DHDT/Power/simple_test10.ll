; ModuleID = 'stonecutter.sc'
source_filename = "stonecutter.sc"

@ra = external global i64 #0
@rb = external global i64 #1
@rt = external global i64 #2
@opc = external global i64 #3
@func = external global i64 #4
@imm = external global i64 #5
@r0 = external global i64 #6
@r1 = external global i64 #7
@r2 = external global i64 #8
@r3 = external global i64 #9
@r4 = external global i64 #10
@r5 = external global i64 #11
@r6 = external global i64 #12
@r7 = external global i64 #13
@r8 = external global i64 #14
@r9 = external global i64 #15
@r10 = external global i64 #16
@r11 = external global i64 #17
@r12 = external global i64 #18
@r13 = external global i64 #19
@r14 = external global i64 #20
@r15 = external global i64 #21
@r16 = external global i64 #22
@r17 = external global i64 #23
@r18 = external global i64 #24
@r19 = external global i64 #25
@r20 = external global i64 #26
@r21 = external global i64 #27
@r22 = external global i64 #28
@r23 = external global i64 #29
@r24 = external global i64 #30
@r25 = external global i64 #31
@r26 = external global i64 #32
@r27 = external global i64 #33
@r28 = external global i64 #34
@r29 = external global i64 #35
@r30 = external global i64 #36
@r31 = external global i64 #37
@GPR = external global i64 #38
@pc = external global i64 #39
@exc = external global i64 #40
@ne = external global i64 #41
@eq = external global i64 #42
@gt = external global i64 #43
@lt = external global i64 #44
@gte = external global i64 #45
@lte = external global i64 #46
@sp = external global i64 #47
@fp = external global i64 #48
@rp = external global i64 #49
@CTRL = external global i64 #50

declare i64 @OUT(i64)

declare i64 @IN(i64)

declare i64 @NOP()

declare i64 @BSEL(i64, i64, i64)

declare i64 @FENCE()

declare i64 @LSS(i64)

declare i64 @CONCAT(i64, i64)

declare i64 @MERGE(i64, i64, i64)

declare i64 @EXTRACTZ(i64, i64, i64)

declare i64 @EXTRACTS(i64, i64, i64)

declare i64 @INSERTZ(i64, i64, i64)

declare i64 @INSERTS(i64, i64, i64)

declare i64 @COMPRESSM(i64, i64)

declare i64 @COMPRESS(i64)

declare i64 @DOZ(i64, i64)

declare i64 @MAJ(i64, i64, i64)

declare i64 @ROTR(i64, i64)

declare i64 @ROTL(i64, i64)

declare i64 @ZEXT(i64, i64)

declare i64 @SEXT(i64, i64)

declare i64 @CTZ(i64)

declare i64 @CLZ(i64)

declare i64 @POPCOUNT(i64)

declare i64 @REVERSE(i64)

declare i64 @NEG(i64)

declare i64 @NOT(i64)

declare i64 @STOREELEM(i64, i64, i64)

declare i64 @LOADELEM(i64, i64)

declare i64 @STORE(i64, i64)

declare i64 @LOAD(i64)

declare i64 @MIN(i64, i64)

declare i64 @MAX(i64, i64)

define i64 @add() #51 {
entry.add:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %addtmp = add i64 %rb, %ra
  store i64 %addtmp, i64* @rt, align 4
  ret i64 %addtmp
}

define i64 @sub() #51 {
entry.sub:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %subtmp = sub i64 %ra, %rb
  store i64 %subtmp, i64* @rt, align 4
  ret i64 %subtmp
}

define i64 @mul() #51 {
entry.mul:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %multmp = mul i64 %rb, %ra
  store i64 %multmp, i64* @rt, align 4
  ret i64 %multmp
}

define i64 @div() #51 {
entry.div:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %divtmp = udiv exact i64 %ra, %rb
  store i64 %divtmp, i64* @rt, align 4
  ret i64 %divtmp
}

define i64 @divu() #51 {
entry.divu:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %divtmp = udiv exact i64 %ra, %rb
  store i64 %divtmp, i64* @rt, align 4
  ret i64 %divtmp
}

define i64 @sll() #51 {
entry.sll:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %shfltmp = shl i64 %ra, %rb
  store i64 %shfltmp, i64* @rt, align 4
  ret i64 %shfltmp
}

define i64 @srl() #51 {
entry.srl:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %lshfrtmp = lshr i64 %ra, %rb
  store i64 %lshfrtmp, i64* @rt, align 4
  ret i64 %lshfrtmp
}

define i64 @sra() #51 {
entry.sra:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %lshfrtmp = lshr i64 %ra, %rb
  store i64 %lshfrtmp, i64* @rt, align 4
  ret i64 %lshfrtmp
}

define i64 @and() #51 {
entry.and:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %andtmp = and i64 %rb, %ra
  store i64 %andtmp, i64* @rt, align 4
  ret i64 %andtmp
}

define i64 @or() #51 {
entry.or:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %ortmp = or i64 %rb, %ra
  store i64 %ortmp, i64* @rt, align 4
  ret i64 %ortmp
}

define i64 @nand() #51 {
entry.nand:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %andtmp = and i64 %rb, %ra
  %calltmp = call i64 @NOT(i64 %andtmp)
  store i64 %calltmp, i64* @rt, align 4
  ret i64 %calltmp
}

define i64 @nor() #51 {
entry.nor:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %ortmp = or i64 %rb, %ra
  %calltmp = call i64 @NOT(i64 %ortmp)
  store i64 %calltmp, i64* @rt, align 4
  ret i64 %calltmp
}

define i64 @xor() #51 {
entry.xor:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %xortmp = xor i64 %rb, %ra
  store i64 %xortmp, i64* @rt, align 4
  ret i64 %xortmp
}

define i64 @cmp.ne() #51 {
entry.cmp.ne:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %cmpne = icmp eq i64 %ra, %rb
  %. = select i1 %cmpne, i64 0, i64 2
  store i64 %., i64* @rt, align 4
  ret i64 undef
}

define i64 @cmp.eq() #51 {
entry.cmp.eq:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %cmpeq = icmp eq i64 %ra, %rb
  %. = select i1 %cmpeq, i64 3, i64 0
  store i64 %., i64* @rt, align 4
  ret i64 undef
}

define i64 @cmp.gt() #51 {
entry.cmp.gt:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %cmptmp = icmp ugt i64 %ra, %rb
  %. = select i1 %cmptmp, i64 4, i64 0
  store i64 %., i64* @rt, align 4
  ret i64 undef
}

define i64 @cmp.lt() #51 {
entry.cmp.lt:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %cmptmp = icmp ult i64 %ra, %rb
  %. = select i1 %cmptmp, i64 5, i64 0
  store i64 %., i64* @rt, align 4
  ret i64 undef
}

define i64 @cmp.gte() #51 {
entry.cmp.gte:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %cmptmp = icmp ult i64 %ra, %rb
  %. = select i1 %cmptmp, i64 0, i64 6
  store i64 %., i64* @rt, align 4
  ret i64 undef
}

define i64 @cmp.lte() #51 {
entry.cmp.lte:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %cmptmp = icmp ugt i64 %ra, %rb
  %. = select i1 %cmptmp, i64 0, i64 7
  store i64 %., i64* @rt, align 4
  ret i64 undef
}

define i64 @lb() #51 {
entry.lb:
  %ra = load i64, i64* @ra, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %ra
  %calltmp = call i64 @LOADELEM(i64 %addtmp, i64 8)
  %calltmp1 = call i64 @SEXT(i64 %calltmp, i64 7)
  store i64 %calltmp1, i64* @rt, align 4
  ret i64 %calltmp1
}

define i64 @lh() #51 {
entry.lh:
  %ra = load i64, i64* @ra, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %ra
  %calltmp = call i64 @LOADELEM(i64 %addtmp, i64 16)
  %calltmp1 = call i64 @SEXT(i64 %calltmp, i64 15)
  store i64 %calltmp1, i64* @rt, align 4
  ret i64 %calltmp1
}

define i64 @lw() #51 {
entry.lw:
  %ra = load i64, i64* @ra, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %ra
  %calltmp = call i64 @LOADELEM(i64 %addtmp, i64 32)
  %calltmp1 = call i64 @SEXT(i64 %calltmp, i64 31)
  store i64 %calltmp1, i64* @rt, align 4
  ret i64 %calltmp1
}

define i64 @ld() #51 {
entry.ld:
  %ra = load i64, i64* @ra, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %ra
  %calltmp = call i64 @LOADELEM(i64 %addtmp, i64 64)
  store i64 %calltmp, i64* @rt, align 4
  ret i64 %calltmp
}

define i64 @sb() #51 {
entry.sb:
  %ra = load i64, i64* @ra, align 4
  %rt = load i64, i64* @rt, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %rt
  %calltmp = call i64 @STOREELEM(i64 %ra, i64 %addtmp, i64 8)
  ret i64 %calltmp
}

define i64 @sh() #51 {
entry.sh:
  %ra = load i64, i64* @ra, align 4
  %calltmp = call i64 @SEXT(i64 %ra, i64 15)
  %rt = load i64, i64* @rt, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %rt
  %calltmp1 = call i64 @STOREELEM(i64 %calltmp, i64 %addtmp, i64 16)
  ret i64 %calltmp1
}

define i64 @sw() #51 {
entry.sw:
  %ra = load i64, i64* @ra, align 4
  %calltmp = call i64 @SEXT(i64 %ra, i64 31)
  %rt = load i64, i64* @rt, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %rt
  %calltmp1 = call i64 @STOREELEM(i64 %calltmp, i64 %addtmp, i64 32)
  ret i64 %calltmp1
}

define i64 @sd() #51 {
entry.sd:
  %ra = load i64, i64* @ra, align 4
  %rt = load i64, i64* @rt, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %rt
  %calltmp = call i64 @STOREELEM(i64 %ra, i64 %addtmp, i64 64)
  ret i64 %calltmp
}

define i64 @lbu() #51 {
entry.lbu:
  %ra = load i64, i64* @ra, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %ra
  %calltmp = call i64 @LOADELEM(i64 %addtmp, i64 8)
  %calltmp1 = call i64 @ZEXT(i64 %calltmp, i64 7)
  store i64 %calltmp1, i64* @rt, align 4
  ret i64 %calltmp1
}

define i64 @lhu() #51 {
entry.lhu:
  %ra = load i64, i64* @ra, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %ra
  %calltmp = call i64 @LOADELEM(i64 %addtmp, i64 16)
  %calltmp1 = call i64 @ZEXT(i64 %calltmp, i64 15)
  store i64 %calltmp1, i64* @rt, align 4
  ret i64 %calltmp1
}

define i64 @lwu() #51 {
entry.lwu:
  %ra = load i64, i64* @ra, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %ra
  %calltmp = call i64 @LOADELEM(i64 %addtmp, i64 32)
  %calltmp1 = call i64 @ZEXT(i64 %calltmp, i64 31)
  store i64 %calltmp1, i64* @rt, align 4
  ret i64 %calltmp1
}

define i64 @sbu() #51 {
entry.sbu:
  %ra = load i64, i64* @ra, align 4
  %calltmp = call i64 @ZEXT(i64 %ra, i64 7)
  %rt = load i64, i64* @rt, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %rt
  %calltmp1 = call i64 @STOREELEM(i64 %calltmp, i64 %addtmp, i64 8)
  ret i64 %calltmp1
}

define i64 @shu() #51 {
entry.shu:
  %ra = load i64, i64* @ra, align 4
  %calltmp = call i64 @ZEXT(i64 %ra, i64 15)
  %rt = load i64, i64* @rt, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %rt
  %calltmp1 = call i64 @STOREELEM(i64 %calltmp, i64 %addtmp, i64 16)
  ret i64 %calltmp1
}

define i64 @swu() #51 {
entry.swu:
  %ra = load i64, i64* @ra, align 4
  %calltmp = call i64 @ZEXT(i64 %ra, i64 31)
  %rt = load i64, i64* @rt, align 4
  %imm = load i64, i64* @imm, align 4
  %addtmp = add i64 %imm, %rt
  %calltmp1 = call i64 @STOREELEM(i64 %calltmp, i64 %addtmp, i64 32)
  ret i64 %calltmp1
}

define i64 @not() #51 {
entry.not:
  %ra = load i64, i64* @ra, align 4
  %calltmp = call i64 @NOT(i64 %ra)
  store i64 %calltmp, i64* @rt, align 4
  ret i64 %calltmp
}

define i64 @bra() #51 {
entry.bra:
  %rt = load i64, i64* @rt, align 4
  store i64 %rt, i64* @pc, align 4
  ret i64 %rt
}

define i64 @br() #51 {
entry.br:
  %pc = load i64, i64* @pc, align 4
  %rt = load i64, i64* @rt, align 4
  %addtmp = add i64 %rt, %pc
  store i64 %addtmp, i64* @pc, align 4
  ret i64 %addtmp
}

define i64 @cadd() #52 {
entry.cadd:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %addtmp = add i64 %rb, %ra
  store i64 %addtmp, i64* @rt, align 4
  ret i64 %addtmp
}

define i64 @brac() #52 {
entry.brac:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %cmpeq = icmp eq i64 %ra, %rb
  %rt = load i64, i64* @rt, align 4
  %pc = load i64, i64* @pc, align 4
  %addtmp = add i64 %pc, 4
  %storemerge = select i1 %cmpeq, i64 %rt, i64 %addtmp
  store i64 %storemerge, i64* @pc, align 4
  ret i64 undef
}

define i64 @brc() #52 {
entry.brc:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %cmpeq = icmp eq i64 %ra, %rb
  %pc = load i64, i64* @pc, align 4
  %rt = load i64, i64* @rt, align 4
  %addtmp = add i64 %rt, %pc
  %addtmp2 = add i64 %pc, 4
  %storemerge = select i1 %cmpeq, i64 %addtmp, i64 %addtmp2
  store i64 %storemerge, i64* @pc, align 4
  ret i64 undef
}

define i64 @ladd() #53 {
entry.ladd:
  %ra = load i64, i64* @ra, align 4
  %rb = load i64, i64* @rb, align 4
  %addtmp = add i64 %rb, %ra
  store i64 %addtmp, i64* @rt, align 4
  ret i64 %addtmp
}

attributes #0 = { "field_name"="ra" "fieldtype"="register" "instformat0"="Arith.if" "instformat1"="ReadCtrl.if" "instformat2"="WriteCtrl.if" "instwidth0"="5" "instwidth1"="5" "instwidth2"="5" "regclasscontainer0"="GPR" "regclasscontainer1"="GPR" "regclasscontainer2"="GPR" }
attributes #1 = { "field_name"="rb" "fieldtype"="register" "instformat0"="Arith.if" "instformat1"="ReadCtrl.if" "instformat2"="WriteCtrl.if" "instwidth0"="5" "instwidth1"="5" "instwidth2"="5" "regclasscontainer0"="GPR" "regclasscontainer1"="CTRL" "regclasscontainer2"="GPR" }
attributes #2 = { "field_name"="rt" "fieldtype"="register" "instformat0"="Arith.if" "instformat1"="ReadCtrl.if" "instformat2"="WriteCtrl.if" "instwidth0"="5" "instwidth1"="5" "instwidth2"="5" "regclasscontainer0"="GPR" "regclasscontainer1"="GPR" "regclasscontainer2"="CTRL" }
attributes #3 = { "field_name"="opc" "fieldtype"="encoding" "instformat0"="Arith.if" "instformat1"="ReadCtrl.if" "instformat2"="WriteCtrl.if" "instwidth0"="5" "instwidth1"="5" "instwidth2"="5" }
attributes #4 = { "field_name"="func" "fieldtype"="encoding" "instformat0"="Arith.if" "instformat1"="ReadCtrl.if" "instformat2"="WriteCtrl.if" "instwidth0"="5" "instwidth1"="5" "instwidth2"="5" }
attributes #5 = { "field_name"="imm" "fieldtype"="immediate" "instformat0"="Arith.if" "instformat1"="ReadCtrl.if" "instformat2"="WriteCtrl.if" "instwidth0"="7" "instwidth1"="7" "instwidth2"="7" }
attributes #6 = { "readOnly"="true" "regclass"="GPR" "register"="r0" }
attributes #7 = { "readWrite"="true" "regclass"="GPR" "register"="r1" }
attributes #8 = { "readWrite"="true" "regclass"="GPR" "register"="r2" }
attributes #9 = { "readWrite"="true" "regclass"="GPR" "register"="r3" }
attributes #10 = { "readWrite"="true" "regclass"="GPR" "register"="r4" }
attributes #11 = { "readWrite"="true" "regclass"="GPR" "register"="r5" }
attributes #12 = { "readWrite"="true" "regclass"="GPR" "register"="r6" }
attributes #13 = { "readWrite"="true" "regclass"="GPR" "register"="r7" }
attributes #14 = { "readWrite"="true" "regclass"="GPR" "register"="r8" }
attributes #15 = { "readWrite"="true" "regclass"="GPR" "register"="r9" }
attributes #16 = { "readWrite"="true" "regclass"="GPR" "register"="r10" }
attributes #17 = { "readWrite"="true" "regclass"="GPR" "register"="r11" }
attributes #18 = { "readWrite"="true" "regclass"="GPR" "register"="r12" }
attributes #19 = { "readWrite"="true" "regclass"="GPR" "register"="r13" }
attributes #20 = { "readWrite"="true" "regclass"="GPR" "register"="r14" }
attributes #21 = { "readWrite"="true" "regclass"="GPR" "register"="r15" }
attributes #22 = { "readWrite"="true" "regclass"="GPR" "register"="r16" }
attributes #23 = { "readWrite"="true" "regclass"="GPR" "register"="r17" }
attributes #24 = { "readWrite"="true" "regclass"="GPR" "register"="r18" }
attributes #25 = { "readWrite"="true" "regclass"="GPR" "register"="r19" }
attributes #26 = { "readWrite"="true" "regclass"="GPR" "register"="r20" }
attributes #27 = { "readWrite"="true" "regclass"="GPR" "register"="r21" }
attributes #28 = { "readWrite"="true" "regclass"="GPR" "register"="r22" }
attributes #29 = { "readWrite"="true" "regclass"="GPR" "register"="r23" }
attributes #30 = { "readWrite"="true" "regclass"="GPR" "register"="r24" }
attributes #31 = { "readWrite"="true" "regclass"="GPR" "register"="r25" }
attributes #32 = { "readWrite"="true" "regclass"="GPR" "register"="r26" }
attributes #33 = { "readWrite"="true" "regclass"="GPR" "register"="r27" }
attributes #34 = { "readWrite"="true" "regclass"="GPR" "register"="r28" }
attributes #35 = { "readWrite"="true" "regclass"="GPR" "register"="r29" }
attributes #36 = { "readWrite"="true" "regclass"="GPR" "register"="r30" }
attributes #37 = { "readWrite"="true" "regclass"="GPR" "register"="r31" }
attributes #38 = { "regfile"="GPR" }
attributes #39 = { "pc"="true" "readOnly"="true" "regclass"="CTRL" "register"="pc" }
attributes #40 = { "readOnly"="true" "regclass"="CTRL" "register"="exc" }
attributes #41 = { "readOnly"="true" "regclass"="CTRL" "register"="ne" }
attributes #42 = { "readOnly"="true" "regclass"="CTRL" "register"="eq" }
attributes #43 = { "readOnly"="true" "regclass"="CTRL" "register"="gt" }
attributes #44 = { "readOnly"="true" "regclass"="CTRL" "register"="lt" }
attributes #45 = { "readOnly"="true" "regclass"="CTRL" "register"="gte" }
attributes #46 = { "readOnly"="true" "regclass"="CTRL" "register"="lte" }
attributes #47 = { "readWrite"="true" "regclass"="CTRL" "register"="sp" }
attributes #48 = { "readWrite"="true" "regclass"="CTRL" "register"="fp" }
attributes #49 = { "readWrite"="true" "regclass"="CTRL" "register"="rp" }
attributes #50 = { "regfile"="CTRL" }
attributes #51 = { "instformat"="Arith.if" }
attributes #52 = { "instformat"="ReadCtrl.if" }
attributes #53 = { "instformat"="WriteCtrl.if" }
