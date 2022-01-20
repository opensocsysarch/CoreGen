//
// _CoreGenNodeCodegen_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/CoreGenNodeCodegen.h"

bool CoreGenNodeCodegen::WritePackageInfo(std::ofstream &O){
  if( !O.is_open() )
    return false;
  O << "// -- Chisel package info" << std::endl;
  if( Common ){
    O << "package Common" << std::endl;
  }else{
    O << "package " << Package << std::endl;
  }
  O << "{" << std::endl << std::endl;
  return true;
}

bool CoreGenNodeCodegen::WriteChiselImports(std::ofstream &O){
  if( !O.is_open() )
    return false;
  O << "// -- Import standard Chisel and Scala packages" << std::endl;

  O << "import chisel3._" << std::endl;
  O << "import chisel3.util_" << std::endl;
  O << "import chisel3.experimental._" << std::endl;
  O << "import Common._" << std::endl << std::endl;

  return true;
}

bool CoreGenNodeCodegen::WriteStdFooter(std::ofstream &O){
  if( !O.is_open() )
    return false;
  O << "}" << std::endl << std::endl;
  return true;
}


// EOF
