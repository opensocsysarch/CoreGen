#-- sc_parser_test47.sc

regclass foo( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)
regclass bar( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)


def foo(N a b c d e f g){
  for ( i = 0; i < a; 1 ){
    for ( j = 0; j < b; 2 ){
      for ( k = 0; k < c; 3 ){
        for ( m = 0; m < d; 4 ){
          for ( o = 0; o < e; 5 ){
            for ( p = 0; p < f; 6 ){
              for ( q = 0; q < g; 7 ){
                if( N > 30 ){
                  if( (i<=3) && ((k>2) && (p<=30)) || (m!=5) ){
                    N = N + i + j + k + m + o + p + q
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}
