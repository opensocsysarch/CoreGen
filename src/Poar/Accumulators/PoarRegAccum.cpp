//
// _PoarRegAccum_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/Accumulators/PoarRegAccum.h"

void PoarRegAccum::AddLocalRegister(CoreGenReg *Reg){
  AddNode(static_cast<CoreGenNode *>(Reg));

  if( Reg->IsVector() ){
    AddWidth(Reg->GetWidth() * Reg->GetDimX());
  }else if( Reg->IsMatrix() ){
    AddWidth(Reg->GetWidth() * Reg->GetDimX() * Reg->GetDimY());
  }else{
    AddWidth(Reg->GetWidth());
  }
}

bool PoarRegAccum::SearchCores(std::vector<CoreGenNode *> &Cores){

  // accumulate registers from each core
  // watch for SMT cores and shared registers across cores
  for( unsigned i=0; i<Cores.size(); i++ ){
    CoreGenCore *C = static_cast<CoreGenCore *>(Cores[i]);
    unsigned TU = C->GetNumThreadUnits();

    std::vector<CoreGenNode *> Regs;
    if( !AccumNodeVisitor(Regs,CGReg,Cores[i]) )
      return false;

    for( unsigned j=0; j<Regs.size(); j++ ){
      CoreGenReg *Reg = static_cast<CoreGenReg *>(Regs[j]);

      if( Reg->IsShared() && !Exists(Regs[j]) ){
        // if the register is shared across cores, but not yet accumulated
        AddLocalRegister(Reg);
      }else if( Reg->IsTUSAttr() ){
        // thread unit shared, add it once
        AddLocalRegister(Reg);
      }else{
        // thread unit private, add it TU times
        for( unsigned k=0; k<TU; k++ ){
          AddLocalRegister(Reg);
        }
      }
    }
  }

  return true;
}

bool PoarRegAccum::SearchRegClasses(){
  std::vector<CoreGenNode *> Regs;
  if( !AccumNodes(Regs,CGReg) )
    return false;

  for( unsigned i=0; i<Regs.size(); i++ ){
    AddNode(Regs[i]);
    AddWidth(static_cast<CoreGenReg *>(Regs[i])->GetWidth());
  }

  return true;
}

bool PoarRegAccum::Accum(){
  // determine if we should run or not
  if( !ShouldRun() )
    return true;

  // flag this accumulator as being executed
  FlagRun();

  // Stage 1: Find all the cores
  std::vector<CoreGenNode *> Cores;
  if( !AccumNodes(Cores,CGCore) ){
    return false;
  }

  // Stage 2: Find all the registers
  // if we find a non-zero number of cores, use this as the marker
  // to search for candidate register classes.  otherwise,
  // just search for register classes
  if( Cores.size() > 0 )
    return SearchCores(Cores);
  else
    return SearchRegClasses();

  return true;
}

// EOF
