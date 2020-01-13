//
// _SMLegalizeSigMap_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenSigMap/Passes/SMLegalizeSigMap.h"

SMLegalizeSigMap::SMLegalizeSigMap(std::string N,
                                   std::vector<SCSig *> SigVect,
                                   std::vector<SCTmp *> TempVect,
                                   std::vector<SigType> TopVect)
  : SMPass(N,SigVect,TempVect,TopVect ){
}

SMLegalizeSigMap::~SMLegalizeSigMap(){
}

bool SMLegalizeSigMap::CheckForwardBr(){
  bool rtn = true;
  for( unsigned i=0; i<SigNames.size(); i++ ){
    std::vector<SCSig *> Sigs = GetSigVect(SigNames[i]);
    // check the last uOp for a forward branch
    SCSig *BrSig = Sigs[Sigs.size()-1];
    if( (BrSig->GetType() >= BR_N) && (BrSig->GetType() <= BR_JR) ){
      // this is a branch signal
      if( (BrSig->GetDistanceTrue() > 0) || (BrSig->GetDistanceFalse() > 0) ){
        SetErrorStr( "Final uOp in instruction " + SigNames[i] + " contains a forward branch");
        rtn = false;
      }
    }
  }
  return rtn;
}

bool SMLegalizeSigMap::CheckBackwardBr(){
  bool rtn = true;
  for( unsigned i=0; i<SigNames.size(); i++ ){
    std::vector<SCSig *> Sigs = GetSigVect(SigNames[i]);
    // check the first uOp for a backward branch
    SCSig *BrSig = Sigs[0];
    if( (BrSig->GetType() >= BR_N) && (BrSig->GetType() <= BR_JR) ){
      // this is a branch signal
      if( (BrSig->GetDistanceTrue() < 0) || (BrSig->GetDistanceFalse() < 0) ){
        SetErrorStr( "First uOp in instruction " + SigNames[i] + " contains a backward branch");
        rtn = false;
      }
    }
  }
  return rtn;
}

bool SMLegalizeSigMap::CheckLegalBr(){
  bool rtn = true;
  for( unsigned i=0; i<SigNames.size(); i++ ){
    std::vector<SCSig *> Sigs = GetSigVect(SigNames[i]);
    for( unsigned j=0; j<Sigs.size(); j++ ){
      SCSig *LSig = Sigs[j];
      if( (LSig->GetType() >= BR_N) && (LSig->GetType() <= BR_JR) ){
        // found a branch signal
        signed TargetTrue = LSig->GetDistanceTrue() + j;
        signed TargetFalse = LSig->GetDistanceFalse() + j;
        if( (TargetTrue<0) || (TargetTrue>((signed)(Sigs.size()-1))) ){
          SetErrorStr( "Branch uOp falls outside uOp block in instruction " + SigNames[i] );
          rtn = false;
        }
        if( (TargetFalse<0) || (TargetFalse>((signed)(Sigs.size()-1))) ){
          SetErrorStr( "Branch uOp falls outside uOp block in instruction " + SigNames[i] );
          rtn = false;
        }
      }
    }
  }
  return rtn;
}

std::vector<SCSig *> SMLegalizeSigMap::GetSigVect(std::string Inst){
  std::vector<SCSig *> CSigs;

  for( unsigned i=0; i<Signals.size(); i++ ){
    if( Signals[i]->GetInst() == Inst ){
      CSigs.push_back(Signals[i]);
    }
  }
  return CSigs;
}

bool SMLegalizeSigMap::GetInstVect(){
  for( unsigned i=0; i<Signals.size(); i++ ){
    SigNames.push_back(Signals[i]->GetInst());
  }
  std::sort(SigNames.begin(), SigNames.end());
  SigNames.erase( std::unique( SigNames.begin(), SigNames.end()), SigNames.end() );
  return true;
}

bool SMLegalizeSigMap::Execute(){
  bool rtn = true;

  // build the instruction vector
  if( !GetInstVect() ){
    return false;
  }

  if( !CheckForwardBr() ){
    rtn = false;
  }

  if( !CheckBackwardBr() ){
    rtn = false;
  }

  if( !CheckLegalBr() ){
    rtn = false;
  }

  return rtn;
}

// EOF
