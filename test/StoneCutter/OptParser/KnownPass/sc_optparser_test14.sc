#-- sc_parser_test14.sc

regclass foo( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)
regclass bar( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)


def foo(n p){
  for ( i = 1; i < n; 5 ){
    for ( j = 0; j < p; 1 ){
      n = i+j
    }
  }
}
