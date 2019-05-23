//
// _CoreGenCodegen_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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

bool CoreGenCodegen::BuildProjMakefile(){
  if( !Proj ){
    Errno->SetError(CGERR_ERROR, "Cannot derive Chisel directory; Project is null" );
    return false;
  }

  std::string MFile;
  std::string ProjRoot = Proj->GetProjRoot();

  if( !CGMkDirP(ProjRoot) ){
    Errno->SetError(CGERR_ERROR, "Could not construct top-level project directory: "
                    + ProjRoot );
    return false;
  }

  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    MFile = ProjRoot + "Makefile";
  }else{
    MFile = ProjRoot + "/Makefile";
  }

  std::ofstream MOutFile;
  MOutFile.open(MFile,std::ios::trunc);
  if( !MOutFile.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open top-level makefile: " + MFile );
    return false;
  }

  // header
  MOutFile << "#!/bin/sh" << std::endl;
  MOutFile << "#" << std::endl;
  MOutFile << "# System Architect top-level project makefile" << std::endl;
  MOutFile << "#" << std::endl << std::endl;

  // top-level variables
  MOutFile << "CGCLI_BIN = `which cgcli`" << std::endl;
  MOutFile << "IRFILES := $(shell find . -maxdepth 1 -type f -name '*.yaml')" << std::endl;
  MOutFile << "SCFILES := $(shell find . -maxdepth 1 -type f -name '*.sc')" << std::endl;


  // PHONY
  MOutFile << std::endl;
  MOutFile << ".PHONY: clean logo help project" << std::endl << std::endl;

  // top-level all
  MOutFile << "all: help" << std::endl << std::endl;

  // banner section
  MOutFile << "logo:" << std::endl;
  MOutFile << "\t@echo \" _____                  _____             ___           _     \"" << std::endl;
  MOutFile << "\t@echo \"|  _  |                /  ___|           / _ \\         | |    \"" << std::endl;
  MOutFile << "\t@echo \"| | | |_ __   ___ _ __ \\ \\--. _   _ ___ / /_\\ \\_ __ ___| |__  \"" << std::endl;
  MOutFile << "\t@echo \"| | | | '_ \\ / _ \\ '_ \\ \\--. \\ | | / __||  _  | '__/ __| '_ \\ \"" << std::endl;
  MOutFile << "\t@echo \"\\ \\_/ / |_) |  __/ | | /\\__/ / |_| \\__ \\| | | | | | (__| | | |\"" << std::endl;
  MOutFile << "\t@echo \" \\___/| .__/ \\___|_| |_\\____/ \\__, |___/\\_| |_/_|  \\___|_| |_|\"" << std::endl;
  MOutFile << "\t@echo \"      | |                      __/ |                          \"" << std::endl;
  MOutFile << "\t@echo \"      |_|                     |___/                           \"" << std::endl;

  // help menu
  MOutFile << "help: logo" << std::endl;
  MOutFile << "\t@echo \"-----------------------------------------------------------------\"" << std::endl;
  MOutFile << "\t@echo \"help:      Print this help menu\"" << std::endl;
  MOutFile << "\t@echo \"project:   Verifies the design, creates Chisel and LLVM compiler\"" << std::endl;
  MOutFile << "\t@echo \"compiler:  Builds the LLVM compiler\"" << std::endl;
  MOutFile << "\t@echo \"chisel:    Builds the Chisel HDL\"" << std::endl;
  MOutFile << "\t@echo \"simulaotr: Builds the cycle-accurate simulator\"" << std::endl;
  MOutFile << "\t@echo \"coregen:   Executes coregen to verify the design\"" << std::endl;
  MOutFile << "\t@echo \"-----------------------------------------------------------------\"" << std::endl;

  // clean
  MOutFile << "clean:" << std::endl;

  // project
  MOutFile << "project: logo coregen chisel compiler" << std::endl;

  // compiler
  MOutFile << "compiler: logo" << std::endl;

  // chisel
  MOutFile << "chisel: logo" << std::endl;

  // simulator
  MOutFile << "simulator: logo" << std::endl;

  // coregen
  MOutFile << "coregen: logo coregen-test %.yaml" << std::endl;
  MOutFile << "\t$(CGCLI_BIN) --verify --pass --ir $<" << std::endl;

  // coregen-test
  MOutFile << "coregen-test:" << std::endl;
  MOutFile << "\ttest !-e $(CGCLI_BIN) && @echo \"cgcli command not in the current path\" && exit 1" << std::endl;

  MOutFile.close();
  return true;
}

