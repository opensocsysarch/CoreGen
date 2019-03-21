//
// _RegClassCG_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/NodeCodegen/RegClassCG.h"

RegClassCG::RegClassCG( CoreGenNode *N, CoreGenProj *P,
                        std::string Pack, std::string Path,
                        CoreGenErrno *E )
  : CoreGenNodeCodegen(N,P,Pack,Path,E) {
}

RegClassCG::~RegClassCG(){
}

bool RegClassCG::WriteRegClass(std::ofstream &O ){
  // write the comment
  O << "//-- " << Node->GetName() << " Register Class " << std::endl;
  O << "class " << CGRemoveDot(Node->GetName())
    << "(n: Int, w: Int, zero: Boolean = false) {" << std::endl;
  O << "\tval rf = Mem(n, UInt(width = w))" << std::endl;
  O << "}" << std::endl;
  return true;
}

bool RegClassCG::Execute(){

  // already generated this register file type
  if( CGFileExists(Path) )
    return true;

  // open the output file
  std::ofstream OutFile;
  OutFile.open(Path,std::ios::trunc);
  if(!OutFile.is_open()){
    Errno->SetError(CGERR_ERROR, "Could not open chisel file for register class:" +
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
  if( !WriteStdChiselImport(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write standard package imports to file : " + 
                    Path );
    OutFile.close();
    return false;
  }

  // write out the package block
  if( !WriteRegClass(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write register class block to file : " +
                    Path );
    OutFile.close();
    return false;
  }

  // close the output file
  OutFile.close();

  return true;
}

// EOF
