#-- sc_parser_test83.sc

regclass foo( float f1_foo(u8 f1.a,u8 f1.b), double d2.bar, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11(bool i11.1,s18 i11.2) )
regclass bar( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)


def foo(a_reg b_reg c_reg x_reg){
  EXTRACTS(a_reg,b_reg);
  c_reg = EXTRACTS(a_reg,b_reg);
}
