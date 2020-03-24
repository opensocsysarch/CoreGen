//
// _CoreGenSigMap_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include <iostream>
#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

CoreGenSigMap::CoreGenSigMap() : TmpIdx(0) {
}

CoreGenSigMap::~CoreGenSigMap(){
  Signals.clear();
}

bool CoreGenSigMap::ExecutePasses(){
  bool rtn = true;

  //
  // LegalizeSigMap Pass
  //
  SMLegalizeSigMap *LegalizeSigMap = new SMLegalizeSigMap("LegalizeSigMap",
                                                          Signals,
                                                          TempRegs,
                                                          TopSigs);

  if( !LegalizeSigMap ){
    Error = "Could not create LegalizeSigMap object";
    return false;
  }
  if( !LegalizeSigMap->Execute() ){
    Error = LegalizeSigMap->GetErrorStr();
    rtn = false;
  }
  delete LegalizeSigMap;


  return rtn;
}

bool CoreGenSigMap::CheckValidNode(const YAML::Node Node,
                                   std::string Token){
  if( !Node[Token] || Node[Token].IsNull() )
    return false;
  return true;
}

bool CoreGenSigMap::InsertSignal( SCSig *S ){
  if( S == nullptr )
    return false;

  if( this->GetNumSignals() > 0 ){
    SCSig *Prev = this->GetSignal(this->GetNumSignals()-1);
    if( (Prev->GetInst() == S->GetInst()) &&
        (Prev->GetName() == S->GetName()) ){
      // duplicate signal found, just return
      return true;
    }else{
      // not a duplicate
      Signals.push_back( S );
    }
  }else{
    Signals.push_back( S );
  }

  return true;
}

SCSig *CoreGenSigMap::GetSignal( unsigned Idx ){
  if( Idx > (Signals.size()-1) )
    return nullptr;

  return Signals[Idx];
}

unsigned CoreGenSigMap::GetMaxSignalWidth( unsigned Idx ){
  if( Idx > (Signals.size()-1) ){
    return 0;
  }

  SigType Type = Signals[Idx]->GetType();
  unsigned Max = Signals[Idx]->GetWidth();

  for( unsigned i=0; i<Signals.size(); i++ ){
    if( Signals[i]->GetType() == Type ){
      if( Signals[i]->GetWidth() > Max )
        Max = Signals[i]->GetWidth();
    }
  }

  return Max;
}

unsigned CoreGenSigMap::GetMinSignalWidth( unsigned Idx ){
  if( Idx > (Signals.size()-1) ){
    return 0;
  }

  SigType Type = Signals[Idx]->GetType();
  unsigned Min = Signals[Idx]->GetWidth();

  for( unsigned i=0; i<Signals.size(); i++ ){
    if( Signals[i]->GetType() == Type ){
      if( Signals[i]->GetWidth() < Min )
        Min = Signals[i]->GetWidth();
    }
  }

  return Min;
}

bool CoreGenSigMap::WriteSigMap(){
  if( SigFile.length() > 0 )
    return WriteSigMap(SigFile);
  return false;
}

bool CoreGenSigMap::WriteSigMap( std::string File ){
  if( File.length() == 0 )
    return false;
  if( Signals.size() == 0 )
    return false;

  // set the file name
  SigFile = File;

  // open the file
  std::ofstream OutYaml(File.c_str());

  // open the yaml emitter
  YAML::Emitter out(OutYaml);

  // open the yaml stream
  out << YAML::BeginMap;

  // write the opt-level signals
  if( !WriteTopLevelSignals(&out) ){
    OutYaml.close();
    return false;
  }

  // write the individual instruction signals
  if( !WriteInstSignals(&out) ){
    OutYaml.close();
    return false;
  }

  // write all the tmp reg info
  if( !WriteTempRegs(&out) ){
    OutYaml.close();
    return false;
  }

  // end the YAML stream
  out << YAML::EndMap;

  // close the file
  OutYaml.close();

  return true;
}

FusedOpType CoreGenSigMap::StrToFusedOpType( std::string Fop ){
  if( Fop == "FOP_UNK" ){
    return FOP_UNK;
  }else if( Fop == "FOP_SEXT" ){
    return FOP_SEXT;
  }else if( Fop == "FOP_ZEXT" ){
    return FOP_ZEXT;
  }else if( Fop == "FOP_NOT" ){
    return FOP_NOT;
  }else if( Fop == "FOP_NEG" ){
    return FOP_NEG;
  }
  return FOP_UNK;
}

