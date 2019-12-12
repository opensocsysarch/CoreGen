//
// _ISACG_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/NodeCodegen/ISACG.h"

ISACG::ISACG( CoreGenNode *N, CoreGenProj *P,
                std::string Pack, std::string Path,
                bool Comm, CoreGenErrno *E )
  : CoreGenNodeCodegen(N,P,Pack,Path,Comm,E) {
}

ISACG::~ISACG(){
}

std::string ISACG::U8ToBinary(uint64_t n, size_t len){
  std::string r = std::bitset<64>(n).to_string();
  return r.substr((r.length()-len),len);
}

std::string ISACG::GetInstBitPat(CoreGenInstFormat *Format,
                                 CoreGenInst *Inst,
                                 bool *Success ){
  // seed the initial string with '?' for all bits
  std::string BitPat(Format->GetFormatWidth(),'?');

  // walk all the fields; if the fields is an encoding field write
  // it to the bitpat string
  uint64_t Enc = 0x00ull;
  bool LocalSuccess = true;
  for( unsigned i=0; i<Format->GetNumFields(); i++ ){
    if( Format->GetFieldType(Format->GetFieldName(i)) == CoreGenInstFormat::CGInstCode ){
      // retrieve the encoding
      Enc = Inst->GetEncoding( Format->GetFieldName(i),
                               &LocalSuccess );
      if( !LocalSuccess ){
        Errno->SetError(CGERR_ERROR, "Could not retrieve encoding from Inst=" +
                        Inst->GetName() + " and Field=" + Format->GetFieldName(i));
        *Success = false;
        return BitPat;
      }

      // convert the string to binary
      size_t pos = Format->GetStartBit(Format->GetFieldName(i));
      size_t len = Format->GetFieldWidth(Format->GetFieldName(i));
      //std::string BinStr = std::bitset<len>(Enc).to_string();
      std::string BinStr = U8ToBinary(Enc,len);

      // insert the string into the master
      BitPat.replace(BitPat.length()-pos-len,len,BinStr);
    }
  }
  *Success = true;
  return BitPat;
}

bool ISACG::WriteISA(std::ofstream &O,
                     std::vector<CoreGenInst *> Insts,
                     std::vector<CoreGenInstFormat *> InstFormats){

  // for each instruction format, walk the instruction vector
  // and codegen each instruction individually
  bool Success = true;
  for( unsigned i=0; i<InstFormats.size(); i++ ){
    O << "object " << CGRemoveDot(InstFormats[i]->GetName()) << "Instructions {" << std::endl;
    for( unsigned j=0; j<Insts.size(); j++ ){
      if( Insts[j]->GetFormat()->GetName() == InstFormats[i]->GetName() ){
        // execute the codegen
        O << "\tdef " << Insts[j]->GetName()
          << CGPrintSpaceStr(Insts[j]->GetName().length(),20)
          << " = BitPat(\"b"
          << GetInstBitPat(InstFormats[i],Insts[j],&Success)
          << "\")" << std::endl;
        if( !Success ){
          return false;
        }
      }
    }
    O << "}" << std::endl << std::endl;
  }

  return true;
}

bool ISACG::Execute(std::vector<CoreGenInst *> Insts,
                    std::vector<CoreGenInstFormat *> InstFormats){

  // open the output file
  std::ofstream OutFile;
  OutFile.open(Path,std::ios::trunc);
  if(!OutFile.is_open()){
    Errno->SetError(CGERR_ERROR, "Could not open chisel file for ISA:" +
                    Path );
    return false;
  }

  // write the header
  OutFile << "// -- " << Node->GetName()
          << " Chisel Output" << std::endl << std::endl;

  // write the package info
  if( !WritePackageInfo(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write standard package info to file : " +
                    Path );
    OutFile.close();
    return false;
  }

  // import the chisel imports
  if( !WriteChiselImports(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write standard Chisel header to file : " + 
                    Path );
    OutFile.close();
    return false;
  }

  // write out the package block
  if( !WriteISA(OutFile,Insts,InstFormats) ){
    Errno->SetError(CGERR_ERROR, "Could not write ISA block to file : " +
                    Path );
    OutFile.close();
    return false;
  }

  if( !WriteStdFooter(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write standard Chisel footer to file : " + 
                    Path );
    OutFile.close();
    return false;
  }

  // close the output file
  OutFile.close();

  return true;
}

// EOF
