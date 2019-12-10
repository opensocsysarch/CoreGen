//
// _SpadSafetyPass_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/SpadSafetyPass.h"

SpadSafetyPass::SpadSafetyPass(std::ostream *O,
                             CoreGenDAG *D,
                             CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1,"SpadSafetyPass",
                "Checks various safety parameters of the scratchpads",
                false,O,D,E ){
}

SpadSafetyPass::~SpadSafetyPass(){
}

bool SpadSafetyPass::TestAddressCollisions(){

  bool rtn = true;

  // generate a vector of pairs for the addresses
  std::vector<std::pair<uint64_t,uint64_t>> Addrs;

  for( unsigned i=0; i<Spads.size(); i++ ){
    Addrs.push_back( std::make_pair(Spads[i]->GetStartAddr(),
                                    Spads[i]->GetStartAddr() +
                                    (uint64_t)(Spads[i]->GetMemSize())) );

  }

  // check the start addresses for overlap
  unsigned Cur = 0;
  for( auto Outer = Addrs.begin(); Outer != Addrs.end(); ++Outer ){
    for( auto Inner = Addrs.begin(); Inner != Addrs.end(); ++Inner ){
      if( Inner != Outer ){
        auto InnerVal = *Inner;
        auto OuterVal = *Outer;
        // check the starting address
        if( (OuterVal.first >= InnerVal.first) &&
            (OuterVal.first <= InnerVal.second) ){
          WriteMsg( "Identified a scratchpad with an address collision: " +
                    Spads[Cur]->GetName() );
          rtn = false;
        }

        // check the ending address
        if( (OuterVal.second >= InnerVal.first) &&
            (OuterVal.second <= InnerVal.second) ){
          WriteMsg( "Identified a scratchpad with an address collision: " +
                    Spads[Cur]->GetName() );
          rtn = false;
        }
      }
    }
    Cur = Cur + 1;
  }

  return rtn;
}

bool SpadSafetyPass::Execute(){
  // Get the correct DAG level
  CoreGenDAG *D1 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D1 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  for( unsigned i=0; i<D1->GetDimSize(); i++ ){
    CoreGenSpad *Spad = static_cast<CoreGenSpad *>(D1->FindNodeByIndex(i));
    if( Spad->GetType() == CGSpad ){
      Spads.push_back(Spad);
    }
  }

  // check the scratchpad collisions
  if( !TestAddressCollisions() ){
    return false;
  }

  return true;
}

// EOF
