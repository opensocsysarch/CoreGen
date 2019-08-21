//
// _CoreGenPseudoInst_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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

bool CoreGenPseudoInst::ValidateSyntax( std::string S ){
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

  // retrieve the instruction format
  CoreGenInstFormat *format = Inst->GetFormat();

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
                         "Syntax structure has a field that does not match the isntruction format; field = " +
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
                         "Syntax structure has a field that does not match the isntruction format; field = " +
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

bool CoreGenPseudoInst::SetSyntax( std::string S ){
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

bool CoreGenPseudoInst::SetNullInst(){
  Inst = nullptr;
  Encodings.clear();
  return true;
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
  return InsertChild(static_cast<CoreGenNode *>(E));
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
