#-- sc_codegen_test_2.sc

instformat FORMAT1( enc opcode, enc func, reg[foo] RT )

regclass foo( float f1_foo(u8 f1.a,u8 f1.b), double d2.bar, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11(bool i11.1,s18 i11.2) )
regclass bar( float f1, double d2, bool b3, u7 i4a,
              u14 i5a, u12 i6a, u67 i7a, s1024 i8a, s3 i9a,
              s21 i10a, s19 i11a)


def foo(foo){
  EXTRACTS(i4,i5,i6);
  i7 = EXTRACTS(i4,i5,i6);
}
