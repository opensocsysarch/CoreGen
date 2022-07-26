#-- sc_parser_test161.sc

def foo(n){
  u64 tmp0 = 0xFFFF0000FFFF0000
  u64 tmp1 = 0x0000FFFF0000FFFF
  u64 tmpres = tmp0 + tmp1
  if( tmpres > 0xF ){
    n = tmpres
  }
}
