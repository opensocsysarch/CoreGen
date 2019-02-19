#-- sc_parser_test91_KF.sc

instformat F1( enc field1, imm field2, reg[foo] field3, reg[bar] field4 )
instformat F2( enc field1, enc field2, reg[bar] field3, reg[bar] field4 )

regclass foo( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)
regclass bar( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)


def foo(a b c field3){
  u64 newreg
  while( a != b ){
    if( a < 10 ){
      a = a + 1
    }else{
      a = a + 2
      c = a + 5
    }
  }
}
