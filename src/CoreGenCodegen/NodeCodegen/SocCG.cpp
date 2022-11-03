//
// _SocCG_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/NodeCodegen/SocCG.h"

SocCG::SocCG( CoreGenNode *N, CoreGenProj *P,
                std::string Pack, std::string Path,
                bool Comm, CoreGenErrno *E )
  : CoreGenNodeCodegen(N,P,Pack,Path,Comm,E) {
}

SocCG::~SocCG(){
}

bool SocCG::WriteSoc(std::ofstream &O){
  CoreGenSoC *SOC = static_cast<CoreGenSoC *>(Node);

  // output a top-level SoC module and instantiate all the cores
  O << "class " << CGRemoveDot(SOC->GetName()) << "((implicit val conf: "
    << CGRemoveDot(Proj->GetProjName()) << "Configuration) extends Module" << std::endl
    << "{" << std::endl
    << "\tval io = IO(new Bundle {" << std::endl
    << "\t\tval dmi = Flipped(new DMIIO())" << std::endl
    << "\t})" << std::endl;

  // TODO: instantiate the debug module

  // instantiate all the cores
  for( unsigned i=0; i<SOC->GetNumCores(); i++ ){
    // instantiate the core
    O << "\tval core" << i << " = Module(new "
      << CGRemoveDot(SOC->GetCore(i)->GetName()) << "())" << std::endl;
    O << "\tcore" << i << ".io := DontCare" << std::endl;

    // TODO: hook up all the NoC ports

    // hook up the reset
    O << "\tcore" << i << ".reset := debug.io.resetcore | reset.toBool" << std::endl;
  }

  O << "}" << std::endl;

  return true;
}

bool SocCG::Execute(){

  // open the output file
  std::ofstream OutFile;
  OutFile.open(Path,std::ios::trunc);
  if(!OutFile.is_open()){
    Errno->SetError(CGERR_ERROR, "Could not open chisel file for SoC:" +
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

  // TODO: extend the chisel imports with all the target cores/ISAs

  // write out the package block
  if( !WriteSoc(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write SoC block to file : " +
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
