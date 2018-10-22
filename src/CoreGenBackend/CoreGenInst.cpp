//
// _CoreGenInst_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenInst.h"

CoreGenInst::CoreGenInst(CoreGenErrno *E)
  : CoreGenNode(CGInst,E), isa(nullptr), format(nullptr){
}

CoreGenInst::CoreGenInst(std::string N,
                         CoreGenISA *I,
                         CoreGenInstFormat *F,
                         CoreGenErrno *E)
  : CoreGenNode(CGInst,N,E), isa(I), format(F){
  InsertChild(static_cast<CoreGenNode *>(F));
  InsertChild(static_cast<CoreGenNode *>(I));
}

CoreGenInst::~CoreGenInst(){
    for( unsigned i=0; i<Encodings.size(); i++ ){
      DeleteChild(static_cast<CoreGenNode *>(Encodings[i]));
      delete Encodings[i];
    }
}

bool CoreGenInst::SetISA( CoreGenISA *ISA ){
  if( isa ){
    // isa already exists
    // remove it, including the child node
    DeleteChild(static_cast<CoreGenNode *>(isa));
  }

  isa = ISA;
  InsertChild(static_cast<CoreGenNode *>(ISA));
  return true;
}

bool CoreGenInst::SetFormat( CoreGenInstFormat *Format ){
  if( format ){
    // format already exists
    // remove it, including the child node
    CoreGenNode *DNode = static_cast<CoreGenNode *>(format);
    DeleteChild(DNode);

    for( unsigned i=0; i<Encodings.size(); i++ ){
      DeleteChild(static_cast<CoreGenNode *>(Encodings[i]));
      delete Encodings[i];
      Encodings.erase(Encodings.end()-i);
    }
  }
  // Insert the new one
  format = Format;
  CoreGenNode *Node = static_cast<CoreGenNode *>(Format);
  InsertChild(Node);

  return true;
}

CoreGenEncoding *CoreGenInst::GetEncoding( unsigned I ){
  if( I > (Encodings.size()-1) ){
    return nullptr;
  }

  return Encodings[I];
}

uint64_t CoreGenInst::GetEncoding( std::string F, bool *Success ){
  if( !format ){
    // no format set
    Errno->SetError( CGERR_ERROR, "No format set in instruction: " +
                     this->GetName() );
    *Success = false;
    return 0x00ull;
  }

  if( !format->IsValidField(F) ){
    Errno->SetError( CGERR_ERROR,
                     "No field of that name in this encoding format : Format="
                     + format->GetName() + ";Field=" + F );
    *Success = false;
    return 0x00ull;
  }

  for( unsigned i=0; i<Encodings.size(); i++ ){
    if( Encodings[i]->GetField() == F ){
      *Success = true;
      return Encodings[i]->GetEncoding();
    }
  }

  Errno->SetError( CGERR_ERROR, "No encoding for field=" + F );
  *Success = false;
  return 0x00ull;
}

bool CoreGenInst::SetEncoding( std::string F, uint64_t V ){
  if( !format ){
    // no format set
    Errno->SetError( CGERR_ERROR, "No format set in instruction: " +
                     this->GetName() );
    return false;
  }

  // search for the field name "F"
  if( !format->IsValidField( F ) ){
    Errno->SetError( CGERR_ERROR,
                     "No field of that name is this encoding format : Format="
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
  InsertChild( static_cast<CoreGenNode *>(E) );
  return true;
}

CoreGenInstFormat *CoreGenInst::GetFormat(){
  return format;
}

// EOF
