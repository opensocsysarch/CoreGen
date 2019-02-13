//
// _SCPass_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCPass.h"

SCPass::SCPass(std::string N,
               Module *TM,
               SCOpts *O,
               SCMsg *M)
  : Name(N), Msgs(M), Opts(O), TheModule(TM) {
  if( Opts->IsVerbose() )
    Msgs->PrintRawMsg( "Executing Pass: " + this->GetName() );
}

SCPass::~SCPass(){
}

void SCPass::PrintMsg( MSG_LEVEL L, const std::string M ){
  Msgs->PrintMsg( L, this->GetName() + " : " + M );
}

void SCPass::PrintRawMsg( const std::string M ){
  Msgs->PrintRawMsg( this->GetName() + " : " + M );
}

// EOF