bool CoreGenCodegen::BuildLLVMDir(){
  if( !Proj ){
    Errno->SetError(CGERR_ERROR, "Cannot derive LLVM directory; Project is null" );
    return false;
  }

  std::string FullDir;
  std::string ProjRoot = Proj->GetProjRoot();
  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    FullDir = ProjRoot + "compiler/LLVM";
    LLVMDir = FullDir;
  }else{
    FullDir = ProjRoot + "/compiler/LLVM";
    LLVMDir = FullDir;
  }

  if( !CGMkDirP(FullDir) ){
    Errno->SetError(CGERR_ERROR, "Could not construct the LLVM source tree: "
                    + FullDir );
    return false;
  }

  return true;
}

bool CoreGenCodegen::ExecuteLLVMCodegen(){

  // Stage 1: Build the top-level makefile
  if( !isTopMakefile ){
    if( !BuildProjMakefile() ){
      return false;
    }
    isTopMakefile = true;
  }

  // Stage 2: Build the Chisel directory structure
  if( !BuildLLVMDir() ){
    return false;
  }

  // Stage 3: Copy the source tree over from the archive

  // Stage 4: Execute the codegen

  return true;
}

bool CoreGenCodegen::BuildStoneCutterDir(){
  if( !Proj ){
    Errno->SetError(CGERR_ERROR, "Cannot derive StoneCutter directory; Project is null" );
    return false;
  }

  std::string FullDir;
  std::string ProjRoot = Proj->GetProjRoot();
  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    FullDir = ProjRoot + "RTL/stonecutter";
    SCDir = FullDir;
  }else{
    FullDir = ProjRoot + "/RTL/stonecutter";
    SCDir = FullDir;
  }

  if( !CGMkDirP(FullDir) ){
    Errno->SetError(CGERR_ERROR, "Could not construct the stonecutter source tree: "
                    + FullDir );
    return false;
  }

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
    FullDir = ProjRoot + "RTL/chisel/src/main/scala";
    ChiselDir = ProjRoot + "RTL/chisel";
  }else{
    FullDir = ProjRoot + "/RTL/chisel/src/main/scala";
    ChiselDir = ProjRoot + "/RTL/chisel";
  }

  if( !CGMkDirP(FullDir) ){
    Errno->SetError(CGERR_ERROR, "Could not construct the chisel source tree: "
                    + FullDir );
    return false;
  }

  if( !CGMkDirP(FullDir+"/common") ){
    Errno->SetError(CGERR_ERROR, "Could not construct the chisel source tree: "
                    + FullDir + "/common" );
    return false;
  }

  if( !CGMkDirP(FullDir+"/top") ){
    Errno->SetError(CGERR_ERROR, "Could not construct the chisel source tree: "
                    + FullDir + "/top" );
    return false;
  }

  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    FullDir = ProjRoot + "RTL/chisel/project";
  }else{
    FullDir = ProjRoot + "/RTL/chisel/project";
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

  std::string SBTFile;
  std::string ProjRoot = Proj->GetProjRoot();
  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    SBTFile = ProjRoot + "RTL/chisel/build.sbt";
  }else{
    SBTFile = ProjRoot + "/RTL/chisel/build.sbt";
  }
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
  std::string PFile;
  std::string ProjRoot = Proj->GetProjRoot();
  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    PFile = ProjRoot + "RTL/chisel/project/build.properties";
  }else{
    PFile = ProjRoot + "/RTL/chisel/project/build.properties";
  }
  std::ofstream POutFile;
  POutFile.open(PFile,std::ios::trunc);
  if( !POutFile.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open chisel project file: " + PFile );
    return false;
  }
  POutFile << "sbt.version = 1.1.1";
  POutFile.close();

  // plugins.sbt
  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    PFile = ProjRoot + "RTL/chisel/project/plugins.sbt";
  }else{
    PFile = ProjRoot + "/RTL/chisel/project/plugins.sbt";
  }
  POutFile.open(PFile,std::ios::trunc);
  if( !POutFile.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open chisel project file: " + PFile );
    return false;
  }
  POutFile << "logLevel := Level.Warn";
  POutFile.close();

  return true;
}

