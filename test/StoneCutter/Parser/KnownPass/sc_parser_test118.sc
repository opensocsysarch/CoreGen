#-- sc_parser_test118.sc

regclass foo( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)
regclass bar( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)


def fooN(n p foo foo){
  u8 intvar1, intvar2

  u8 test1 = 0
  test1 = 1

  while( intvar1 > intvar2 ){
    n = n << p
  }

  intvar1 = intvar2
}
