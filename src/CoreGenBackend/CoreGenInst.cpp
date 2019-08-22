//
// _CoreGenInst_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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

bool CoreGenInst::SetNullFormat(){
  Encodings.clear();
  format = nullptr;
  return true;
}

bool CoreGenInst::SetNullISA(){
  isa = nullptr;
  return true;
}

bool CoreGenInst::ValidateSyntax(std::string S){
  // check the first character
  if( (S[0]=='$') || (S[0]=='%') ){
    Errno->SetError( CGERR_ERROR, "Instruction syntax cannot start with '$' or '%' : " +
                     this->GetName() );
    return false;
  }

  // remove the commas and parenthesis
  std::string TmpS = S;
  std::replace(TmpS.begin(), TmpS.end(), ',', ' ');
  std::replace(TmpS.begin(), TmpS.end(), '(', ' ');
  std::replace(TmpS.begin(), TmpS.end(), ')', ' ');

  // break the syntax into space delimited chunks
  std::istringstream iss(TmpS);
  std::vector<std::string> results;
  for( std::string ts; iss >> ts; ){
    results.push_back(ts);
  }

  // check the syntax of all the variables
  for( unsigned i=1; i<results.size(); i++ ){
    std::string tmp = results[i];

    // check the field names for correct naming convention and class
    if( tmp[0] == '%' ){
      // ensure this is a register class field
      // remove the precending character
      tmp.erase(std::remove(tmp.begin(), tmp.end(), '%'), tmp.end());
      if( !format->IsValidField(tmp) ){
        Errno->SetError( CGERR_ERROR,
                         "Syntax structure has a field that does not match the instruction format; field = " +
                         tmp + "; instruction format = " + format->GetName() + "; instruction = " + 
                         this->GetName() );
        return false;
      }
      if( format->GetFieldType(tmp) != CoreGenInstFormat::CGInstReg ){
        Errno->SetError( CGERR_ERROR,
                         "Syntax structure must designate field=" + tmp +
                         " as a register class using preceding '%'; instruction format=" +
                         format->GetName() + "; instruction = " + this->GetName() );
        return false;
      }
    }else if( tmp[0] == '$' ){
      // ensure this is an immediate field
      // remove the precending character
      tmp.erase(std::remove(tmp.begin(), tmp.end(), '$'), tmp.end());
      if( !format->IsValidField(tmp) ){
        Errno->SetError( CGERR_ERROR,
                         "Syntax structure has a field that does not match the instruction format; field = " +
                         tmp + "; instruction format = " + format->GetName() + "; instruction = " + 
                         this->GetName() );
        return false;
      }
      if( format->GetFieldType(tmp) != CoreGenInstFormat::CGInstImm ){
        Errno->SetError( CGERR_ERROR,
                         "Syntax structure must designate field=" + tmp +
                         " as an immediate using preceding '$'; instruction format=" +
                         format->GetName() + "; instruction = " + this->GetName() );
        return false;
      }
    }else if( tmp[0] == '!'){
      // found a constant immediate value, this is correct
    }else{
      Errno->SetError( CGERR_ERROR,
                       "Syntax structure has incomplete syntax; variable= " +
                       tmp + " must be preceded by a '$' or '%' : " +
                       this->GetName() );
      return false;
    }
  }

  return true;
}

bool CoreGenInst::SetSyntax( std::string S ){
  if( S.length() == 0 ){
    Errno->SetError( CGERR_ERROR, "Syntax string is null: " +
                     this->GetName() );
    return false;
  }
  if( !ValidateSyntax(S) ){
    return false;
  }
  Syntax = S;
  return true;
}

bool CoreGenInst::SetISA( CoreGenISA *ISA ){
  if( isa ){
    // isa already exists
    // remove it, including the child node
    DeleteChild(static_cast<CoreGenNode *>(isa));
  }

  isa = ISA;
  return InsertChild(static_cast<CoreGenNode *>(ISA));
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
  return InsertChild(Node);
}

CoreGenEncoding *CoreGenInst::GetEncoding(std::string S){
  for( unsigned i=0; i<Encodings.size(); i++ ){
    if( Encodings[i]->GetField() == S ){
      return Encodings[i];
    }
  }
  return nullptr;
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
  return InsertChild( static_cast<CoreGenNode *>(E) );
}

CoreGenInstFormat *CoreGenInst::GetFormat(){
  return format;
}

// EOF