bool CoreGenCodegen::BuildChisel(){
  if( !Proj ){
    Errno->SetError(CGERR_ERROR, "Cannot derive Chisel directory; Project is null" );
    return false;
  }

  std::string FullDir;
  std::string ProjRoot = Proj->GetProjRoot();
  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    FullDir = ProjRoot + "RTL/chisel/src/main/scala";
  }else{
    FullDir = ProjRoot + "/RTL/chisel/src/main/scala";
  }

  CoreGenChiselCodegen *CG = new CoreGenChiselCodegen(Top,
                                                      Proj,
                                                      FullDir,
                                                      Errno);
  bool rtn = CG->Execute();
  delete CG;
  return rtn;
}

bool CoreGenCodegen::BuildChiselCommonPackage(){
  if( !Proj ){
    Errno->SetError(CGERR_ERROR, "Cannot derive Chisel directory; Project is null" );
    return false;
  }

  std::string FullDir;
  std::string ProjRoot = Proj->GetProjRoot();
  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    FullDir = ProjRoot + "RTL/chisel/src/main/scala";
  }else{
    FullDir = ProjRoot + "/RTL/chisel/src/main/scala";
  }

  //  write out the package.scala file
  std::string CommonFile = FullDir + "/package.scala";

  std::ofstream MOutFile;
  MOutFile.open(CommonFile,std::ios::trunc);
  if( !MOutFile.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open common package.scala : " + CommonFile );
    return false;
  }

  MOutFile << "//-- common/package.scala" << std::endl << std::endl;
  MOutFile << "package Common" << std::endl << std::endl;

  MOutFile << "import chisel3._" << std::endl;
  MOutFile << "import chisel3.util_" << std::endl;
  MOutFile << "import scala.math._" << std::endl;

  MOutFile.close();

  return true;
}

bool CoreGenCodegen::BuildChiselMakefile(){
  if( !Proj ){
    Errno->SetError(CGERR_ERROR, "Cannot derive Chisel directory; Project is null" );
    return false;
  }

  std::string MFile;
  std::string ProjRoot = Proj->GetProjRoot();
  if( ProjRoot[ProjRoot.length()-1] == '/' ){
    MFile = ProjRoot + "RTL/chisel/Makefile";
  }else{
    MFile = ProjRoot + "/RTL/chisel/Makefile";
  }

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
  MOutFile << ".PHONY: clean" << std::endl;

  // output clean directive
  MOutFile << "clean:" << std::endl;
  MOutFile << "\t@echo \"Cleaning Verilog Source\" && rm -Rf generated-src" << std::endl;

  MOutFile.close();
  return true;
}

