//
// _CoreGenCodegen_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/CoreGenCodegen.h"

CoreGenCodegen::CoreGenCodegen(CoreGenNode *T,
                               CoreGenProj *P,
                               CoreGenErrno *E)
  : Top(T), Proj(P), Errno(E) {
}

CoreGenCodegen::~CoreGenCodegen(){
}

bool CoreGenCodegen::ExecuteLLVMCodegen(){
  return true;
}

bool CoreGenCodegen::BuildChiselDir(){
  if( !Proj ){
    Errno->SetError(CGERR_ERROR, "Cannot derive Chisel directory; Project is null" );
    return false;
  }

  std::string FullDir;
  std::string ProjRoot = Proj->GetProjRoot();
  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    FullDir = Proj->GetProjRoot() + "RTL/chisel/src/main/scala";
  }else{
    FullDir = Proj->GetProjRoot() + "/RTL/chisel/src/main/scala";
  }

  if( !CGMkDirP(FullDir) ){
    Errno->SetError(CGERR_ERROR, "Could not construct the chisel source tree: "
                    + FullDir );
    return false;
  }

  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    FullDir = Proj->GetProjRoot() + "RTL/chisel/project";
  }else{
    FullDir = Proj->GetProjRoot() + "/RTL/chisel/project";
  }
  if( !CGMkDirP(FullDir) ){
    Errno->SetError(CGERR_ERROR, "Could not construct chisel project tree: "
                    + FullDir );
    return false;
  }

  return true;
}

// Constructs the Chisel `build.sbt` file
// Derived from the FreeChipsProject chisel-template build.sbt
// Automatically injects project-specific information
// https://github.com/freechipsproject/chisel-template/blob/release/build.sbt
bool CoreGenCodegen::BuildChiselSBT(){
  if( !Proj ){
    Errno->SetError(CGERR_ERROR, "Cannot derive Chisel directory; Project is null" );
    return false;
  }

  std::string SBTFile = Proj->GetProjRoot() + "/RTL/chisel/build.sbt";
  std::ofstream SOutFile;
  SOutFile.open(SBTFile,std::ios::trunc);
  if( !SOutFile.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open chisel sbt file: " + SBTFile );
    return false;
  }

  // def scalacOptionsVerion
  SOutFile << "def scalacOptionsVersion(scalaVersion: String): Seq[String] = {" << std::endl;
  SOutFile << "  Seq() ++ {" << std::endl;
  SOutFile << "    CrossVersion.partialVersion(scalaVersion) match {" << std::endl;
  SOutFile << "      case Some((2, scalaMajor: Long)) if scalaMajor < 12 => Seq()" << std::endl;
  SOutFile << "      case _ => Seq(\"-Xsource:2.11\")" << std::endl;
  SOutFile << "    }" << std::endl;
  SOutFile << "  }" << std::endl;
  SOutFile << "}" << std::endl << std::endl;

  // def javacOptionsVersion
  SOutFile << "def javacOptionsVersion(scalaVersion: String): Seq[String] = {" << std::endl;
  SOutFile << "  Seq() ++ {" << std::endl;
  SOutFile << "    CrossVersion.partialVersion(scalaVersion) match {" << std::endl;
  SOutFile << "      case Some((2, scalaMajor: Long)) if scalaMajor < 12 =>" << std::endl;
  SOutFile << "        Seq(\"-source\", \"1.7\", \"-target\", \"1.7\")" << std::endl;
  SOutFile << "      case _ =>" << std::endl;
  SOutFile << "        Seq(\"-source\", \"1.8\", \"-target\", \"1.8\")" << std::endl;
  SOutFile << "    }" << std::endl;
  SOutFile << "  }" << std::endl;
  SOutFile << "}" << std::endl << std::endl;

  // everything else
  SOutFile << "name := \"" << Proj->GetProjName() << "\"" << std::endl << std::endl;

  unsigned Major = 0;
  unsigned Minor = 0;
  if( !Proj->GetChiselVersion(&Major,&Minor) ){
    Errno->SetError(CGERR_ERROR, "Could not retrieve Chisel version" );
    SOutFile.close();
    return false;
  }
  SOutFile << "version := \""
           << std::to_string(Major) << "."
           << std::to_string(Minor) << "\"" << std::endl << std::endl;

  SOutFile << "scalaVersion := \"2.11.12\"" << std::endl << std::endl;

  SOutFile << "crossScalaVersions := Seq(\"2.11.12\",\"2.12.4\")" << std::endl << std::endl;

  SOutFile << "resolvers ++= Seq(" << std::endl;
  SOutFile << "  Resolver.sonatypeRepo(\"snapshots\")," << std::endl;
  SOutFile << "  Resolver.sonatypeRepo(\"releases\")" << std::endl;
  SOutFile << ")" << std::endl << std::endl;

  SOutFile << "defaultVersions = Map(" << std::endl;
  SOutFile << "  \"chisel3\" -> \"3.1.+\"," << std::endl;
  SOutFile << "  \"chisel-iotesters\" -> \"[1.2.4,1.3.0[\"" << std::endl;
  SOutFile << "  )" << std::endl << std::endl;

  SOutFile << "libraryDependencies ++= Seq(\"chisel3\",\"chisel-iotesters\").map {" << std::endl;
  SOutFile << "  dep: String => \"edu.berkeley.cs\" \%\% dep \% sys.props.getOrElse(dep + \"Version\", defaultVersions(dep)) }"
           << std::endl << std::endl;

  SOutFile << "scalacOptions ++= scalacOptionsVersion(scalaVersion.value)" << std::endl << std::endl;

  SOutFile << "javacOptions ++= javacOptionsVersion(scalaVersion.value)" << std::endl << std::endl;

  SOutFile.close();
  return true;
}

