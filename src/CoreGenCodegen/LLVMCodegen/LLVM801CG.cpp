//
// _LLVM801CG_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/LLVMCodegen/LLVM801CG.h"

LLVM801CG::LLVM801CG( CoreGenNode *T,
                      CoreGenProj *P,
                      CoreGenArchEntry *EN,
                      std::string R,
                      CoreGenErrno *E )
  : CoreGenLLVMCodegen(T,P,EN,R,E,"8.0.1") {
}

LLVM801CG::~LLVM801CG(){
}

bool LLVM801CG::GenerateBuildImpl(){
  return true;
}

bool LLVM801CG::GenerateCPUDriver(){
  return true;
}

bool LLVM801CG::TIGenerateTopLevelTablegen(){

  std::string OutFile = LLVMRoot + "/" + TargetName + ".td";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the top-level tablegen file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName << ".td - Describe the " << TargetName
          << " Machine -------*- tablegen -*-===//" << std::endl;
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl << std::endl;

  OutStream << "include \"llvm/Target/Target.td\"" << std::endl << std::endl;

  // print out all the subtargets
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  OutStream << "// " << TargetName << " subtarget features and instructon predicates" << std::endl;
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  for( unsigned i=0; i<Subtargets.size(); i++ ){
    OutStream << "def Feature" << Subtargets[i] << std::endl
              << "    : SubtargetFeature<\"" << Subtargets[i]
              << "\", \"Has\"" << Subtargets[i] << "\", \"true\", \"Subtarget "
              << Subtargets[i] << "\">;" << std::endl;
    OutStream << "def Has" << Subtargets[i] << " : Predicate<\"Subtarget->has"
              << Subtargets[i] << "()\">, AssemblerPredicate<\"Feature"
              << Subtargets[i] << "\">;" << std::endl;
  }

  // registers, calling concentions and instruction descriptions
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  OutStream << "// " << TargetName << " registers, calling conventions and instruction descriptions" << std::endl;
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  OutStream << "include \"" << TargetName << "RegisterInfo.td\"" << std::endl;
  // TBD: OutStream << "include \"" << TargetName << "CallingConv.td\"" << std::endl;
  OutStream << "include \"" << TargetName << "InstrInfo.td\"" << std::endl;

  // processors supported
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  OutStream << "// " << TargetName << " supported processors" << std::endl;
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;

  OutStream << "def : ProcesorModel<\"generic-" << TargetName
            << "\", NoSchedModel, []>;" << std::endl;

  // define the target
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  OutStream << "// " << TargetName << " target definitions" << std::endl;
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  OutStream << "def " << TargetName << "InstrInfo : InstrInfo {" << std::endl;
  OutStream << "  let guessInstructionProperties = 0;" << std::endl;
  OutStream << "}" << std::endl << std::endl;

  OutStream << TargetName << "AsmParser : AsmParser {" << std::endl;
  OutStream << "  let ShouldEmitMatchRegisterAltName = 1;" << std::endl;
  OutStream << "  let AllowDuplicateRegisterNames = 1;" << std::endl;
  OutStream << "}" << std::endl << std::endl;

  OutStream << TargetName << "AsmWriter : AsmWriter {" << std::endl;
  OutStream << "  let PassSubtarget = 1;" << std::endl;
  OutStream << "}" << std::endl << std::endl;

  OutStream << TargetName << " : Target {" << std::endl;
  OutStream << "  let InstructionSet = " << TargetName << "InstrInfo;" << std::endl;
  OutStream << "  let AssemblyParsers = [" << TargetName << "AsmParser];" << std::endl;
  OutStream << "  let AssemblyWriters = [" << TargetName << "AsmWriter];" << std::endl;
  OutStream << "  let AllowRegisterRenaming = 1;" << std::endl;
  OutStream << "}" << std::endl;

  // close the file
  OutStream.close();

  return true;
}

unsigned LLVM801CG::TIGenerateInstFormatBits(unsigned NumFormats){
  unsigned NBits = 0;

  NBits = (unsigned)(log2f((float)(NumFormats))/log2f(2.));

  return NBits;
}

std::string LLVM801CG::TIGenerateRegClassImmList(CoreGenInstFormat *Format){
  std::string Str;
  bool first = true;

  for( unsigned i=0; i<Format->GetNumFields(); i++ ){
    if( Format->GetFieldType(Format->GetFieldName(i)) == CoreGenInstFormat::CGInstReg ){
      if( first ){
        first = false;
      }else{
        Str += ",";
      }
      Str += (Format->GetFieldRegClass(Format->GetFieldName(i))->GetName() +
              ":$" +
              Format->GetFieldName(i));
    }else if( Format->GetFieldType(Format->GetFieldName(i)) == CoreGenInstFormat::CGInstImm ){
      if( first ){
        first = false;
      }else{
        Str += ",";
      }
      Str += ("imm" +
              std::to_string(Format->GetFieldWidth(Format->GetFieldName(i))) +
              ":$" +
              Format->GetFieldName(i));
    }
  }

  return Str;
}