SigType CoreGenSigMap::StrToSigType( std::string Sig ){
  if( Sig == "SIGUNK" ){
    return SIGUNK;
  }else if( Sig == "SIGINSTF" ){
    return SIGINSTF;
  }else if( Sig == "ALU_ADD" ){
    return ALU_ADD;
  }else if( Sig == "ALU_SUB" ){
    return ALU_SUB;
  }else if( Sig == "ALU_SLL" ){
    return ALU_SLL;
  }else if( Sig == "ALU_SRL" ){
    return ALU_SRL;
  }else if( Sig == "ALU_SRA" ){
    return ALU_SRA;
  }else if( Sig == "ALU_AND" ){
    return ALU_AND;
  }else if( Sig == "ALU_OR" ){
    return ALU_OR;
  }else if( Sig == "ALU_XOR" ){
    return ALU_XOR;
  }else if( Sig == "ALU_SLT" ){
    return ALU_SLT;
  }else if( Sig == "ALU_SLTU" ){
    return ALU_SLTU;
  }else if( Sig == "ALU_COPY" ){
    return ALU_COPY;
  }else if( Sig == "ALU_MUL" ){
    return ALU_MUL;
  }else if( Sig == "ALU_DIV" ){
    return ALU_DIV;
  }else if( Sig == "ALU_REM" ){
    return ALU_REM;
  }else if( Sig == "ALU_FADD" ){
    return ALU_FADD;
  }else if( Sig == "ALU_FSUB" ){
    return ALU_FSUB;
  }else if( Sig == "ALU_FMUL" ){
    return ALU_FMUL;
  }else if( Sig == "ALU_FDIV" ){
    return ALU_FDIV;
  }else if( Sig == "ALU_FREM" ){
    return ALU_FREM;
  }else if( Sig == "PC_INCR" ){
    return PC_INCR;
  }else if( Sig == "PC_BRJMP" ){
    return PC_BRJMP;
  }else if( Sig == "PC_JALR" ){
    return PC_JALR;
  }else if( Sig == "BR_N" ){
    return BR_N;
  }else if( Sig == "BR_NE" ){
    return BR_NE;
  }else if( Sig == "BR_EQ" ){
    return BR_EQ;
  }else if( Sig == "BR_GE" ){
    return BR_GE;
  }else if( Sig == "BR_GEU" ){
    return BR_GEU;
  }else if( Sig == "BR_LT" ){
    return BR_LT;
  }else if( Sig == "BR_LTU" ){
    return BR_LTU;
  }else if( Sig == "BR_J" ){
    return BR_J;
  }else if( Sig == "BR_JR" ){
    return BR_JR;
  }else if( Sig == "MUX_NE" ){
    return MUX_NE;
  }else if( Sig == "MUX_EQ" ){
    return MUX_EQ;
  }else if( Sig == "MUX_GT" ){
    return MUX_GT;
  }else if( Sig == "MUX_GTU" ){
    return MUX_GTU;
  }else if( Sig == "MUX_GE" ){
    return MUX_GE;
  }else if( Sig == "MUX_GEU" ){
    return MUX_GEU;
  }else if( Sig == "MUX_LT" ){
    return MUX_LT;
  }else if( Sig == "MUX_LTU" ){
    return MUX_LTU;
  }else if( Sig == "MUX_LE" ){
    return MUX_LE;
  }else if( Sig == "MUX_LEU" ){
    return MUX_LEU;
  }else if( Sig == "MUX" ){
    return MUX;
  }else if( Sig == "REG_READ" ){
    return REG_READ;
  }else if( Sig == "REG_WRITE" ){
    return REG_WRITE;
  }else if( Sig == "AREG_READ" ){
    return AREG_READ;
  }else if( Sig == "AREG_WRITE" ){
    return AREG_WRITE;
  }else if( Sig == "MEM_READ" ){
    return MEM_READ;
  }else if( Sig == "MEM_WRITE" ){
    return MEM_WRITE;
  }else if( Sig == "FENCE" ){
    return FENCE;
  }
  return SIGUNK;
}

bool CoreGenSigMap::ReadTopLevelSignals(const YAML::Node& TopNodes){
  if( TopNodes.size() == 0 ){
    Error = "No top-level signals found";
    return false;
  }

  for( unsigned i=0; i<TopNodes.size(); i++ ){
    const YAML::Node& Node = TopNodes[i];
    if( !CheckValidNode(Node,"Signal") ){
      Error = "Top-level signal node contains no name";
      return false;
    }
    std::string LSig = Node["Signal"].as<std::string>();
    if( LSig.length() == 0 ){
      Error = "Top level signal has a null name";
      return false;
    }
    TopSigs.push_back(StrToSigType(LSig));
  }
  return true;
}

