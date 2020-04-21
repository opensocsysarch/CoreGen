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
  OutStream << "include \"" << TargetName << "CallingConv.td\"" << std::endl;
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
              << "\", [" << AddrMode << "], " << RegClasses[i]->GetMaxWidth()  //TODO: we need to fix this based upon the memory model
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
    Errno->SetError(CGERR_ERROR, "Could not open the SystemOperands tablegen file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName << "SystemOperands.td - " << TargetName
            << "Symbolic Operands ---*- tablegen -*-===//" << std::endl;

  OutStream << "include \"llvm/TableGen/SearchableTable.td\"" << std::endl << std::endl;

  // TBD whether we need anything else here

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateCallingConvTablegen(){
  std::string OutFile = LLVMRoot + "/" + TargetName + "CallingConv.td";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the CallingConv tablegen file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName << "CallingConv.td - " << TargetName
            << "Calling convention ---*- tablegen -*-===//" << std::endl;

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
              << " : Operand<" << AddrMode << ">, ImmLeaf<" << AddrMode << ", [{return isUInt<"
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

  // Stage 5: generate system operands tablegen: done;
  if( !TIGenerateSystemTablegen() )
    return false;

  // Stage 6: generate calling convention tablegen: done;
  if( !TIGenerateCallingConvTablegen() )
    return false;

  return true;
}

bool LLVM801CG::TIGenerateISelDag(){
  // <TargetName>ISelDAGToDAG.cpp
  std::string OutFile = LLVMRoot + "/" + TargetName + "ISelDAGToDAG.cpp";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the ISelDAGToDAG.cpp file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "ISelDAGToDAG.cpp - A dag to dag inst selector for "
            << TargetName << "------===//" << std::endl << std::endl;

  // write out the headers
  OutStream << "#include \"MCTargetDesc/RISCVMCTargetDesc.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << ".h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "TargetMachine.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineFrameInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/SelectionDAGISel.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/Debug.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/MathExtras.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/raw_ostream.h\"" << std::endl << std::endl;

  OutStream << "using namespace llvm;" << std::endl << std::endl;

  OutStream << "#define DEBUG_TYPE \"" << TargetName << "-isel\"" << std::endl << std::endl;

  OutStream << "namespace {" << std::endl;
  OutStream << "class " << TargetName << "DAGToDAGISel final : public SelectionDAGISel {" << std::endl;
  OutStream << "  const " << TargetName << "Subtarget *Subtarget;" << std::endl << std::endl;
  OutStream << "public:" << std::endl;
  OutStream << "  explicit " << TargetName << "DAGToDAGISel(" << TargetName << "TargetMachine &TargetMachine)" << std::endl;
  OutStream << "      : SelectionDAGISel(TargetMachine) {}" << std::endl << std::endl;
  OutStream << "  StringRef getPassName() const override {" << std::endl;
  OutStream << "    return \"" << TargetName
            << " DAG->DAG Pattern Instruction Selection\";" << std::endl << std::endl;
  OutStream << "  }" << std::endl << std::endl;

  OutStream << "  bool runOnMachineFunction(MachineFunction &MF) override {" << std::endl;
  OutStream << "    Subtarget = &MF.getSubtarget<" << TargetName << "Subtarget>();"<< std::endl;
  OutStream << "    return SelectionDAGISel::runOnMachineFunction(MF);" << std::endl;
  OutStream << "  }" << std::endl << std::endl;

  OutStream << "  void PostprocessISelDAG() override;" << std::endl << std::endl;

  OutStream << "  void Select(SDNode *Node) override;" << std::endl << std::endl;

  OutStream << "  bool SelectInlineAsmMemoryOperand(const SDValue &Op, unsigned ConstraintID,"
            << std::endl
            << "                                    std::vector<SDValue> &OutOps) override;"
            << std::endl << std::endl;

  OutStream << "#include \"" << TargetName << "GenDAGISel.inc\"" << std::endl << std::endl;
  OutStream << "private:" << std::endl;
  OutStream << "}" << std::endl << std::endl;

  OutStream << "void " << TargetName << "DAGToDAGISel::PostprocessISelDAG() {"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "void " << TargetName << "DAGToDAGISel::Select(SDNode *Node) {" << std::endl;
  OutStream << "  if (Node->isMachineOpcode()) {" << std::endl;
  OutStream << "    LLVM_DEBUG(dbgs() << \"== \"; Node->dump(CurDAG); dbgs() << \"\n\");" << std::endl;
  OutStream << "    Node->setNodeId(-1);" << std::endl;
  OutStream << "    return;" << std::endl;
  OutStream << "  }" << std::endl << std::endl;

  OutStream << "  unsigned Opcode = Node->getOpcode();" << std::endl;
  OutStream << "  MVT XLenVT = Subtarget->getXLenVT();" << std::endl; //TODO: do we need this?
  OutStream << "  SDLoc DL(Node);" << std::endl;
  OutStream << "  EVT VT = Node->getValueType(0);" << std::endl;
  OutStream << "  SelectCode(Node);" << std::endl;

  OutStream << "}" << std::endl << std::endl;

  OutStream << "bool " << TargetName << "DAGToDAGISel::SelectInlineAsmMemoryOperand(" << std::endl;
  OutStream << "  const SDValue &Op, unsigned ConstraintID, std::vector<SDValue> &OutOps) {" << std::endl;
  OutStream << "  switch (ConstraintID) {" << std::endl
            << "  case InlineAsm::Constraint_i:" << std::endl
            << "  case InlineAsm::Constraint_m:" << std::endl
            << "    OutOps.push_back(Op);" << std::endl
            << "    return false;" << std::endl
            << "  default:" << std::endl
            << "    break;" << std::endl
            << "  }" << std::endl << std::endl
            << "  return true;" << std::endl;
  OutStream << "}" << std::endl;

  OutStream << "FunctionPass *llvm::create" << TargetName << "ISelDag("
            << TargetName << "TargetMachine &TM) {" << std::endl
            << "  return new " << TargetName << "DAGToDAGISel(TM);"
            << std::endl << "}" << std::endl << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateInstLowering(){
  // <TargetName>ISelLowering.{h,cpp}
  // write out the header first
  std::string OutFile = LLVMRoot + "/" + TargetName + "ISelLowering.h";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the ISelLowering header file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "ISelLowering.h - " << TargetName << " DAG Lowering Interface"
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#ifndef LLVM_LIB_TARGET_" << TargetName
            << "_" << TargetName << "ISELLOWERING_H" << std::endl;
  OutStream << "#define LLVM_LIB_TARGET_" << TargetName
            << "_" << TargetName << "ISELLOWERING_H" << std::endl;

  OutStream << "#include \"" << TargetName << ".h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/SelectionDAG.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/TargetLowering.h\"" << std::endl << std::endl;

  std::cout << "namespace llvm {" << std::endl;
  std::cout << "class " << TargetName << "Subtarget;" << std::endl;
  std::cout << "enum NodeType : unsigned {" << std::endl
            << "\tFIRST_NUMBER = ISD::BUILTIN_OP_END," << std::endl
            << "\tRET_FLAG," << std::endl
            << "\tURET_FLAG," << std::endl
            << "\tSRET_FLAG," << std::endl
            << "\tMRET_FLAG," << std::endl
            << "\tCALL," << std::endl
            << "\tSELECT_CC" << std::endl
            << "\tTAIL" << std::endl
            << "};" << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "class " << TargetName << "TargetLowering : public TargetLowering {" << std::endl;
  OutStream << "\tconst " << TargetName << "Subtarget &Subtarget;" << std::endl;
  OutStream << "public:" << std::endl;
  OutStream << "\texplicit " << TargetName << "TargetLowering(const TargetMachine &TM," << std::endl
            << "\t\tconst " << TargetName << "Subtarget &STI);" << std::endl;
  OutStream << "\tSDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;" << std::endl;

  OutStream << "private:" << std::endl;

  OutStream << "};" << std::endl << "}" << std::endl;

  OutStream << "#endif" << std::endl;
  OutStream.close();

  // write out the implementation file
  OutFile = LLVMRoot + "/" + TargetName + "ISelLowering.cpp";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the ISelLowering implementation file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "ISelLowering.cpp - " << TargetName << " DAG Lowering Interface"
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#include \"" << TargetName << "ISelLowering.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "\"" << std::endl;
  OutStream << "#include \"" << TargetName << "MachineFunctionInfo.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "RegisterInfo.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "Subtarget.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "TargetMachine.h\"" << std::endl;
  OutStream << "#include \"llvm/ADT/SmallSet.h\"" << std::endl;
  OutStream << "#include \"llvm/ADT/Statistic.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/CallingConvLower.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineFrameInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineFunction.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineInstrBuilder.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineRegisterInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/SelectionDAGISel.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/TargetLoweringObjectFileImpl.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/ValueTypes.h\"" << std::endl;
  OutStream << "#include \"llvm/IR/DiagnosticInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/IR/DiagnosticPrinter.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/Debug.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/ErrorHandling.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/raw_ostream.h\"" << std::endl << std::endl;

  OutStream << "using namespace llvm;" << std::endl;

  OutStream << "#define DEBUG_TYPE \"" << TargetName << "-lower\"" << std::endl << std::endl;

  OutStream << TargetName << "TargetLowering::" << TargetName
            << "TargetLowering(const TargetMachine &TM," << std::endl
            << "\t\tconst " << TargetName << "Subtarget &STI)" << std::endl
            << "\t : TargetLowering(TM), Subtarget(STI) {" << std::endl;
  OutStream << "}" << std::endl;

  OutStream << "SDValue " << TargetName
            << "TargetLowering::LowerOperation(SDValue OP," << std::endl
            << "\tSelectionDAG &DAG) const {" << std::endl
            << "\treport_fatal_error(\"unimplemented lowering\");" << std::endl
            << "}" << std::endl;

  return true;
}

bool LLVM801CG::TIGenerateRegInfo(){

  // write out the header first
  std::string OutFile = LLVMRoot + "/" + TargetName + "RegisterInfo.h";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the Subtarget header file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "RegisterInfo.h - " << TargetName << " Register Information Impl"
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#ifndef LLVM_LIB_TARGET_" << TargetName << "_"
            << TargetName << "REGISTERINFO_H" << std::endl;
  OutStream << "#define LLVM_LIB_TARGET_" << TargetName << "_"
            << TargetName << "REGISTERINFO_H" << std::endl << std::endl;

  OutStream << "#include \"llvm/CodeGen/TargetRegisterInfo.h\"" << std::endl << std::endl;

  OutStream << "#define GET_REGINFO_HEADER" << std::endl;
  OutStream << "#include \"" << TargetName << "GenRegisterInfo.inc\"" << std::endl;

  OutStream << "namespace llvm {" << std::endl << std::endl;

  OutStream << "struct " << TargetName << "RegisterInfo : public " << TargetName
            << "GenRegisterInfo {" << std::endl;

  OutStream << "  " << TargetName << "RegisterInfo(unsigned HwMode);" << std::endl;
  OutStream << "  const uint32_t *getCallPreservedMask(const MachineFunction &MF,"
            << std::endl
            << "                                       CallingConv::ID) const override;" << std::endl;

  OutStream << "  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;" << std::endl;

  OutStream << "  BitVector getReservedRegs(const MachineFunction &MF) const override;" << std::endl;

  OutStream << "  bool isConstantPhysReg(unsigned PhysReg) const override;" << std::endl;

  OutStream << "  const uint32_t *getNoPreservedMask() const override;" << std::endl;

  OutStream << "  void eliminateFrameIndex(MachineBasicBlock::iterator MI, int SPAdj," << std::endl
            << "                           unsigned FIOperandNum," << std::endl
            << "                           RegScavenger *RS = nullptr) const override;" << std::endl;

  OutStream << "  unsigned getFrameRegister(const MachineFunction &MF) const override;" << std::endl;

  OutStream << "  bool requiresRegisterScavenging(const MachineFunction &MF) const override {"
            << std::endl
            << "    return true;"
            << std::endl
            << "  }" << std::endl;

  OutStream << "  bool requiresFrameIndexScavenging(const MachineFunction &MF) const override {"
            << std::endl
            << "    return true;"
            << std::endl
            << "  }" << std::endl;

  OutStream << "  bool trackLivenessAfterRegAlloc(const MachineFunction &) const override {"
            << std::endl
            << "    return true;"
            << std::endl
            << "  }" << std::endl;

  OutStream << "};" << std::endl;
  OutStream << "}" << std::endl << std::endl;

  OutStream << "#endif" << std::endl << std::endl;

  OutStream.close();

  // write out the implementation file
  OutFile = LLVMRoot + "/" + TargetName + "RegisterInfo.cpp";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the Subtarget cpp file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "RegisterInfo.cpp - " << TargetName << " Register Information"
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#include \"" << TargetName << "RegisterInfo.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << ".h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "Subtarget.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineFrameInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineFunction.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineInstrBuilder.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/RegisterScavenging.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/TargetFrameLowering.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/TargetInstrInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/ErrorHandling.h\"" << std::endl << std::endl;

  OutStream << "#define GET_REGINFO_TARGET_DESC" << std::endl;
  OutStream << "#include \"" << TargetName << "GenRegisterInfo.inc\"" << std::endl << std::endl;

  OutStream << "using namespace llvm;" << std::endl << std::endl;

  OutStream << TargetName << "RegisterInfo::" << TargetName << "RegisterInfo(unsigned HwMode)" << std::endl;
  OutStream << "   : " << TargetName << "GenRegisterInfo("
            << TargetName << "::" << FirstReg << "0, 0, 0, HwMode) {}" << std::endl << std::endl;

  OutStream << "const MCPhysReg *"
            << TargetName << "RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {"
            << std::endl
            << "  return ABI_Unknown;"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "BitVector " << TargetName
            << "RegisterInfo::getReservedRegs(const MachineFunction &MF) const {"
            << std::endl
            << "  const TargetFrameLowering *TFI = getFrameLowering(MF);"
            << std::endl
            << "  BitVector Reserved(getNumRegs());"
            << std::endl
            << "  return Reserved;"
            << std::endl
            << "}"
            << std::endl << std::endl;

  OutStream << "bool " << TargetName << "RegisterInfo::isConstantPhysReg(unsigned PhysReg) const {"
            << std::endl
            << "  return PhysReg == " << TargetName << "::" << FirstReg << ";"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "const uint32_t *" << TargetName << "RegisterInfo::getNoPreservedMask() const {"
            << std::endl
            << "  return 0;"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "void " << TargetName << "RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,"
            << "int SPAdj, unsigned FIOperandNum, RegScavenger *RS) const {"
            << std::endl
            << "  assert(SPAdj == 0 && \"Unexpected non-zero SPAdj value\");"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "unsigned " << TargetName << "RegisterInfo::getFrameRegister(const MachineFunction &MF) const {"
            << std::endl
            << "  return " << TargetName << ":" << FirstReg << ";" << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "const uint32_t *" << TargetName << "RegisterInfo::getCallPreservedMask(const MachineFunction & MF,"
            << "CallingConv::ID /*CC*/) const {"
            << std::endl
            << "  return ABI_Unknown;"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream.close();

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

  //       see reference here: https://llvm.org/doxygen/classllvm_1_1MVT.html
  OutStream << "  MVT XLenVT = MVT::" << AddrMode <<";" << std::endl;
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
  // {TargetName}TargetObjectFile.h/cpp

  // header file
  std::string OutFile = LLVMRoot + "/" + TargetName + "TargetObjectFile.h";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the target object file header file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "TargetObjectFile.h - Define Object Info " << TargetName
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#ifndef LLVM_LIB_TARGET_" << TargetName << "_"
            << TargetName << "TARGETOBJECTFILE_H" << std::endl;
  OutStream << "#define LLVM_LIB_TARGET_" << TargetName << "_"
            << TargetName << "TARGETOBJECTFILE_H" << std::endl;

  OutStream << "namespace llvm {" << std::endl;
  OutStream << "class " << TargetName << "TargetMachine;" << std::endl << std::endl;

  OutStream << "class " << TargetName << "TargetObjectFile : public TargetLoweringObjectFileELF {" << std::endl;

  OutStream << "  MCSection *SmallDataSection;" << std::endl;
  OutStream << "  MCSection *SmallBSSSection;" << std::endl;
  OutStream << "  unsigned SSThreshold = 8;" << std::endl;
  OutStream << "public:" << std::endl;
  OutStream << "  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;" << std::endl;
  OutStream << "  bool isGlobalInSmallSection(const GlobalObject *GO, const TargetMachine &TM) const override;" << std::endl;
  OutStream << "  MCSection *SelectSectionForGlobal(const GlobalObject *GO, SectionKind Kind,const TargetMachine &TM) const override;" << std::endl;
  OutStream << "  MCSection *getSectionForConstant(const DataLayout &DL, SectionKind Kind,"
            << "                                   const Constant *C, unsigned &Align) const override;" << std::endl;
  OutStream << "  void getModuleMetadata(Module &M) override;" << std::endl;

  OutStream << "};" << std::endl;
  OutStream << "}" << std::endl;
  OutStream << "#endif" << std::endl;

  OutStream.close();

  // implementation file
  OutFile = LLVMRoot + "/" + TargetName + "TargetObjectFile.cpp";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the target object file implementation file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "TargetObjectFile.cpp - Define Object Info " << TargetName
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#include \"" << TargetName << "TargetObjectFile.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "TargetMachine.h\"" << std::endl;
  OutStream << "#include \"llvm/BinaryFormat/ELF.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCContext.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCSectionELF.h\"" << std::endl << std::endl;

  OutStream << "using namespace llvm;" << std::endl << std::endl;

  OutStream << "void " << TargetName << "ELFTargetObjectFile::Initialize(MCContext &Ctx,"
            << std::endl
            << "                const TargetMachine &TM) {"
            << std::endl
            << "  TargetLoweringObjectFileELF::Initialize(Ctx, TM);"
            << std::endl
            << "  InitializeELF(TM.Options.UseInitArray);"
            << std::endl
            << "}"
            << std::endl << std::endl;

  OutStream << "bool " << TargetName << "ELFTargetObjectFile::isGlobalInSmallSection("
            << std::endl
            << "    const GlobalObject *GO, const TargetMachine &TM) const {"
            << std::endl
            << "\\\\ ENTER TARGET SPECIFIC INFO HERE"
            << std::endl
            << "  return true;"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "MCSection *" << TargetName << "ELFTargetObjectFile::SelectSectionForGlobal("
            << std::endl
            << "    const GlobalObject *GO, SectionKind Kind, const TargetMachine &TM) const {"
            << std::endl
            << "  if (Kind.isBSS() && isGlobalInSmallSection(GO, TM))"
            << std::endl
            << "    return SmallBSSSection;"
            << std::endl
            << "  if (Kind.isData() && isGlobalInSmallSection(GO, TM))"
            << "    return SmallDataSection;"
            << std::endl
            << "  return TargetLoweringObjectFileELF::SelectSectionForGlobal(GO, Kind, TM);"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "void " << TargetName << "ELFTargetObjectFile::getModuleMetadata(Module &M) {"
            << std::endl
            << "  SmallVector<Module::ModuleFlagEntry, 8> ModuleFlags;"
            << std::endl
            << "  M.getModuleFlagsMetadata(ModuleFlags);"
            << std::endl
            << "  for(const auto &MFE : ModuleFlags) {"
            << std::endl
            << "    StringRef Key = MFE.Key->getString();"
            << std::endl
            << "    if (Key == \"SmallDataLimit\") {"
            << std::endl
            << "      SSThreshold = mdconst::extract<ConstantInt>(MFE.Val)->getZExtValue();"
            << std::endl
            << "      break;"
            << std::endl
            << "    }"
            << std::endl
            << "  }"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "MCSection *" << TargetName << "ELFTargetObjectFile::getSectionForConstant("
            << std::endl
            << "  const DataLayout &DL, SectionKind Kind, const Constant *C, unsigned &Align) const {"
            << std::endl
            << "  return TargetLoweringObjectFileELF::getSectionForConstant(DL, Kind, C, Align);"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateAsmParser(){

  // Stage 1: generate the CMakeLists.txt
  std::string OutFile = LLVMRoot + "/AsmParser/CMakeLists.txt";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open AsmParser CMakeLists.txt file: " + OutFile );
    return false;
  }

  OutStream << "add_llvm_library(LLVM" << TargetName << "AsmParser" << std::endl
            << "  " << TargetName << "AsmParser.cpp" << std::endl
            << "   }" << std::endl;

  OutStream.close();

  // Stage 2: generate the LLVMBuild.txt file
  OutFile = LLVMRoot + "/AsmParser/LLVMBuild.txt";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open AsmParser LLVMBuild.txt file: " + OutFile );
    return false;
  }

  OutStream << ";===- ./lib/Target/" << TargetName << "/AsmParser/LLVMBuild.txt ---------------*- Conf -*--===;" << std::endl;

  OutStream << std::endl
            << "[component_0]" << std::endl
            << "type = Library" << std::endl
            << "name = " << TargetName << "AsmParser" << std::endl
            << "parent = " << TargetName << std::endl
            << "required_libraries = MC MCParser " << TargetName
            << "Desc " << TargetName << "Info Support" << std::endl
            << "add_to_library_groups = " << TargetName << std::endl;

  OutStream.close();

  // Stage 3: generate the actual AsmParser
  OutFile = LLVMRoot + "/AsmParser/" + TargetName + "AsmParser.cpp";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open AsmParser implementation file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "AsmParser.cpp - Define AsmParser for " << TargetName
            << " -----------===//" << std::endl << std::endl;

  OutStream << "#include \"MCTargetDesc/" << TargetName << "AsmBackend.h\"" << std::endl;
  OutStream << "#include \"MCTargetDesc/" << TargetName << "MCExpr.h\"" << std::endl;
  OutStream << "#include \"MCTargetDesc/" << TargetName << "MCTargetDesc.h\"" << std::endl;
  OutStream << "#include \"MCTargetDesc/" << TargetName << "MCTargetStreamer.h\"" << std::endl;
  OutStream << "#include \"TargetInfo/" << TargetName << "TargetInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/ADT/STLExtras.h\"" << std::endl;
  OutStream << "#include \"llvm/ADT/SmallVector.h\"" << std::endl;
  OutStream << "#include \"llvm/ADT/StringSwitch.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCAssembler.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCContext.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCExpr.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCInst.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCInstBuilder.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCObjectFileInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCParser/MCAsmLexer.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCParser/MCParsedAsmOperand.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCParser/MCTargetAsmParser.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCRegisterInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCStreamer.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCSubtargetInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/Casting.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/MathExtras.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/TargetRegistry.h\"" << std::endl;
  OutStream << "#include <limits>" << std::endl;
  OutStream << std::endl << std::endl;

  OutStream << "using namespace llvm;" << std::endl << std::endl;

  OutStream << "#define DEBUG_TYPE \"" << TargetName << "-asm-parser\"" << std::endl << std::endl;

  OutStream << "namespace {" << std::endl
            << "struct " << TargetName << "Operand;" << std::endl << std::endl;

  // -- output MCTargetAsmParser class def
  OutStream << "class " << TargetName << "AsmParser : public MCTargetAsmParser {" << std::endl;
  OutStream << "\tSMLoc getLoc() const { return getParser().getTok().getLoc(); }" << std::endl;
  OutStream << "\t" << TargetName << "TargetStreamer &getTargetStreamer() {"
            << std::endl
            << "\t\tMCTargetStreamer &TS = *getParser().getStreamer().getTargetStreamer();"
            << std::endl
            << "\t\treturn static_cast<" << TargetName << "TargetStreamer &>(TS);"
            << "\t}" << std::endl;

  OutStream << "\tunsigned validateTargetOperandClass(MCParsedAsmOperand &Op, unsigned Kind) override;" << std::endl;

  OutStream << "\tbool MatchAndEmitInstruction(SMLoc IDLoc, unsigned &Opcode,"
            << std::endl
            << "\t\tOperandVector &Operands, MCStreamer &Out, uint64_t &ErrorInfo,"
            << "\t\tbool MatchingInlineAsm) override;"
            << std::endl;

  OutStream << "\tbool ParseRegister(unsigned &RegNo, SMLoc &StartLoc, SMLoc &EndLoc) override;" << std::endl;

  OutStream << "\tbool ParseInstruction(ParseInstructionInfo &Info, StringRef Name,"
            << "\t\tSMLoc NameLoc, OperandVector &Operands) override;" << std::endl;

  OutStream << "\tbool ParseDirective(AsmToken DirectiveID) override;" << std::endl;

  OutStream << "#define GET_ASSEMBLER_HEADER" << std::endl;
  OutStream << "#include \"" << TargetName << "GenAsmMatcher.inc\"" << std::endl << std::endl;

  OutStream << "public:" << std::endl;
  OutStream << "\tenum " << TargetName << "MatchResultTy {" << std::endl;
  OutStream << "\t\tMatch_Dummy = FIRST_TARGET_MATCH_RESULT_TY," << std::endl;
  OutStream << "#define GET_OPERAND_DIAGNOSTIC_TYPES" << std::endl;
  OutStream << "#include \"" << TargetName << "GenAsmMatcher.inc\"" << std::endl;
  OutStream << "#undef GET_OPERAND_DIAGNOSTIC_TYPES" << std::endl;
  OutStream << "\t};" << std::endl << std::endl;

  OutStream << "\t" << TargetName << "AsmParser(const MCSubtargetInfo &STI, MCAsmParser &Parser,"
            << std::endl
            << "\t\t\tconst MCInstrInfo &MII, const MCTargetOptions &Options)"
            << std::endl
            << "\t\t: MCTargetAsmParser(Options, STI, MII) {"
            << std::endl
            << "\t\tParser.addAliasForDirective(\".half\", \".2byte\");"
            << "\t\tParser.addAliasForDirective(\".hword\", \".2byte\");"
            << "\t\tParser.addAliasForDirective(\".word\", \".4byte\");"
            << "\t\tParser.addAliasForDirective(\".dword\", \".8byte\");"
            << "\t\tsetAvailableFeatures(ComputeAvailableFeatures(STI.getFeatureBits()));"
            << std::endl
            << "\t}" << std::endl;

  OutStream << "};" << std::endl << std::endl;

  // -- output MCParsedAsmOperand class def
  OutStream << "struct " << TargetName << "Operand : public MCParsedAsmOperand {" << std::endl;

  OutStream << "\tenum KindTy {" << std::endl
            << "\t\tToken," << std::endl
            << "\t\tRegister," << std::endl
            << "\t\tImmediate" << std::endl
            << "\t} Kind;" << std::endl;

  OutStream << "\tstruct RegOp {" << std::endl
            << "\t\tunsigned RegNum;" << std::endl
            << "\t};" << std::endl;

  OutStream << "\tstruct ImmOp" << std::endl
            << "\t\tconst MCExpr *Val;" << std::endl
            << "\t};" << std::endl;

  OutStream << "\tSMLoc StartLoc, EndLoc;" << std::endl;

  OutStream << "\tunion {" << std::endl
            << "\t\tStringRef Tok;" << std::endl
            << "\t\tRegOp Reg;" << std::endl
            << "\t\tImmOp Imm;" << std::endl
            << "\t};" << std::endl;

  OutStream << "\t" << TargetName << "Operand(KindTy K) : MCParsedAsmOperand(), Kind(K) {}" << std::endl;

  OutStream << std::endl << std::endl;

  OutStream << "\tpublic:" << std::endl;
  OutStream << "\t\t" << TargetName << "Operand(const "
            << TargetName << "Operand &o) : MCParsedAsmOperand() {"
            << std::endl;
  OutStream << "\t\t\tKind = o.Kind;" << std::endl;
  OutStream << "\t\t\tStartLoc = o.StartLoc;" << std::endl;
  OutStream << "\t\\ttEndLoc = o.EndLoc;" << std::endl;
  OutStream << "\t\t\tswitch(Kind){" << std::endl;
  OutStream << "\t\t\tcase Register:" << std::endl
            << "\t\t\t\tReg = o.Reg;" << std::endl
            << "\t\t\t\tbreak;" << std::endl;
  OutStream << "\t\t\tcase Immediate:" << std::endl
            << "\t\t\t\tImm = o.Imm;" << std::endl
            << "\t\t\t\tbreak;" << std::endl;
  OutStream << "\t\t\tcase Token:" << std::endl
            << "\t\t\t\tTok = o.Tok;" << std::endl
            << "\t\t\t\tbreak;" << std::endl;
  OutStream << "\t\t\t}" << std::endl;
  OutStream << "\t\t}" << std::endl;

  OutStream << "\tbool isToken() const override { return Kind == Token; }" << std::endl;
  OutStream << "\tbool isReg() const override { return Kind == Register; }" << std::endl;
  OutStream << "\tbool isImm() const override { return Kind == Immediate; }" << std::endl;
  OutStream << "\tbool isMem() const override { return false; }" << std::endl;

  OutStream << "\tSMLoc getStartLoc() const override { return StartLoc; }" << std::endl;
  OutStream << "\tSMLoc getEndLoc() const override { return EndLoc; }" << std::endl;

  OutStream << "\tunsigned getReg() const override {"
            << std::endl
            << "\t\tassert(Kind == Register && \"Invalid type access!\");"
            << std::endl
            << "\treturn Reg.RegNum;"
            << std::endl
            << "\t}" << std::endl << std::endl;

  OutStream << "\tvoid print(raw_ostream &OS) const override {"
            << std::endl
            << "\t\tswitch(Kind) {"
            << std::endl
            << "\t\tcase Immedaite:" << std::endl
            << "\t\t\tOS << *getImm();" << std::endl
            << "\t\t\tbreak;" << std::endl
            << "\t\tcase Register:" << std::endl
            << "\t\t\tOS << \"<register x\";" << std::endl
            << "\t\t\tbreak;" << std::endl
            << "\t\tcase Token:" << std::endl
            << "\t\t\tOS << \"'\" << getToken() << \"'\";" << std::endl
            << "\t\t\tbreak;" << std::endl
            << "\t\tcase SystemRegister:" << std::endl
            << "\t\t\tOS << \"<sysreg: \" << getSysReg() << '>';" << std::endl
            << "\t\t\tbreak;" << std::endl
            << "\t\t}" << std::endl
            << "\t}" << std::endl << std::endl;

  OutStream << "};" << std::endl;
  OutStream << "}" << std::endl;  // end anonymous namespace

  // -- output all the implementation details
  OutStream << "#define GET_REGISTER_MATCHER" << std::endl;
  OutStream << "#define GET_MATCHER_IMPLEMENTATION" << std::endl;
  OutStream << "#include \"" << TargetName << "GenAsmMatcher.inc\""
            << std::endl << std::endl;

  OutStream << "extern \"C\" void LLVMInitialize" << TargetName << "AsmParser() {"
            << "\tRegisterMCAsmParser<" << TargetName << "AsmParser> X(getThe"
            << TargetName << "Target());" << std::endl
            << "}" << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateDisass(){
  // Stage 1: generate the cmake script
  std::string OutFile = LLVMRoot + "/Disassembler/CMakeLists.txt";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open disassembler CMakeLists: " + OutFile );
    return false;
  }

  OutStream << "add_llvm_library(LLVM" << TargetName << "Disassembler"
            << std::endl
            << "\t" << TargetName << "Disassembler.cpp"
            << std::endl
            << "\t)" << std::endl;

  OutStream.close();

  // Stage 2: generate the LLVMBuild file
  OutFile = LLVMRoot + "/Disassembler/LLVMBuild.txt";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open Disassembler LLVMBuild.txt file: " + OutFile );
    return false;
  }

  OutStream << ";===- ./lib/Target/" << TargetName << "/Disassembler/LLVMBuild.txt ---------------*- Conf -*--===;" << std::endl;

  OutStream << std::endl
            << "[component_0]" << std::endl
            << "type = Library" << std::endl
            << "name = " << TargetName << "Disassembler" << std::endl
            << "parent = " << TargetName << std::endl
            << "required_libraries = MCDisassembler "
            << TargetName << "Info Support" << std::endl
            << "add_to_library_groups = " << TargetName << std::endl;

  OutStream.close();

  // Stage 3: generate the disassembler implementation
  OutFile = LLVMRoot + "/Disassembler/" + TargetName + "Disassembler.cpp";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open Disassembler implementation file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "Disassembler.cpp - Define AsmParser for " << TargetName
            << " -----------===//" << std::endl << std::endl;

  OutStream << "#include \"MCTargetDesc/" << TargetName << "MCTargetDesc.h\"" << std::endl;
  OutStream << "#include \"TargetInfo/" << TargetName << "TargetInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCContext.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCDisassembler/MCDisassembler.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCFixedLenDisassembler.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCInst.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCRegisterInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCSubtargetInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/Endian.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/TargetRegistry.h\"" << std::endl << std::endl;

  OutStream << "using namespace llvm;" << std::endl << std::endl;

  OutStream << "#define DEBUG_TYPE \"" << TargetName << "-disassembler\"" << std::endl << std::endl;

  OutStream << "typedef MCDisassembler::DecodeStatus DecodeStatus;" << std::endl << std::endl;

  OutStream << "namespace {" << std::endl;
  OutStream << "class " << TargetName << "Disassembler : public MCDisassembler {" << std::endl;
  OutStream << "public: " << std::endl;
  OutStream << "\t" << TargetName << "Disassembler(const MCSubtargetInfo &STI, MCContext &Ctx)"
            << std::endl
            << "\t\t\t: MCDisassembler(STI, Ctx) {}" << std::endl;
  OutStream << "\tDecodeStatus getInstruction(MCInst &Instr, uint64_t &Size,"
            << std::endl
            << "\t\t\tArrayRef<uint8_t> Bytes, uint64_t Address,"
            << std::endl
            << "\t\t\traw_ostream &VStream,raw_ostream &CStream) const override;" << std::endl;
  OutStream << "};" << std::endl;
  OutStream << "}" << std::endl; // end anonymous namespace

  OutStream << "static MCDisassembler *create" << TargetName << "Disassembler(const Target &T,"
            << std::endl
            << "\t\t\tconst MCSubtargetInfo &STI,MCContext &Ctx) {"
            << std::endl
            << "\treturn new " << TargetName << "Disassembler(STI, Ctx);"
            << std::endl
            << "}" << std::endl;

  OutStream << "extern \"C\" void LLVMInitialize" << TargetName << "Disassembler() {"
            << std::endl
            << "\tTargetRegistry::RegisterMCDisassembler(getThe" << TargetName << "Target(),"
            << std::endl
            << "\t\t\tcreate" << TargetName << "Disassembler);"
            << std::endl
            << "}" << std::endl;

  OutStream << "DecodeStatus " << TargetName << "Disassembler::getInstruction(MCInst &MI, uint64_t &Size,"
            << std::endl
            << "\t\t\tArrayRef<uint8_t> Bytes, uint64_t Address,"
            << std::endl
            << "\t\t\traw_ostream &OS, raw_ostream &CS) const {"
            << std::endl;
  // TODO: set the size of the instruction
  OutStream << "\treturn decodeInstruction(DecoderTable, MI, Insn, Address, this, STI);" << std::endl;
  OutStream << "}" << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateInstPrinter(){
  std::string OutFile = LLVMRoot + "/" + TargetName + "AsmPrinter.cpp";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open AsmPrinter file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "AsmPrinter.cpp - Define AsmPrinter for " << TargetName
            << " -----------===//" << std::endl << std::endl;

  OutStream << "#include \"" << TargetName << ".h\"" << std::endl;
  OutStream << "#include \"MCTargetDesc/" << TargetName << "InstPrinter.h\"" << std::endl;
  OutStream << "#include \"MCTargetDesc/" << TargetName << "MCExpr.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "TargetMachine.h\"" << std::endl;
  OutStream << "#include \"TargetInfo/" << TargetName << "TargetInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/AsmPrinter.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineConstantPool.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineFunctionPass.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineInstr.h\"" << std::endl;
  OutStream << "#include \"llvm/CodeGen/MachineModuleInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCAsmInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCInst.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCStreamer.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCSymbol.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/TargetRegistry.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/raw_ostream.h\"" << std::endl;

  OutStream << "using namespace llvm;" << std::endl << std::endl;

  OutStream << "#define DEBUG_TYPE \"asm-printer\"" << std::endl << std::endl;

  OutStream << "namespace{" << std::endl;
  OutStream << "class " << TargetName << "AsmPrinter : public AsmPrinter {" << std::endl;
  OutStream << "public:" << std::endl;
  OutStream << "\t explicit " << TargetName << "AsmPrinter(TargetMachine &TM,"
            << std::endl
            << "\t\t\tstd::unique_ptr<MCStreamer> Streamer)"
            << std::endl
            << "\t\t: AsmPrinter(TM, std::move(Streamer)) {}" << std::endl;
  OutStream << "\tStringRef getPassName() const override { return \""
            << TargetName << " Assembly Printer\"; }" << std::endl;
  OutStream << "\tvoid EmitInstruction(const MachineInstr *MI) override;" << std::endl;
  OutStream << "\tbool PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,"
            << std::endl
            << "\t\tconst char *ExtraCode, raw_ostream &OS) override;" << std::endl;
  OutStream << "\tbool PrintAsmMemoryOperand(const MachineInstr *MI, unsigned OpNo,"
            << std::endl
            << "\t\tconst char *ExtraCode, raw_ostream &OS) override;" << std::endl;
  //OutStream << "\tvoid EmitToStreamer(MCStreamer &S, const MCInst &Inst);" << std::endl;
  OutStream << "\tbool emitPseudoExpansionLowering(MCStreamer &OutStreamer,const MachineInstr *MI);" << std::endl;
  OutStream << "\tbool lowerOperand(const MachineOperand &MO, MCOperand &MCOp) const {"
            << std::endl
            << "\t\treturn Lower" << TargetName << "MachineOperandToMCOperand(MO, MCOp, *this);"
            << std::endl
            << "\t}" << std::endl;
  OutStream << "};" << std::endl;
  OutStream << "}" << std::endl;

  if( PInsts.size() > 0 ){
    OutStream << "#include \"" << TargetName << "GenMCPseudoLowering.inc\"" << std::endl;
  }

  OutStream << "void " << TargetName << "AsmPrinter::EmitInstruction(const MachineInstr *MI) {" << std::endl;
  if( PInsts.size() > 0 ){
    OutStream << "\tif (emitPseudoExpansionLowering(*OutStreamer, MI))"
              << std::endl
              << "\t\treturn ;" << std::endl;
  }
  OutStream << "\tMCInt TmpInst;"
            << std::endl
            << "\tLower" << TargetName << "MachineInstrToMCInst(MI, TmpInst, *this);"
            << std::endl
            << "\tEmitToStreamer(*OutStreamer, TmpInst);" << std::endl;
  OutStream << "}" << std::endl << std::endl;

  OutStream << "bool " << TargetName << "AsmPrinter::PrintAsmOperand(const MachineInstr *MI, unsigned OpNo,"
            << "\t\t\tconst char *ExtraCode, raw_ostream &OS) {"
            << std::endl;
  OutStream << "\tif (!AsmPrinter::PrintAsmOperand(MI, OpNo, ExtraCode, OS))" << std::endl;
  OutStream << "\t\treturn false;" << std::endl;
  OutStream << "\tif(!ExtraCode) {" << std::endl;
  OutStream << "\t\tconst MachineOperand &MO = MI->getOperand(OpNo);";
  OutStream << "\t\tswitch(MO.getType()) {"
            << std::endl
            << "\t\tcase MachineOperand::MO_Immediate:"
            << std::endl
            << "\t\t\tOS << MO.getImm();" << std::endl << "\t\t\treturn false;"
            << std::endl
            << "\t\tcase MachineOperand::MO_Register:"
            << std::endl
            << "\t\t\tOS << " << TargetName << "InstPrinter::getRegisterName(MO.getReg());"
            << std::endl
            << "\t\t\treturn false;"
            << "\t\tdefault:"
            << std::endl
            << "\t\t\tbreak;"
            << std::endl
            << "\t\t}" << std::endl;
  OutStream << "\t}" << std::endl;
  OutStream << "\treturn true;" << std::endl;
  OutStream << "}" << std::endl << std::endl;

  OutStream << "bool " << TargetName << "AsmPrinter::PrintAsmMemoryOperand(const MachineInstr *MI,"
            << std::endl
            << "\t\t\tunsigned OpNo, const char *ExtraCode, raw_ostream &OS) {"
            << std::endl;
  OutStream << "\tif(!ExtraCode) {"
            << std::endl
            << "\t\tconst MachineOperand &MO = MI->getOperand(OpNo);"
            << std::endl
            << "\t\tif(!MO.isReg())"
            << std::endl
            << "\t\tOS << \"0(\" << "
            << TargetName << "InstPrinter::getRegisterName(MO.getReg())"
            << " << \")\";"
            << std::endl
            << "\t\treturn false;"
            << "\t}" << std::endl;
  OutStream << "\treturn AsmPrinter::PrintAsmMemoryOperand(MI, OpNo, ExtraCode, OS);" << std::endl;
  OutStream << "}" << std::endl << std::endl;

  OutStream << "extern \"C\" void LLVMInitialize" << TargetName << "AsmPrinter() {"
            << std::endl
            << "\tRegisterAsmPrinter<" << TargetName << "AsmPrinter> X(getThe"
            << TargetName << "Target());" << std::endl
            << "}" << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateMCBuild(){
  // Stage 1: generate the cmake script
  std::string OutFile = LLVMRoot + "/MCTargetDesc/CMakeLists.txt";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open MCTargetDesc CMakeLists: " + OutFile );
    return false;
  }

  OutStream << "add_llvm_library(LLVM" << TargetName << "Desc"
            << std::endl
            << "\t" << TargetName << "AsmBackend.cpp"
            << std::endl
            << "\t" << TargetName << "ELFObjectWriter.cpp"
            << std::endl
            << "\t" << TargetName << "InstPrinter.cpp"
            << std::endl
            << "\t" << TargetName << "MCAsmInfo.cpp"
            << std::endl
            << "\t" << TargetName << "MCCodeEmitter.cpp"
            << std::endl
            << "\t" << TargetName << "MCExpr.cpp"
            << std::endl
            << "\t" << TargetName << "MCTargetDesc.cpp"
            << std::endl
            << "\t" << TargetName << "TargetStreamer.cpp"
            << std::endl
            << "\t" << TargetName << "ELFStreamer.cpp"
            << std::endl
            << "\t)" << std::endl;

  OutStream.close();

  // Stage 2: generate the LLVMBuild file
  OutFile = LLVMRoot + "/MCTargetDesc/LLVMBuild.txt";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open MCTargetDesc LLVMBuild.txt file: " + OutFile );
    return false;
  }

  OutStream << ";===- ./lib/Target/" << TargetName << "/MCTargetDesc/LLVMBuild.txt ---------------*- Conf -*--===;" << std::endl;

  OutStream << std::endl
            << "[component_0]" << std::endl
            << "type = Library" << std::endl
            << "name = " << TargetName << "Desc" << std::endl
            << "parent = " << TargetName << std::endl
            << "required_libraries = MC "
            << TargetName << "Info Support" << std::endl
            << "add_to_library_groups = " << TargetName << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateMCAsmBackend(){
  return true;
}

bool LLVM801CG::TIGenerateMCElfObjWriter(){
  return true;
}

bool LLVM801CG::TIGenerateMCElfStreamer(){
  // Stage 1: ELFStreamer.h
  std::string OutFile = LLVMRoot + "/MCTargetDesc/" + TargetName + "ELFStreamer.h";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the ELFStreamer header file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "ELFStreamer.h - Define Target Descriptions" << TargetName
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#ifndef LLVM_LIB_TARGET_" << TargetName
            << "_" << TargetName << "ELFSTREAMER_H" << std::endl;
  OutStream << "#define LLVM_LIB_TARGET_" << TargetName
            << "_" << TargetName << "ELFSTREAMER_H" << std::endl;

  OutStream << "#include \"" << TargetName << "TargetStreamer.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCELFStreamer.h\"" << std::endl << std::endl;

  OutStream << "namespace llvm {" << std::endl;

  OutStream << "class " << TargetName << "TargetELFStreamer : public "
            << TargetName << "TargetStreamer {" << std::endl;
  OutStream << "public:" << std::endl;
  OutStream << "\tMCELFStreamer &getStreamer();" << std::endl;
  OutStream << "\t" << TargetName << "TargetELFStreamer(MCStreamer &S, const MCSubtargetInfo &STI);" << std::endl;

  OutStream << "};" << std::endl << "}" << std::endl;

  OutStream << "#endif" << std::endl;

  OutStream.close();

  // Stage 2: ELFStreamer.cpp
  OutFile = LLVMRoot + "/MCTargetDesc/" + TargetName + "ELFStreamer.cpp";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the ELFStreamer implementation file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "ELFStreamer.cpp - Define ELFStreamer for " << TargetName
            << " -----------===//" << std::endl << std::endl;

  OutStream << "#include \"" << TargetName << "ELFStreamer.h\"" << std::endl;
  OutStream << "#include \"MCTargetDesc/" << TargetName << "AsmBackend.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "MCTargetDesc.h\"" << std::endl;
  OutStream << "#include \"llvm/BinaryFormat/ELF.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCSubtargetInfo.h\"" << std::endl;

  OutStream << "using namespace llvm;" << std::endl << std::endl;

  OutStream << TargetName << "TargetELFStreamer::" << TargetName
            << "TargetELFStreamer(MCStreamer &S,const MCSubtargetInfo &STI)"
            << std::endl
            << "\t:" << TargetName << "TargetStreamer(S) {"
            << std::endl;

  OutStream << "\tMCAssembler &MCA = getStreamer().getAssembler();" << std::endl;
  OutStream << "\tconst FeatureBitset &Features = STI.getFeatureBits();" << std::endl;
  OutStream << "\tauto &MAB = static_cast<"
            << TargetName << "AsmBackend &>(MCA.getBackend());" << std::endl;
  OutStream << "\t" << TargetName << "ABI::ABI ABI = MAB.getTargetABI();" << std::endl;

  OutStream << "\tunsigned EFlags = MCA.getELFHeaderEFlags();" << std::endl;
  OutStream << "\t//TODO: IMPLEMENT THE ABI" << std::endl;
  OutStream << "\tMCA.setELFHeaderEFlags(EFlags);" << std::endl;

  OutStream << "}" << std::endl;

  OutStream << "MCELFStreamer &" << TargetName
            << "TargetELFStreamer::getStreamer() {"
            << std::endl
            << "\treturn static_cast<MCELFStreamer &>(Streamer);"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateMCFixupKinds(){
  // Stage 1: FixupKinds.h
  std::string OutFile = LLVMRoot + "/MCTargetDesc/" + TargetName + "FixupKinds.h";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the FixupKinds header file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "FixupKinds.h - Define Fixup Kinds" << TargetName
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#ifndef LLVM_LIB_TARGET_" << TargetName
            << "_MCTARGETDESC_" << TargetName
            << "FIXUPKINDS_H" << std::endl;
  OutStream << "#define LLVM_LIB_TARGET_" << TargetName
            << "_MCTARGETDESC_" << TargetName
            << "FIXUPKINDS_H" << std::endl;

  OutStream << "#include \"llvm/MC/MCFixup.h\"" << std::endl;

  OutStream << "#undef " << TargetName << std::endl << std::endl;

  OutStream << "namespace llvm{" << std::endl
            << "namespace " << TargetName << std::endl
            << "enum Fixups {" << std::endl;

  OutStream << "\tfixup_" << TargetName << "_branch = FirstTargetFixupKind,"
            << std::endl;
  OutStream << "\tfixup_" << TargetName << "_call," << std::endl;
  OutStream << "\tfixup_" << TargetName << "_invalid," << std::endl;
  OutStream << "\tNumTargetFixupKinds = fixup_" << TargetName
            << "_invalid - FirstTargetFixupKind" << std::endl;

  OutStream << "};" << std::endl
            << "}" << std::endl
            << "}" << std::endl;

  OutStream << "#endif" << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateMCInstPrinter(){
  return true;
}

bool LLVM801CG::TIGenerateMCAsmInfo(){
  // Stage 1: MCAsmInfo.h
  std::string OutFile = LLVMRoot + "/MCTargetDesc/" + TargetName + "MCAsmInfo.h";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the MCAsmInfo header file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "MCAsmInfo.h - Define Target Descriptions" << TargetName
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#ifndef LLVM_LIB_TARGET_" << TargetName
            << "_MCTARGETDESC_" << TargetName << "MCASMINFO_H" << std::endl;
  OutStream << "#define LLVM_LIB_TARGET_" << TargetName
            << "_MCTARGETDESC_" << TargetName << "MCASMINFO_H" << std::endl;

  OutStream << "#include \"llvm/MC/MCAsmInfoELF.h\"" << std::endl;

  OutStream << "namespace llvm{" << std::endl;
  OutStream << "class Triple;" << std::endl;
  OutStream << "class " << TargetName << "MCAsmInfo : public MCAsmInfoELF {" << std::endl;
  OutStream << "\tvoid anchor() override;" << std::endl;
  OutStream << "public:" << std::endl;
  OutStream << "\texplicit " << TargetName << "MCAsmInfo(const Triple &TargetTriple);" << std::endl;
  OutStream << "};" << std::endl;
  OutStream << "}" << std::endl; //end namespace

  OutStream << "#endif" << std::endl;

  OutStream.close();

  // Stage 2: MCAsmInfo.cpp
  OutFile = LLVMRoot + "/MCTargetDesc/" + TargetName + "MCAsmInfo.cpp";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the MCAsmInfo implementation file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "MCAsmInfo.cpp - Define MCAsmInfo for " << TargetName
            << " -----------===//" << std::endl << std::endl;

  OutStream << "#include \"" << TargetName << "MCAsmInfo.h" << std::endl;
  OutStream << "#include \"llvm/ADT/Triple.h\"" << std::endl;
  OutStream << "using namespace llvm;" << std::endl << std::endl;
  OutStream << "void " << TargetName << "MCAsmInfo::anchor() {}" << std::endl;
  OutStream << TargetName << "MCAsmInfo::" << TargetName << "MCAsmInfo(const Triple &TT) {" << std::endl;
  OutStream << "\tCodePointerSize = 8;"
            << "\tCommentString = \"#\";"
            << "\tAlignmentIsInBytes = false;"
            << "\tSupportsDebugInformation = true;"
            << "\tData16bitsDirective = \"\\t.half\\t\";"
            << "\tData32bitsDirective = \"\\t.word\\t\";"
            << std::endl;
  OutStream << "}" << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateMCCodeEmitter(){
  return true;
}

bool LLVM801CG::TIGenerateMCExpr(){
  return true;
}

bool LLVM801CG::TIGenerateMCTargetDescCore(){
  // Stage 1: MCTargetDesc.h
  std::string OutFile = LLVMRoot + "/MCTargetDesc/" + TargetName + "MCTargetDesc.h";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the MCTargetDesc header file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "MCTargetDesc.h - Define Target Descriptions" << TargetName
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#ifndef LLVM_LIB_TARGET_" << TargetName << "MCTARGETDESC_"
            << TargetName << "TARGETDESC_H" << std::endl;
  OutStream << "#define LLVM_LIB_TARGET_" << TargetName << "MCTARGETDESC_"
            << TargetName << "TARGETDESC_H" << std::endl << std::endl;

  OutStream << "#include \"llvm/Config/config.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCTargetOptions.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/DataTypes.h\"" << std::endl;
  OutStream << "#include <memory>" << std::endl << std::endl;

  OutStream << "namespace llvm {" << std::endl;

  OutStream << "class MCAsmBackend;"  << std::endl
            << "class MCCodeEmitter;" << std::endl
            << "class MCContext;" << std::endl
            << "class MCInstrInfo;" << std::endl
            << "class MCObjectTargetWriter;" << std::endl
            << "class MCRegisterInfo;" << std::endl
            << "class MCSubtargetInfo;" << std::endl
            << "class StringRef;" << std::endl
            << "class Target;" << std::endl
            << "class Triple;" << std::endl
            << "class raw_ostream;" << std::endl
            << "class raw_pwrite_stream;" << std::endl << std::endl;

  OutStream << "MCCodeEmitter *create" << TargetName << "MCCodeEmitter(const MCInstrInfo &MCII,"
            << "\t\t\tconst MCRegisterInfo &MRI, MCContext &Ctx);" << std::endl;
  OutStream << "MCAsmBackend *create" << TargetName << "AsmBackend(const Target &T, const MCSubtargetInfo &STI,"
            << "\t\t\tconst MCRegisterInfo &MRI, const MCTargetOptions &Options);" << std::endl;
  OutStream << "std::unique_ptr<MCObjectTargetWriter> create" << TargetName
            << "ELFObjectWriter(uint8_t OSABI,"
            << std::endl
            << "\t\t\tbool Is64Bit);" << std::endl;
  OutStream << "}" << std::endl << std::endl; // end namespace

  OutStream << "#define GET_REGINFO_ENUM" << std::endl;
  OutStream << "#include \"" << TargetName << "GenRegisterInfo.inc\"" << std::endl;
  OutStream << std::endl;
  OutStream << "#define GET_SUBTARGETINFO_ENUM" << std::endl;
  OutStream << "#include \"" << TargetName << "GenSubtargetInfo.inc\"" << std::endl;

  OutStream << "#endif" << std::endl;

  OutStream.close();

  // Stage 2: MCTargetDesc.cpp
  OutFile = LLVMRoot + "/MCTargetDesc/" + TargetName + "MCTargetDesc.cpp";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the MCTargetDesc implementation file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "MCTargetDesc.cpp - Define TargetDesc for " << TargetName
            << " -----------===//" << std::endl << std::endl;

  OutStream << "#include \"" << TargetName << "MCTargetDesc.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "ELFStreamer.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "InstPrinter.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "MCAsmInfo.h\"" << std::endl;
  OutStream << "#include \"" << TargetName << "TargetStreamher.h\"" << std::endl;
  OutStream << "#include \"TargetInfo/" << TargetName << "TargetInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/ADT/STLExtras.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCAsmInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCInstrInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCRegisterInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCStreamer.h\"" << std::endl;
  OutStream << "#include \"llvm/MC/MCSubtargetInfo.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/ErrorHandling.h\"" << std::endl;
  OutStream << "#include \"llvm/Support/TargetRegistry.h\"" << std::endl << std::endl;

  OutStream << "#define GEN_INSTRINFO_MC_DESC" << std::endl;
  OutStream << "#include \"" << TargetName << "GenInstrInfo.inc\"" << std::endl << std::endl;

  OutStream << "#define GEN_REGINFO_MC_DESC" << std::endl;
  OutStream << "#include \"" << TargetName << "GenRegisterInfo.inc\"" << std::endl << std::endl;

  OutStream << "#define GEN_SUBTARGETINFO_MC_DESC" << std::endl;
  OutStream << "#include \"" << TargetName << "GenSubtargetInfo.inc\"" << std::endl << std::endl;

  OutStream << "using namespace llvm;" << std::endl << std::endl;

  OutStream << "static MCInstrInfo *create" << TargetName << "MCInstrInfo() {"
            << std::endl
            << "\tMCInstrInfo *X = new MCInstrInfo();"
            << std::endl
            << "\tInit" << TargetName << "MCInstrInfo(X);"
            << std::endl
            << "\treturn X;"
            << std::endl
            << "}" << std::endl << std::endl;

  std::string TempReg = RegClasses[0]->GetReg(0)->GetName();
  OutStream << "static MCRegisterInfo *create" << TargetName << "MCRegisterInof(const Triple &TT) {"
            << std::endl
            << "\tMCRegisterInfo *X = new MCRegisterInfo();"
            << std::endl
            << "\tInit" << TargetName << "MCRegisterInof(X, "
            << TargetName << "::" << TempReg << ");"
            << std::endl
            << "\treturn X;"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "static MCSubtargetInfo *create" << TargetName << "MCSubtargetInfo(const Triple &TT,"
            << std::endl
            << "\t\t\tStringRef CPU, StringRef FS) {"
            << std::endl
            << "\tstd::string CPUName = CPU;"
            << std::endl
            << "\tif(CPUName.empty()) CPUName = \"generic-" << TargetName << "\";"
            << std::endl
            << "\treturn create" << TargetName << "MCSubtargetInfoImpl(TT, CPUName, FS);"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "static MCInstPrinter *create" << TargetName << "MCInstPrinter(const Triple &T,"
            << std::endl
            << "\t\t\tunsigned SyntaxVariant, const MCAsmInfo &MAI, const MCInstrInfo &MII,"
            << std::endl
            << "\t\t\tconst MCRegisterInfo &MRI) {"
            << std::endl
            << "\treturn new " << TargetName << "InstPrinter(MAI, MII, MRI);"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "static MCTargetStreamer *create" << TargetName << "ObjectTargetStreamer(MCStreamer &S, "
            << "const MCSubtargetInfo &STI) {"
            << std::endl
            << "\tconst Triple &TT = STI.getTargetTriple();"
            << "\tif(TT.isOSBinFormatELF()) return new " << TargetName << "TargetELFStreamer(S,STI);"
            << std::endl
            << "\treturn nullptr;"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "static MCTargetStreamer *create" << TargetName << "AsmTargetStreamer(MCStream &S"
            << std::endl
            << "\t\t\tformatted_raw_ostream &OS, MCInstPrinter *InstPrint,"
            << std::endl
            << "\t\t\tbool isVerboseAsm) {"
            << std::endl
            << "\treturn new " << TargetName << "TargetAsmStreamer(S, OS);"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream << "extern \"C\" void LLVMInitialize" << TargetName << "TargetMC() {"
            << std::endl
            << "\tfor (Target *T : {&getThe" << TargetName << "Target()}) {"
            << "\t\tTargetRegistry::RegisterMCAsmInfo(*T, create" << TargetName << "MCAsmInfo);"
            << std::endl
            << "\t\tTargetRegistry::RegisterMCInstrInfo(*T, create" << TargetName << "MCInstrInfo);"
            << std::endl
            << "\t\tTargetRegistry::RegisterMCRegInfo(*T, create" << TargetName << "MCRegisterInfo);"
            << std::endl
            << "\t\tTargetRegistry::RegisterMCAsmBackend(*T, create" << TargetName << "AsmBackend);"
            << std::endl
            << "\t\tTargetRegistry::RegisterMCCodeEmitter(*T, create" << TargetName << "MCCodeEmitter);"
            << std::endl
            << "\t\tTargetRegistry::RegisterMCInstPrinter(*T, create" << TargetName << "MCInstPrinter);"
            << std::endl
            << "\t\tTargetRegistry::RegisterMCSubtargetInfo(*T, create" << TargetName << "MCSubtargetInfo);"
            << std::endl
            << "\t\tTargetRegistry::RegisterObjectTargetStreamer(*T, create" << TargetName << "ObjectTargetStreamer);"
            << std::endl
            << "\t\tTargetRegistry::RegisterAsmTargetStreamer(*T, create" << TargetName << "AsmTargetStreamer);"
            << std::endl
            << "\t}"
            << std::endl
            << "}" << std::endl << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateMCTargetStreamer(){
  // Stage 1: TargetStreamer.h
  std::string OutFile = LLVMRoot + "/MCTargetDesc/" + TargetName + "TargetStreamer.h";
  std::ofstream OutStream;
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the TargetStreamer header file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "TargetStreamer.h - Define Target Descriptions" << TargetName
            << " -------*- C++ -*-===//" << std::endl << std::endl;

  OutStream << "#ifndef LLVM_LIB_TARGET_" << TargetName << "_" << TargetName
            << "TARGETSTREAMER_H" << std::endl;
  OutStream << "#define LLVM_LIB_TARGET_" << TargetName << "_" << TargetName
            << "TARGETSTREAMER_H" << std::endl;

  OutStream << "#include \"llvm/MC/MCStreamer.h\"" << std::endl << std::endl;

  OutStream << "namespace llvm {" << std::endl << std::endl;

  OutStream << "class " << TargetName << "TargetStreamer : public MCTargetStreamer {" << std::endl;
  OutStream << "public: " << std::endl;
  OutStream << "\t" << TargetName << "TargetStreamer(MCStreamer &S);" << std::endl;
  OutStream << "};" << std::endl << std::endl;

  OutStream << "class " << TargetName << "TargetAsmStreamer : public "
            << TargetName << "TargetStreamer {" << std::endl;
  OutStream << "\tformatted_raw_ostream &OS;" << std::endl;
  OutStream << "public:" << std::endl;
  OutStream << "\t" << TargetName << "TargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS);"
            << std::endl
            << "};" << std::endl;

  OutStream << "}" << std::endl << std::endl;

  OutStream << "#endif" << std::endl;

  OutStream.close();

  // Stage 2: TargetStreamer.cpp
  OutFile = LLVMRoot + "/MCTargetDesc/" + TargetName + "TargetStreamer.cpp";
  OutStream.open(OutFile,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open the TargetStreamer implementation file: " + OutFile );
    return false;
  }

  OutStream << "//===-- " << TargetName
            << "TargetStreamer.cpp - Define TargetDesc for " << TargetName
            << " -----------===//" << std::endl << std::endl;

  OutStream << "#include \"" << TargetName << "TargetStreamer.h" << std::endl;
  OutStream << "#include \"llvm/Support/FormattedStream.h\"" << std::endl << std::endl;

  OutStream << "using namespace llvm;" << std::endl << std::endl;

  OutStream << TargetName << "TargetStreamer::" << TargetName
            << "TargetStreamer((MCStreamer &S) : MCTargetStreamer(S) {}" << std::endl << std::endl;

  OutStream << TargetName << "TargetAsmStreamer::" << TargetName
            << "TargetAsmStreamer(MCStreamer &S, formatted_raw_ostream &OS)"
            << std::endl
            << "\t:" << TargetName << "TargetStreamer(S), OS(OS) {}" << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::TIGenerateMCTargetDesc(){

  // Stage 1: CmakeLists.txt and LLVMBuild.txt; done;
  if( !TIGenerateMCBuild() )
    return false;

  // Stage 2: <TargetName>AsmBackend.{h,cpp}
  if( !TIGenerateMCAsmBackend() )
    return false;

  // Stage 3: <TargetName>ElfObjectWriter.cpp
  if( !TIGenerateMCElfObjWriter() )
    return false;

  // Stage 4: <TargetName>ElfStreamer{.h,cpp}; done;
  if( !TIGenerateMCElfStreamer() )
    return false;

  // Stage 5: <TargetName>FixupKinds.h; done;
  if( !TIGenerateMCFixupKinds() )
    return false;

  // Stage 6: <TargetName>InstPrinter,{h,cpp}
  if( !TIGenerateMCInstPrinter() )
    return false;

  // Stage 7: <TargetName>MCAsmInfo.{h,cpp}; done;
  if( !TIGenerateMCAsmInfo() )
    return false;

  // Stage 8: <TargetName>MCCodeEmitter.cpp
  if( !TIGenerateMCCodeEmitter() )
    return false;

  // Stage 9: <TargetName>MCExpr.{h,cpp}
  if( !TIGenerateMCExpr() )
    return false;

  // Stage 10: <TargetName>TargetDesc.{h,cpp}; done;
  if( !TIGenerateMCTargetDescCore() )
    return false;

  // Stage 11: <TargetName>TargetStreamer.{h,cpp}; done;
  if( !TIGenerateMCTargetStreamer() )
    return false;

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
  //OutStream << "add_subdirectory(Utils)" << std::endl;

  OutStream.close();

  return true;
}

bool LLVM801CG::GenerateTargetImpl(){

  // Stage 1: Create the tablegen infrastructure; done;
  if( !TIGenerateTablegen() )
    return false;

  // Stage 2: Create the ISelDagToDag template; done;
  if( !TIGenerateISelDag() )
    return false;

  // Stage 3: Create the top-level header; done;
  if( !TIGenerateTargetHeader() )
    return false;

  // Stage 4: Create the instruction lowering template; done;
  if( !TIGenerateInstLowering() )
    return false;

  // Stage 5: Create the register info template; done;
  if( !TIGenerateRegInfo() )
    return false;

  // Stage 6: Create the subtarget info template; done;
  if( !TIGenerateSubtargetInfo() )
    return false;

  // Stage 7: Create the target machine template; done;
  if( !TIGenerateTargetMach() )
    return false;

  // Stage 8: Create the target object file template; done;
  if( !TIGenerateTargetObj() )
    return false;

  // Stage 9: Create the AsmParser; done;
  if( !TIGenerateAsmParser() )
    return false;

  // Stage 10: Create the Disassembler; done;
  if( !TIGenerateDisass() )
    return false;

  // Stage 11: Create the InstPrinter; done;
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
