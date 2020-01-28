//
// _CoreGenLLVMCodegen_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/CoreGenLLVMCodegen.h"

CoreGenLLVMCodegen::CoreGenLLVMCodegen( CoreGenNode *T,
                                        CoreGenProj *P,
                                        CoreGenArchEntry *EN,
                                        std::string R,
                                        CoreGenErrno *E )
  : Top(T), Proj(P), Entry(EN), LLVMRoot(R), Errno(E) {
}

CoreGenLLVMCodegen::~CoreGenLLVMCodegen(){
}

bool CoreGenLLVMCodegen::GenerateBuildImpl(){
  return true;
}

bool CoreGenLLVMCodegen::GenerateCPUDriver(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateTablegen(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateISelDag(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateMachInfo(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateInstLowering(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateRegInfo(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateSubtargetInfo(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateTargetMach(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateTargetObj(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateAsmParser(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateDisass(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateInstPrinter(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateMCTargetDesc(){
  return true;
}

bool CoreGenLLVMCodegen::TIGenerateTargetInfo(){

  // Stage 1: generate the target info file
  std::string OutFile = LLVMRoot + "/TargetInfo/" + TargetName + "TargetInfo.cpp";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open target info file: " + OutFile );
    return false;
  }

  OutStream << "#include \"llvm/Support/TargetRegistry.h\"" << std::endl;
  OutStream << "using namespace llvm;" << std::endl << std::endl;

  OutStream << "namespace llvm{" << std::endl;
  for( unsigned i=0; i<Subtargets.size(); i++ ){
    OutStream << "Target &getThe" << TargetName << Subtargets[i] << "Target() {" << std::endl;
    OutStream << "  static Target The" << TargetName << Subtargets[i] << "Target;" << std::endl;
    OutStream << "  return The" << TargetName << Subtargets[i] << "Target;" << std::endl;
    OutStream << "}" << std::endl << std::endl;
  }
  OutStream << "}" << std::endl;

  OutStream << "extern \"C\" void LLVMInitialize" << TargetName << "TargetInfo() {" << std::endl;
  for( unsigned i=0; i<Subtargets.size(); i++ ){
    OutStream << "  RegisterTarget<Triple::" << Subtargets[i]
              << "> X(getThe" << TargetName << Subtargets[i] << "Target(), \""
              << TargetName << Subtargets[i] << "\", \""
              << TargetName << " " << Subtargets[i] << " subtarget\","
              << "\"" << TargetName << "\");" << std::endl;
  }
  OutStream << "}" << std::endl;

  OutStream.close();

  // Stage 2: generate the cmake driver
  OutFile = LLVMRoot + "/TargetInfo/CMakeLists.txt";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open target info CMakeLists: " + OutFile );
    return false;
  }

  OutStream << "add_llvm_library(LLVM" << TargetName << "Info" << std::endl
            << " " << TargetName << "TargetInfo.cpp" << std::endl
            << " )" << std::endl;

  OutStream.close();

  // Stage 3: generate the LLVMBuild driver
  OutFile = LLVMRoot + "/TargetInfo/LLVMBuild.txt";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open target info LLVMBuild: " + OutFile );
    return false;
  }

  OutStream << ";===- ./lib/Target/"
            << TargetName
            << "/TargetInfo/LLVMBuild.txt --------------*- Conf -*--===;"
            << std::endl;
  OutStream << "[component_0]" << std::endl
            << "type = Library" << std::endl
            << "name = " << TargetName << "Info" << std::endl
            << "parent = " << TargetName << std::endl
            << "required_libraries = Support" << std::endl
            << "add_to_library_groups = " << TargetName << std::endl;

  OutStream.close();

  return true;
}

bool CoreGenLLVMCodegen::TIGenerateCmake(){
  return true;
}

bool CoreGenLLVMCodegen::GenerateTargetImpl(){

  // Stage 1: Create the tablegen infrastructure
  if( !TIGenerateTablegen() )
    return false;

  // Stage 2: Create the ISelDagToDag template
  if( !TIGenerateISelDag() )
    return false;

  // Stage 3: Create the machine function info
  if( !TIGenerateMachInfo() )
    return false;

  // Stage 4: Create the instruction lowering template
  if( !TIGenerateInstLowering() )
    return false;

  // Stage 5: Create the register info template
  if( !TIGenerateRegInfo() )
    return false;

  // Stage 6: Create the subtarget info template
  if( !TIGenerateSubtargetInfo() )
    return false;

  // Stage 7: Create the target machine template
  if( !TIGenerateTargetMach() )
    return false;

  // Stage 8: Create the target object file template
  if( !TIGenerateTargetObj() )
    return false;

  // Stage 9: Create the AsmParser
  if( !TIGenerateAsmParser() )
    return false;

  // Stage 10: Create the Disassembler
  if( !TIGenerateDisass() )
    return false;

  // Stage 11: Create the InstPrinter
  if( !TIGenerateInstPrinter() )
    return false;

  // Stage 12: Create the MCTargetDesc
  if( !TIGenerateMCTargetDesc() )
    return false;

  // Stage 13: Create the TargetInfo
  if( !TIGenerateTargetInfo() )
    return false;

  // Stage 14: Create the top-level CMake script
  if( !TIGenerateCmake() )
    return false;

  return true;
}

bool CoreGenLLVMCodegen::GenerateTargetDir(){
  CoreGenNode *SocNode = nullptr;

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGSoc )
      SocNode = Top->GetChild(i);
  }

  if( !SocNode ){
    Errno->SetError(CGERR_ERROR, "No SoC node found in the project" );
    return false;
  }

  TargetName = SocNode->GetName();
  std::string TargetDir = LLVMRoot + "/llvm/lib/Target/" + TargetName;

  // generate the top-level target directory
  if( !CGMkDir(TargetDir) ){
    Errno->SetError(CGERR_ERROR, "Could not create LLVM target directory: " +
                    TargetDir );
    return false;
  }

  // generate all the subdirectories
  // -- AsmParser
  if( !CGMkDir(TargetDir + "/AsmParser") ){
    Errno->SetError(CGERR_ERROR, "Could not create LLVM AsmParser directory: " +
                    TargetDir + "/AsmParser" );
    return false;
  }

  // -- Disassembler
  if( !CGMkDir(TargetDir + "/Disassembler") ){
    Errno->SetError(CGERR_ERROR, "Could not create LLVM Disassembler directory: " +
                    TargetDir + "/Disassembler" );
    return false;
  }

  // -- InstPrinter
  if( !CGMkDir(TargetDir + "/InstPrinter") ){
    Errno->SetError(CGERR_ERROR, "Could not create LLVM InstPrinter directory: " +
                    TargetDir + "/InstPrinter" );
    return false;
  }

  // -- MCTargetDesc
  if( !CGMkDir(TargetDir + "/MCTargetDesc") ){
    Errno->SetError(CGERR_ERROR, "Could not create LLVM MCTargetDesc directory: " +
                    TargetDir + "/MCTargetDesc" );
    return false;
  }

  // -- TargetInfo
  if( !CGMkDir(TargetDir + "/TargetInfo") ){
    Errno->SetError(CGERR_ERROR, "Could not create LLVM TargetInfo directory: " +
                    TargetDir + "/TargetInfo" );
    return false;
  }

  return true;
}

bool CoreGenLLVMCodegen::GenerateSubtargets(){
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGISA )
      Subtargets.push_back(Top->GetChild(i)->GetName());
  }
  return true;
}

bool CoreGenLLVMCodegen::Execute(){

  // Stage 1: generate subtargets
  if( !GenerateSubtargets() )
    return false;

  // Stage 2: generate the directory structure for the new target
  if( !GenerateTargetDir() )
    return false;

  // Stage 3: generate the codegen blocks for each ISA
  if( !GenerateTargetImpl() )
    return false;

  // Stage 4: generate the CPU driver
  if( !GenerateCPUDriver() )
    return false;

  // Stage 5: generate the build infrastructure
  if( !GenerateBuildImpl() )
    return false;

  return true;
}

// EOF