bool CoreGenSigMap::ReadInstSignals(const YAML::Node& InstNodes){
  if( InstNodes.size() == 0 )
    return false;

  for( unsigned i=0; i<InstNodes.size(); i++ ){
    const YAML::Node& Node = InstNodes[i];
    if( !CheckValidNode(Node,"Inst") ){
      Error = "Instruction block has no name";
      return false;
    }
    std::string Name = Node["Inst"].as<std::string>();
    if( Node["Signals"] ){
      const YAML::Node& SNode = Node["Signals"];
      for( unsigned j=0; j<SNode.size(); j++ ){
        const YAML::Node& LSNode = SNode[j];
        if( !CheckValidNode(LSNode,"Signal") ){
          Error = "Instruction signal has no name";
          return false;
        }
        std::string SigName = LSNode["Signal"].as<std::string>();

        if( !CheckValidNode(LSNode,"Type") ){
          Error = "Instruction signal has no type: Instruction=" + Name;
          return false;
        }
        SigType Type = StrToSigType(LSNode["Type"].as<std::string>());

        if( !CheckValidNode(LSNode,"Width") ){
          Error = "Instruction signal has no width: Instruction=" + Name;
          return false;
        }
        unsigned Width = LSNode["Width"].as<unsigned>();

        signed DT = 0;
        if( CheckValidNode(LSNode,"DistanceTrue") ){
          // if the node is valid, read it.
          // otherwise, set the value to zero
          DT = LSNode["DistanceTrue"].as<signed>();
        }

        signed DF = 0;
        if( CheckValidNode(LSNode,"DistanceFalse") ){
          // if the node is valid, read it.
          // otherwise, set the value to zero
          DF = LSNode["DistanceFalse"].as<signed>();
        }

        std::string PipeStage;
        if( CheckValidNode(LSNode,"PipeStage") ){
          PipeStage = LSNode["PipeStage"].as<std::string>();
        }

        std::string FusedOp;
        if( CheckValidNode(LSNode,"FusedOp") ){
          FusedOp = LSNode["FusedOp"].as<std::string>();
        }

        Signals.push_back(new SCSig(Type,Width,DT,DF,Name,SigName,PipeStage));
        if( FusedOp.length() > 0 ){
          // write the fused op to the latest signal
          FusedOpType FType = StrToFusedOpType(FusedOp);
          Signals[Signals.size()-1]->SetFusedType(FType);
        }

        // check for inputs
        if( LSNode["Inputs"] ){
          const YAML::Node& INode = LSNode["Inputs"];
          for( unsigned k=0; k<INode.size(); k++ ){
            const YAML::Node& LINode = INode[k];
            if( !CheckValidNode(LINode,"Input") ){
              Error = "Input has no name";
              return false;
            }
            std::string InputStr = LINode["Input"].as<std::string>();
            if( InputStr.length() == 0 ){
              Error = "Input has a null name";
              return false;
            }
            Signals[Signals.size()-1]->InsertInput(InputStr);
          }
        } // end check for inputs
      }
    }
  }

  return true;
}

bool CoreGenSigMap::ReadTmpRegs(const YAML::Node& TmpNodes){

  for( unsigned i=0; i<TmpNodes.size(); i++ ){
    const YAML::Node& Node = TmpNodes[i];
    if( !CheckValidNode(Node,"Temp") ){
      Error = "Temp register has no name";
      return false;
    }
    std::string Name = Node["Temp"].as<std::string>();

    if( !CheckValidNode(Node,"Width") ){
      Error = "Temp register has no width: Temp=" + Name;
      return false;
    }
    unsigned Width = Node["Width"].as<unsigned>();

    if( Node["Mappings"] ){
      const YAML::Node& MNode = Node["Mappings"];
      SCTmp *T = nullptr;
      for( unsigned j=0; j<MNode.size(); j++ ){
        const YAML::Node& MSNode = MNode[j];
        std::string Inst = MSNode["Map"].as<std::string>();
        std::string IRName = MSNode["IRName"].as<std::string>();

        if( j==0 ){
          // create a new object
          T = new SCTmp(Name,Width,Inst,IRName);
        }else{
          // insert a new pair into the object
          T->InsertTmpPair(Inst,IRName);
        }
      }
      // add the object to our vector of tmps
      TempRegs.push_back(T);
    }
  }

  return true;
}