std::string LLVM801CG::TIGenerateRegImmList(CoreGenInstFormat *Format){
  std::string Str;
  bool first = true;

  for( unsigned i=0; i<Format->GetNumFields(); i++ ){
    if( Format->GetFieldType(Format->GetFieldName(i)) == CoreGenInstFormat::CGInstReg ){
      if( first ){
        first = false;
      }else{
        Str += ",";
      }
      Str += ("$" +
              Format->GetFieldName(i));
    }else if( Format->GetFieldType(Format->GetFieldName(i)) == CoreGenInstFormat::CGInstImm ){
      if( first ){
        first = false;
      }else{
        Str += ",";
      }
      Str += ("$" +
              Format->GetFieldName(i));
    }
  }

  return Str;
}

std::string LLVM801CG::TIGenerateInstFields(CoreGenInstFormat *Format){
  std::string Str;
  bool first = true;

  for( unsigned i=0; i<Format->GetNumFields(); i++ ){
    if( Format->GetFieldType(Format->GetFieldName(i)) ==
        CoreGenInstFormat::CGInstCode ){
      // found an encoding field
      if( first ){
        first = false;
      }else{
        Str += ",";
      }
      Str += Format->GetFieldName(i);
      if( first ){
        first = false;
      }
    }
  }

  return Str;
}

std::string LLVM801CG::TIGenerateInstArgsFields(CoreGenInstFormat *Format){
  std::string Str;
  bool first = true;

  for( unsigned i=0; i<Format->GetNumFields(); i++ ){
    if( Format->GetFieldType(Format->GetFieldName(i)) ==
        CoreGenInstFormat::CGInstCode ){
      // found an encoding field
      if( first ){
        first = false;
      }else{
        Str += ",";
      }
      Str += ("bits<" + std::to_string(Format->GetFieldWidth(Format->GetFieldName(i))) +
              "> " + Format->GetFieldName(i) );
      if( first ){
        first = false;
      }
    }
  }

  return Str;
}

std::string LLVM801CG::TIDerivePseudoInstArgs(CoreGenPseudoInst *PInst){
  std::string Str;
  bool first = true;

  CoreGenInstFormat *F = PInst->GetInst()->GetFormat();

  for( unsigned i=0; i<F->GetNumFields(); i++ ){
    if( F->GetFieldType(F->GetFieldName(i)) == CoreGenInstFormat::CGInstReg ){
      if( first ){
        first = false;
      }else{
        Str += ",";
      }

      bool isEnc = false;
      uint64_t Enc = 0x00ull;

      for( unsigned j=0; j<PInst->GetNumEncodings(); j++ ){
        if( PInst->GetEncoding(j)->GetField() ==
            F->GetFieldName(i) ){
          isEnc = true;
          Enc = PInst->GetEncoding(j)->GetEncoding();
        }
      }

      if( isEnc ){
        // find the register name that matches 'Enc'
        CoreGenRegClass *RC = F->GetFieldRegClass(F->GetFieldName(i));
        Str += RC->GetRegByIndex((unsigned)(Enc))->GetName();
      }else{
        Str += (F->GetFieldRegClass(F->GetFieldName(i))->GetName() + ":$" +
                F->GetFieldName(i));
      }

    }else if( F->GetFieldType(F->GetFieldName(i)) == CoreGenInstFormat::CGInstImm ){
      if( first ){
        first = false;
      }else{
        Str += ",";
      }

      bool isEnc = false;
      uint64_t Enc = 0x00ull;

      for( unsigned j=0; j<PInst->GetNumEncodings(); j++ ){
        if( PInst->GetEncoding(j)->GetField() ==
            F->GetFieldName(i) ){
          isEnc = true;
          Enc = PInst->GetEncoding(j)->GetEncoding();
        }
      }

      if( isEnc ){
        Str += (std::to_string(Enc));
      }else{
        Str += ("imm" +
              std::to_string(F->GetFieldWidth(F->GetFieldName(i))) +
              ":$" +
              F->GetFieldName(i));
      }
    }
  }

  return Str;
}