bool CoreGenCodegen::BuildISAChisel( CoreGenISA *ISA,
                                     std::vector<CoreGenInst *> Insts ){
  std::vector<CoreGenInstFormat *> IF;
  std::vector<CoreGenRegClass *> RC;

  // stage 1: gather a list of all the instruction formats
  for( unsigned i=0; i<Insts.size(); i++ ){
    IF.push_back(static_cast<CoreGenInstFormat *>(Insts[i]->GetFormat()));
  }

  // stage 2: gather a list of all the register classes
  for( unsigned i=0; i<IF.size(); i++ ){
    for( unsigned j=0; j<IF[i]->GetNumFields(); j++ ){
      // for each field, interrogate it to determine whether it is a register class
      if( IF[i]->GetFieldType(IF[i]->GetFieldName(j)) ==
          CoreGenInstFormat::CGInstReg ){

        // make sure we don't insert any redundants
        CoreGenRegClass *TmpRC = static_cast<CoreGenRegClass *>(
            IF[i]->GetFieldRegClass(IF[i]->GetFieldName(j)));
        if( std::find(RC.begin(), RC.end(), TmpRC) == RC.end() ){
          RC.push_back(TmpRC);
        }
      } // end GetFieldType
    }// end GetNumFields
  }// end IF.size

  // stage 3: generate the output chisel file
  std::string OutFile = SCDir + "/" + ISA->GetName() + ".sc";
  std::ofstream MOutFile;
  MOutFile.open(OutFile,std::ios::trunc);
  if( !MOutFile.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open StoneCutter output file" + OutFile );
    return false;
  }

  // stage 4: write the header
  MOutFile << "#-- StoneCutter source file for ISA=" << ISA->GetName() << std::endl;
  MOutFile << std::endl << std::endl;

  // stage 5: write out the instruction formats
  MOutFile << "# Instruction Formats" << std::endl;
  for( unsigned i=0; i<IF.size(); i++ ){
    // protect against duplicates across instructions
    if( std::find(InstFormatsVect.begin(),
                  InstFormatsVect.end(),
                  IF[i]->GetName()) == InstFormatsVect.end() ){
      // inst format not found
      // record it to the top-level instformats vector
      // and write it to the file
      InstFormatsVect.push_back(IF[i]->GetName());

      MOutFile << "instformat " << IF[i]->GetName() << "(";
      for( unsigned j=0; j<IF[i]->GetNumFields(); j++ ){
        switch( IF[i]->GetFieldType(IF[i]->GetFieldName(j)) ){
        case CoreGenInstFormat::CGInstReg:
          MOutFile << "reg["
                   << IF[i]->GetFieldRegClass(IF[i]->GetFieldName(j))->GetName()
                   << "] "
                   << IF[i]->GetFieldName(j);
          break;
        case CoreGenInstFormat::CGInstCode:
          MOutFile << "enc " << IF[i]->GetFieldName(j);
          break;
        case CoreGenInstFormat::CGInstImm:
          MOutFile << "imm " << IF[i]->GetFieldName(j);
          break;
        default:
          Errno->SetError(CGERR_ERROR,
                          "No valid field type for instruction format=" +
                          IF[i]->GetName() + "; field=" + IF[i]->GetFieldName(j) );
          return false;
          break;
        }
        // print a comma if necessary
        if( j!=(IF[i]->GetNumFields()-1) ){
          MOutFile << ",";
        }
      }
      MOutFile << ")" << std::endl;
    }// end if ( std::find )
  }// end IF.size()

  MOutFile << std::endl;

  // stage 6: write out the register classes
  MOutFile << "# Register Class Definitions" << std::endl;
  for( unsigned i=0; i<RC.size(); i++ ){
    MOutFile << "regclass " << RC[i]->GetName() << "(";

    // write out all the registers
    for( unsigned j=0; j<RC[i]->GetNumReg(); j++ ){

      CoreGenReg *REG = static_cast<CoreGenReg *>(RC[i]->GetReg(j));

      std::string REGNAME;
      if( REG->IsPCAttr() ){
        REGNAME = REG->GetName() + "[PC]";
      }else{
        REGNAME = REG->GetName();
      }

      if( REG->GetNumSubRegs() > 0 ){
        // write out the register with the subregs
        MOutFile << " u" << REG->GetWidth() << " " << REGNAME << "(";
        for( unsigned k=0; k<REG->GetNumSubRegs(); k++ ){
          std::string Name;
          unsigned Start;
          unsigned End;
          if( !REG->GetSubReg(k,Name,Start,End) ){
            Errno->SetError(CGERR_ERROR, "Could not retrieve subreg from register=" + REG->GetName() );
            MOutFile.close();
            return false;
          }
          MOutFile << " u" << ((End-Start)+1) << " " << Name;
          if( k != (REG->GetNumSubRegs()-1) ){
            MOutFile << ",";
          }
        } // end subregs
        MOutFile << ")";
      }else{
        // write out the register with no subregs
        MOutFile << " u" << REG->GetWidth() << " " << REGNAME;
      }

      // print a comma between registers
      if( j != (RC[i]->GetNumReg()-1) ){
        MOutFile << ",";
      }
    }// end writing out the registers

    MOutFile << " )" << std::endl;
  }// end writing register classes

  MOutFile << std::endl << std::endl;
  MOutFile << "# Instruction Definitions" << std::endl;

  // stage 7: write out the instructions
  for( unsigned i=0; i<Insts.size(); i++ ){
    MOutFile << "# " << Insts[i]->GetName() << std::endl;
    MOutFile << "def " << Insts[i]->GetName()
              << ":" << Insts[i]->GetFormat()->GetName() << "( ";

    // retrieve the instruction format and print out all the field
    // names and immediate values
    CoreGenInstFormat *LIF = Insts[i]->GetFormat();
    for( unsigned j=0; j<LIF->GetNumFields(); j++ ){
      if( (LIF->GetFieldType(LIF->GetFieldName(j)) == CoreGenInstFormat::CGInstReg) || 
          (LIF->GetFieldType(LIF->GetFieldName(j)) == CoreGenInstFormat::CGInstImm) ){
        MOutFile << LIF->GetFieldName(j) << " ";
      }
    }
    MOutFile << ")" << std::endl;
    MOutFile << "{" << std::endl << Insts[i]->GetImpl() << std::endl << "}" << std::endl << std::endl;
  }

  // stage 8: close the file
  MOutFile.close();

  return true;
}

