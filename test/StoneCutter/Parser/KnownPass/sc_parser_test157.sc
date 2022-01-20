#-- sc_parser_test157.sc

regclass foo( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)
regclass bar( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)


def foo(n p){
  u64 vec<45>, vec<5>
  u64 mat<5,2>
  n = n / 3
}