bool CoreGenSigMap::ReadSigMap( std::string File ){
  if( File.length() == 0 )
    return false;

  SigFile = File;

  // load the file
  YAML::Node IR;
  try{
    IR = YAML::LoadFile(File);
  }catch(YAML::ParserException& e){
    Error = "Yaml parser failed: caught exception";
    return false;
  }
  if( IR.IsNull() ){
    Error = "Signal map is empty";
    return false;
  }

  // the file is open and has nodes, read everything that is valid
  // read the top-level signals
  const YAML::Node& TopNodes = IR["SignalTop"];
  if( CheckValidNode(IR,"SignalTop") ){
    if( !ReadTopLevelSignals(TopNodes) )
      return false;
  }

  // read the instruction signals
  const YAML::Node& InstNodes = IR["Instructions"];
  if( CheckValidNode(IR,"Instructions") ){
    if( !ReadInstSignals(InstNodes) )
      return false;
  }

  // read the temporary register values
  const YAML::Node& TmpNodes = IR["Temps"];
  if( CheckValidNode(IR,"Temps") ){
    if( !ReadTmpRegs(TmpNodes) )
      return false;
  }

  return true;
}

unsigned CoreGenSigMap::GetNumInst(){
  std::vector<std::string> SigNames;
  for( unsigned i=0; i<Signals.size(); i++ ){
    SigNames.push_back(Signals[i]->GetInst());
  }
  std::sort(SigNames.begin(), SigNames.end());
  SigNames.erase( std::unique( SigNames.begin(), SigNames.end()), SigNames.end() );

  return SigNames.size();
}

std::vector<std::string> CoreGenSigMap::GetInstVect(){
  std::vector<std::string> SigNames;
  for( unsigned i=0; i<Signals.size(); i++ ){
    SigNames.push_back(Signals[i]->GetInst());
  }
  std::sort(SigNames.begin(), SigNames.end());
  SigNames.erase( std::unique( SigNames.begin(), SigNames.end()), SigNames.end() );
  return SigNames;
}

std::vector<SCSig *> CoreGenSigMap::GetSigVect(std::string Inst){
  std::vector<SCSig *> CSigs;
  for( unsigned i=0; i<Signals.size(); i++ ){
    if( Signals[i]->GetInst() == Inst ){
      CSigs.push_back(Signals[i]);
    }
  }
  return CSigs;
}

std::vector<std::string> CoreGenSigMap::GetPipeVect(){
  std::vector<std::string> V;

  for( unsigned i=0; i<Signals.size(); i++ ){
    if( Signals[i]->IsPipeDefined() ){
      V.push_back(Signals[i]->GetPipeName());
    }
  }

  std::sort( V.begin(), V.end() );
  V.erase( std::unique(V.begin(),V.end()), V.end() );
  return V;
}

bool CoreGenSigMap::WriteInstSignals(YAML::Emitter *out){
  if( out == nullptr )
    return false;

  *out << YAML::Key << "Instructions" << YAML::BeginSeq;

  // walk all the signals and derive the names
  std::vector<std::string> SigNames;
  for( unsigned i=0; i<Signals.size(); i++ ){
    SigNames.push_back(Signals[i]->GetInst());
  }
  std::sort(SigNames.begin(), SigNames.end());
  SigNames.erase( std::unique( SigNames.begin(), SigNames.end()), SigNames.end() );

  // walk all the signal names, derive all the signals and write them out
  for( unsigned i=0; i<SigNames.size(); i++ ){
    //std::vector<SCSig *> Sigs;
    std::vector<SCSig *> CSigs;

    // retrieve all the signals for the target instruction
    for( unsigned j=0; j<Signals.size(); j++ ){
      if( Signals[j]->GetInst() == SigNames[i] )
        CSigs.push_back(Signals[j]);
    }

    // write out the instruction
    *out << YAML::BeginMap << YAML::Key << "Inst" << YAML::Value << SigNames[i];
    *out << YAML::Key << "Signals" << YAML::Value << YAML::BeginSeq;
    for( unsigned j=0; j<CSigs.size(); j++ ){
      *out << YAML::BeginMap;
      *out << YAML::Key << "Signal" << YAML::Value << CSigs[j]->GetName();
      *out << YAML::Key << "Type" << YAML::Value << CSigs[j]->SigTypeToStr();
      *out << YAML::Key << "Width" << YAML::Value << CSigs[j]->GetWidth();
      *out << YAML::Key << "DistanceTrue" << YAML::Value << CSigs[j]->GetDistanceTrue();
      *out << YAML::Key << "DistanceFalse" << YAML::Value << CSigs[j]->GetDistanceFalse();
      if( CSigs[j]->IsPipeDefined() ){
        *out << YAML::Key << "PipeStage" << YAML::Value << CSigs[j]->GetPipeName();
      }
      if( CSigs[j]->GetFusedType() != FOP_UNK )
        *out << YAML::Key << "FusedOp" << YAML::Value << CSigs[j]->FusedOpTypeToStr();

      // determine whether we need to write out the input block
      if( CSigs[j]->GetNumInputs() > 0 ){
        *out << YAML::Key << "Inputs" << YAML::Value << YAML::BeginSeq << YAML::BeginMap;
        for( unsigned k=0; k<CSigs[j]->GetNumInputs() ; k++ ){
          *out << YAML::Key << "Input" << YAML::Value << CSigs[j]->GetInput(k);
        }
        *out << YAML::EndMap << YAML::EndSeq;
      }
      *out << YAML::EndMap;
    }
    *out << YAML::EndSeq;
    *out << YAML::EndMap;
  }

  *out << YAML::EndSeq;

  return true;
}

