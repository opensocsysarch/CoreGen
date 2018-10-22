//
// _CoreGenPseudoInst_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenPseudoInst.h"

CoreGenPseudoInst::CoreGenPseudoInst(std::string N,
                                     CoreGenInst *I,
                                     CoreGenErrno *E)
  : CoreGenNode(CGPInst,N,E), Inst(I), ISA(nullptr) {
  InsertChild(static_cast<CoreGenNode *>(I));
  ISA = I->GetISA();
  InsertChild(static_cast<CoreGenNode *>(ISA));
}

CoreGenPseudoInst::~CoreGenPseudoInst(){
    for( unsigned i=0; i<Encodings.size(); i++ ){
      CoreGenEncoding *E = Encodings[i];
      DeleteChild(static_cast<CoreGenNode *>(E));
      delete E;
    }
}

bool CoreGenPseudoInst::SetTargetInst( CoreGenInst *I ){
  if( Inst ){
    // inst mapping already exists, remove it
    DeleteChild(Inst);

    // now delete all the field mappings
    for( unsigned i=0; i<Encodings.size(); i++ ){
      CoreGenEncoding *E = Encodings[i];
      DeleteChild(static_cast<CoreGenNode *>(E));
      delete E;
      Encodings.erase(Encodings.end()-i);
    }
  }

  // Insert the new one
  Inst = I;
  CoreGenNode *Node = static_cast<CoreGenNode *>(Inst);
  InsertChild(Node);

  // Check the ISA endcoding
  if( ISA ){
    DeleteChild(static_cast<CoreGenNode *>(ISA));
    ISA = Inst->GetISA();
    InsertChild(static_cast<CoreGenNode *>(ISA));
  }

  return true;
}

CoreGenEncoding *CoreGenPseudoInst::GetEncoding( unsigned I ){
  if( I > (Encodings.size()-1) ){
    return nullptr;
  }

  return Encodings[I];
}

bool CoreGenPseudoInst::SetEncoding( std::string F, uint64_t V ){

  if( !Inst ){
    Errno->SetError( CGERR_ERROR,
                     "No instruction base set for pseudo instruction="
                     + this->GetName() );
    return false;
  }

  CoreGenInstFormat *format = Inst->GetFormat();
  if( !format ){
    Errno->SetError( CGERR_ERROR,
                      "No format set for base instruction="
                      + Inst->GetName() );
    return false;
  }

  if( !format->IsValidField( F ) ){
    Errno->SetError( CGERR_ERROR,
                     "No field of that name in this encoding format : Format="
                     + format->GetName() + ";Field=" + F );
    return false;
  }

  CoreGenEncoding *E = new CoreGenEncoding( V,
                                            format->GetFieldWidth(F),
                                            F,
                                            this->GetName(),
                                            Errno );

  // add it to our encoding vector
  Encodings.push_back(E);

  // make it a dependency
  InsertChild(static_cast<CoreGenNode *>(E));

  return true;
}

uint64_t CoreGenPseudoInst::GetEncoding( std::string F ){
  if( !Inst ){
    Errno->SetError( CGERR_ERROR,
                     "No instruction base set for pseudo instruction="
                     + this->GetName() );
    return false;
  }

  CoreGenInstFormat *format = Inst->GetFormat();
  if( !format ){
    Errno->SetError( CGERR_ERROR,
                      "No format set for base instruction="
                      + Inst->GetName() );
    return false;
  }

  if( !format->IsValidField(F) ){
    Errno->SetError( CGERR_ERROR,
                     "No field of that name is this encoding format : Format="
                     + format->GetName() + ";Field=" + F );
    return 0x00ull;
  }

  for( unsigned i=0; i<Encodings.size(); i++ ){
    if( Encodings[i]->GetField() == F ){
      return Encodings[i]->GetEncoding();
    }
  }

  Errno->SetError( CGERR_ERROR, "No encoding for field=" + F );
  return 0x00ull;
}

// EOF
