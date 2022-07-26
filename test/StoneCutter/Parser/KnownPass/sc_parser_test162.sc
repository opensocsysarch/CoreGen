#-- sc_parser_test162.sc

def foo(n){
  u16 tmp0 = 0b1111000011110000
  u16 tmp1 = 0b0000111100001111
  u16 tmpres = tmp0 + tmp1
  if( tmpres > 0b1 ){
    n = tmpres
  }
}
