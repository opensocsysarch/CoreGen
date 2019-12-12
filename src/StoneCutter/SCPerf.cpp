//
// _SCPerf_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCPerf.h"

SCPerf::SCPerf()
  : mStream(NULL) {
  mStream = &std::cout;
}

SCPerf::SCPerf(std::ostream *sb)
  : mStream(sb){
}

SCPerf::~SCPerf(){
}

bool SCPerf::InsertUOp(std::string Inst){
  if( Inst.length() == 0 ){
    return false;
  }

  for( unsigned i=0; i<UOps.size(); i++ ){
    if( UOps[i].first == Inst ){
      UOps[i].second += 1;
      return true;
    }
  }

  UOps.push_back(std::make_pair(Inst,1));

  return true;
}

void SCPerf::PrintStats(){
  *mStream << "=============================================================" << std::endl;
  *mStream << "   Instruction       MicroOps" << std::endl;
  *mStream << "=============================================================" << std::endl;
  for( unsigned i=0; i<UOps.size(); i++ ){
    *mStream << " - " << UOps[i].first << std::setw(20-UOps[i].first.length())
             << UOps[i].second << std::endl;
  }
  *mStream << "=============================================================" << std::endl;
}

// EOF
