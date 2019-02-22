#-- sc_parser_test112.sc

instformat F1( enc field1, imm field2, reg[foo] field3, reg[bar] field4 )

regclass foo( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)
regclass bar( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)


def foo(a b c){
  do{
    b = b << c
    a = a-1
  }while(a > field1)
}
