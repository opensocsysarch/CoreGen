#-- sc_parser_test9.sc

regclass foo( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11)
regclass bar( float f1, double d2, bool b3, u7 i4,
              u14 i5, u12 i6, u67 i7, s1024 i8, s3 i9,
              s21 i10, s19 i11, u64 n, u64 p)

def test1(n p){
  pipe p0{
    n = n + p
  }
}

def test2(n p){
  pipe p0{
    p = p * n
    n = n - p
  }
}

def test4(n p){
  pipe p0{
    p = p * n
  }
  pipe p1{
    n = n - p
  }
}

def test3(n p){
  pipe p1{
    n = NOP()
  }
}

def foo(n p){
  pipe p2{
  for ( i = 1; i < n; 1 ){
    n = i+1
    p = p+2
  }
  }
}
