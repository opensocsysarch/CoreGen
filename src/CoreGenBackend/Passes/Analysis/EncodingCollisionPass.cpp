//
// _EncodingCollisionPass_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/EncodingCollisionPass.h"
#include <bitset>

EncodingCollisionPass::EncodingCollisionPass(std::ostream *O,
                                             CoreGenDAG *D,
                                             CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,3,"EncodingCollisionPass",
                "Identifies any potential ISA encoding collisions",
                false,O,D,E){
}

EncodingCollisionPass::~EncodingCollisionPass(){
}

bool EncodingCollisionPass::Process64bitInsts(std::vector<CoreGenInst *> Insts,
                                              CoreGenInstFormat *F){

  std::vector<uint64_t> Encs;
  srand(time(NULL));

  for( unsigned i=0; i<Insts.size(); i++ ){
    uint64_t Enc = 0x00ull;

    // get all the required encodings
    // if the field is a code, write the speific code in the encoding mask
    // if the field is a register field or immediate, generate a random input value
    for( unsigned j=0; j<F->GetNumFields(); j++ ){
      unsigned StartBit = F->GetStartBit( F->GetFieldName(j) );
      uint64_t Mask = 0x00ull;

      for( unsigned k=0; k<F->GetFieldWidth(F->GetFieldName(j)); k++ ){
        Mask |= (1 << (uint64_t)(StartBit + k ));
      }

      // OR in the encoding
      if( F->GetFieldType(F->GetFieldName(j)) == CoreGenInstFormat::CGInstCode ){
        CoreGenEncoding *E = Insts[i]->GetEncoding(F->GetFieldName(j));
        if( !E ){
          WriteMsg( "Failed to retrieve required encoding field \"" +
                    F->GetFieldName(j) + "\" from instruction \"" +
                    Insts[i]->GetName() + "\"");
          return false;
        }
#if 0
        std::cout << "StartBit = " << StartBit << std::endl
                  << "Mask     = 0b" << std::bitset<64>(Mask) << std::endl
                  << "Field    = " << E->GetEncoding() << std::endl
                  << "Enc|     = 0b" << std::bitset<64>(E->GetEncoding()<<StartBit) << std::endl;
        //Enc |= ((E->GetEncoding()<<StartBit) & Mask);
#endif
        Enc |= ((E->GetEncoding()<<StartBit));
      }else{
        uint64_t RandField = (uint64_t)(rand()) % Mask;
        Enc |= ((RandField<<StartBit) & Mask);
      }
    }

    // add it to our vector
    Encs.push_back(Enc);
  }

  // now that we have all our values coalesced in a vector
  // look for any collisions
  std::sort( Encs.begin(), Encs.end() );
  std::vector<uint64_t>::iterator it;
  it = std::adjacent_find(Encs.begin(), Encs.end());
  if( it != Encs.end() ){
    std::string IName = Insts[0]->GetISA()->GetName();
    WriteMsg( "Encoding collision in ISA=" + IName );
    return false;
  }

  return true;
}

