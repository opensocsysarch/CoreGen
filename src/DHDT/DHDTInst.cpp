//
// _DHDTInst_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/DHDT/DHDTInst.h"

DHDTInst::DHDTInst(std::string InstFile,
                   CoreGenBackend &CG)
  : isOpen(false), LineNo(0), CG(CG) {
  IFile.open(InstFile,std::ios::in);
  if( !IFile.is_open() ){
    std::cout << "Error : could not open instruction file: " << InstFile << std::endl;
  }
  isOpen = true;
}

DHDTInst::~DHDTInst(){
  if( IFile.is_open() ){
    IFile.close();
    isOpen = false;
  }
}

bool DHDTInst::IsComment(std::string Inst){
  if( Inst.length() <= 2 ){
    return true;
  }if( Inst[0] == '#' ){
    return true;
  }else if( (Inst[0] == '/') && (Inst[1] == '/') ){
    return true;
  }

  // not a comment
  return false;
}

DInst *DHDTInst::BuildHexInstPayload(std::string Inst){
  DInst *payload = nullptr;

  // strip the first two characters
  Inst.erase(Inst.begin()+2);

  // allocate the buffer for the appropriate number of bits
  payload = new DInst(Inst, Inst.length()*16);

  // Write the byte pattern (little-endian)
  // we have to parse the string right to left
  unsigned BytePos = 0;
  std::stringstream ss;
  uint16_t TmpInt;
  for( unsigned pos=Inst.length(); pos >= 0; pos-- ){
    std::string Tmp = std::string(1, Inst[pos]);
    ss << std::hex << Tmp;
    ss >> TmpInt;
    if( !payload->Write2Byte(BytePos,TmpInt) ){
      std::cout << "Failed to convert hex to payload at line: "
                << this->GetLineNumber() << std::endl;
      delete payload;
      return nullptr;
    }
    BytePos+=2;
  }

  return payload;
}

DInst *DHDTInst::BuildBinaryInstPayload(std::string Inst){
  DInst *payload = nullptr;

  // strip the first two characters
  Inst.erase(Inst.begin()+2);

  // allocate the buffer for the appropriate number of bits
  payload = new DInst(Inst, Inst.length());

  signed pos  = Inst.length()-1;
  uint8_t Tmp   = 0;
  for( unsigned i=0; i<payload->GetByteLen(); i++ ){
    Tmp = 0;
    for( unsigned j=0; j<8; j++ ){
      if( (pos-j) >= 0 ){
        // read the bit
        if( Inst[pos-j] == '1' ){
          Tmp |= (1<<j);
        }
      }
    }

    if( !payload->WriteByte(i,Tmp) ){
      std::cout << "Failed to convert binary to payload at line: "
                << this->GetLineNumber() << std::endl;
      delete payload;
      return nullptr;
    }
  }

  return payload;
}

bool DHDTInst::CacheInstNodes(){
  CoreGenNode *Top = CG.GetTop();
  if( Top == nullptr ){
    return false;
  }

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGInst ){
      Insts.push_back( static_cast<CoreGenInst *>(Top->GetChild(i)) );
    }
  }

  return true;
}

std::vector<std::string> DHDTInst::GetAsmTokens(std::string Inst, char delim){
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(Inst);
  while(std::getline(tokenStream, token, delim)){
    tokens.push_back(token);
  }

  return tokens;
}

DInst *DHDTInst::AssemblePayload(CoreGenInst *Inst,
                                 std::string AsmArgs,
                                 std::string InstArgs){
  DInst *payload = nullptr;
  std::vector<std::string> AVect = GetAsmTokens(AsmArgs,',');
  std::vector<std::string> IVect = GetAsmTokens(InstArgs,',');
  return payload;
}

DInst *DHDTInst::BuildAsmInstPayload(std::string Inst){

  // first, we need to cache all the instruction objects
  if( Insts.size() == 0 ){
    if( !CacheInstNodes() ){
      std::cout << "Error : CoreGen object is invalid" << std::endl;
      return nullptr;
    }
  }

  // tokenize the instruction string
  std::vector<std::string> AsmTokens = GetAsmTokens(Inst, ' ');

  // now token[0] is the instruction name
  for( unsigned i=0; i<Insts.size(); i++ ){
    if( Insts[i]->IsSyntax() ){
      std::string InstSyntax = Insts[i]->GetSyntax();
      std::vector<std::string> InstTokens = GetAsmTokens(InstSyntax, ' ');

      if( AsmTokens[0] == InstTokens[0] ){
        // found a match, assemble the payload
        return AssemblePayload(Insts[i], AsmTokens[1], InstTokens[1]);
      }
    }
  }

  return nullptr;
}

DInst *DHDTInst::BuildInstPayload(std::string Inst){
  // determine what type of payload it is
  if( (Inst[0] == '0') && (Inst[1] == 'x') ){
    // hex number
    return BuildHexInstPayload(Inst);
  }else if( (Inst[0] == '0') && (Inst[1] == 'b') ){
    // binary number
    return BuildBinaryInstPayload(Inst);
  }else if( isalpha(Inst[0]) ){
    // assembly
    return BuildAsmInstPayload(Inst);
  }

  return nullptr;
}

DInst *DHDTInst::ReadInst(){
  if( !this->IsOpen() )
    return nullptr;

  // end of file?
  if( IFile.eof() )
    return nullptr;

  std::string tp;
  while(getline(IFile,tp)){
    LineNo++;
    if( !IsComment(tp) ){
      // build an instruction payload
      return BuildInstPayload(tp);
    }
  }

  return nullptr;
}

// EOF