bool LLVM801CG::TIGenerateISATablegen(){

  std::string OutFile = LLVMRoot + "/" + TargetName + "InstrFormats.td";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the InstrFormats tablegen file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName << "InstrFormats.td - " << TargetName
            << "Instruction Formats ---*- tablegen -*-===//" << std::endl;

  // output the instformat definition
  unsigned NBits = TIGenerateInstFormatBits(Formats.size());
  OutStream << "class InstFormat<bits<" << NBits << "> val> {" << std::endl;
  OutStream << "  bits<" << NBits << "> Value = val;" << std::endl;
  OutStream << "}" << std::endl << std::endl;

  // output all the instruction format in numeric order
  for( unsigned i=0; i<Formats.size(); i++ ){
    OutStream << "def InstFormat" << Formats[i]->GetName()
              << " : InstFormat<" << i << ">;" << std::endl;
  }
  OutStream << std::endl << std::endl;

  // output each instruction format template
  for( unsigned i=0; i<Formats.size(); i++ ){
    // emit the definition
    OutStream << "class " << Formats[i]->GetName() << "Inst"
              << "<dag outs, dat ins, string opcodestr, string argstr," << std::endl
              << "    list<dag> pattern, InstFormat format," << std::endl
              << "  : Instruction {" << std::endl;

    // emit the body
    // -- size of the instruction
    OutStream << "  field bits<" << Formats[i]->GetFormatWidth() << "> SoftFail = 0;" << std::endl;
    OutStream << "  let Size = " << Formats[i]->GetFormatWidth()/8 << ";" << std::endl << std::endl;
    OutStream << "  let Namespace = \"" << TargetName << "\";" << std::endl << std::endl;

    // -- assign all the dag nodes
    OutStream << "  dag OutOperandList = outs;" << std::endl
              << "  dag InOperandList = ins; " << std::endl
              << "  let AsmString = opcodestr # \"\\t\" # argstr;" << std::endl
              << "  let Pattern = pattern;" << std::endl << std::endl;


    OutStream << "  let TSFlags{4-0} = format.Value;" << std::endl;

    // emit the closing brace
    OutStream << "}" << std::endl << std::endl;
  }

  // emit each template
  for( unsigned i=0; i<Formats.size(); i++ ){
    OutStream << "class " << Formats[i]->GetName()
              << "<" << TIGenerateInstArgsFields(Formats[i])
              << ",dag outs, dat ins, string opcodestr, string argstr>"
              << std::endl
              << "    : " << Formats[i]->GetName()
              << "Inst<out, ins, opcodestr, argstr, [], "
              << "InstFormat" << Formats[i]->GetName() << "> {" << std::endl;

    // -- output all the register or immediate args
    for( unsigned j=0; j<Formats[i]->GetNumFields(); j++ ){
      CoreGenInstFormat::CGInstField IF = Formats[i]->GetFieldType(Formats[i]->GetFieldName(j));
      if( (IF==CoreGenInstFormat::CGInstReg) ||
          (IF==CoreGenInstFormat::CGInstImm) ){
        OutStream << "  bits<" << Formats[i]->GetFieldWidth(Formats[i]->GetFieldName(j))
                  << "> " << Formats[i]->GetFieldName(j) << ";" << std::endl;
      }
    }

    // -- assign all the encodings
    for( unsigned j=0; j<Formats[i]->GetNumFields(); j++ ){
      std::string FName = Formats[i]->GetFieldName(j);
      CoreGenInstFormat::CGInstField IF = Formats[i]->GetFieldType(FName);

      // only assign the known values
      if( IF != CoreGenInstFormat::CGInstUnk ){
        if( Formats[i]->GetFieldWidth(FName) == 1 ){
          // single bit field
          OutStream << "  let Inst{" << Formats[i]->GetStartBit(FName)
                    << "} = " << FName << ";" << std::endl;
        }else{
          // multi-bit field
          OutStream << "  let Inst{" << Formats[i]->GetEndBit(FName) << "-"
                    << Formats[i]->GetStartBit(FName)
                    << "} = " << FName << ";" << std::endl;
        }
      }
    }

    // -- close the class
    OutStream << "}" << std::endl << std::endl;
  }

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateRegisterTablegen(){

  std::string OutFile = LLVMRoot + "/" + TargetName + "RegisterInfo.td";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the RegisterInfo tablegen file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName << "RegisterInfo.td - " << TargetName
            << "Register Definitions ---*- tablegen -*-===//" << std::endl;

  OutStream << "let Namespace = \"" << TargetName << "\" in {" << std::endl;

  // stage 1: walk all the register classes and generate the top-level class defs
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    OutStream << "class " << TargetName << RegClasses[i]->GetName()
              << "<bits<" << TIGenerateInstFormatBits(RegClasses[i]->GetMaxWidth())
              << "> Enc, string n, list<string> alt = []> : Register<n> {" << std::endl;
    OutStream << "  let HWEncoding{"
              << TIGenerateInstFormatBits(RegClasses[i]->GetMaxWidth())-1
              << "-0} = Enc;" << std::endl;
    OutStream << "  let AltNames = alt;" << std::endl;
    OutStream << "}" << std::endl;
  }

  OutStream << "def ABIRegAltname : RegAltNameIndex;" << std::endl;
  OutStream << "}" << std::endl << std::endl; // end the namespace

  // stage 2: output the sequence of registers from each register class
  //          track the global dwarf number
  unsigned DwarfNum = 0;
  CoreGenReg *Reg = nullptr;
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    OutStream << "let RegAltNameIndices = [ABIRegAltName] in {" << std::endl;
    for( unsigned j=0; j<RegClasses[i]->GetNumReg(); j++ ){
      Reg = RegClasses[i]->GetReg(j);

      OutStream << "  def " << UpperCase(Reg->GetName())
                << " : " << TargetName << RegClasses[i]->GetName()
                << "<" << Reg->GetIndex()
                << ",\"" << Reg->GetName() << "\", [\""
                << Reg->GetName() << "\"]>, DwarfRegNum<["
                << DwarfNum << "]>;" << std::endl;
      DwarfNum++;
    }
    OutStream << "}" << std::endl << std::endl;
  }

  // Stage 3: output each of the register class definitions
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    OutStream << "def " << UpperCase(RegClasses[i]->GetName())
              << " : RegisterClass<\"" << TargetName
              << "\", [i32], " << RegClasses[i]->GetMaxWidth()  //TODO: we need to fix this based upon the memory model
              << ", (add" << std::endl;
    for( unsigned j=0; j<RegClasses[i]->GetNumReg(); j++ ){
      OutStream << "    " << UpperCase(RegClasses[i]->GetReg(j)->GetName());
      if( j != RegClasses[i]->GetNumReg()-1 )
        OutStream << ",";
      OutStream << std::endl;
    }
    // TODO: this needs to be fixed based upon the memory model
    unsigned MW = RegClasses[i]->GetMaxWidth();
    OutStream << "  )> {" << std::endl;
    OutStream << "  let RegInfos = RegInfoByHwMode<[DefaultMode]," << std::endl
              << "                                 RegInfo<"
              << MW << "," << MW << "," << MW << ">]>;" << std::endl;
    OutStream << "}" << std::endl << std::endl;
  }

  OutStream.close();

  return true;
}