bool EncodingCollisionPass::ProcessLongInsts(std::vector<CoreGenInst *> Insts,
                                             CoreGenInstFormat *F){
  std::vector<ENCLongInt *> Encs;
  srand(time(NULL));

  // get the total width of the instructions
  unsigned NVals = F->GetFormatWidth()/64;
  if( (F->GetFormatWidth()%64) > 0 ){
    NVals++;
  }

  for( unsigned i=0; i<Insts.size(); i++ ){
    // create a new encoding
    Encs.push_back( new ENCLongInt(F->GetFormatWidth(),NVals) );


    // write each field into the sample encoding
    for( unsigned j=0; j<Insts[i]->GetNumEncodings(); j++ ){
      CoreGenEncoding *E = Insts[i]->GetEncoding(j);
      if( !Encs[Encs.size()-1]->SetMask( F->GetStartBit( E->GetField() ),
                                         F->GetEndBit( E->GetField() ),
                                         E->GetEncoding() ) ){
        // failed to assign mask, return an error
        WriteMsg( "Error setting instruction mask for instruction = " +
                  Insts[i]->GetName() );
        Errno->SetError( CGERR_ERROR, this->GetName() +
                         " : Error setting instruction mask for instruction = " +
                         Insts[i]->GetName() );
        for( unsigned k=0; k<Insts.size(); k++ ){
          delete Encs[k];
        }
        return false;
      }
    }

    // write the register and immediate fields
    for( unsigned j=0; j<F->GetNumFields(); j++ ){
      if( F->GetFieldType(F->GetFieldName(j)) != CoreGenInstFormat::CGInstCode ){
        unsigned StartBit = F->GetStartBit( F->GetFieldName(j) );
        unsigned EndBit   = F->GetEndBit( F->GetFieldName(j) );
        uint64_t Mask = 0x00ull;

        for( unsigned k=0; k<F->GetFieldWidth(F->GetFieldName(j)); k++ ){
          if( k < 64 )
            Mask |= 1 << (uint64_t)(StartBit + k );
        }
        uint64_t RandField = (uint64_t)(rand()) % Mask;
        if( !Encs[Encs.size()-1]->SetMask( StartBit,
                                            EndBit,
                                            RandField ) ){
          // failed to assign mask, return an error
          WriteMsg( "Error setting instruction mask for instruction = " +
                    Insts[i]->GetName() );
          Errno->SetError( CGERR_ERROR, this->GetName() +
                          " : Error setting instruction mask for instruction = " +
                          Insts[i]->GetName() );
          for( unsigned k=0; k<Insts.size(); k++ ){
            delete Encs[k];
          }
          return false;
        }
      }
    }
  }// end insts loop

  // now that we have all our values coalesced in a vector
  // look for any collisions
  std::sort( Encs.begin(), Encs.end() );
  std::vector<ENCLongInt *>::iterator it;
  it = std::adjacent_find(Encs.begin(), Encs.end());
  if( it != Encs.end() ){
    std::string IName = Insts[0]->GetISA()->GetName();
    WriteMsg( "Encoding collision in ISA=" + IName );
    for( unsigned i=0; i<Insts.size(); i++ ){
      delete Encs[i];
    }
    return false;
  }

  // clean up the memory
  for( unsigned i=0; i<Insts.size(); i++ ){
    delete Encs[i];
  }

  return true;
}

bool EncodingCollisionPass::ExamineISANode(CoreGenDAG *D,
                                           unsigned IsaNode){
  CoreGenInstFormat *F = NULL;
  std::vector<CoreGenInst *> Insts;

  // find the first instruction using this ISA node
  // such that we can retrieve the format
  for( unsigned i=0; i<D->GetDimSize(); i++ ){
    if( (D->AdjMat[i][IsaNode] == 1) &&
        (D->FindNodeByIndex(i)->GetType() == CGInst) ){
      // drop the instructions in a vector
      Insts.push_back(static_cast<CoreGenInst *>(D->FindNodeByIndex(i)));
    }
  }

  // we only need to perform further comparisons if
  // we have more than one instruction
  if( Insts.size() > 1 ){
    F = Insts[0]->GetFormat();
  }else{
    return true;
  }

  // process the instruction vector
  if( F->GetFormatWidth() <= 64 ){
    // this is the fast path
    return Process64bitInsts( Insts, F );
  }else{
    // instructions with larger formats are harder to compare
    // use the slow path
    return ProcessLongInsts( Insts, F );
  }

  return true;
}

bool EncodingCollisionPass::Execute(){
  // Get the correct DAG level: 3
  CoreGenDAG *D3 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D3 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  bool rtn = true;

  // walk the DAG nodes and identify unique ISA nodes
  // for each isa node, process all its instructions
  for( unsigned i=0; i<D3->GetDimSize(); i++ ){
    if( D3->FindNodeByIndex(i)->GetType() == CGISA ){
      if( !ExamineISANode(D3,i) ){
        rtn = false;
      }
    }
  }

  return rtn;
}

// EOF