bool CoreGenCodegen::BuildChiselProject(){
  if( !Proj ){
    Errno->SetError(CGERR_ERROR, "Cannot derive Chisel directory; Project is null" );
    return false;
  }

  // build.properties
  std::string PFile = Proj->GetProjRoot() + "/RTL/chisel/project/build.properties";
  std::ofstream POutFile;
  POutFile.open(PFile,std::ios::trunc);
  if( !POutFile.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open chisel project file: " + PFile );
    return false;
  }
  POutFile << "sbt.version = 1.1.1";
  POutFile.close();

  // plugins.sbt
  PFile = Proj->GetProjRoot() + "/RTL/chisel/project/plugins.sbt";
  POutFile.open(PFile,std::ios::trunc);
  if( !POutFile.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open chisel project file: " + PFile );
    return false;
  }
  POutFile << "logLevel := Level.Warn";
  POutFile.close();

  return true;
}

bool CoreGenCodegen::BuildChiselMakefile(){
  if( !Proj ){
    Errno->SetError(CGERR_ERROR, "Cannot derive Chisel directory; Project is null" );
    return false;
  }

  std::string MFile = Proj->GetProjRoot() + "/RTL/chisel/Makefile";
  std::ofstream MOutFile;
  MOutFile.open(MFile,std::ios::trunc);
  if( !MOutFile.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open chisel makefile: " + MFile );
    return false;
  }

  MOutFile << "#!/bin/bash" << std::endl << "#" << std::endl;
  MOutFile << "# Chisel makefile autogenerated by System Architect" << std::endl;
  MOutFile << "# " << CGCurrentDateTime() << std::endl << std::endl;

  // output build directives
  // TODO

  // output clean directive
  MOutFile << "clean:" << std::endl;
  MOutFile << "\t@echo \"Cleaning Verilog Source\" && rm -Rf generated-src" << std::endl;

  MOutFile.close();
  return true;
}

bool CoreGenCodegen::ExecuteChiselCodegen(){

  // Stage 1: Build the Chisel directory structure
  if( !BuildChiselDir() ){
    return false;
  }

  // Stage 2: Walk the top-level modules and generate chisel

  // Stage 3: Build the Chisel makefile
  if( !BuildChiselMakefile() ){
    return false;
  }

  // Stage 4: Build the Chisel SBT file
  if( !BuildChiselSBT() ){
    return false;
  }

  // Stage 5: Build the supplementary project files
  if( !BuildChiselProject() ){
    return false;
  }

  return true;
}

bool CoreGenCodegen::Execute(){
  if( !ExecuteChiselCodegen() ){
    return false;
  }
  if( !ExecuteLLVMCodegen() ){
    return false;
  }
  return true;
}

// EOF