bool CoreGenCodegen::BuildStoneCutterFiles(){

  // for each ISA we have defined, build a vector
  // of instructions that have inline stonecutter
  // create a single stonecutter file each ISA
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGISA ){
      // grab the ISA object
      CoreGenISA *ISA = static_cast<CoreGenISA *>(Top->GetChild(i));

      // build a vector of instructions that have inline stonecutter RTL
      std::vector<CoreGenInst *> Insts;
      for( unsigned j=0; j<Top->GetNumChild(); j++ ){
        if( Top->GetChild(j)->GetType() == CGInst ){
          CoreGenInst *INST = static_cast<CoreGenInst *>(Top->GetChild(j));
          if( (INST->GetISA() == ISA) && (INST->IsImpl()) ){
            Insts.push_back(INST);
          }
        }
      }

      // our vector is constructed, now build the output chisel
      if( Insts.size() > 0 ){
        if( !BuildISAChisel(ISA,Insts) ){
          return false;
        }
      }

    }// end if
  }// end for

  return true;
}

bool CoreGenCodegen::ExecuteChiselCodegen(){

  // Stage 1: Build the top-level makefile
  if( !isTopMakefile ){
    if( !BuildProjMakefile() ){
      return false;
    }
    isTopMakefile = true;
  }

  // Stage 2: Build the Chisel directory structure
  if( !BuildChiselDir() ){
    return false;
  }

  // Stage 3: Build the StoneCutter directory structure
  if( !BuildStoneCutterDir() ){
    return false;
  }

  // Stage 4: Walk the ISA graphs and build StoneCutter source using inline RTL
  if( !BuildStoneCutterFiles() ){
    return false;
  }

  // Stage 6: Walk the top-level modules and generate chisel
  //          This is the bulk of the code generation logic
  if( !BuildChisel() ){
    return false;
  }

  // Stage 7: Build the Chisel makefile
  if( !BuildChiselMakefile() ){
    return false;
  }

  // Stage 8: Build the Chisel SBT file
  if( !BuildChiselSBT() ){
    return false;
  }

  // Stage 9: Build the supplementary project files
  if( !BuildChiselProject() ){
    return false;
  }

  // Stage 10: Build the common package definition
  if( !BuildChiselCommonPackage() ){
    return false;
  }

  return true;
}

bool CoreGenCodegen::Execute(){
  if( !isTopMakefile ){
    if( !BuildProjMakefile() ){
      return false;
    }
    isTopMakefile = true;
  }
  if( !ExecuteChiselCodegen() ){
    return false;
  }
  if( !ExecuteLLVMCodegen() ){
    return false;
  }
  return true;
}

// EOF
