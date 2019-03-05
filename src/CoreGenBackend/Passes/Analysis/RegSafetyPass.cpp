//
// _RegSafetyPass_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/RegSafetyPass.h"

RegSafetyPass::RegSafetyPass(std::ostream *O,
                             CoreGenDAG *D,
                             CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,3,"RegSafetyPass",
                "Checks various safety parameters of the register files",
                false,O,D,E ){
}

RegSafetyPass::~RegSafetyPass(){
}

// Look for registers with unitialized indices
bool RegSafetyPass::FindMissingRegIndices(CoreGenReg *R){
  if( !R->IsIndexSet() ){
    WriteMsg( "Identified a register with no initialized index; Register=" +
              R->GetName() );
    return false;
  }
  return true;
}

// Look for registers whose initialized values are beyond the width
bool RegSafetyPass::FindRegWidthRestrictions(CoreGenReg *R){
  if( R->IsFixedValue() ){
    int Width = R->GetWidth();
    if( Width <= 64 ){
      // sub-64 bit
      uint64_t Val = 0x00ull;
      for( int i=0; i<Width; i++ ){
        Val |= (1<<i);
      }

      std::vector<uint64_t> FixedVals;
      if( !R->GetFixedVals(FixedVals) ){
        WriteMsg( "Error retrieving fixed values for Register=" + R->GetName());
        return false;
      }

      if( FixedVals.size() == 0 ){
        WriteMsg( "No fixed values found for Register=" + R->GetName());
        return false;
      }

      if( FixedVals[0] > Val ){
        WriteMsg( "Fixed value for Register=" + R->GetName() +
                  " exceeds maximum register width" );
        return false;
      }
    }else{
      // multi-precision
    }
  }
  return true;
}

bool RegSafetyPass::FindOverlappingSubRegNames(CoreGenReg *R){

  if( R->GetNumSubRegs() == 1 ){
    // no sense in checking against a single register
    return true;
  }

  std::unordered_map<std::string,unsigned> NameMap;

  for( unsigned i=0; i<R->GetNumSubRegs(); i++ ){
    std::string Name;
    unsigned Start;
    unsigned End;
    if( !R->GetSubReg(i,Name,Start,End) ){
      WriteMsg( "Failed to retrieve SubReg details at index=" +
                std::to_string(i) + " for register=" + R->GetName() );
      return false;
    }
    NameMap[Name]++;
  }

  bool rtn = true;

  for( auto val : NameMap ){
    if( val.second > 1 ){
      WriteMsg( "Found duplicate SubReg names in register=" +
                R->GetName() + "; SubReg=" + val.first );
      rtn = false;
    }
  }

  return rtn;
}

bool RegSafetyPass::FindOverlappingSubRegFields(CoreGenReg *R){

  if( R->GetNumSubRegs() == 1 ){
    // no sense in checking against a single register
    return true;
  }

  std::vector<unsigned> StartBits;
  std::vector<unsigned> EndBits;

  for( unsigned i=0; i<R->GetNumSubRegs(); i++ ){
    std::string Name;
    unsigned Start;
    unsigned End;
    if( !R->GetSubReg(i,Name,Start,End) ){
      WriteMsg( "Failed to retrieve SubReg details at index=" +
                std::to_string(i) + " for register=" + R->GetName() );
      return false;
    }
    StartBits.push_back(Start);
    EndBits.push_back(End);
  }

  std::sort(StartBits.begin(), StartBits.end());
  std::sort(EndBits.begin(), EndBits.end());

  bool rtn = true;

  // Examine the overlap
  for( unsigned i=0; i<(StartBits.size()-1); i++ ){
    if( StartBits[i] >= EndBits[i+1] ){
      WriteMsg( "Found overlapping SubReg bitspace for register=" +
                R->GetName() + " at StartBit=" +
                std::to_string(StartBits[i]) + " and EndBit=" +
                std::to_string(EndBits[i+1]) );
      rtn = false;
    }
    if( StartBits[i+1] <= EndBits[i] ){
      WriteMsg( "Found overlapping SubReg bitspace for register=" +
                R->GetName() + " at StartBit=" +
                std::to_string(StartBits[i+1]) + " and EndBit=" +
                std::to_string(EndBits[i]) );
      rtn = false;
    }
  }

  return rtn;
}

bool RegSafetyPass::TestSubRegs(CoreGenReg *R){
  if( R->GetNumSubRegs() == 0 ){
    return true;
  }

  bool rtn = true;

  if( !FindOverlappingSubRegNames(R) ){
    rtn = false;
  }

  if( !FindOverlappingSubRegFields(R) ){
    rtn = false;
  }

  return rtn;
}

bool RegSafetyPass::Execute(){
  // Get the correct DAG level
  CoreGenDAG *D3 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D3 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  bool rtn = true;


  for( unsigned i=0; i<D3->GetDimSize(); i++ ){
    CoreGenReg *REG = static_cast<CoreGenReg *>(D3->FindNodeByIndex(i));
    if( REG->GetType() == CGReg ){
      if( !FindMissingRegIndices(REG) ){
        rtn = false;
      }
      if( !FindRegWidthRestrictions(REG) ){
        rtn = false;
      }
      if( !TestSubRegs(REG) ){
        rtn = false;
      }
    }
  }

  return rtn;
}

// EOF
