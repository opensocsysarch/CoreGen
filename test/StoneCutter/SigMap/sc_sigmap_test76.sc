#-- sc_parser_test76.sc

regclass foo( float f1_foo, double d2.bar, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)
regclass bar( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)


def foo(a_reg b_reg c_reg x_reg){
  COMPRESSM(a_reg,b_reg);
  c_reg = COMPRESSM(a_reg,b_reg);
}