bool CoreGenSigMap::WriteTempRegs(YAML::Emitter *out){
  if( out == nullptr )
    return false;

  *out << YAML::Key << "Temps" << YAML::BeginSeq;
  for( unsigned i=0; i<TempRegs.size(); i++ ){
    *out << YAML::BeginMap << YAML::Key << "Temp" << YAML::Value << TempRegs[i]->GetName();
    *out << YAML::Key << "Width" << YAML::Value << TempRegs[i]->GetWidth();
    *out << YAML::Key << "Mappings" << YAML::Value << YAML::BeginSeq;

    for( unsigned j=0; j<TempRegs[i]->GetNumMappings(); j++ ){
      *out << YAML::BeginMap;
      std::pair<std::string,std::string> Map = TempRegs[i]->GetMap(j);
      *out << YAML::Key << "Map" << YAML::Value << Map.first;
      *out << YAML::Key << "IRName" << YAML::Value << Map.second;
      *out << YAML::EndMap;
    }

    *out << YAML::EndSeq;
    *out << YAML::EndMap;
  }
  *out << YAML::EndSeq;

  return true;
}

bool CoreGenSigMap::WriteTopLevelSignals(YAML::Emitter *out){
  if( out == nullptr )
    return false;

  std::vector<std::string> Sigs;
  for( unsigned i=0; i<Signals.size(); i++ ){
    Sigs.push_back(Signals[i]->GetName());
  }

  std::sort(Sigs.begin(),Sigs.end());
  Sigs.erase( std::unique(Sigs.begin(),Sigs.end()), Sigs.end());

  *out << YAML::Key << "SignalTop" << YAML::BeginSeq;
  for( unsigned i=0; i<Sigs.size(); i++ ){
    *out << YAML::BeginMap << YAML::Key
         << "Signal" << YAML::Value << Sigs[i]
         << YAML::EndMap;
  }
  *out << YAML::EndSeq;

  return true;
}

std::string CoreGenSigMap::GetTempReg( std::string Inst, std::string IRName,
                                       unsigned width ){

  // Search the current set of temporaries.  If we find an existing match
  // on the IRName and the width, use it.  In LLVM SSA form, we should not
  // see duplicate temporaries within a target instruction
  for( unsigned i=0; i<TempRegs.size(); i++ ){
    if( TempRegs[i]->IsIRFound(IRName) && (TempRegs[i]->GetWidth() == width) ){
      TempRegs[i]->InsertTmpPair( Inst, IRName );
      return TempRegs[i]->GetName();
    }
  }

  // Could not find an existing temporary, return a new temp
  std::string TmpName = "ALUTMP" + std::to_string(TmpIdx) + "_" + std::to_string(width);
  TmpIdx++;
  TempRegs.push_back( new SCTmp(TmpName,width,Inst,IRName) );
  return TmpName;
}

std::string CoreGenSigMap::GetTempMap( std::string Inst, std::string IRName ){
  for( unsigned i=0; i<TempRegs.size(); i++ ){
    if( TempRegs[i]->IsPairFound(Inst,IRName) )
      return TempRegs[i]->GetName();
  }
  std::string NStr = "";
  return NStr;
}

// EOF
