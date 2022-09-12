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
#include <bitset>

DHDTInst::DHDTInst(std::string InstFile,
                   CoreGenBackend &CG)
  : isOpen(false), LineNo(0), CG(CG) {
  IFile.open(InstFile,std::ios::in);
  if( !IFile.is_open() ){
    std::cout << "Error : could not open instruction file: " << InstFile << std::endl;
  }
  isOpen = true;

  // cache all the instruction nodes
  if( Insts.size() == 0 ){
    if( !CacheInstNodes() ){
      std::cout << "Error : CoreGen object is invalid" << std::endl;
    }
  }

  // cache all the vliw stages
  if( Stages.size() == 0 ){
    if( !CacheVLIWNodes() ){
      std::cout << "Error : CoreGen object is invalid" << std::endl;
    }
  }

}

DHDTInst::~DHDTInst(){
  if( IFile.is_open() ){
    IFile.close();
    isOpen = false;
  }

  if( Masks.size() > 0 )
    Masks.clear();
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

uint8_t DHDTInst::HexToInt(char C){
  switch(C){
  case '1':
    return 1;
    break;
  case '2':
    return 2;
    break;
  case '3':
    return 3;
    break;
  case '4':
    return 4;
    break;
  case '5':
    return 5;
    break;
  case '6':
    return 6;
    break;
  case '7':
    return 7;
    break;
  case '8':
    return 8;
    break;
  case '9':
    return 9;
    break;
  case 'a':
  case 'A':
    return 10;
    break;
  case 'b':
  case 'B':
    return 11;
    break;
  case 'c':
  case 'C':
    return 12;
    break;
  case 'd':
  case 'D':
    return 13;
    break;
  case 'e':
  case 'E':
    return 14;
    break;
  case 'f':
  case 'F':
    return 15;
    break;
  case '0':
  default:
    return 0;
    break;
  }

  return 0;
}

DInst *DHDTInst::BuildHexInstPayload(std::string Inst){
  DInst *payload = nullptr;

  // strip the first two characters
  Inst.erase(0,2);

  // allocate the buffer for the appropriate number of bits
  payload = new DInst("0x"+Inst, Inst.length()*4);

  // Write the byte pattern (little-endian)
  // we have to parse the string right to left
  //unsigned BytePos = payload->GetByteLen()-1;

  signed pos = Inst.length()-1;
  for( unsigned i=0; i<payload->GetByteLen(); i++ ){
    uint8_t Tmp = 0;

    // first nibble
    std::string T1 = std::string(1,Inst[pos]);
    Tmp |= HexToInt(T1[0]);
    pos--;

    // second nibble
    if( (pos) >= 0 ){
      std::string T2 = std::string(1,Inst[pos]);
      Tmp |= (HexToInt(T2[0])<<4);
      pos--;
    }

    if( !payload->WriteByte(i,Tmp) ){
      std::cout << "Failed to convert hex to payload at line: "
                << this->GetLineNumber() << std::endl;
      delete payload;
      return nullptr;
    }
  }

  return payload;
}

DInst *DHDTInst::BuildBinaryInstPayload(std::string Inst){
  DInst *payload = nullptr;

  // strip the first two characters
  Inst.erase(0,2);

  // allocate the buffer for the appropriate number of bits
  payload = new DInst("0b"+Inst, Inst.length());

  signed pos  = Inst.length()-1;
  for( unsigned i=0; i<payload->GetByteLen(); i++ ){
    uint8_t Tmp = 0;
    for( unsigned j=0; j<8; j++ ){
      if( (pos-j) >= 0 ){
        // read the bit
        if( Inst[pos-j] == '1' ){
          Tmp |= (1<<j);
        }
      }
    }

    // reset pos
    pos = (pos-8);

    if( !payload->WriteByte(i,Tmp) ){
      std::cout << "Failed to convert binary to payload at line: "
                << this->GetLineNumber() << std::endl;
      delete payload;
      return nullptr;
    }
  }

  return payload;
}

bool DHDTInst::BuildInstMask(CoreGenInst *Inst){

  DInstMask *M = new DInstMask(Inst,Inst->GetFormat());
  Masks.push_back(M);
  for( unsigned i=0; i<Inst->GetNumEncodings(); i++ ){
    M->AddEncoding(Inst->GetEncoding(i));
  }

  return true;
}

bool DHDTInst::CacheVLIWNodes(){
  CoreGenNode *Top = CG.GetTop();
  if( Top == nullptr ){
    return false;
  }

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGVLIW ){
      Stages.push_back( static_cast<CoreGenVLIWStage *>(Top->GetChild(i)) );
    }
  }

  return true;
}

