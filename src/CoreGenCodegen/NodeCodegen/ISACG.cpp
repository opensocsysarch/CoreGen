//
// _ISACG_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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

std::string ISACG::GetInstBitPat(CoreGenInstFormat *Format,
                                 CoreGenInst *Inst){
  std::string BitPat(Format->GetFormatWidth(),'?');
  return BitPat;
}

bool ISACG::WriteISA(std::ofstream &O,
                     std::vector<CoreGenInst *> Insts,
                     std::vector<CoreGenInstFormat *> InstFormats){

  // for each instruction format, walk the instruction vector
  // and codegen each instruction individually
  for( unsigned i=0; i<InstFormats.size(); i++ ){
    O << "object " << CGRemoveDot(InstFormats[i]->GetName()) << "Instructions {" << std::endl;
    for( unsigned j=0; j<Insts.size(); j++ ){
      if( Insts[j]->GetFormat()->GetName() == InstFormats[i]->GetName() ){
        // execute the codegen
        O << "def " << Insts[j]->GetName()
          << CGPrintSpaceStr(Insts[j]->GetName().length(),20)
          << " = BitPat(\""
          << GetInstBitPat(InstFormats[i],Insts[j])
          << "\")" << std::endl;
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
