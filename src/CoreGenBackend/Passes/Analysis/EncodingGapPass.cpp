//
// _EncodingGapPass_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/EncodingGapPass.h"

EncodingGapPass::EncodingGapPass(std::ostream *O,
                                             CoreGenDAG *D,
                                             CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1,"EncodingGapPass",
                "Identifies any potential unused gaps in ISA encoding formats",
                false,O,D,E){
}

EncodingGapPass::~EncodingGapPass(){
}

void EncodingGapPass::ExamineFieldGap( unsigned Width,
                                       std::vector<unsigned> StartBits,
                                       std::vector<unsigned> EndBits ){
  std::vector<unsigned> UndefinedBits;
  bool found = false;
  // examine each bit in the instruction field and determine
  // if it falls within a defined field
  for( unsigned i=0; i<Width; i++ ){
    for( unsigned j=0; j<StartBits.size(); j++ ){
      if( (i >= StartBits[j]) && (i<=EndBits[j]) ){
        found = true;
      }
    }
    if( !found ){
      UndefinedBits.push_back(i);
    }
    found = false;
  }

  // print the undefined bits
  std::string Bits = "[";
  for( unsigned i=0; i<UndefinedBits.size(); i++ ){
    Bits += std::to_string(UndefinedBits[i]);
    Bits += " ";
  }
  Bits += "]";
  WriteMsg( "Undefined encoding bits = " + Bits );
}

bool EncodingGapPass::ExamineGap( CoreGenDAG *D, CoreGenInstFormat *IF ){

  // retrieve all the starting and ending bit positions
  // for each field.  insert each into a {start,end} vector.
  // sort the vectors and compare the i'th ending and the i'th+1
  // starting bits to find the gaps.
  std::vector<unsigned> StartBits;
  std::vector<unsigned> EndBits;

  // get all our start+end values
  for( unsigned i=0; i<IF->GetNumFields(); i++ ){
    StartBits.push_back( IF->GetStartBit( IF->GetFieldName(i) ) );
    EndBits.push_back( IF->GetEndBit( IF->GetFieldName(i) ) );
  }

  // make sure we have enough encoding blocks to worry about
  if( StartBits.size() < 2 ){
    return true;
  }

  // sort the vectors
  std::sort(StartBits.begin(), StartBits.end());
  std::sort(EndBits.begin(), EndBits.end());

  bool rtn = true;

  for( unsigned i=0; i<(StartBits.size()-1); i++ ){
    if( StartBits[i+1] != (EndBits[i]+1) ){
      WriteMsg( "Encountered a gap in the encoding of InstFormat = " +
                IF->GetName() + " between bits " + std::to_string(EndBits[i]) +
                " and " + std::to_string(StartBits[i+1]) );
      rtn = false;
    }
  }

  // now count all the bits to see if the number of bits in the format width
  // matches the number of bits defined
  unsigned Width = 0;
  for( unsigned i=0; i<StartBits.size(); i++){
    Width += ((EndBits[i] - StartBits[i])+1);
  }

  if( Width != IF->GetFormatWidth() ){
    WriteMsg( "Field definitions do not encapsulate instruction format width; InstFormat = " +
              IF->GetName() + " defines " + std::to_string(IF->GetFormatWidth()) +
              " bits; found " + std::to_string(Width) + " bits" );
    ExamineFieldGap(IF->GetFormatWidth(),
                    StartBits,
                    EndBits);
    rtn = false;
  }

  return rtn;
}

bool EncodingGapPass::Execute(){
  // Get the correct DAG level: 3
  CoreGenDAG *D1 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D1 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  bool rtn = true;

  // walk the DAG nodes and identify unique ISA nodes
  // for each isa node, process all its instructions
  for( unsigned i=0; i<D1->GetDimSize(); i++ ){
    if( D1->FindNodeByIndex(i)->GetType() == CGInstF ){
      CoreGenInstFormat *IF = static_cast<CoreGenInstFormat *>(D1->FindNodeByIndex(i));
      if( !ExamineGap( D1, IF ) ){
        rtn = false;
      }
    }
  }

  return rtn;
}

// EOF