std::map<std::string,unsigned> LLVM801CG::TIGenerateImmFieldVector(){
  std::map<std::string,unsigned> Imms;
  // walk all the instruction formats and find all the immediate fields
  std::string FieldName;
  unsigned Width;
  for( unsigned i=0; i<Formats.size(); i++ ){
    for( unsigned j=0; j<Formats[i]->GetNumFields(); j++ ){
      FieldName = Formats[i]->GetFieldName(j);
      if( Formats[i]->GetFieldType(FieldName) == CoreGenInstFormat::CGInstImm ){
        // found an immediate field
        Width = Formats[i]->GetFieldWidth(FieldName);
        Imms.insert( std::pair<std::string,unsigned>("imm" + std::to_string(Width),
                                                     Width) );
      }
    }
  }
  return Imms;
}

bool LLVM801CG::TIGenerateSystemTablegen(){
  std::string OutFile = LLVMRoot + "/" + TargetName + "SystemOperands.td";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the InstrInfo tablegen file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName << "SystemOperands.td - " << TargetName
            << "Symbolic Operands ---*- tablegen -*-===//" << std::endl;

  OutStream << "include \"llvm/TableGen/SearchableTable.td\"" << std::endl << std::endl;

  // TBD whether we need anything else here

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateInstTablegen(){

  std::string OutFile = LLVMRoot + "/" + TargetName + "InstrInfo.td";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the InstrInfo tablegen file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName << "InstrInfo.td - " << TargetName
            << "Instruction Definitions ---*- tablegen -*-===//" << std::endl;

  // Stage 1: write out operand fields & bare symbols
  std::map<std::string, unsigned> ImmFields = TIGenerateImmFieldVector();
  std::map<std::string,unsigned>::iterator it;
  for( it = ImmFields.begin(); it != ImmFields.end(); ++it ){
    OutStream << "def " << it->first
              << " : Operand<i32>, ImmLeaf<i32, [{return isUInt<"
              << it->second << ">(Imm);}]> {" << std::endl;
    OutStream << "  let ParserMatchClass = UImmAsmOperand<5>;" << std::endl;
    OutStream << "  let DecoderMethod =\"decodeUImmOperand<"
              << it->second << ">\";" << std::endl;
    OutStream << "}" << std::endl << std::endl;
  }

  OutStream << "def BareSymbol : AsmOperandClass {" << std::endl
            << "  let Name = \"BareSymbol\";" << std::endl
            << "  let RenderMethod = \"addImmOperands\";" << std::endl
            << "  let DiagnosticType = \"InvalidBareSymbol\";" << std::endl
            << "  let ParserMethod = \"parseBareSymbol\";" << std::endl << std::endl;

  // Stage 2: write out the instruction format includes
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  OutStream << "//===-- Instruction Formats" << std::endl;
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  OutStream << "include \"" << TargetName << "InstrFormats.td\"" << std::endl << std::endl;

  // Stage 3: write out the instruction class templates
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  OutStream << "//===-- Instruction Class Templates" << std::endl;
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;

  for( unsigned i=0; i<Formats.size(); i++ ){
    OutStream << "let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in" << std::endl;
    OutStream << "class " << Formats[i]->GetName() << "Impl<"
              << TIGenerateInstArgsFields(Formats[i]) << ", string opcodestr>"
              << std::endl
              << "    : " << Formats[i]->GetName() << "<"
              << TIGenerateInstFields(Formats[i])
              << ",(outs), (ins " << TIGenerateRegClassImmList(Formats[i])
              << "), opcodestr, \"" << TIGenerateRegImmList(Formats[i])
              << "\">;" << std::endl << std::endl;
  }

  // Stage 4: write out all the instructions
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  OutStream << "//===-- Instructions" << std::endl;
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;

  for( unsigned i=0; i<Insts.size(); i++ ){
    CoreGenInstFormat *F = Insts[i]->GetFormat();
    bool first = true;
    OutStream << "def " << Insts[i]->GetName() << " : "
              << F->GetName() + "Impl<";
    for( unsigned j=0; j<F->GetNumFields(); j++ ){
      if( F->GetFieldType(F->GetFieldName(j)) == CoreGenInstFormat::CGInstCode ){
        // found an encoding field, output it
        if( first ){
          first = false;
        }else{
          OutStream << ",";
        }
        OutStream << Insts[i]->GetEncoding(F->GetFieldName(j));
      }
    }
    OutStream << "\"" << Insts[i]->GetName() << "\"";
    OutStream << ">;" << std::endl;
  }

  // Stage 5: write out all the pseudo instructions
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;
  OutStream << "//===-- Pseudo Instructions" << std::endl;
  OutStream << "//===----------------------------------------------------------------------===//" << std::endl;

  for( unsigned i=0; i<PInsts.size(); i++ ){
    OutStream << "def : InstAlias<\""
              << PInsts[i]->GetName() << "\",("
              << PInsts[i]->GetInst()->GetName() << " "
              << TIDerivePseudoInstArgs(PInsts[i])
              << ")>;" << std::endl;
  }

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateTablegen(){

  // Stage 1: generate the top-level SoC tablegen file: done;
  if( !TIGenerateTopLevelTablegen() )
    return false;

  // Stage 2: generate format tablegens for each ISA: done;
  if( !TIGenerateISATablegen() )
    return false;

  // Stage 3: generate register info tablegen: done;
  if( !TIGenerateRegisterTablegen() )
    return false;

  // Stage 4: generate instruction info tablegen: done;
  if( !TIGenerateInstTablegen() )
    return false;

  // Stage 5: generate system operands tablegen???
  if( !TIGenerateSystemTablegen() )
    return false;

  // Stage 6: generate calling convention tablegen?

  return true;
}

bool LLVM801CG::TIGenerateISelDag(){
  // <TargetName>ISelDAGToDAG.cpp
  return true;
}

bool LLVM801CG::TIGenerateInstLowering(){
  // <TargetName>ISelLowering.{h,cpp}
  return true;
}

bool LLVM801CG::TIGenerateRegInfo(){
  return true;
}

bool LLVM801CG::TIGenerateSubtargetInfo(){

  // Stage 1: Generate the header file
  std::string OutFile = LLVMRoot + "/" + TargetName + "Subtarget.h";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the Subtarget header file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "Subtarget.h - Define Subtarget for the " << TargetName
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#ifndef LLVM_LIB_TARGET_" << TargetName << "_"
            << TargetName << "SUBTARGET_H" << std::endl;
  OutStream << "#define LLVM_LIB_TARGET_" << TargetName << "_"
            << TargetName << "SUBTARGET_H" << std::endl << std::endl;

  // print the headers
  OutStream << "#include \"" << TargetName << "FrameLowering.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "ISelLowering.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "InstrInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/SelectionDAGTargetInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/TargetSubtargetInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/IR/DataLayout.h\"" << std::endl;
  OutStream << "#include \"llvm/Target/TargetMachine.h\"" << std::endl << std::endl;

  OutStream << "#define GET_SUBTARGETINFO_HEADER" << std::endl;
  OutStream << "#include \"" << TargetName << "GenSubtargetInfo.inc\""
            << std::endl << std::endl;

  OutStream << "namespace llvm {" << std::endl;
  OutStream << "class StringRef;" << std::endl << std::endl;

  OutStream << "class " << TargetName << "Subtarget : public "
            << TargetName << "GenSubtargetInfo {" << std::endl;

  // output the body of the class
  OutStream << "  virtual void anchor()" << std::endl;
  for( unsigned i=0; i<Subtargets.size(); i++ ){
    OutStream << "  bool Has" << Subtargets[i] << " = false; " << std::endl;
  }
  OutStream << "  bool EnableLinkerRelax = false;" << std::endl;
  // TODO: derive the MVT type
  //       see reference here: https://llvm.org/doxygen/classllvm_1_1MVT.html
  OutStream << "  MVT XLenVT = MVT::i32;" << std::endl;
  OutStream << "  " << TargetName << "FrameLowering FrameLowering;" << std::endl;
  OutStream << "  " << TargetName << "InstrInfo InstrInfo;" << std::endl;
  OutStream << "  " << TargetName << "RegisterInfo RegInfo;" << std::endl;
  OutStream << "  " << TargetName << "TargetLowering TLInfo;" << std::endl;
  OutStream << "  SelectionDAGTargetInfo TSInfo;" << std::endl << std::endl;

  OutStream << "  " << TargetName << "Subtarget &initializeSubtargetDependencies("
            << "StringRef CPU, StringRef FS, bool Is64Bit);" << std::endl << std::endl;

  OutStream << "public: " << std::endl << std::endl;

  OutStream << "  " << TargetName << "Subtarget(const Triple &TT, const std::string &CPU,"
            << " const std::string &FS, const TargetMachine &TM);" << std::endl << std::endl;

  OutStream << "  void ParseSubtargetFeatures(StringRef CPU, StringRef FS);" << std::endl << std::endl;

  OutStream << "  const " << TargetName << "FrameLowering *getFrameLowering() const override {" << std::endl;
  OutStream << "    return &FrameLowering;" << std::endl;
  OutStream << "  }" << std::endl;
  OutStream << "  const " << TargetName << "InstrInfo *getInstrInfo() const override { return &InstrInfo; }" << std::endl;
  OutStream << "  const " << TargetName << "RegisterInfo *getRegisterInfo() const override {" << std::endl;
  OutStream << "    return &RegInfo;" << std::endl;
  OutStream << "  }" << std::endl;
  OutStream << "  const " << TargetName << "TargetLowering *getTargetLowering() const override {" << std::endl;
  OutStream << "    return &TSInfo;" << std::endl;
  OutStream << "  }" << std::endl;
  OutStream << "  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {" << std::endl;
  OutStream << "    return &TSinfo;" << std::endl;
  OutStream << "  }" << std::endl << std::endl;

  for( unsigned i=0; i<Subtargets.size(); i++ ){
    OutStream << "  bool has" << Subtargets[i] << "() const { return Has"
              << Subtargets[i] << "; }" << std::endl;
  }

  OutStream << std::endl;

  OutStream << "  bool enableLinkerRelax() const { return EnableLinkerRelax; }" << std::endl;
  OutStream << "  MVT getXLenVT() const { return XLenVT; }" << std::endl;

  OutStream << "};" << std::endl;

  OutStream << "} # End LLVM namespace" << std::endl;
  OutStream << "#endif" << std::endl;

  OutStream.close();

  // Stage 2: generate the cpp file
  OutFile = LLVMRoot + "/" + TargetName + "Subtarget.cpp";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the Subtarget implementation file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "Subtarget.cpp - " << TargetName << " Subtarget Information "
            << "------------------===//" << std::endl;

  OutStream << "#include \"" << TargetName << "Subtarget.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << ".h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "FrameLowering.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/TargetRegistry.h\"" << std::endl << std::endl;

  OutStream << "using namespace llvm;" << std::endl << std::endl;

  OutStream << "#define DEBUG_TYPE \"" << TargetName << "-subtarget\"" << std::endl << std::endl;

  OutStream << "#define GET_SUBTARGETINFO_TARGET_DESC" << std::endl;
  OutStream << "#define GET_SUBTARGETINFO_CTOR" << std::endl;
  OutStream << "#include \"" << TargetName << "SubtargetInfo.inc\"" << std::endl << std::endl;

  OutStream << "void " << TargetName << "Subtarget::anchor() {}" << std::endl << std::endl;

  OutStream << TargetName << "Subtarget &" << TargetName
            << "Subtarget::::initializeSubtargetDependencies(StringRef CPU,"
            << std::endl
            << "                                             StringRef FS) {"
            << std::endl
            << "  std::string CPUName = CPU;" << std::endl
            << "  if( CPUName.empty() )  CPUName = \"generic-" << TargetName << "\";"
            << std::endl
            << "  ParseSubtargetFeatures(CPUName, FS);"
            << std::endl
            << "  return *this;" << std::endl
            << "}" << std::endl << std::endl;

  OutStream << TargetName << "Subtarget::" << TargetName << "Subtarget("
            << "          const Triple &TT, const std::string &CPU,"
            << std::endl
            << "          const std::string &FS, const TargetMachine &TM)"
            << std::endl
            << "     : " << TargetName << "GenSubtargetInfo(TT, CPU, FS),"
            << std::endl
            << "       FrameLowering(initializeSubtargetDependencies(CPU, FS, TT.isArch64Bit())),"
            << std::endl
            << "       InstrInfo(), RegInfo(getHwMode()), TLInfo(TM, *this) {}"
            << std::endl << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateTargetMach(){
  // <TargetName>TargetMachine.{h,cpp}
  std::string OutFile = LLVMRoot + "/" + TargetName + "TargetMachine.h";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the target machine header file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "TargetMachine.h - Define TargetMachine for " << TargetName
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#ifndef LLVM_LIB_TARGET_" << TargetName << "_"
            << TargetName << "TARGETMACHINE_H" << std::endl;
  OutStream << "#define LLVM_LIB_TARGET_" << TargetName << "_"
            << TargetName << "TARGETMACHINE_H" << std::endl << std::endl;

  OutStream << "#include \"MCTargetDesc/" << TargetName << "MCTargetDesc.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "Subtarget.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/SelectionDAGTargetInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/IR/DataLayout.h\"" << std::endl;
  OutStream << "#include \"llvm/Target/TargetMachine.h\"" << std::endl << std::endl;

  OutStream << "namespace llvm{" << std::endl;
  OutStream << "class " << TargetName << "TargetMachine : public LLVMTargetMachine {" << std::endl;
  OutStream << "  std::unique_ptr<TargetLoweringObjectFile> TLOF;" << std::endl;
  OutStream << "  " << TargetName << "Subtarget Subtarget;" << std::endl;
  OutStream << "public:" << std::endl;
  OutStream << "  " << TargetName << "TargetMachine(const Target &T, const Triple &TT, StringRef CPU,"
            << std::endl
            << "                     StringRef FS, const TargetOptions &Options,"
            << std::endl
            << "                     Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,"
            << std::endl
            << "                     CodeGenOpt::Level OL, bool JIT);" << std::endl;
  OutStream << "  const " << TargetName << "Subtarget *getSubtargetImpl(const Function &) const override {"
            << std::endl
            << "  return &Subtarget;"
            << std::endl
            << "}" << std::endl << std::endl;
  OutStream << "  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;" << std::endl;
  OutStream << "  TargetLoweringObjectFile *getObjFileLowering() const override {"
            << "    return TLOF.get();"
            << std::endl
            << "}"
            << std::endl;
  OutStream << "};" << std::endl;
  OutStream << "}" << std::endl;
  OutStream << "#endif" << std::endl;

  OutStream.close();

  OutFile = LLVMRoot + "/" + TargetName + "TargetMachine.cpp";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the target machine implementation file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "TargetMachine.cpp - Define TargetMachine for " << TargetName
            << " -----------===//" << std::endl << std::endl;

  OutStream << "#include \"" << TargetName << ".h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "TargetMachine.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "TargetObjectFile.h\"" << std::endl;
  OutStream << "#include \"llvm/ADT/STLExtras.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/Passes.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/TargetLoweringObjectFileImpl.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/TargetPassConfig.h\"" << std::endl;
  OutStream << "#include \"llvm/IR/LegacyPassManager.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/FormattedStream.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/TargetRegistry.h\"" << std::endl;
  OutStream << "#include \"llvm/Target/TargetOptions.h\"" << std::endl << std::endl;

  OutStream << "using namespace llvm;" << std::endl;

  OutStream << "extern \"C\" void LLVMInitialize" << TargetName << "Target() {"
            << "  RegisterTargetmachine<" << TargetName
            << "TargetMachine> X(getThe" << TargetName << "Target());"
            << std::endl
            << "  auto PR = PassRegistry::getPassRegistry();"
            << std::endl
            << "  initialize" << TargetName << "ExpandPseudoPass(*PR);"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "static std::string computeDataLayout(const Triple &TT) {"
            << std::endl
            // TODO : calculate this based upon the derived memory model
            << "  return \"e-m:e-p:64:64-i64:64-i128:128-n64-S128\""
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "static Reloc::Model getEffectiveRelocModel(const Triple &TT,"
            << "                                           Optional<Reloc::Model> RM) {"
            << std::endl
            << "  if (!RM.hasValue() )"
            << std::endl
            << "    return Reloc::Static;"
            << std::endl
            << "  return *RM;"
            << std::endl << std::endl;

  OutStream << TargetName << "TargetMachine::" << TargetName << "TargetMachine("
            << "     const Target &T, const Triple &TT,"
            << std::endl
            << "     StringRef CPU, StringRef FS,"
            << std::endl
            << "     const TargetOptions &Options,"
            << std::endl
            << "     Optional<Reloc::Model> RM,"
            << std::endl
            << "     Optional<CodeModel::Model> CM,"
            << std::endl
            << "     CodeGenOpt::Level OL, bool JIT)"
            << std::endl
            << "   : LLVMTargetMachine(T, computeDataLayout(TT), TT, CPU, FS, Options,"
            << std::endl
            << "                 getEffectiveRelocModel(TT, RM),"
            << std::endl
            << "                 getEffectiveCodeModel(CM, CodeModel::Small), OL),"
            << "     TLOF(make_unique<RISCVELFTargetObjectFile>()),"
            << "     Subtarget(TT, CPU, FS, *this) {"
            << "  initAsmInfo();"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "namespace {" << std::endl;
  OutStream << "class " << TargetName << "PassConfig ; public TargetPassConfig {"
            << std::endl
            << "public:"
            << std::endl
            << "  " << TargetName << "PassConfig(" << TargetName
            << "TargetMachine &TM, PassManagerBase &PM)" << std::endl
            << "       : TargetPassConfig(TM, PM) {}"
            << std::endl << std::endl
            << TargetName << "TargetMachine &get" << TargetName << "TargetMachine() const {"
            << std::endl
            << "    return getTM<" << TargetName << "TargetMachine>();"
            << std::endl
            << "  }" << std::endl << std::endl
            << "  void addIRPasses() override;" << std::endl
            << "  void addInstSelector() override;" << std::endl
            << "  void addPreEmitPass() override;" << std::endl
            << "  void addPreEmitPass2() override;" << std::endl
            << "  void addPreRegAlloc() override;" << std::endl
            << "};"<< std::endl << "}" << std::endl << std::endl;

  OutStream << "TargetPassConfig *" << TargetName
            << "TargetMachine::createPassConfig(PassManagerBase &PM) {"
            << "  return new " << TargetName << "PassConfig(*this, PM);" << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "void " << TargetName << "PassConfig::addIRPasses() {"
            << std::endl
            << "  TargetPassConfig::addIRPasses();"
            << std::endl
            << "}"
            << std::endl << std::endl;

  OutStream << "bool " << TargetName << "PassConfig::addInstSelector() {"
            << std::endl
            << "  addPass(create" << TargetName << "ISelDag(get"
            << TargetName << "TargetMachine()));"
            << std::endl
            << "  return false;"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "void " << TargetName << "PassConfig::addPreEmitPass() { }"
            << std::endl << std::endl;

  OutStream << "void " << TargetName << "PassConfig::addPreEmitPass2() { }"
            << std::endl << std::endl;

  OutStream << "void " << TargetName << "PassConfig::addPreRegAlloc() {"
            << std::endl
            << "}" << std::endl << std::endl;

  return true;
}

bool LLVM801CG::TIGenerateTargetObj(){
  return true;
}

bool LLVM801CG::TIGenerateAsmParser(){
  return true;
}

bool LLVM801CG::TIGenerateDisass(){
  return true;
}

bool LLVM801CG::TIGenerateInstPrinter(){
  return true;
}

bool LLVM801CG::TIGenerateMCTargetDesc(){
  return true;
}

bool LLVM801CG::TIGenerateTargetInfo(){

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

bool LLVM801CG::TIGenerateTargetHeader(){
  std::string OutFile = LLVMRoot + "/" + TargetName + ".h";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open top-level header file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << ".h - Top-level interface for " << TargetName
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#ifndef LLVM_LIB_TARGET_" << TargetName << "_" << TargetName << "_H" << std::endl;
  OutStream << "#define LLVM_LIB_TARGET_" << TargetName << "_" << TargetName << "_H" << std::endl;
  OutStream << std::endl;
  OutStream << "#include \"llvm/Target/TargetMachine.h\"" << std::endl << std::endl;
  OutStream << "namespace llvm {" << std::endl;

  OutStream << "void Lower" << TargetName
            << "MachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,"
            << std::endl
            << "                      const AsmPrinter &AP);" << std::endl;
  OutStream << "bool Lower" << TargetName
            << "MachineOperandToMCOperand(const MachineOperand &MO,"
            << "                      MCOperand &MCOp, const AsmPrinter &AP);"
            << std::endl;
  OutStream << "FunctionPass *create" << TargetName << "ISelDag("
            << TargetName << "TargetMachine &TM);" << std::endl;

  OutStream << "} // end LLVM namespace" << std::endl;
  OutStream << "#endif" << std::endl;
  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateCmake(){
  std::string OutFile = LLVMRoot + "/CMakeLists.txt;";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open top-level CMakeLists: " + OutFile );
    return false;
  }

  OutStream << "set(LLVM_TARGET_DEFINITIONS " << TargetName << ".td)" << std::endl << std::endl;

  OutStream << "tablegen(LLVM " << TargetName << "GenAsmMatcher.inc -gen-asm-matcher)" << std::endl;
  OutStream << "tablegen(LLVM " << TargetName << "GenAsmWriter.inc -gen-asm-writer)" << std::endl;
  OutStream << "tablegen(LLVM " << TargetName << "GenCompressInstEmitter.inc -gen-compress-inst-emitter)" << std::endl;
  OutStream << "tablegen(LLVM " << TargetName << "GenDAGISel.inc -gen-dag-isel)" << std::endl;
  OutStream << "tablegen(LLVM " << TargetName << "GenDisassemblerTables.inc -gen-disassembler)" << std::endl;
  OutStream << "tablegen(LLVM " << TargetName << "GenInstrInfo.inc -gen-instr-info)" << std::endl;
  OutStream << "tablegen(LLVM " << TargetName << "GenMCCodeEmitter.inc -gen-emitter)" << std::endl;
  OutStream << "tablegen(LLVM " << TargetName << "GenMCPseudoLowering.inc -gen-pseudo-lowering)" << std::endl;
  OutStream << "tablegen(LLVM " << TargetName << "GenRegisterInfo.inc -gen-register-info)" << std::endl;
  OutStream << "tablegen(LLVM " << TargetName << "GenSubtargetInfo.inc -gen-subtarget)" << std::endl;
  OutStream << "tablegen(LLVM " << TargetName << "GenSystemOperands.inc -gen-searchable-tables)" << std::endl << std::endl;

  OutStream << "add_public_tablegen_target(" << TargetName << "CommonTableGen)" << std::endl << std::endl;

  OutStream << "add_llvm_target(" << TargetName << "CodeGen" << std::endl
            << " " << TargetName << "AsmPrinter.cpp" << std::endl
            << " " << TargetName << "ExpandPseudoInsts.cpp" << std::endl
            << " " << TargetName << "FrameLowering.cpp" << std::endl
            << " " << TargetName << "InstrInfo.cpp" << std::endl
            << " " << TargetName << "ISelDAGToDAG.cpp" << std::endl
            << " " << TargetName << "ISelLowering.cpp" << std::endl
            << " " << TargetName << "MCInstLower.cpp" << std::endl
            << " " << TargetName << "MergeBaseOffset.cpp" << std::endl
            << " " << TargetName << "RegisterInfo.cpp" << std::endl
            << " " << TargetName << "Subtarget.cpp" << std::endl
            << " " << TargetName << "TargetMachine.cpp" << std::endl
            << " " << TargetName << "TargetObjectFile.cpp" << std::endl
            << ")" << std::endl << std::endl;

  OutStream << "add_subdirectory(AsmParser)" << std::endl;
  OutStream << "add_subdirectory(Disassembler)" << std::endl;
  OutStream << "add_subdirectory(InstPrinter)" << std::endl;
  OutStream << "add_subdirectory(MCTargetDesc)" << std::endl;
  OutStream << "add_subdirectory(TargetInfo)" << std::endl;
  OutStream << "add_subdirectory(Utils)" << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::GenerateTargetImpl(){

  // Stage 1: Create the tablegen infrastructure
  if( !TIGenerateTablegen() )
    return false;

  // Stage 2: Create the ISelDagToDag template
  if( !TIGenerateISelDag() )
    return false;

  // Stage 3: Create the top-level header; done;
  if( !TIGenerateTargetHeader() )
    return false;

  // Stage 4: Create the instruction lowering template
  if( !TIGenerateInstLowering() )
    return false;

  // Stage 5: Create the register info template
  if( !TIGenerateRegInfo() )
    return false;

  // Stage 6: Create the subtarget info template: done;
  if( !TIGenerateSubtargetInfo() )
    return false;

  // Stage 7: Create the target machine template: done;
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

  // Stage 13: Create the TargetInfo; done;
  if( !TIGenerateTargetInfo() )
    return false;

  // Stage 14: Create the top-level CMake script; done;
  if( !TIGenerateCmake() )
    return false;

  return true;
}

bool LLVM801CG::GenerateTargetDir(){
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

bool LLVM801CG::Execute(){

  // Stage 1: Execute the backend codegen to generate all the necessary vectors
  if( !ExecuteBackend() )
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