bool DHDTInst::CacheInstNodes(){
  CoreGenNode *Top = CG.GetTop();
  if( Top == nullptr ){
    return false;
  }

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGInst ){
      Insts.push_back( static_cast<CoreGenInst *>(Top->GetChild(i)) );
      if( !BuildInstMask(Insts[Insts.size()-1]) )
        return false;
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

DInst *DHDTInst::BuildAsmInstPayload(std::string Inst){

  // tokenize the instruction string
  std::vector<std::string> AsmTokens = GetAsmTokens(Inst, ' ');

  // now token[0] is the instruction name
  for( unsigned i=0; i<Insts.size(); i++ ){
    if( Insts[i]->IsSyntax() ){
      std::string InstSyntax = Insts[i]->GetSyntax();
      std::vector<std::string> InstTokens = GetAsmTokens(InstSyntax, ' ');

      if( AsmTokens[0] == InstTokens[0] ){
        // found a match, assemble the payload
        return new DInst(Inst,Insts[i]);
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

std::string DHDTInst::rtrim(const std::string &s){
  const std::string WHITESPACE = " \n\r\t\f\v";
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string DHDTInst::ltrim(const std::string &s){
  const std::string WHITESPACE = " \n\r\t\f\v";
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

std::string DHDTInst::Trim(const std::string &s){
  return rtrim(ltrim(s));
}

DInst *DHDTInst::ReadInst(){
  if( !this->IsOpen() )
    return nullptr;

  // end of file?
  if( IFile.eof() )
    return nullptr;

  std::string tp;
  std::string ts;
  while(getline(IFile,tp)){
    LineNo++;
    ts = Trim(tp);
    if( ts.length() > 0 ){
      if( !IsComment(ts) ){
        // build an instruction payload
        return BuildInstPayload(ts);
      }
    }
  }

  return nullptr;
}

std::string DHDTInst::CrackInst(DInst *Inst){
  std::string Name;

  // Uncomment this line to print debug info
  // Inst->PrintEncBits();

  // see if we've pre-cracked the payload
  if( Inst->GetInstPtr() ){
    return Inst->GetInstPtr()->GetName();
  }

  // check the payload to see if it matches an instruction mask
  for( unsigned i=0; i<Masks.size(); i++ ){
    bool match = true;
    if( Masks[i]->GetFormat()->GetFormatWidth() == Inst->GetBitLen() ){
      // same number of bits, continue matching
      for( unsigned j=0; j<Masks[i]->GetNumEncodings(); j++ ){
        CoreGenEncoding *E = Masks[i]->GetEncoding(j);
        if( Inst->ExtractBits(Masks[i]->GetFormat()->GetStartBit(E->GetField()),
                              Masks[i]->GetFormat()->GetEndBit(E->GetField())) != 
            E->GetEncoding() ){
          match = false;
        }
      }
      if( match ){
        return Masks[i]->GetInst()->GetName();
      }
    }
  }

  // check the payload to see if it matches a vliw stage
  // if we make it this far, then it must be a vliw stage
  // otherwise, we didn't match anything
  if( Stages.size() > 0 ){
    Name = "__VLIW"; // special keyword to trigger VLIW handler
  }

  return Name;
}

// EOF
