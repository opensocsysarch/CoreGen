//
// _CoreGenYaml_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenYaml.h"
#include <fstream>

CoreGenYaml::CoreGenYaml( std::string F, CoreGenPluginMgr *P,
                          CoreGenEnv *En, CoreGenProj *Pr, CoreGenErrno *E )
  : FileName(F), Errno(E), ParserErrorLine(0), PluginMgr(P), Env(En),
    Proj(Pr), PluginPtr(nullptr) {
}

CoreGenYaml::~CoreGenYaml(){
}

CoreGenNode *CoreGenYaml::NameToNode( std::string Target,
                           std::vector<CoreGenSoC *>  &Socs,
                           std::vector<CoreGenCore *>  &Cores,
                           std::vector<CoreGenCache *>  &Caches,
                           std::vector<CoreGenISA *>  &ISAs,
                           std::vector<CoreGenInstFormat *>  &Formats,
                           std::vector<CoreGenInst *>  &Insts,
                           std::vector<CoreGenPseudoInst *>  &PInsts,
                           std::vector<CoreGenRegClass *>  &RegClasses,
                           std::vector<CoreGenReg *>  &Regs,
                           std::vector<CoreGenComm *> &Comms,
                           std::vector<CoreGenSpad *> &Spads,
                           std::vector<CoreGenMCtrl *> &MCtrls,
                           std::vector<CoreGenVTP *> &VTPs,
                           std::vector<CoreGenExt *>  &Exts,
                           std::vector<CoreGenPlugin *> &Plugins ){
  if( Target.length() == 0 ){
    return nullptr;
  }

  CoreGenNode *N = nullptr;

  // socs
  for( unsigned i=0; i<Socs.size(); i++ ){
    N = static_cast<CoreGenNode *>(Socs[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // cores
  for( unsigned i=0; i<Cores.size(); i++ ){
    N = static_cast<CoreGenNode *>(Cores[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // caches
  for( unsigned i=0; i<Caches.size(); i++ ){
    N = static_cast<CoreGenNode *>(Caches[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // isas
  for( unsigned i=0; i<ISAs.size(); i++ ){
    N = static_cast<CoreGenNode *>(ISAs[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // formats
  for( unsigned i=0; i<Formats.size(); i++ ){
    N = static_cast<CoreGenNode *>(Formats[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // insts
  for( unsigned i=0; i<Insts.size(); i++ ){
    N = static_cast<CoreGenNode *>(Insts[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // pinsts
  for( unsigned i=0; i<PInsts.size(); i++ ){
    N = static_cast<CoreGenNode *>(PInsts[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // regclasses
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    N = static_cast<CoreGenNode *>(RegClasses[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // regs
  for( unsigned i=0; i<Regs.size(); i++ ){
    N = static_cast<CoreGenNode *>(Regs[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // comms
  for( unsigned i=0; i<Comms.size(); i++ ){
    N = static_cast<CoreGenNode *>(Comms[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // spads
  for( unsigned i=0; i<Spads.size(); i++ ){
    N = static_cast<CoreGenNode *>(Spads[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // mctrls
  for( unsigned i=0; i<MCtrls.size(); i++ ){
    N = static_cast<CoreGenNode *>(MCtrls[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // vtps
  for( unsigned i=0; i<VTPs.size(); i++ ){
    N = static_cast<CoreGenNode *>(VTPs[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // exts
  for( unsigned i=0; i<Exts.size(); i++ ){
    N = static_cast<CoreGenNode *>(Exts[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  // plugins
  for( unsigned i=0; i<Plugins.size(); i++ ){
    N = static_cast<CoreGenNode *>(Plugins[i]);
    if( N->GetName() == Target ){
      return N;
    }
    N = nullptr;
  }

  return nullptr;
}

CGRTLType CoreGenYaml::StrToCGRTL(std::string I){
  if( (I=="Unknown") || (I=="unknown") ){
    return RTLUnk;
  }else if( (I=="Chisel") || (I=="chisel") ){
    return RTLChisel;
  }else if( (I=="Verilog") || (I=="verilog") ){
    return RTLVerilog;
  }
  return RTLUnk;
}

std::string CoreGenYaml::CGRTLToStr(CGRTLType T){
  switch( T ){
  case RTLChisel:
    return "Chisel";
    break;
  case RTLVerilog:
    return "Verilog";
    break;
  case RTLUnk:
  default:
    return "Unknown";
    break;
  }
}

bool CoreGenYaml::IsValidName(std::string N){
  // YAML Node Name Rules:
  // - must have a length > 0
  // - must start with an alpha character
  // - must only contain alphanumeric characters, except '.'

  if( N.length() == 0 ){
    return false;
  }
  if( !std::isalpha(N[0]) ){
    return false;
  }
  for( unsigned i=0; i<N.length(); i++ ){
    if( N[i] != '.' ){
      if( !std::isalnum(N[i]) ){
        return false;
      }
    }
  }
  return true;
}

unsigned CoreGenYaml::GetLineNum(const YAML::Node Node){
  return Node.Mark().line;
}

void CoreGenYaml::PrintParserError(const YAML::Node Node,
                                   std::string Name,
                                   std::string Token){
  if( Token.length() > 0 ){
    Errno->SetError(CGERR_ERROR, "Error in parsing IR node at "
                    + Name + ":" + Token + " near line "
                    + std::to_string(GetLineNum(Node)) );
  }else{
    Errno->SetError(CGERR_ERROR, "Error in parsing IR node at "
                    + Name + " near line "
                    + std::to_string(GetLineNum(Node)) );
  }
  ParserErrorLine = GetLineNum(Node);
}

bool CoreGenYaml::IsDuplicate( const YAML::Node Node,
                               CoreGenNode *N,
                               std::vector<CoreGenNode *> V){
  for( unsigned i=0; i<V.size(); i++ ){
    if( N->GetName() == V[i]->GetName() ){
      Errno->SetError(CGERR_ERROR, "Error: encountered duplicate node: " +
                                    N->GetName() + " near line " +
                                    std::to_string(GetLineNum(Node)) );
      return true;
    }
  }
  return false;
}

bool CoreGenYaml::CheckValidSequenceNode(const YAML::Node Node,
                                 std::string Token ){
  if( !Node[Token] || Node[Token].IsNull() || !Node[Token].IsSequence() ){
    return false;
  }
  return true;
}

bool CoreGenYaml::CheckValidMapNode(const YAML::Node Node,
                                 std::string Token ){
  if( !Node[Token] || Node[Token].IsNull() || !Node[Token].IsMap() ){
    return false;
  }
  return true;
}

bool CoreGenYaml::CheckValidNode(const YAML::Node Node,
                                 std::string Token ){
  if( !Node[Token] || Node[Token].IsNull()){
    return false;
  }
  return true;
}

void CoreGenYaml::WriteCoreYaml( YAML::Emitter *out,
                                 std::vector<CoreGenCore *> const &Cores ){
  *out << YAML::Key << "Cores";
  *out << YAML::BeginSeq;

  for( unsigned i=0; i<Cores.size(); i++ ){
    // write out the name as an anchor
    *out << YAML::BeginMap;
    *out << YAML::Key << "Core";
    *out << YAML::Value << Cores[i]->GetName();

    // write out all the internal references as aliases

    *out << YAML::Key << "ThreadUnits";
    *out << YAML::Value << Cores[i]->GetNumThreadUnits();

    CoreGenCache *Cache = Cores[i]->GetCache();
    if( Cache ){
      *out << YAML::Key << "Cache";
      *out << YAML::Value << Cache->GetName();
    }

    CoreGenISA *ISA = Cores[i]->GetISA();
    if( ISA ){
      *out << YAML::Key << "ISA";
      *out << YAML::Value << ISA->GetName();
    }

    // register classes
    if( Cores[i]->GetNumRegClass() > 0 ){
      *out << YAML::Key << "RegisterClasses" << YAML::Value << YAML::BeginSeq;
      for( unsigned j=0; j<Cores[i]->GetNumRegClass(); j++ ){
        *out << YAML::BeginMap;
        *out << YAML::Key << "RegClass";
        *out << YAML::Value << Cores[i]->GetRegClass(j)->GetName();
        *out << YAML::EndMap;
      }
      *out << YAML::EndSeq;
    }

    // scheduler parameters
    *out << YAML::Key << "Scheduler"
         << YAML::Value << Cores[i]->CGSchedToStr(Cores[i]->GetSched());

    // extensions
    if( Cores[i]->GetNumExt() > 0 ){
      *out << YAML::Key << "Extensions" << YAML::Value << YAML::BeginSeq;
      for( unsigned j=0; j<Cores[i]->GetNumExt(); j++ ){
        *out << YAML::BeginMap;
        *out << YAML::Key << "Extension";
        *out << YAML::Value << Cores[i]->GetExt(j)->GetName();
        *out << YAML::EndMap;
      }
      *out << YAML::EndSeq;
    }

    if( Cores[i]->IsRTL() ){
      *out << YAML::Key << "RTL";
      *out << YAML::Value << Cores[i]->GetRTL();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Cores[i]->GetRTLType());
    }

    if( Cores[i]->IsRTLFile() ){
      *out << YAML::Key << "RTLFile";
      *out << YAML::Value << Cores[i]->GetRTLFile();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Cores[i]->GetRTLType());
    }
    if( Cores[i]->IsPlugin() ){
      *out << YAML::Key << "Override"
           << YAML::Value << Cores[i]->GetPlugin()->GetName();
    }
    if( Cores[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << Cores[i]->GetNotes();
    }

    *out << YAML::EndMap;
  }

  *out << YAML::EndSeq;
}

void CoreGenYaml::WriteSocYaml( YAML::Emitter *out,
                                std::vector<CoreGenSoC *> const &Socs ){
  *out << YAML::Key << "Socs";
  *out << YAML::BeginSeq;

  for( unsigned i=0; i<Socs.size(); i++ ){
    // write out the Soc name as an anchor
    *out << YAML::BeginMap;
    *out << YAML::Key << "Soc";
    *out << YAML::Value << Socs[i]->GetName();

    unsigned C = Socs[i]->GetNumCores();
    if( C > 0 ){
      // write all the details
      *out << YAML::Key << "Cores" << YAML::Value << YAML::BeginSeq;
      for( unsigned j=0; j<C; j++ ){
        *out << YAML::BeginMap;
        *out << YAML::Key << "Core";
        *out << YAML::Value << Socs[i]->GetCore(j)->GetName();
        *out << YAML::EndMap;
      }
      *out << YAML::EndSeq;
    }
    if( Socs[i]->IsRTL() ){
      *out << YAML::Key << "RTL";
      *out << YAML::Value << Socs[i]->GetRTL();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Socs[i]->GetRTLType());
    }

    if( Socs[i]->IsRTLFile() ){
      *out << YAML::Key << "RTLFile";
      *out << YAML::Value << Socs[i]->GetRTLFile();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Socs[i]->GetRTLType());
    }
    if( Socs[i]->IsPlugin() ){
      *out << YAML::Key << "Override"
           << YAML::Value << Socs[i]->GetPlugin()->GetName();
    }
    if( Socs[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << Socs[i]->GetNotes();
    }

    *out << YAML::EndMap;
  }
  *out << YAML::EndSeq;
}

void CoreGenYaml::PrintCache( YAML::Emitter *out,
                              CoreGenCache *Cache,
                              std::vector<std::string> &Printed ){
  for( unsigned i=0; i<CNames.size(); i++ ){
    if( CNames[i] == Cache->GetName() ){
      // this cache node has already been referenced
      return ;
    }
  }

  for( unsigned i=0; i<Printed.size(); i++ ){
    if( Printed[i] == Cache->GetName() ){
      // this cache has already been referenced
      return ;
    }
  }

  Printed.push_back(Cache->GetName());

  if( Cache->IsParentLevel() ){
    PrintCache( out, Cache->GetSubCache(), Printed );
  }

  // this cache node has not yet been referenced, print it
  *out << YAML::BeginMap;
  *out << YAML::Key << "Cache";
  *out << YAML::Value << Cache->GetName();
  *out << YAML::Key << "Sets";
  *out << YAML::Value << Cache->GetSets();
  *out << YAML::Key << "Ways";
  *out << YAML::Value << Cache->GetWays();
  *out << YAML::Key << "LineSize";
  *out << YAML::Value << Cache->GetLineSize();
  if( Cache->IsParentLevel() &&
      (Cache->GetSubCache() != nullptr) ){
    *out << YAML::Key << "SubLevel";
    *out << YAML::Value << Cache->GetSubCache()->GetName();
  }

  if( Cache->IsRTL() ){
    *out << YAML::Key << "RTL";
    *out << YAML::Value << Cache->GetRTL();
    *out << YAML::Key << "RTLType";
    *out << YAML::Value << CGRTLToStr(Cache->GetRTLType());
  }
  if( Cache->IsRTLFile() ){
    *out << YAML::Key << "RTLFile";
    *out << YAML::Value << Cache->GetRTLFile();
    *out << YAML::Key << "RTLType";
    *out << YAML::Value << CGRTLToStr(Cache->GetRTLType());
  }
  if( Cache->IsPlugin() ){
    *out << YAML::Key << "Override"
         << YAML::Value << Cache->GetPlugin()->GetName();
   }
    if( Cache->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << Cache->GetNotes();
    }

  *out << YAML::EndMap;
}

void CoreGenYaml::WriteCacheYaml( YAML::Emitter *out,
                                  std::vector<CoreGenCache *> const &Caches ){
  *out << YAML::Key << "Caches";
  *out << YAML::BeginSeq;

  // recursive cache printer
  std::vector<std::string> Printed;
  for( unsigned i=0; i<Caches.size(); i++ ){
    PrintCache( out, Caches[i], Printed );
  }

  *out << YAML::EndSeq;
}

void CoreGenYaml::WriteISAYaml(YAML::Emitter *out,
                               std::vector<CoreGenISA *> const &ISAs ){
  *out << YAML::Key << "ISAs";
  *out << YAML::BeginSeq;

  for( unsigned i=0; i<ISAs.size(); i++ ){
    *out << YAML::BeginMap;
    *out << YAML::Key << "ISAName" << YAML::Value << ISAs[i]->GetName();

    *out << YAML::Key << "VLIW" << YAML::Value << ISAs[i]->IsVLIW();

    if( ISAs[i]->IsRTL() ){
      *out << YAML::Key << "RTL";
      *out << YAML::Value << ISAs[i]->GetRTL();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(ISAs[i]->GetRTLType());
    }
    if( ISAs[i]->IsRTLFile() ){
      *out << YAML::Key << "RTLFile";
      *out << YAML::Value << ISAs[i]->GetRTLFile();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(ISAs[i]->GetRTLType());
    }
    if( ISAs[i]->IsPlugin() ){
      *out << YAML::Key << "Override"
           << YAML::Value << ISAs[i]->GetPlugin()->GetName();
    }
    if( ISAs[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << ISAs[i]->GetNotes();
    }
    *out << YAML::EndMap;
  }

  *out << YAML::EndSeq;
}

void CoreGenYaml::WriteVLIWStageYaml(YAML::Emitter *out,
                                     std::vector<CoreGenVLIWStage *> const &Stages ){
  *out << YAML::Key << "VLIWStages";
  *out << YAML::BeginSeq;

  for( unsigned i=0; i<Stages.size(); i++ ){
    *out << YAML::BeginMap;
    *out << YAML::Key << "VLIWStageName" << YAML::Value << Stages[i]->GetName();
    *out << YAML::Key << "InstFormat" <<
            YAML::Value << Stages[i]->GetFormat()->GetName();

    // inputs
    *out << YAML::Key << "Inputs" << YAML::Value << YAML::BeginSeq;
    std::vector<std::string> Inputs = Stages[i]->GetInputs();
    for( unsigned i=0; i<Inputs.size(); i++ ){
      *out << YAML::Key << Inputs[i];
    }
    *out << YAML::EndSeq;

    // outputs
    *out << YAML::Key << "Outputs" << YAML::Value << YAML::BeginSeq;
    std::vector<std::string> Outputs= Stages[i]->GetInputs();
    for( unsigned i=0; i<Outputs.size(); i++ ){
      *out << YAML::Key << Outputs[i];
    }
    *out << YAML::EndSeq;

    if( Stages[i]->IsImpl() ){
      *out << YAML::Key << "Impl";
      *out << YAML::Value << Stages[i]->GetImpl();
    }

    *out << YAML::EndMap;
  }
  *out << YAML::EndSeq; // End VLIW Stages
}

void CoreGenYaml::WriteInstFormatYaml(YAML::Emitter *out,
                                      std::vector<CoreGenInstFormat *> const &Formats ){
  *out << YAML::Key << "InstFormats";
  *out << YAML::BeginSeq; // Begin InstFormat

  for( unsigned i=0; i<Formats.size(); i++ ){
    *out << YAML::BeginMap;

    *out << YAML::Key << "InstFormatName";
    *out << YAML::Value << Formats[i]->GetName();

    *out << YAML::Key << "ISA";
    *out << YAML::Value << Formats[i]->GetISA()->GetName();

    *out << YAML::Key << "FormatWidth";
    *out << YAML::Value << Formats[i]->GetFormatWidth();

    if( Formats[i]->GetNumFields() > 0 ){
      // add all the inst format fields
      *out << YAML::Key << "Fields" << YAML::Value << YAML::BeginSeq;
      for( unsigned j=0; j<Formats[i]->GetNumFields(); j++ ){
        *out << YAML::BeginMap;
        std::string LField = Formats[i]->GetFieldName(j);
        *out << YAML::Key << "FieldName";
        *out << YAML::Value << LField;

        CoreGenInstFormat::CGInstField F = Formats[i]->GetFieldType(LField);
        *out << YAML::Key << "FieldType";
        *out << YAML::Value << Formats[i]->CGInstFieldToStr(F);
        *out << YAML::Key << "FieldWidth";
        *out << YAML::Value << Formats[i]->GetFieldWidth(LField);
        *out << YAML::Key << "StartBit";
        *out << YAML::Value << Formats[i]->GetStartBit(LField);
        *out << YAML::Key << "EndBit";
        *out << YAML::Value << Formats[i]->GetEndBit(LField);
        *out << YAML::Key << "MandatoryField";
        if( Formats[i]->GetMandatoryFlag(LField) ){
          *out << YAML::Value << "true";
        }else{
          *out << YAML::Value << "false";
        }
        if( F == CoreGenInstFormat::CGInstReg ){
          *out << YAML::Key << "RegClass";
          *out << YAML::Value << Formats[i]->GetFieldRegClass(LField)->GetName();
          *out << YAML::Key << "RegIsDestination";
          if( Formats[i]->GetRegFieldIsDest(LField)){
            *out << YAML::Value << "true";
          }else{
            *out << YAML::Value << "false";
          }
        }

        *out << YAML::EndMap;
      }
      *out << YAML::EndSeq;
    }

    if( Formats[i]->IsRTL() ){
      *out << YAML::Key << "RTL";
      *out << YAML::Value << Formats[i]->GetRTL();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Formats[i]->GetRTLType());
    }
    if( Formats[i]->IsRTLFile() ){
      *out << YAML::Key << "RTLFile";
      *out << YAML::Value << Formats[i]->GetRTLFile();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Formats[i]->GetRTLType());
    }
    if( Formats[i]->IsPlugin() ){
      *out << YAML::Key << "Override"
           << YAML::Value << Formats[i]->GetPlugin()->GetName();
    }
    if( Formats[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << Formats[i]->GetNotes();
    }

    *out << YAML::EndMap; // End InstFormat->GetName()
  }
  *out << YAML::EndSeq; // End InstFormat
}

void CoreGenYaml::WriteInstYaml(YAML::Emitter *out,
                                std::vector<CoreGenInst *> const &Insts ){
  *out << YAML::Key << "Insts";
  *out << YAML::BeginSeq;

  for( unsigned i=0; i<Insts.size(); i++ ){
    *out << YAML::BeginMap;
    *out << YAML::Key << "Inst";
    *out << YAML::Value << Insts[i]->GetName();

    if( Insts[i]->GetISA() != nullptr ){
      *out << YAML::Key << "ISA";
      *out << YAML::Value << Insts[i]->GetISA()->GetName();
    }

    if( Insts[i]->GetFormat() != nullptr ){
      *out << YAML::Key << "InstFormat";
      *out << YAML::Value << Insts[i]->GetFormat()->GetName();
    }

    // encodings
    if(Insts[i]->GetNumEncodings() > 0){  
      *out << YAML::Key << "Encodings" << YAML::Value << YAML::BeginSeq;
      for( unsigned j=0; j<Insts[i]->GetNumEncodings(); j++ ){
        CoreGenEncoding *E = Insts[i]->GetEncoding(j);
        *out << YAML::BeginMap;
        *out << YAML::Key << "EncodingField";
        *out << YAML::Value << E->GetField();
        *out << YAML::Key << "EncodingWidth";
        *out << YAML::Value << E->GetLength();
        *out << YAML::Key << "EncodingValue";
        *out << YAML::Value << E->GetEncoding();
        *out << YAML::EndMap;
      }
      *out << YAML::EndSeq;
    }

    // stonecutter impl
    if( Insts[i]->IsImpl() ){
      *out << YAML::Key << "Impl";
      *out << YAML::Value << Insts[i]->GetImpl();
    }

    if( Insts[i]->IsSyntax() ){
      *out << YAML::Key << "Syntax";
      *out << YAML::Value << Insts[i]->GetSyntax();
    }

    if( Insts[i]->IsRTL() ){
      *out << YAML::Key << "RTL";
      *out << YAML::Value << Insts[i]->GetRTL();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Insts[i]->GetRTLType());
    }
    if( Insts[i]->IsRTLFile() ){
      *out << YAML::Key << "RTLFile";
      *out << YAML::Value << Insts[i]->GetRTLFile();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Insts[i]->GetRTLType());
    }
    if( Insts[i]->IsPlugin() ){
      *out << YAML::Key << "Override"
           << YAML::Value << Insts[i]->GetPlugin()->GetName();
    }
    if( Insts[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << Insts[i]->GetNotes();
    }

    *out << YAML::EndMap; // individual instruction
  }

  *out <<  YAML::EndSeq;
}

void CoreGenYaml::WritePseudoInstYaml(YAML::Emitter *out,
                                      std::vector<CoreGenPseudoInst *> const &PInsts){
  *out << YAML::Key << "PseudoInsts";
  *out << YAML::BeginSeq;

  for( unsigned i=0; i<PInsts.size(); i++ ){
    *out << YAML::BeginMap;
    *out << YAML::Key << "PseudoInst";
    *out << YAML::Value << PInsts[i]->GetName();

    if( PInsts[i]->GetISA() != nullptr ){
      *out << YAML::Key << "ISA";
      *out << YAML::Value << PInsts[i]->GetISA()->GetName();
    }

    if( PInsts[i]->GetInst() != nullptr ){
      *out << YAML::Key << "Inst";
      *out << YAML::Value << PInsts[i]->GetInst()->GetName();
    }

    // encodings
    if(PInsts[i]->GetNumEncodings() > 0){
      *out << YAML::Key << "Encodings" << YAML::Value << YAML::BeginSeq;
      for( unsigned j=0; j<PInsts[i]->GetNumEncodings(); j++ ){
        CoreGenEncoding *E = PInsts[i]->GetEncoding(j);
        *out << YAML::BeginMap;
        *out << YAML::Key << "EncodingField";
        *out << YAML::Value << E->GetField();
        *out << YAML::Key << "EncodingWidth";
        *out << YAML::Value << E->GetLength();
        *out << YAML::Key << "EncodingValue";
        *out << YAML::Value << E->GetEncoding();
        *out << YAML::EndMap;
      }
      *out << YAML::EndSeq;
    }

    if( PInsts[i]->IsSyntax() ){
      *out << YAML::Key << "Syntax";
      *out << YAML::Value << PInsts[i]->GetSyntax();
    }
    if( PInsts[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << PInsts[i]->GetNotes();
    }

    *out << YAML::EndMap;
  }

  *out << YAML::EndSeq;
}

void CoreGenYaml::WriteRegClassYaml(YAML::Emitter *out,
                                    std::vector<CoreGenRegClass *> const &RegClasses){
  *out << YAML::Key << "RegClasses";
  *out << YAML::BeginSeq;
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    *out << YAML::BeginMap;
    *out << YAML::Key << "RegisterClassName";
    *out << YAML::Value << RegClasses[i]->GetName();

    *out << YAML::Key << "Registers" << YAML::Value << YAML::BeginSeq;
    for( unsigned j=0; j<RegClasses[i]->GetNumReg(); j++ ){
      *out << YAML::Key << RegClasses[i]->GetReg(j)->GetName();
    }
    *out << YAML::EndSeq;

    *out << YAML::Key << "ReadPorts" << YAML::Value << RegClasses[i]->GetReadPorts();
    *out << YAML::Key << "WritePorts" << YAML::Value << RegClasses[i]->GetWritePorts();

    if( RegClasses[i]->IsRTL() ){
      *out << YAML::Key << "RTL";
      *out << YAML::Value << RegClasses[i]->GetRTL();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(RegClasses[i]->GetRTLType());
    }
    if( RegClasses[i]->IsRTLFile() ){
      *out << YAML::Key << "RTLFile";
      *out << YAML::Value << RegClasses[i]->GetRTLFile();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(RegClasses[i]->GetRTLType());
    }
    if( RegClasses[i]->IsPlugin() ){
      *out << YAML::Key << "Override"
           << YAML::Value << RegClasses[i]->GetPlugin()->GetName();
    }
    if( RegClasses[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << RegClasses[i]->GetNotes();
    }

    *out << YAML::EndMap;
  }
  *out << YAML::EndSeq;
}

void CoreGenYaml::WriteRegYaml(YAML::Emitter *out,
                               std::vector<CoreGenReg *> const &Regs){
  *out << YAML::Key << "Registers";
  *out << YAML::BeginSeq;
  for( unsigned i=0; i<Regs.size(); i++ ){
    // all the relevant register details
    *out << YAML::BeginMap;

    *out << YAML::Key << "RegName";
    *out << YAML::Value << Regs[i]->GetName();

    *out << YAML::Key << "Width";
    *out << YAML::Value << Regs[i]->GetWidth();

    *out << YAML::Key << "Index";
    *out << YAML::Value << Regs[i]->GetIndex();

    if( Regs[i]->GetPseudoName().length() > 0 ){
      *out << YAML::Key << "PseudoName";
      *out << YAML::Value << Regs[i]->GetPseudoName();
    }

    *out << YAML::Key << "IsFixedValue";
    if( Regs[i]->IsFixedValue() ){
      *out << YAML::Value << "true";
      // TODO: write out an array of values
      std::vector<uint64_t> Vals;
      Regs[i]->GetFixedVals(Vals);
      if( Vals.size() > 0 ){
        *out << YAML::Key << "FixedValue";
        *out << YAML::Value << Vals[0];
      }
    }else{
      *out << YAML::Value << "false";
    }

    *out << YAML::Key << "IsSIMD";
    if( Regs[i]->IsSIMD() ){
      *out << YAML::Value << "true";
      *out << YAML::Key << "SIMDWidth";
      *out << YAML::Value << Regs[i]->GetSIMDWidth();
    }else{
      *out << YAML::Value << "false";
    }

    *out << YAML::Key << "RWReg";
    if( Regs[i]->IsRWAttr() ){
      *out << YAML::Value << "true";
    }else{
      *out << YAML::Value << "false";
    }

    *out << YAML::Key << "ROReg";
    if( Regs[i]->IsROAttr() ){
      *out << YAML::Value << "true";
    }else{
      *out << YAML::Value << "false";
    }

    *out << YAML::Key << "CSRReg";
    if( Regs[i]->IsCSRAttr() ){
      *out << YAML::Value << "true";
    }else{
      *out << YAML::Value << "false";
    }

    *out << YAML::Key << "AMSReg";
    if( Regs[i]->IsAMSAttr() ){
      *out << YAML:: Value << "true";
    }else{
      *out << YAML:: Value << "false";
    }

    *out << YAML::Key << "TUSReg";
    if( Regs[i]->IsTUSAttr() ){
      *out << YAML:: Value << "true";
    }else{
      *out << YAML:: Value << "false";
    }

    *out << YAML::Key << "PCReg";
    if( Regs[i]->IsPCAttr() ){
      *out << YAML:: Value << "true";
    }else{
      *out << YAML:: Value << "false";
    }

    *out << YAML::Key << "Shared";
    if( Regs[i]->IsShared() ){
      *out << YAML:: Value << "true";
    }else{
      *out << YAML:: Value << "false";
    }

    if( Regs[i]->IsVector() ){
      *out << YAML::Key << "Vector" << YAML::Value << "true";
      *out << YAML::Key << "DimX" << YAML::Value << Regs[i]->GetDimX();
    }

    if( Regs[i]->IsMatrix() ){
      *out << YAML::Key << "Matrix" << YAML::Value << "true";
      *out << YAML::Key << "DimX" << YAML::Value << Regs[i]->GetDimX();
      *out << YAML::Key << "DimY" << YAML::Value << Regs[i]->GetDimY();
    }


    // write out the subregister encodings
    if( Regs[i]->GetNumSubRegs() > 0 ){
      *out << YAML::Key << "SubRegs" << YAML::Value << YAML::BeginSeq;
      for( unsigned j=0; j<Regs[i]->GetNumSubRegs(); j++ ){
        *out << YAML::BeginMap;

        std::string SRName;
        unsigned SRStart;
        unsigned SREnd;
        if( !Regs[i]->GetSubReg(j,SRName,SRStart,SREnd) ){
          // dump out
          return ;
        }
        *out << YAML::Key << "SubReg" << YAML::Value << SRName;
        *out << YAML::Key << "StartBit" << YAML::Value << SRStart;
        *out << YAML::Key << "EndBit" << YAML::Value << SREnd;

        *out << YAML::EndMap;
      }
      *out << YAML::EndSeq;
    }

    if( Regs[i]->IsRTL() ){
      *out << YAML::Key << "RTL";
      *out << YAML::Value << Regs[i]->GetRTL();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Regs[i]->GetRTLType());
    }
    if( Regs[i]->IsRTLFile() ){
      *out << YAML::Key << "RTLFile";
      *out << YAML::Value << Regs[i]->GetRTLFile();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Regs[i]->GetRTLType());
    }
    if( Regs[i]->IsPlugin() ){
      *out << YAML::Key << "Override"
           << YAML::Value << Regs[i]->GetPlugin()->GetName();
    }
    if( Regs[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << Regs[i]->GetNotes();
    }

    *out << YAML::EndMap;
  }
  *out << YAML::EndSeq;
}

void CoreGenYaml::WriteCommYaml( YAML::Emitter *out,
                                 std::vector<CoreGenComm *> const &Comms ){
  *out << YAML::Key << "Comms";
  *out << YAML::BeginSeq;
  for( unsigned i=0; i<Comms.size(); i++ ){
    *out << YAML::BeginMap;
    *out << YAML::Key << "Comm";
    *out << YAML::Value << Comms[i]->GetName();

    *out << YAML::Key << "Type";
    switch( Comms[i]->GetCommType() ){
    case CGCommP2P:
      *out << YAML::Value << "P2P";
      break;
    case CGCommBus:
      *out << YAML::Value << "Bus";
      break;
    case CGCommNoc:
      *out << YAML::Value << "NOC";
      break;
    case CGCommUnk:
    default:
      *out << YAML::Value << "Unknown";
      break;
    }

    *out << YAML::Key << "Width";
    *out << YAML::Value << Comms[i]->GetWidth();

    if(Comms[i]->GetNumEndpoints() > 0){
      *out << YAML::Key << "Endpoints" << YAML::Value << YAML::BeginSeq;
      for( unsigned j=0; j<Comms[i]->GetNumEndpoints(); j++ ){
        *out << YAML::Key << Comms[i]->GetEndpoint(j)->GetName();
      }
      *out << YAML::EndSeq;
    }

    if( Comms[i]->IsRTL() ){
      *out << YAML::Key << "RTL";
      *out << YAML::Value << Comms[i]->GetRTL();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Comms[i]->GetRTLType());
    }
    if( Comms[i]->IsRTLFile() ){
      *out << YAML::Key << "RTLFile";
      *out << YAML::Value << Comms[i]->GetRTLFile();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Comms[i]->GetRTLType());
    }
    if( Comms[i]->IsPlugin() ){
      *out << YAML::Key << "Override"
           << YAML::Value << Comms[i]->GetPlugin()->GetName();
    }
    if( Comms[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << Comms[i]->GetNotes();
    }

    *out << YAML::EndMap;
  }
  *out << YAML::EndSeq;
}

void CoreGenYaml::WriteSpadYaml( YAML::Emitter *out,
                                 std::vector<CoreGenSpad *> const &Spads ){
  *out << YAML::Key << "Scratchpads";
  *out << YAML::BeginSeq;
  for( unsigned i=0; i<Spads.size(); i++ ){
    *out << YAML::BeginMap;
    *out << YAML::Key << "Scratchpad";
    *out << YAML::Value << Spads[i]->GetName();
    *out << YAML::Key << "MemSize";
    *out << YAML::Value << Spads[i]->GetMemSize();
    *out << YAML::Key << "RqstPorts";
    *out << YAML::Value << Spads[i]->GetRqstPorts();
    *out << YAML::Key << "RspPorts";
    *out << YAML::Value << Spads[i]->GetRspPorts();
    *out << YAML::Key << "StartAddr";
    *out << YAML::Value << Spads[i]->GetStartAddr();

    if( Spads[i]->IsRTL() ){
      *out << YAML::Key << "RTL";
      *out << YAML::Value << Spads[i]->GetRTL();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Spads[i]->GetRTLType());
    }
    if( Spads[i]->IsRTLFile() ){
      *out << YAML::Key << "RTLFile";
      *out << YAML::Value << Spads[i]->GetRTLFile();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Spads[i]->GetRTLType());
    }
    if( Spads[i]->IsPlugin() ){
      *out << YAML::Key << "Override"
           << YAML::Value << Spads[i]->GetPlugin()->GetName();
    }
    if( Spads[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << Spads[i]->GetNotes();
    }

    *out << YAML::EndMap;
  }
  *out << YAML::EndSeq;
}

void CoreGenYaml::WriteMCtrlYaml( YAML::Emitter *out,
                                  std::vector<CoreGenMCtrl *> const &MCtrls ){
  *out << YAML::Key << "MemoryControllers";
  *out << YAML::BeginSeq;
  for( unsigned i=0; i<MCtrls.size(); i++ ){
    *out << YAML::BeginMap;
    *out << YAML::Key << "MemoryController";
    *out << YAML::Value << MCtrls[i]->GetName();
    *out << YAML::Key << "Ports";
    *out << YAML::Value << MCtrls[i]->GetNumInputPorts();

    switch( MCtrls[i]->GetMemOrder() ){
    default:
    case CGWeak:
      *out << YAML::Key << "MemoryOrder";
      *out << YAML::Value << "Weak";
      break;
    case CGTSO:
      *out << YAML::Key << "MemoryOrder";
      *out << YAML::Value << "TSO";
      break;
    case CGStrong:
      *out << YAML::Key << "MemoryOrder";
      *out << YAML::Value << "Strong";
      break;
    }

    if( MCtrls[i]->IsRTL() ){
      *out << YAML::Key << "RTL";
      *out << YAML::Value << MCtrls[i]->GetRTL();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(MCtrls[i]->GetRTLType());
    }
    if( MCtrls[i]->IsRTLFile() ){
      *out << YAML::Key << "RTLFile";
      *out << YAML::Value << MCtrls[i]->GetRTLFile();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(MCtrls[i]->GetRTLType());
    }
    if( MCtrls[i]->IsPlugin() ){
      *out << YAML::Key << "Override"
           << YAML::Value << MCtrls[i]->GetPlugin()->GetName();
    }
    if( MCtrls[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << MCtrls[i]->GetNotes();
    }

    *out << YAML::EndMap;
  }
  *out << YAML::EndSeq;
}

void CoreGenYaml::WriteVTPYaml( YAML::Emitter *out,
                                std::vector<CoreGenVTP *> const &VTPs){
  *out << YAML::Key << "VTPControllers";
  *out << YAML::BeginSeq;
  for( unsigned i=0; i<VTPs.size(); i++ ){
    *out << YAML::BeginMap;
    *out << YAML::Key << "VTP";
    *out << YAML::Value << VTPs[i]->GetName();
    if( VTPs[i]->IsPlugin() ){
      *out << YAML::Key << "Override"
           << YAML::Value << VTPs[i]->GetPlugin()->GetName();
    }
    if( VTPs[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << VTPs[i]->GetNotes();
    }
    *out << YAML::EndMap;
  }
  *out << YAML::EndSeq;
}

void CoreGenYaml::WriteExtYaml( YAML::Emitter *out,
                                std::vector<CoreGenExt *> const &Exts ){
  *out << YAML::Key << "Extensions";
  *out << YAML::BeginSeq;

  for( unsigned i=0; i<Exts.size(); i++ ){
    *out << YAML::BeginMap;
    *out << YAML::Key << "Extension";
    *out << YAML::Value << Exts[i]->GetName();

    // Type
    *out << YAML::Key << "Type";
    switch( Exts[i]->GetType() ){
    case CGExtTemplate:
      *out << YAML::Value << "template";
      break;
    case CGExtModule:
      *out << YAML::Value << "module";
      break;
    case CGExtComm:
      *out << YAML::Value << "comm";
      break;
    case CGExtUnk:
    default:
      *out << YAML::Value << "unknown";
      break;
    }

    // Write all the backend info
    if( Exts[i]->GetNumRegs() > 0 ){
      WriteRegYaml(out, *(Exts[i]->GetRegs()) );
    }

    if( Exts[i]->GetNumRegClasses() > 0 ){
      WriteRegClassYaml(out, *(Exts[i]->GetRegClasses()) );
    }

    if( Exts[i]->GetNumISAs() > 0 ){
      WriteISAYaml(out, *(Exts[i]->GetISAs()) );
    }

    if( Exts[i]->GetNumFormats() > 0 ){
      WriteInstFormatYaml(out, *(Exts[i]->GetInstFormats()) );
    }

    if( Exts[i]->GetNumInsts() > 0 ){
      WriteInstYaml(out, *(Exts[i]->GetInsts()) );
    }

    if( Exts[i]->GetNumPseudoInsts() > 0 ){
      WritePseudoInstYaml(out, *(Exts[i]->GetPseudoInsts()) );
    }

    if( Exts[i]->GetNumCaches() > 0 ){
      WriteCacheYaml(out, *(Exts[i]->GetCaches()) );
    }

    if( Exts[i]->GetNumComms() > 0 ){
      WriteCommYaml(out, *(Exts[i]->GetComms()) );
    }

    if( Exts[i]->GetNumSpads() > 0 ){
      WriteSpadYaml(out, *(Exts[i]->GetSpads()) );
    }

    if( Exts[i]->GetNumMCtrls() > 0 ){
      WriteMCtrlYaml(out, *(Exts[i]->GetMCtrls()) );
    }

    if( Exts[i]->GetNumVTPs() > 0 ){
      WriteVTPYaml(out, *(Exts[i]->GetVTPs()) );
    }

    if( Exts[i]->GetNumExts() > 0 ){
      WriteExtYaml(out, *(Exts[i]->GetExts()) );
    }

    if( Exts[i]->GetNumCores() > 0 ){
      WriteCoreYaml(out, *(Exts[i]->GetCores()) );
    }

    if( Exts[i]->IsRTL() ){
      *out << YAML::Key << "RTL";
      *out << YAML::Value << Exts[i]->GetRTL();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Exts[i]->GetRTLType());
    }
    if( Exts[i]->IsRTLFile() ){
      *out << YAML::Key << "RTLFile";
      *out << YAML::Value << Exts[i]->GetRTLFile();
      *out << YAML::Key << "RTLType";
      *out << YAML::Value << CGRTLToStr(Exts[i]->GetRTLType());
    }
    if( Exts[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << Exts[i]->GetNotes();
    }

    *out << YAML::EndMap;
  }
  *out << YAML::EndSeq;
}

void CoreGenYaml::WritePluginYaml( YAML::Emitter *out,
                                   std::vector<CoreGenPlugin *> const &Plugins ){
  *out << YAML::Key << "Plugins";
  *out << YAML::BeginSeq;

  for( unsigned i=0; i<Plugins.size(); i++ ){
    *out << YAML::BeginMap;
    *out << YAML::Key << "Plugin";
    *out << YAML::Value << Plugins[i]->GetName();

    *out << YAML::Key << "PluginName";
    *out << YAML::Value << Plugins[i]->GetPluginName();

    //-- write plugin version info
    unsigned Major = 0;
    unsigned Minor = 0;
    unsigned Patch = 0;
    Plugins[i]->GetVersion(&Major,&Minor,&Patch);
    *out << YAML::Key << "MajorVersion" << YAML::Value << Major;
    *out << YAML::Key << "MinorVersion" << YAML::Value << Minor;
    *out << YAML::Key << "PatchVersion" << YAML::Value << Patch;

    // write out the feature sets
    if( Plugins[i]->GetNumFeatures() > 0 ){
      *out << YAML::Key << "Features" << YAML::Value << YAML::BeginSeq;
      for( unsigned j=0; j<Plugins[i]->GetNumFeatures(); j++ ){
        *out << YAML::BeginMap;
        *out << YAML::Key << "FeatureName";
        *out << YAML::Value << Plugins[i]->GetFeatureName(j);

        CGFeatureType FType = Plugins[i]->GetFeatureType(j);
        CGFeatureVal FVal = Plugins[i]->GetFeatureValue(j);
        *out << YAML::Key << "FeatureType";

        switch( FType ){
        case CGFUnsigned:
          *out << YAML::Value << "Unsigned";
          *out << YAML::Key << "FeatureValue";
          *out << YAML::Value << FVal.UnsignedData;
          break;
        case CGFUin32t:
          *out << YAML::Value << "Uint32t";
          *out << YAML::Key << "FeatureValue";
          *out << YAML::Value << FVal.Uint32tData;
          break;
        case CGFInt32t:
          *out << YAML::Value << "Int32t";
          *out << YAML::Key << "FeatureValue";
          *out << YAML::Value << FVal.Int32tData;
          break;
        case CGFUint64t:
          *out << YAML::Value << "Uint64t";
          *out << YAML::Key << "FeatureValue";
          *out << YAML::Value << FVal.Uint64tData;
          break;
        case CGFInt64t:
          *out << YAML::Value << "Int64t";
          *out << YAML::Key << "FeatureValue";
          *out << YAML::Value << FVal.Int64tData;
          break;
        case CGFFloat:
          *out << YAML::Value << "Float";
          *out << YAML::Key << "FeatureValue";
          *out << YAML::Value << FVal.FloatData;
          break;
        case CGFDouble:
          *out << YAML::Value << "Double";
          *out << YAML::Key << "FeatureValue";
          *out << YAML::Value << FVal.DoubleData;
          break;
        case CGFString:
          *out << YAML::Value << "String";
          *out << YAML::Key << "FeatureValue";
          *out << YAML::Value << FVal.StringData;
          break;
        case CGFBool:
          *out << YAML::Value << "Bool";
          *out << YAML::Key << "FeatureValue";
          *out << YAML::Value << FVal.BoolData;
          break;
        case CGFUnknown:
        default:
          *out << YAML::Value << "Unknown";
          break;
        }

        *out << YAML::EndMap;
      }
      *out << YAML::EndSeq;
    }

    // write out all the intermediate nodes
    // -- caches
    if( Plugins[i]->GetNumCaches() > 0 ){
      WriteCacheYaml(out, *(Plugins[i]->GetCaches()) );
    }

    //-- cores
    if( Plugins[i]->GetNumCores() > 0 ){
      WriteCoreYaml(out, *(Plugins[i]->GetCores()) );
    }

    //-- insts
    if( Plugins[i]->GetNumInsts() > 0 ){
      WriteInstYaml(out, *(Plugins[i]->GetInsts()) );
    }

    //-- pseudo insts
    if( Plugins[i]->GetNumPseudoInsts() > 0 ){
      WritePseudoInstYaml(out, *(Plugins[i]->GetPseudoInsts()) );
    }

    //-- inst formats
    if( Plugins[i]->GetNumFormats() > 0 ){
      WriteInstFormatYaml(out, *(Plugins[i]->GetInstFormats()) );
    }

    //-- regs
    if( Plugins[i]->GetNumRegs() > 0 ){
      WriteRegYaml(out, *(Plugins[i]->GetRegs()) );
    }

    //-- reg classes
    if( Plugins[i]->GetNumRegClasses() > 0 ){
      WriteRegClassYaml(out, *(Plugins[i]->GetRegClasses()) );
    }

    //-- socs
    if( Plugins[i]->GetNumSocs() > 0 ){
      WriteSocYaml( out, *(Plugins[i]->GetSocs()) );
    }

    //-- isa's
    if( Plugins[i]->GetNumISAs() > 0 ){
      WriteISAYaml(out, *(Plugins[i]->GetISAs()) );
    }

    //-- exts
    if( Plugins[i]->GetNumExts() > 0 ){
      WriteExtYaml(out, *(Plugins[i]->GetExts()) );
    }

    //-- comms
    if( Plugins[i]->GetNumComms() > 0 ){
      WriteCommYaml(out, *(Plugins[i]->GetComms()) );
    }

    //-- spads
    if( Plugins[i]->GetNumSpads() > 0 ){
      WriteSpadYaml(out, *(Plugins[i]->GetSpads()) );
    }

    //-- memory controllers
    if( Plugins[i]->GetNumMCtrls() > 0 ){
      WriteMCtrlYaml(out, *(Plugins[i]->GetMCtrls()) );
    }

    //-- VtP nodes
    if( Plugins[i]->GetNumVTPs() > 0 ){
      WriteVTPYaml(out, *(Plugins[i]->GetVTPs()) );
    }

    if( Plugins[i]->HasNotes() ){
      *out << YAML::Key << "Notes"
           << YAML::Value << Plugins[i]->GetNotes();
    }

    *out << YAML::EndMap;
  }
  *out << YAML::EndSeq;
}


void CoreGenYaml::WriteProjYaml( YAML::Emitter *out,
                                CoreGenProj *Proj ){
  *out << YAML::Key << "ProjectInfo";
  *out << YAML::BeginSeq;
  *out << YAML::BeginMap;


  *out << YAML::Key << "ProjectName";
  *out << YAML::Value << Proj->GetProjName();

  *out << YAML::Key << "ProjectRoot";
  *out << YAML::Value << Proj->GetProjRoot();

  *out << YAML::Key << "ProjectType";
  switch( Proj->GetProjType() ){
  case CGSocProj:
    *out << YAML::Value << "soc";
    break;
  case CGModProj:
    *out << YAML::Value << "module";
    break;
  case CGExtProj:
    *out << YAML::Value << "extension";
    break;
  case CGNULLProj:
  default:
    *out << YAML::Value << "unknown";
    break;
  }

  unsigned Major = 0;
  unsigned Minor = 0;
  Proj->GetChiselVersion(&Major,&Minor);
  *out << YAML::Key << "ChiselMajorVersion";
  *out << YAML::Value << Major;
  *out << YAML::Key << "ChiselMinorVersion";
  *out << YAML::Value << Minor;

  *out << YAML::EndMap;
  *out << YAML::EndSeq;
}

bool CoreGenYaml::WriteYaml(  std::vector<CoreGenSoC *> const &Socs,
                              std::vector<CoreGenCore *> const &Cores,
                              std::vector<CoreGenCache *> const &Caches,
                              std::vector<CoreGenISA *> const &ISAs,
                              std::vector<CoreGenInstFormat *> const &Formats,
                              std::vector<CoreGenInst *> const &Insts,
                              std::vector<CoreGenPseudoInst *> const &PInsts,
                              std::vector<CoreGenRegClass *> const &RegClasses,
                              std::vector<CoreGenReg *> const &Regs,
                              std::vector<CoreGenComm *> const &Comms,
                              std::vector<CoreGenSpad *> const &Spads,
                              std::vector<CoreGenMCtrl *> const &MCtrls,
                              std::vector<CoreGenVTP *> const &VTPs,
                              std::vector<CoreGenVLIWStage *> const &Stages,
                              std::vector<CoreGenExt *> const &Exts,
                              std::vector<CoreGenPlugin *> const &Plugins ){

  // sanity check
  if( FileName.length() == 0 ){
    Errno->SetError(CGERR_ERROR, "YAML output file name is null" );
    return false;
  }

  // Check to see if the file exists, if it does make a backup
  bool IsPresent = false;
  if( CGFileExists( FileName ) ){
    IsPresent = true;
    CGFileCopy(FileName, FileName+".bak");
  }

  // open the output file
  std::ofstream OutYaml(FileName.c_str());

  // open the YAML stream
  YAML::Emitter out(OutYaml);
  out << YAML::BeginMap;

  // output all the CoreGen nodes
  // -- Project data
  if( Proj != nullptr ){
    WriteProjYaml(&out,Proj);
  }

  // -- Registers
  if( Regs.size() > 0 ){
    WriteRegYaml(&out,Regs);
  }

  // -- Reg Classes
  if( RegClasses.size() > 0 ){
    WriteRegClassYaml(&out,RegClasses);
  }

  // -- ISAs
  if( ISAs.size() > 0 ){
    WriteISAYaml(&out,ISAs);
  }

  // -- Formats
  if( Formats.size() > 0 ){
    WriteInstFormatYaml(&out,Formats);
  }

  // -- VLIWStages
  if( Stages.size() > 0 ){
    WriteVLIWStageYaml(&out,Stages);
  }

  // -- Insts
  if( Insts.size() > 0 ){
    WriteInstYaml(&out,Insts);
  }

  // -- PseudoInsts
  if( PInsts.size() > 0 ){
    WritePseudoInstYaml(&out,PInsts);
  }

  // -- Caches
  if( Caches.size() > 0 ){
    WriteCacheYaml(&out,Caches);
  }

  // -- Comm Channels
  if( Comms.size() > 0 ){
    WriteCommYaml(&out,Comms);
  }

  //-- Scratchpads
  if( Spads.size() > 0 ){
    WriteSpadYaml(&out,Spads);
  }

  //-- Memory Controllers
  if( MCtrls.size() > 0 ){
    WriteMCtrlYaml(&out,MCtrls);
  }

  //-- VTPs
  if( VTPs.size() > 0 ){
    WriteVTPYaml(&out,VTPs);
  }

  // -- Extensions
  if( Exts.size() > 0 ){
    WriteExtYaml(&out,Exts);
  }

  // -- Cores
  if( Cores.size() > 0 ){
    WriteCoreYaml(&out,Cores);
  }

  // -- Plugins
  if( Plugins.size() > 0 ){
    WritePluginYaml(&out,Plugins);
  }

  // -- SoCs
  if( Socs.size() > 0 ){
    WriteSocYaml(&out,Socs);
  }

  // end the YAML sequence
  out << YAML::EndMap;

  // close the output file
  OutYaml.close();

  // If the backup is around, delete it
  if( IsPresent ){
    CGDeleteFile(FileName+".bak");
  }

  return true;
}

bool CoreGenYaml::ReadProjYaml(const YAML::Node& ProjNodes ){

  if( ProjNodes.size() == 0 ){
    // no project nodes
    return false;
  }

  const YAML::Node& Node = ProjNodes[0];
  try{
    if( !CheckValidNode(Node,"ProjectName") ){
      PrintParserError(ProjNodes,
                      "ProjectInfo",
                      "ProjectName");
      return false;
    }
  }catch(const YAML::InvalidNode &){
    PrintParserError(ProjNodes,
                      "ProjectInfo",
                      "ProjectName");
    return false;
  }
  std::string ProjectName = Node["ProjectName"].as<std::string>();
  if( !IsValidName(ProjectName) ){
    Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + ProjectName );
    return false;
  }
  Proj->SetProjName(ProjectName);

  if( !CheckValidNode(Node,"ProjectRoot") ){
    PrintParserError(Node,"ProjectInfo","ProjectRoot");
    return false;
  }
  std::string ProjectRoot = Node["ProjectRoot"].as<std::string>();
  if( Proj->GetProjRoot().length() == 0 )
    Proj->SetProjRoot(ProjectRoot);

  if( !CheckValidNode(Node,"ProjectType") ){
    PrintParserError(Node,"ProjectInfo","ProjectType");
    return false;
  }
  std::string ProjectType = Node["ProjectType"].as<std::string>();
  if( ProjectType == "soc" ){
    Proj->SetProjType(CGSocProj);
  }else if( ProjectType == "module" ){
    Proj->SetProjType(CGModProj);
  }else if( ProjectType == "extension" ){
    Proj->SetProjType(CGExtProj);
  }else if( ProjectType == "unknown" ){
    Proj->SetProjType(CGNULLProj);
  }else{
    PrintParserError(Node,"ProjectInfo","ProjectType");
    return false;
  }

  if( Node["ChiselMajorVersion"] ){
    if( !CheckValidNode(Node,"ChiselMajorVersion") ){
      PrintParserError(Node,"ProjectInfo","ChiselMajorVersion");
      return false;
    }
    unsigned CMaV = 0;
    try{
      CMaV = Node["ChiselMajorVersion"].as<unsigned>();
    }catch(YAML::BadConversion& e){
      Errno->SetError(CGERR_ERROR, "Error in parsing ChilseMajorVersion: "
                      + std::string(e.what()));
    return false;
    }

    if( !CheckValidNode(Node,"ChiselMinorVersion") ){
      PrintParserError(Node,"ProjectInfo","ChiselMinorVersion");
      return false;
    }
    unsigned CMiV = 0;
    try{
      CMiV = Node["ChiselMinorVersion"].as<unsigned>();
    }catch(YAML::BadConversion& e){
      Errno->SetError(CGERR_ERROR, "Error in parsing ChilseMinorVersion: "
                      + std::string(e.what()));
    return false;
    }

    Proj->SetChiselVersion(CMaV,CMiV);
  }

  return true;
}

bool CoreGenYaml::ReadRegisterYaml(const YAML::Node& RegNodes,
                                   std::vector<CoreGenReg *> &Regs){
  for( unsigned i=0; i<RegNodes.size(); i++ ){
    const YAML::Node& Node = RegNodes[i];
    if( !CheckValidNode(Node,"RegName") ){
      PrintParserError(RegNodes,
                       "Register",
                       "RegName");
      return false;
    }
    std::string Name = Node["RegName"].as<std::string>();

    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }

    if( !CheckValidNode(Node,"Width") ){
      PrintParserError(Node,
                       "Register",
                       "Width");
      return false;
    }
    int Width = 0;
    try{
      Width = Node["Width"].as<int>();
    }catch(YAML::BadConversion& e){
      Errno->SetError(CGERR_ERROR, "Error in parsing register width: "
                      + std::string(e.what()));
      return false;
    }

    if( !CheckValidNode(Node,"Index") ){
      PrintParserError(Node,
                       "Register",
                       "Index");
      return false;
    }
    int Index = 0;
    try{
      Index = Node["Index"].as<int>();
    }catch(YAML::BadConversion& e){
      Errno->SetError(CGERR_ERROR, "Error in parsing register index: "
                      + std::string(e.what()));
      return false;
    }

    if( !CheckValidNode(Node,"IsFixedValue") ){
      PrintParserError(Node,
                       "Register",
                       "IsFixedValue");
      return false;
    }

    bool IFV = false;
    try{
      IFV = Node["IsFixedValue"].as<bool>();
    }catch(YAML::BadConversion& e){
      Errno->SetError(CGERR_ERROR, "Error in parsing register IsFixedValue: "
                      + std::string(e.what()));
      return false;
    }

    std::vector<uint64_t> FixedVals;

    if( IFV ){
      if( !CheckValidNode(Node,"FixedValue") ){
        PrintParserError(Node,
                         "Register",
                         "FixedValue");
        return false;
      }
      uint64_t TVal = 0x00ull;
      try{
        TVal = Node["FixedValue"].as<uint64_t>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register FixedValue: "
                        + std::string(e.what()));
        return false;
      }
      FixedVals.push_back( TVal );
    }

    bool IsSIMD = false;
    bool RWReg = true;
    bool ROReg = false;
    bool CSRReg = false;
    bool AMSReg = false;
    bool TUSReg = false;
    bool PCReg = false;
    bool IsShared = false;
    bool IsVector = false;
    bool IsMatrix = false;
    unsigned DimX = 0;
    unsigned DimY = 0;

    if( CheckValidNode(Node,"IsSIMD") ){
      try{
        IsSIMD = Node["IsSIMD"].as<bool>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register IsSIMD: "
                        + std::string(e.what()));
        return false;
      }
    }

    if( CheckValidNode(Node,"RWReg") ){
      try{
        RWReg = Node["RWReg"].as<bool>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register RWReg: "
                        + std::string(e.what()));
        return false;
      }
    }

    if( CheckValidNode(Node,"ROReg") ){
      try{
        ROReg = Node["ROReg"].as<bool>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register ROReg: "
                        + std::string(e.what()));
        return false;
      }
    }

    if( CheckValidNode(Node,"CSRReg") ){
      try{
        CSRReg = Node["CSRReg"].as<bool>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register CSRReg: "
                        + std::string(e.what()));
        return false;
      }
    }

    if( CheckValidNode(Node,"AMSReg") ){
      try{
        AMSReg = Node["AMSReg"].as<bool>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register AMSReg: "
                        + std::string(e.what()));
        return false;
      }
    }

    if( CheckValidNode(Node,"TUSReg") ){
      try{
        TUSReg = Node["TUSReg"].as<bool>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register TUSReg: "
                        + std::string(e.what()));
        return false;
      }
    }

    if( CheckValidNode(Node,"PCReg") ){
      try{
        PCReg = Node["PCReg"].as<bool>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register PCReg: "
                        + std::string(e.what()));
        return false;
      }
    }

    if( CheckValidNode(Node,"Shared") ){
      try{
        IsShared = Node["Shared"].as<bool>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register Shared: "
                        + std::string(e.what()));
        return false;
      }
    }

    if( CheckValidNode(Node,"Vector") ){
      try{
        IsVector = Node["Vector"].as<bool>();
        if( IsVector ){
          // parse DimX
          try{
            DimX = Node["DimX"].as<unsigned>();
            if( DimX == 0 ){
              Errno->SetError(CGERR_ERROR, "Error in parsing DimX: Cannot be '0'");
              return false;
            }
          }catch(YAML::BadConversion& e){
            Errno->SetError(CGERR_ERROR, "Error in parsing register Vector:DimX: "
                            + std::string(e.what()));
            return false;
          }
        }
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register Vector: "
                        + std::string(e.what()));
        return false;
      }
    }

    if( CheckValidNode(Node,"Matrix") ){
      try{
        IsMatrix = Node["Matrix"].as<bool>();
        if( IsMatrix ){
          // parse DimX
          try{
            DimX = Node["DimX"].as<unsigned>();
          }catch(YAML::BadConversion& e){
            Errno->SetError(CGERR_ERROR, "Error in parsing register Matrix:DimX: "
                            + std::string(e.what()));
            return false;
          }
          // parse DimY
          try{
            DimY = Node["DimY"].as<unsigned>();
          }catch(YAML::BadConversion& e){
            Errno->SetError(CGERR_ERROR, "Error in parsing register Matrix:DimY: "
                            + std::string(e.what()));
            return false;
          }

          if( (DimX==0) || (DimY==0) ){
            Errno->SetError(CGERR_ERROR, "Error in parsing DimX/DimY: Cannot be '0'");
            return false;
          }
        }
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register Matrix: "
                        + std::string(e.what()));
        return false;
      }
    }


    // build a new register node
    CoreGenReg *R = new CoreGenReg(Name,Index,Width,Errno);
    if( R == nullptr ){
      return false;
    }

    if( CheckValidNode(Node,"PseudoName") ){
      R->SetPseudoName(Node["PseudoName"].as<std::string>());
    }

    // set the simd attrs
    if( IsSIMD && CheckValidNode(Node,"SIMDWidth") ){
      int SIMDWidth = 0;
      try{
        SIMDWidth = Node["SIMDWidth"].as<int>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing register SIMDWidth: "
                        + std::string(e.what()));
        delete R;
        return false;
      }
      R->SetSIMD(SIMDWidth);
    }

    if( IFV ){
      if( !R->SetFixedVals(FixedVals) ){
        return false;
      }
    }

    // set the register attributes
    uint32_t Attrs = 0x00;
    if( RWReg ){
      Attrs |= CoreGenReg::CGRegRW;
    }
    if( ROReg ){
      Attrs |= CoreGenReg::CGRegRO;
    }
    if( CSRReg ){
      Attrs |= CoreGenReg::CGRegCSR;
    }
    if( AMSReg ){
      Attrs |= CoreGenReg::CGRegAMS;
    }
    if( TUSReg ){
      Attrs |= CoreGenReg::CGRegTUS;
    }
    if( PCReg ){
      Attrs |= CoreGenReg::CGRegPC;
    }
    R->SetAttrs(Attrs);
    R->SetShared(IsShared);

    if( IsVector ){
      R->SetVector(DimX);
    }
    if( IsMatrix ){
      R->SetMatrix(DimX,DimY);
    }

    // Check for subregisters
    const YAML::Node& SNode = Node["SubRegs"];
    if( SNode ){
      if( SNode.size() == 0 ){
        PrintParserError(SNode, "SubRegs", "SubReg" );
        return false;
      }
      for( unsigned j=0; j<SNode.size(); j++ ){
        const YAML::Node& LSNode = SNode[j];

        // name
        if( !CheckValidNode(LSNode, "SubReg") ){
          PrintParserError(LSNode, "SubRegs", "SubReg" );
          return false;
        }
        std::string SName = LSNode["SubReg"].as<std::string>();

        // start bit
        if( !CheckValidNode(LSNode, "StartBit") ){
          PrintParserError(LSNode, "SubRegs", "StartBit" );
          return false;
        }
        unsigned SB = 0;
        try{
          SB = LSNode["StartBit"].as<unsigned>();
        }catch(YAML::BadConversion& e){
          Errno->SetError(CGERR_ERROR, "Error in parsing subregister StartBit: "
                          + std::string(e.what()));
          delete R;
          return false;
        }

        // end bit
        if( !CheckValidNode(LSNode, "EndBit") ){
          PrintParserError(LSNode, "SubRegs", "EndBit" );
          return false;
        }
        unsigned EB = 0;
        try{
          EB = LSNode["EndBit"].as<unsigned>();
        }catch(YAML::BadConversion& e){
          Errno->SetError(CGERR_ERROR, "Error in parsing subregister EndBit: "
                          + std::string(e.what()));
          delete R;
          return false;
        }

        // insert the subreg into the register
        if( !R->InsertSubReg(SName,SB,EB) ){
          return false;
        }
      }
    }

    // Check for custom RTL
    if( CheckValidNode(Node,"RTL") ){
      R->SetRTL( Node["RTL"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLFile") ){
      R->SetRTLFile( Node["RTLFile"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLType") ){
      if( !R->SetRTLType( StrToCGRTL(Node["RTLType"].as<std::string>()) ) ){
        return false;
      }
    }

    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + Name );
        return false;
      }
      R->SetOverriddenNode(PNode);
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      R->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(R),
                    std::vector<CoreGenNode *>(Regs.begin(),Regs.end()) ) ){
      return false;
    }
    // add the register object
    Regs.push_back(R);
  }
  return true;
}

bool CoreGenYaml::ReadVLIWStageYaml(const YAML::Node& StageNodes,
                                    std::vector<CoreGenVLIWStage *> &Stages,
                                    std::vector<CoreGenInstFormat *> &Formats){
  for( unsigned i=0; i<StageNodes.size(); i++ ){
    const YAML::Node& Node = StageNodes[i];
    if( !CheckValidNode(Node, "VLIWStageName") ){
      PrintParserError(Node,
                      "VLIWStages",
                      "VLIWStageName");
      return false;
    }
    std::string Name = Node["VLIWStageName"].as<std::string>();

    if( !CheckValidNode(Node, "InstFormat") ){
      PrintParserError(Node,
                       "VLIWStages",
                       "InstFormat");
      return false;
    }
    std::string FormatName = Node["InstFormat"].as<std::string>();
    CoreGenInstFormat *Format = nullptr;

    // derive the instformat object
    for( unsigned j=0; j<Formats.size(); j++ ){
      if( Formats[j]->GetName() == FormatName ){
        Format = Formats[j];
      }
    }
    if( Format == nullptr ){
      Errno->SetError(CGERR_ERROR,
                      "No instruction format found for InstFormat: " +
                      FormatName);
      return false;
    }

    // create the object
    CoreGenVLIWStage *Stage = new CoreGenVLIWStage(Name, Format, Errno);

    if( Node["Inputs"] ){
      const YAML::Node& INode = Node["Inputs"];
      for( unsigned j=0; j<INode.size(); j++ ){
        Stage->InsertInput(INode[j].as<std::string>());
      }
    }

    if( Node["Outputs"] ){
      const YAML::Node& ONode = Node["Outputs"];
      for( unsigned j=0; j<ONode.size(); j++ ){
        Stage->InsertOutput(ONode[j].as<std::string>());
      }
    }

    if( Node["Impl"] ){
      std::string Tmp = Node["Impl"].as<std::string>();

      // try to open this file 
      std::ifstream ExtFile (Tmp.c_str());
      // temp string for holding implementation
      std::stringstream ExtImpl; 
      if( ExtFile.is_open() ){ // File Exists 
        ExtImpl << ExtFile.rdbuf();
        ExtFile.close();
        if( !Stage->SetImpl(ExtImpl.str()) ){
          return false;
        }
      }else{ // not external file
        if( !Stage->SetImpl( Node["Impl"].as<std::string>() ) ){
          return false;
        }
      }
    }

    // insert the stage
    Stages.push_back(Stage);
  }

  return true;
}

bool CoreGenYaml::ReadRegisterClassYaml(const YAML::Node& RegClassNodes,
                                        std::vector<CoreGenRegClass *> &RegClasses,
                                        std::vector<CoreGenReg *> &Regs){
  for( unsigned i=0; i<RegClassNodes.size(); i++ ){
    const YAML::Node& Node = RegClassNodes[i];
    if( !CheckValidNode(Node,"RegisterClassName") ){
      PrintParserError(Node,
                       "RegClasses",
                       "RegisterClassName");
      return false;
    }
    std::string Name = Node["RegisterClassName"].as<std::string>();

    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }

    CoreGenRegClass *RC = new CoreGenRegClass( Name, Errno );
    if( RC == nullptr ){
      Errno->SetError(CGERR_ERROR, "Error creating new register class for " + Name );
      return false;
    }

    // walk all the registers
    if( Node["Registers"] ){
      const YAML::Node& RNode = Node["Registers"];
      if( RNode.size() == 0 ){
        Errno->SetError(CGERR_WARN,
                        "Warning: No registers defined for RegisterClass " + Name
                        + " at line " + std::to_string(GetLineNum(RNode)) );
      }
      for( unsigned j=0; j<RNode.size(); j++ ){
        // insert each node into the register class
        std::string RName = RNode[j].as<std::string>();

        bool found = false;
        for( unsigned k=0; k<Regs.size(); k++ ){
          if( Regs[k]->GetName() == RName ){
            RC->InsertReg(Regs[k]);
            found = true;
          }
        } // end unsigned k
        if( !found ){
          Errno->SetError(CGERR_ERROR, "Error: register not found: "
                          + RName + " at line "
                          + std::to_string(GetLineNum(RNode)+j+1));
          return false;
        }// end if
      } // end unsigned j
    } // end if

    if( CheckValidNode(Node,"ReadPorts") ){
      unsigned RP = 0;
      try{
        RP = Node["ReadPorts"].as<unsigned>();
      }catch(YAML::BadConversion& e){
          Errno->SetError(CGERR_ERROR, "Error in parsing register class ReadPorts: "
                          + std::string(e.what()));
          delete RC;
          return false;
      }

      RC->SetReadPorts(RP);
    }

    if( CheckValidNode(Node,"WritePorts") ){
      unsigned WP = 0;
      try{
        WP = Node["WritePorts"].as<unsigned>();
      }catch(YAML::BadConversion& e){
          Errno->SetError(CGERR_ERROR, "Error in parsing register class WritePorts: "
                          + std::string(e.what()));
          delete RC;
          return false;
      }

      RC->SetWritePorts(WP);
    }

    if( CheckValidNode(Node,"RTL") ){
      RC->SetRTL( Node["RTL"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLFile") ){
      RC->SetRTLFile( Node["RTLFile"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLType") ){
      if( !RC->SetRTLType( StrToCGRTL(Node["RTLType"].as<std::string>()) ) ){
        return false;
      }
    }

    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + Name );
        return false;
      }
      RC->SetOverriddenNode(PNode);
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      RC->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(RC),
                    std::vector<CoreGenNode *>(RegClasses.begin(),RegClasses.end()) ) ){
      return false;
    }

    RegClasses.push_back(RC);
  }

  return true;
}

bool CoreGenYaml::ReadISAYaml(const YAML::Node& ISANodes,
                              std::vector<CoreGenISA *> &ISAs){
  for( unsigned i=0; i<ISANodes.size(); i++ ){
    const YAML::Node& Node = ISANodes[i];
    if( !CheckValidNode(Node,"ISAName") ){
      PrintParserError(Node,
                       "ISA",
                       "ISAName");
      return false;
    }
    std::string ISAName = Node["ISAName"].as<std::string>();

    if( !IsValidName(ISAName) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + ISAName );
      return false;
    }

    CoreGenISA *ISA = new CoreGenISA(ISAName, Errno);

    // check to see if the ISA is VLIW
    bool isVLIW = false;
    if( CheckValidNode(Node,"VLIW") ){
      isVLIW = Node["VLIW"].as<bool>();
    }
    ISA->SetVLIW(isVLIW);

    if( CheckValidNode(Node,"RTL") ){
      ISA->SetRTL( Node["RTL"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLFile") ){
      ISA->SetRTLFile( Node["RTLFile"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLType") ){
      if( !ISA->SetRTLType( StrToCGRTL(Node["RTLType"].as<std::string>()) ) ){
        return false;
      }
    }

    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + ISAName );
        return false;
      }
      ISA->SetOverriddenNode(PNode);
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      ISA->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(ISA),
                    std::vector<CoreGenNode *>(ISAs.begin(),ISAs.end()) ) ){
      return false;
    }

    ISAs.push_back(ISA);
  }
  return true;
}

bool CoreGenYaml::ReadInstFormatYaml(const YAML::Node& InstFormatNodes,
                                     std::vector<CoreGenInstFormat *> &Formats,
                                     std::vector<CoreGenISA *> &ISAs,
                                     std::vector<CoreGenRegClass *> &RegClasses){
  for( unsigned i=0; i<InstFormatNodes.size(); i++ ){
    const YAML::Node& Node = InstFormatNodes[i];
    if( !CheckValidNode(Node,"InstFormatName") ){
      PrintParserError(InstFormatNodes[i],
                       "InstFormat",
                       "InstFormatName");
      return false;
    }
    std::string Name = Node["InstFormatName"].as<std::string>();

    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }

    if( !CheckValidNode(Node,"ISA") ){
      PrintParserError(Node,
                       "InstFormat",
                       "ISA");
      return false;
    }
    std::string ISA = Node["ISA"].as<std::string>();

    CoreGenISA *LISA = nullptr;

    // find the associated ISA
    for( unsigned k=0; k<ISAs.size(); k++ ){
      if( ISAs[k]->GetName() == ISA ){
        LISA = ISAs[k];
      }
    }

    if( LISA == nullptr ){
      // couldnt find the associated ISA
      Errno->SetError(CGERR_ERROR,
                      "Error: ISA not found: "
                      + ISA + " at line "
                      + std::to_string(GetLineNum(Node)));
      return false;
    }

    CoreGenInstFormat *IF = new CoreGenInstFormat( Name, LISA, Errno );
    if( IF == nullptr ){
      Errno->SetError(CGERR_ERROR,
                      "Error: Could not create InstructionFormat object" );
      return false;
    }

    if( !CheckValidNode(Node,"Fields") ){
      PrintParserError(Node,
                       "InstFormat",
                       "Fields");
      return false;
    }

    const YAML::Node& FNode = Node["Fields"];
    if( FNode.size() == 0 ){
      Errno->SetError(CGERR_ERROR,
                      "Error: No fields defined for InstructionFormat "
                       + Name + " at line "
                       + std::to_string(GetLineNum(FNode)) );
      return false;
    }
    if( FNode ){
      for( unsigned j=0; j<FNode.size(); j++ ){
        const YAML::Node& LFNode = FNode[j];

        if( !CheckValidNode(LFNode,"FieldName") ){
          PrintParserError(LFNode, "Fields", "FieldName" );
          return false;
        }
        std::string FieldName = LFNode["FieldName"].as<std::string>();

        if( !CheckValidNode(LFNode,"FieldType") ){
          PrintParserError(LFNode, "Fields", "FieldType" );
          return false;
        }
        std::string FieldType = LFNode["FieldType"].as<std::string>();

        if( !CheckValidNode(LFNode,"StartBit") ){
          PrintParserError(LFNode,"Fields","StartBit");
          return false;
        }
        int StartBit = 0;
        try{
          StartBit = LFNode["StartBit"].as<int>();
        }catch(YAML::BadConversion& e){
          Errno->SetError(CGERR_ERROR, "Error in parsing InstFormat StartBit: "
                          + std::string(e.what()));
          delete IF;
          return false;
        }

        if( !CheckValidNode(LFNode,"EndBit") ){
          PrintParserError(LFNode,"Fields","EndBit");
          return false;
        }
        int EndBit = 0;
        try{
          EndBit = LFNode["EndBit"].as<int>();
        }catch(YAML::BadConversion& e){
          Errno->SetError(CGERR_ERROR, "Error in parsing InstFormat EndBit: "
                          + std::string(e.what()));
          delete IF;
          return false;
        }

        if( !CheckValidNode(LFNode,"MandatoryField") ){
          PrintParserError(LFNode,"Fields","MandatoryField");
          return false;
        }
        bool Mand = false;
        try{
          Mand = LFNode["MandatoryField"].as<bool>();
        }catch(YAML::BadConversion& e){
          Errno->SetError(CGERR_ERROR, "Error in parsing InstFormat MandatoryField: "
                          + std::string(e.what()));
          delete IF;
          return false;
        }

        CoreGenInstFormat::CGInstField FT;
        if( FieldType == "CGInstReg" ){
          FT = CoreGenInstFormat::CGInstReg;
        }else if( FieldType == "CGInstCode" ){
          FT = CoreGenInstFormat::CGInstCode;
        }else if( FieldType == "CGInstImm" ){
          FT = CoreGenInstFormat::CGInstImm;
        }else{
          Errno->SetError(CGERR_ERROR,
                  "Error: Could not decode instruction field type: \""
                  + FieldType + "\" for instruction format " + Name
                  + " at line " + std::to_string(GetLineNum(LFNode)));
          return false;
        }

        if( !IF->InsertField(FieldName,StartBit,EndBit,FT,Mand) ){
          return false;
        }

        if( LFNode["RegClass"] ){
          // handle reg class
          if( !CheckValidNode(LFNode, "RegClass") ){
            PrintParserError(LFNode, "Fields", "RegClass");
            return false;
          }
          std::string RegClass = LFNode["RegClass"].as<std::string>();

          CoreGenRegClass *RC = nullptr;
          for( unsigned k=0; k<RegClasses.size(); k++ ){
            if( RegClasses[k]->GetName() == RegClass ){
              RC = RegClasses[k];
            }
          }
          if( RC == nullptr ){
            Errno->SetError(CGERR_ERROR,
                      "Error: RegisterClass not found: "
                      + RegClass + " at line "
                      + std::to_string(GetLineNum(LFNode["RegClass"])));
            return false;
          }

          if( !IF->InsertRegFieldMap(FieldName,RC) ){
            Errno->SetError(CGERR_ERROR,
                  "Error: Could not insert register field mapping into InstructionFormat="
                  + Name + " for field " + FieldName);
            return false;
          }

          //Right now we are making it optional to specify whether a register is a destination
          // If the user does not specify we set it to "false" by default
          bool IsDest = false;
          if( CheckValidNode(LFNode,"RegIsDestination") ){
            try{
              IsDest = LFNode["RegIsDestination"].as<bool>();
            }catch(YAML::BadConversion& e){
              Errno->SetError(CGERR_ERROR, "Error in parsing InstFormat RegIsDestination: "
                              + std::string(e.what()));
              delete IF;
              return false;
            }
          }

          if( !IF->SetRegFieldIsDestFlag(FieldName, IsDest) ){
            Errno->SetError(CGERR_ERROR,
                  "Error: Could not set destination register flag into InstructionFormat="
                  + Name + " for field " + FieldName);
            return false;
          }
        }
      }
    }

    if( CheckValidNode(Node,"RTL") ){
      IF->SetRTL( Node["RTL"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLFile") ){
      IF->SetRTLFile( Node["RTLFile"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLType") ){
      if( !IF->SetRTLType( StrToCGRTL(Node["RTLType"].as<std::string>()) ) ){
        return false;
      }
    }

    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + Name );
        return false;
      }
      IF->SetOverriddenNode(PNode);
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      IF->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(IF),
                    std::vector<CoreGenNode *>(Formats.begin(),Formats.end()) ) ){
      return false;
    }

    Formats.push_back(IF);
  }
  return true;
}

bool CoreGenYaml::ReadInstYaml(const YAML::Node& InstNodes,
                               std::vector<CoreGenInst *> &Insts,
                               std::vector<CoreGenInstFormat *> &Formats,
                               std::vector<CoreGenISA *> &ISAs){
  for( unsigned i=0; i<InstNodes.size(); i++ ){
    const YAML::Node& Node = InstNodes[i];

    if( !CheckValidNode(Node,"Inst") ){
      PrintParserError(InstNodes,
                       "Inst",
                       "Insts");
      return false;
    }
    std::string Name = Node["Inst"].as<std::string>();
    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }


    std::string ISAName;
    if( CheckValidNode(Node,"ISA") ){
      ISAName = Node["ISA"].as<std::string>();
    }

    std::string InstFormat;
    if( CheckValidNode(Node,"InstFormat") ){
      InstFormat = Node["InstFormat"].as<std::string>();
    }

    // decode the ISA and InstFormat
    CoreGenInstFormat *IF = nullptr;
    CoreGenISA *ISA = nullptr;
    for( unsigned j=0; j<Formats.size(); j++ ){
      if( Formats[j]->GetName() == InstFormat ){
        IF = Formats[j];
      }
    }
    if( IF == nullptr && (InstFormat.length() > 0 ) ){
      Errno->SetError(CGERR_ERROR,
                      "No InstFormat found to match " + InstFormat +
                      " for instruction " + Name );
      return false;
    }
    for( unsigned j=0; j<ISAs.size(); j++ ){
      if( ISAs[j]->GetName() == ISAName ){
        ISA = ISAs[j];
      }
    }
    if( ISA == nullptr && (ISAName.length() > 0 ) ){
      Errno->SetError(CGERR_ERROR,
                      "No ISA found to match " + ISAName +
                      " for instruction " + Name );
      return false;
    }

    // create a new object
    CoreGenInst *Inst = new CoreGenInst( Name, ISA, IF, Errno );
    if( Inst == nullptr ){
      return false;
    }

    // walk all the encodings
    if( Node["Encodings"] ){
      const YAML::Node& FNode = Node["Encodings"];
      for( unsigned k=0; k<FNode.size(); k++ ){
        const YAML::Node& LFNode = FNode[k];
        std::string FieldName = LFNode["EncodingField"].as<std::string>();

        if( !CheckValidNode(LFNode,"EncodingValue") ){
          PrintParserError(FNode,
                            "Encodings",
                            "EncodingValue");
          return false;
        }
        int Value = 0;
        try{
          Value = LFNode["EncodingValue"].as<int>();
        }catch(YAML::BadConversion& e){
          Errno->SetError(CGERR_ERROR, "Error in parsing instruction EncodingValue: "
                          + std::string(e.what()));
          delete Inst;
          return false;
        }

        if( !Inst->SetEncoding(FieldName,Value) ){
          return false;
        }
      }
    }

    if( Node["Impl"] ){
      if( !Inst->SetImpl( Node["Impl"].as<std::string>() ) ){
        return false;
      }
    }
    if( Node["Syntax"] ){
      if( !Inst->SetSyntax( Node["Syntax"].as<std::string>() ) ){
        return false;
      }
    }
    if( Node["RTL"] ){
      Inst->SetRTL( Node["RTL"].as<std::string>());
    }
    if( Node["RTLFile"] ){
      Inst->SetRTLFile( Node["RTLFile"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLType") ){
      if( !Inst->SetRTLType( StrToCGRTL(Node["RTLType"].as<std::string>()) ) ){
        return false;
      }
    }
    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + Name );
        return false;
      }
      Inst->SetOverriddenNode(PNode);
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      Inst->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(Inst),
                    std::vector<CoreGenNode *>(Insts.begin(),Insts.end()) ) ){
      return false;
    }
    Insts.push_back(Inst);
  }
  return true;
}

bool CoreGenYaml::ReadPseudoInstYaml(const YAML::Node& PInstNodes,
                                    std::vector<CoreGenPseudoInst *> &PInsts,
                                    std::vector<CoreGenInst *> &Insts,
                                    std::vector<CoreGenISA *> &ISAs){
  for( unsigned i=0; i<PInstNodes.size(); i++ ){
    const YAML::Node& Node = PInstNodes[i];
    if( !CheckValidNode(Node,"PseudoInst") ){
      PrintParserError(PInstNodes,
                       "PseudoInst",
                       "PseudoInsts");
      return false;
    }

    std::string Name = Node["PseudoInst"].as<std::string>();
    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }

    std::string ISAName;
    if( CheckValidNode(Node,"ISA") ){
      ISAName = Node["ISA"].as<std::string>();
    }

    std::string InstName;
    if( CheckValidNode(Node,"Inst") ){
      InstName = Node["Inst"].as<std::string>();
    }

    // decode the isa and instruction
    CoreGenISA *ISA = nullptr;
    CoreGenInst *Inst = nullptr;
    for( unsigned j=0; j<ISAs.size(); j++ ){
      if( ISAs[j]->GetName() == ISAName ){
        ISA = ISAs[j];
      }
    }
    if( ISA == nullptr && (ISAName.length() > 0 ) ){
      Errno->SetError(CGERR_ERROR, "Invalid Node: " + ISAName );
      return false;
    }
    for( unsigned j=0; j<Insts.size(); j++ ){
      if( Insts[j]->GetName() == InstName ){
        Inst = Insts[j];
      }
    }
    if( Inst == nullptr && (InstName.length() > 0 ) ){
      Errno->SetError(CGERR_ERROR, "Invalid Node: " + InstName );
      return false;
    }

    CoreGenPseudoInst *P = new CoreGenPseudoInst(Name,Inst,Errno);
    if( P == nullptr ){
      Errno->SetError(CGERR_ERROR, "Error creating pseudo instruction: " + Name );
      return false;
    }

    // walk all the encodings
    if( Node["Encodings"] ){
      const YAML::Node& FNode = Node["Encodings"];
      for( unsigned k=0; k<FNode.size(); k++ ){
        const YAML::Node& LFNode = FNode[k];
        std::string FieldName = LFNode["EncodingField"].as<std::string>();
        int Value = 0;
        try{
          Value = LFNode["EncodingValue"].as<int>();
        }catch(YAML::BadConversion& e){
          Errno->SetError(CGERR_ERROR, "Error in parsing pseudo instruction EncodingValue: "
                          + std::string(e.what()));
          delete P;
          return false;
        }

        if( !P->SetEncoding(FieldName,Value) ){
          return false;
        }
      }
    }

    if( Node["Syntax"] ){
      if( !P->SetSyntax( Node["Syntax"].as<std::string>() ) ){
        return false;
      }
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      P->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(P),
                    std::vector<CoreGenNode *>(PInsts.begin(),PInsts.end()) ) ){
      return false;
    }

    PInsts.push_back(P);
  }
  return true;
}

bool CoreGenYaml::ReadDataPathYaml(const YAML::Node& DataPathNodes,
                                std::vector<CoreGenDataPath *> &DataPaths){

   for( unsigned i=0; i<DataPathNodes.size(); i++){
     const YAML::Node& Node = DataPathNodes[i];
     if( !CheckValidNode(Node, "Pipeline") ){
       PrintParserError(DataPathNodes,
                          "DataPaths",
                          "Pipeline");
       return false;
    }
   
    
    std::string Name = Node["Pipeline"].as<std::string>();

    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node name: " + Name);
      return false;
    } 
  
    std::string Style = Node["Style"].as<std::string>();

    CoreGenDataPath *D = new CoreGenDataPath(Name, Style, Errno);
    DataPaths.push_back(D);
   }


   return true;
}

bool CoreGenYaml::ReadCacheYaml(const YAML::Node& CacheNodes,
                                std::vector<CoreGenCache *> &Caches){
  for( unsigned i=0; i<CacheNodes.size(); i++ ){
    const YAML::Node& Node = CacheNodes[i];
    if( !CheckValidNode(Node,"Cache") ){
      PrintParserError(CacheNodes,
                       "Cache",
                       "Caches");
      return false;
    }

    std::string Name = Node["Cache"].as<std::string>();

    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }

    int Sets = 0;
    try{
      Sets = Node["Sets"].as<int>();
    }catch(YAML::BadConversion& e){
      Errno->SetError(CGERR_ERROR, "Error in parsing cache Sets: "
                      + std::string(e.what()));
      return false;
    }

    int Ways = 0;
    try {
      Ways = Node["Ways"].as<int>();
    }catch(YAML::BadConversion& e){
      Errno->SetError(CGERR_ERROR, "Error in parsing cache Ways: "
                      + std::string(e.what()));
      return false;
    }
    unsigned LineSize = 64;

    if( CheckValidNode(Node,"LineSize") ){
      try{
        LineSize = Node["LineSize"].as<unsigned>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing cache LineSize: "
                        + std::string(e.what()));
        return false;
      }
    }

    std::string SubLevel;
    if( Node["SubLevel"] ){
      SubLevel = Node["SubLevel"].as<std::string>();
    }

    CoreGenCache *C = new CoreGenCache( Name, Sets, Ways, Errno );
    if( C == nullptr ){
      return false;
    }
    C->SetLineSize(LineSize);

    // get the sublevel
    if( SubLevel.length() > 0 ){
      CoreGenCache *SC = nullptr;
      for( unsigned j=0; j<Caches.size(); j++ ){
        if( Caches[j]->GetName() == SubLevel ){
          SC = Caches[j];
        }
      }
      if( SC == nullptr ){
        return false;
      }

      C->SetChildCache( SC );
      SC->SetParentCache( C );  // set the parent of the child
    }

    if( Node["RTL"] ){
      C->SetRTL( Node["RTL"].as<std::string>());
    }
    if( Node["RTLFile"] ){
      C->SetRTLFile( Node["RTLFile"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLType") ){
      if( !C->SetRTLType( StrToCGRTL(Node["RTLType"].as<std::string>()) ) ){
        return false;
      }
    }
    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + Name );
        return false;
      }
      C->SetOverriddenNode(PNode);
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      C->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(C),
                    std::vector<CoreGenNode *>(Caches.begin(),Caches.end()) ) ){
      return false;
    }
    Caches.push_back(C);
  }

  return true;
}

bool CoreGenYaml::ReadCoreYaml(const YAML::Node& CoreNodes,
                               std::vector<CoreGenCore *> &Cores,
                               std::vector<CoreGenCache *> &Caches,
                               std::vector<CoreGenISA *> &ISAs,
                               std::vector<CoreGenRegClass *> &RegClasses,
                               std::vector<CoreGenExt *> &Exts){
  for( unsigned i=0; i<CoreNodes.size(); i++ ){
    const YAML::Node& Node = CoreNodes[i];
    if( !CheckValidNode(Node,"Core") ){
      PrintParserError(CoreNodes,
                       "Core",
                       "Cores");
      return false;
    }
    std::string Name = Node["Core"].as<std::string>();
    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }
    std::string Cache;
    if( Node["Cache"] ){
      Cache = Node["Cache"].as<std::string>();
    }
    std::string ISAName;
    if( Node["ISA"] ){
      ISAName = Node["ISA"].as<std::string>();
    }

    // handle the isa
    CoreGenISA *ISA = nullptr;
    if(ISAName.size() > 0){
      for( unsigned j=0; j<ISAs.size(); j++ ){
        if( ISAs[j]->GetName() == ISAName ){
          ISA = ISAs[j];
        }
      }
      if( ISA == nullptr ){
        return false;
      }
    }

    // create the object
    CoreGenCore *C = new CoreGenCore( Name, ISA, Errno );
    if( C == nullptr ){
      return false;
    }

    // handle the thread units
    unsigned ThreadUnits = 1;
    if( Node["ThreadUnits"] ){
      if( !CheckValidNode(Node,"ThreadUnits") ){
        return false;
      }

      try{
        ThreadUnits = Node["ThreadUnits"].as<unsigned>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing core ThreadUnits: "
                        + std::string(e.what()));
        return false;
      }

      if( !C->SetNumThreadUnits(ThreadUnits) ){
        return false;
      }
    }

    // handle the cache
    if( Cache.length() > 0 ){
      CoreGenCache *CA = nullptr;
      for( unsigned j=0; j<Caches.size(); j++ ){
        if( Caches[j]->GetName() == Cache ){
          CA = Caches[j];
        }
      }
      if( CA == nullptr ){
        return false;
      }
      C->InsertCache( CA );
    }

    // handle the register classes
    const YAML::Node& FNode = Node["RegisterClasses"];
    if( FNode ){
      for( unsigned k=0; k<FNode.size(); k++ ){
        const YAML::Node& LFNode = FNode[k];
        std::string RegClassName = LFNode["RegClass"].as<std::string>();
        CoreGenRegClass *RC = nullptr;
        for( unsigned a=0; a<RegClasses.size(); a++ ){
          if( RegClasses[a]->GetName() == RegClassName ){
            RC = RegClasses[a];
          }
        }
        if( RC == nullptr ){
          Errno->SetError(CGERR_ERROR, "Invalid RegisterClass for Core: "
                          + RegClassName + ":" + C->GetName() );
          return false;
        }
        C->InsertRegClass(RC);
      }
    }

    // Scheduler
    CGSched Sched = SMTUnk;
    if( Node["Scheduler"] ){
      if( !CheckValidNode(Node,"Scheduler") ){
        return false;
      }

      try{
        Sched = C->StrToCGSched(Node["Scheduler"].as<std::string>());
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing core Scheduler: "
                        + std::string(e.what()));
        return false;
      }

      if( !C->SetSched(Sched) )
        return false;
    }

    // Extensions
    const YAML::Node& ENode = Node["Extensions"];
    if( ENode ){
      for( unsigned k=0; k<ENode.size(); k++ ){
        const YAML::Node& LENode = ENode[k];
        std::string ExtName = LENode["Extension"].as<std::string>();
        CoreGenExt *EC = nullptr;
        for( unsigned a=0; a<Exts.size(); a++ ){
          if( Exts[a]->GetName() == ExtName ){
            EC = Exts[a];
          }
        }
        if( EC == nullptr ){
          Errno->SetError(CGERR_ERROR, "Invalid Extension for Core: "
                          + ExtName + ":" + C->GetName() );
          return false;
        }
        C->InsertExt( EC );
      }
    }

    if( Node["RTL"] ){
      C->SetRTL( Node["RTL"].as<std::string>());
    }
    if( Node["RTLFile"] ){
      C->SetRTLFile( Node["RTLFile"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLType") ){
      if( !C->SetRTLType( StrToCGRTL(Node["RTLType"].as<std::string>()) ) ){
        return false;
      }
    }
    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + Name );
        return false;
      }
      C->SetOverriddenNode(PNode);
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      C->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(C),
                    std::vector<CoreGenNode *>(Cores.begin(),Cores.end()) ) ){
      return false;
    }
    Cores.push_back(C);
  }
  return true;
}

bool CoreGenYaml::ReadSocYaml(const YAML::Node& SocNodes,
                              std::vector<CoreGenSoC *> &Socs,
                              std::vector<CoreGenCore *> &Cores){
  for( unsigned i=0; i<SocNodes.size(); i++ ){
    const YAML::Node& Node = SocNodes[i];
    if( !CheckValidNode(Node,"Soc") ){
      PrintParserError(SocNodes,
                       "Soc",
                       "Socs");
      return false;
    }
    std::string Name = Node["Soc"].as<std::string>();

    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }

    CoreGenSoC *S = new CoreGenSoC( Name, Errno );
    if( S == nullptr ){
      Errno->SetError(CGERR_ERROR, "Could not create CoreGenSoC object for "
                      + Name );
      return false;
    }

    // handle the cores
    const YAML::Node& FNode = Node["Cores"];
    if( FNode ){
      for( unsigned j=0; j<FNode.size(); j++ ){
        const YAML::Node& LFNode = FNode[j];
        std::string CoreName = LFNode["Core"].as<std::string>();
        CoreGenCore *C = nullptr;
        for( unsigned k=0; k<Cores.size(); k++ ){
          if( Cores[k]->GetName() == CoreName ){
            C = Cores[k];
          }
        }
        if( C == nullptr ){
          Errno->SetError(CGERR_ERROR, "No core defined for " + CoreName );
          return false;
        }
        S->InsertCore(C);
      }
    }

    if( Node["RTL"] ){
      S->SetRTL( Node["RTL"].as<std::string>());
    }
    if( Node["RTLFile"] ){
      S->SetRTLFile( Node["RTLFile"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLType") ){
      if( !S->SetRTLType( StrToCGRTL(Node["RTLType"].as<std::string>()) ) ){
        return false;
      }
    }
    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + Name );
        return false;
      }
      S->SetOverriddenNode(PNode);
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      S->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(S),
                    std::vector<CoreGenNode *>(Socs.begin(),Socs.end()) ) ){
      return false;
    }
    Socs.push_back(S);
  }
  return true;
}

bool CoreGenYaml::ReadSpadYaml(const YAML::Node& SpadNodes,
                               std::vector<CoreGenSpad *> &Spads){
  for( unsigned i=0; i<SpadNodes.size(); i++ ){
    const YAML::Node& Node = SpadNodes[i];
    if( !CheckValidNode(Node,"Scratchpad") ){
      PrintParserError(SpadNodes,
                       "Scratchpad",
                       "Scratchpads");
      return false;
    }
    std::string Name = Node["Scratchpad"].as<std::string>();

    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }

    unsigned MemSize = 0;
    unsigned RqstPorts = 0;
    unsigned RspPorts = 0;
    uint64_t StartAddr = 0x00ull;
    if( Node["MemSize"] ){
      try{
        MemSize = Node["MemSize"].as<unsigned>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing spad MemSize: "
                        + std::string(e.what()));
        return false;
      }
    }
    if( Node["RqstPorts"] ){
      try{
        RqstPorts = Node["RqstPorts"].as<unsigned>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing spad RqstPorts: "
                        + std::string(e.what()));
        return false;
      }
    }
    if( Node["RspPorts"] ){
      try{
        RspPorts = Node["RspPorts"].as<unsigned>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing spad RspPorts: "
                        + std::string(e.what()));
        return false;
      }
    }
    if( Node["StartAddr"] ){
      try{
        StartAddr = Node["StartAddr"].as<uint64_t>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing spad StartAddr: "
                        + std::string(e.what()));
        return false;
      }
    }

    CoreGenSpad *S = new CoreGenSpad(Name,Errno,MemSize,
                                     RqstPorts,RspPorts);

    if( Node["RTL"] ){
      S->SetRTL( Node["RTL"].as<std::string>());
    }
    if( Node["RTLFile"] ){
      S->SetRTLFile( Node["RTLFile"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLType") ){
      if( !S->SetRTLType( StrToCGRTL(Node["RTLType"].as<std::string>()) ) ){
        return false;
      }
    }
    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + Name );
        return false;
      }
      S->SetOverriddenNode(PNode);
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      S->SetNotes( Node["Notes"].as<std::string>() );
    }

    S->SetStartAddr( StartAddr );

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(S),
                    std::vector<CoreGenNode *>(Spads.begin(),Spads.end()) ) ){
      return false;
    }
    Spads.push_back(S);
  }
  return true;
}

bool CoreGenYaml::ReadMCtrlYaml(const YAML::Node& MCtrlNodes,
                                std::vector<CoreGenMCtrl *> &MCtrls){
  for( unsigned i=0; i<MCtrlNodes.size(); i++ ){
    const YAML::Node& Node = MCtrlNodes[i];
    if( !CheckValidNode(Node,"MemoryController") ){
      PrintParserError(MCtrlNodes,
                       "MemoryController",
                       "MemoryControllers");
      return false;
    }
    std::string Name = Node["MemoryController"].as<std::string>();

    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }

    unsigned Ports = 0;
    if( Node["Ports"] ){
      Ports = Node["Ports"].as<unsigned>();
    }

    std::string OrderStr;
    CGMemOrder Order = CGWeak;

    if( CheckValidNode(Node,"MemoryOrder") ){
      OrderStr = Node["MemoryOrder"].as<std::string>();
      if( OrderStr == "Weak" ){
        Order = CGWeak;
      }else if( OrderStr == "TSO" ){
        Order = CGTSO;
      }else if( OrderStr == "Strong" ){
        Order = CGStrong;
      }else{
        Errno->SetError(CGERR_ERROR, "Invalid Memory Order Type: " + OrderStr );
        return false;
      }
    }

    CoreGenMCtrl *M = new CoreGenMCtrl(Name,Errno,Ports,Order);

    if( Node["RTL"] ){
      M->SetRTL( Node["RTL"].as<std::string>());
    }
    if( Node["RTLFile"] ){
      M->SetRTLFile( Node["RTLFile"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLType") ){
      if( !M->SetRTLType( StrToCGRTL(Node["RTLType"].as<std::string>()) ) ){
        return false;
      }
    }
    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + Name );
        return false;
      }
      M->SetOverriddenNode(PNode);
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      M->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(M),
                    std::vector<CoreGenNode *>(MCtrls.begin(),MCtrls.end()) ) ){
      return false;
    }
    MCtrls.push_back(M);
  }
  return true;
}

bool CoreGenYaml::ReadVTPYaml( const YAML::Node& VTPNodes,
                               std::vector<CoreGenVTP *> &VTPs){
  for( unsigned i=0; i<VTPNodes.size(); i++ ){
    const YAML::Node& Node = VTPNodes[i];
    if( !CheckValidNode(Node,"VTP") ){
      PrintParserError(VTPNodes,
                       "VTP",
                       "VTPControllers");
      return false;
    }
    std::string Name = Node["VTP"].as<std::string>();

    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }

    CoreGenVTP *V = new CoreGenVTP(Name,Errno);
    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + Name );
        return false;
      }
      V->SetOverriddenNode(PNode);
    }

    // check for additional node notes
    if( CheckValidNode(Node,"Notes") ){
      V->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(V),
                    std::vector<CoreGenNode *>(VTPs.begin(),VTPs.end()) ) ){
      return false;
    }
    VTPs.push_back(V);
  }
  return true;
}

bool CoreGenYaml::ReadPluginYaml(const YAML::Node& PluginNodes,
                                 std::vector<CoreGenPlugin *> &Plugins){
  for( unsigned i=0; i<PluginNodes.size(); i++ ){
    const YAML::Node& Node = PluginNodes[i];
    if( !CheckValidNode(Node, "Plugin") ){
      PrintParserError(Node,
                       "Plugins",
                       "Plugin");
      return false;
    }
    std::string NodeName = Node["Plugin"].as<std::string>();
    if( !IsValidName(NodeName) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + NodeName );
      return false;
    }

    if( !CheckValidNode(Node,"PluginName") ){
      PrintParserError(Node,
                       "Plugins",
                       "PluginName");
      return false;
    }
    // plugin name
    std::string Name = Node["PluginName"].as<std::string>();

    // plugin version
    unsigned Major = 0;
    unsigned Minor = 0;
    unsigned Patch = 0;
    if( Node["MajorVersion"] ){
      try{
        Major = Node["MajorVersion"].as<unsigned>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing plugin MajorVersion: "
                        + std::string(e.what()));
        return false;
      }
    }
    if( Node["MinorVersion"] ){
      try{
        Minor = Node["MinorVersion"].as<unsigned>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing plugin MinorVersion: "
                        + std::string(e.what()));
        return false;
      }
    }
    if( Node["PatchVersion"] ){
      try{
        Patch = Node["PatchVersion"].as<unsigned>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing plugin PatchVersion: "
                        + std::string(e.what()));
        return false;
      }
    }

    std::string Notes;
    if( CheckValidNode(Node,"Notes") ){
      Notes = Node["Notes"].as<std::string>();
    }

    // if the plugin loads, continue parsing
    // otherwise, throw an error
    if( !PluginMgr ){
      // plugin manager failed to load
      Errno->SetError(CGERR_ERROR, "Plugin manager is not laoded");
      return false;
    }

    // check to see if the plugin is loaded
    // if not, load it
    // otherwise, clone it
    if( !PluginMgr->GetPlugin(NodeName,Major,Minor,Patch) ){
      #if (COREGEN_PLATFORM == Darwin)
      std::string PluginLib = NodeName + ".dylib";
      #elif (COREGEN_PLATFORM == Linux)
      std::string PluginLib = Name + ".so";
      #endif
      std::string PluginPath = Env->GetPluginRoot() +
                             "/" + NodeName + "/" +
                             std::to_string(Major)+"."+
                             std::to_string(Minor)+"."+
                             std::to_string(Patch) + "/" +
                             PluginLib;
      if( !PluginMgr->LoadPlugin(PluginPath) ){
        // failed to load the plugin
        return false;
      }
    }

    // plugin should be loaded now
    // insert cloned copy
    CoreGenPlugin *NewPlugin = PluginMgr->GetPlugin(NodeName,
                                                    Major,
                                                    Minor,
                                                    Patch)->ClonePlugin(Name);
    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(NewPlugin),
                    std::vector<CoreGenNode *>(Plugins.begin(),Plugins.end()) ) ){
      return false;
    }
    Plugins.push_back(NewPlugin);

    if( Notes.length() > 0 ){
      NewPlugin->SetNotes(Notes);
    }

    // retrieve the features
    if( Node["Features"] ){
      const YAML::Node& FNode = Node["Features"];
      for( unsigned j=0; j<FNode.size(); j++ ){
        const YAML::Node& FNodeJ = FNode[j];
        if( !CheckValidNode(FNodeJ,"FeatureName") ){
          PrintParserError(FNodeJ,
                           "Features",
                           "FeatureName" );
          return false;
        }
        //QUESTION: account for non uniqueness between plugins?
        std::string FeatureName = FNodeJ["FeatureName"].as<std::string>();

        // feature type and feature value
        if( FNodeJ["FeatureType"] ){
          CGFeatureVal FVal;
          std::string FTypeStr = FNodeJ["FeatureType"].as<std::string>();

          if( !CheckValidNode(FNodeJ,"FeatureValue") ){
            PrintParserError(FNodeJ,
                             "Features",
                             "FeatureValue" );
            return false;
          }
          std::string FValStr = FNodeJ["FeatureValue"].as<std::string>();
          std::string::size_type sz;

          // convert o CGFeatureType and CGFeatureVal
          if( FTypeStr == "Unsigned" ){
            FVal.UnsignedData = (unsigned)(std::stoi(FValStr,&sz));
          }
          if( FTypeStr == "Uint32t" ){
            FVal.Uint32tData = (uint32_t)(std::stoi(FValStr,&sz));
          }
          if( FTypeStr == "Int32t" ){
            FVal.Int32tData = (int32_t)(std::stoi(FValStr,&sz));
          }
          if( FTypeStr == "Uint64t" ){
            FVal.Uint64tData = (uint64_t)(std::stoul(FValStr,nullptr,0));
          }
          if( FTypeStr == "Int64t" ){
            FVal.Uint64tData = (int64_t)(std::stol(FValStr,nullptr,0));
          }
          if( FTypeStr == "Float" ){
            FVal.FloatData = (float)(std::stof(FValStr,&sz));
          }
          if( FTypeStr == "Double" ){
            FVal.DoubleData = (float)(std::stof(FValStr,&sz));
          }
          if( FTypeStr == "String" ){
            FVal.StringData = FValStr;
          }
          if( FTypeStr == "Bool" ){
            FVal.BoolData = std::stoi(FValStr,&sz);
          }
          if( FTypeStr == "Unknown" ){
            // default to unsigned integer
            FVal.UnsignedData = (unsigned)(std::stoi(FValStr,&sz));
          }
          NewPlugin->SetFeatureValue(FeatureName, FVal);
        }
      }
    }

    //-- Registers
    if( Node["Registers"] ){
      const YAML::Node& RegNodes = Node["Registers"];
      if( !ReadRegisterYaml(RegNodes,NewPlugin->GetRegVect()) ){
        return false;
      }
    }

    //-- Register Classes
    if( Node["RegClasses"] ){
      const YAML::Node& RegClassNodes = Node["RegClasses"];
      if( !ReadRegisterClassYaml(RegClassNodes,
                               NewPlugin->GetRegClassVect(),
                               NewPlugin->GetRegVect()) ){
        return false;
      }
    }

    //-- ISAs
    if( Node["ISAs"] ){
      const YAML::Node& ISANodes = Node["ISAs"];
      if( !ReadISAYaml(ISANodes,NewPlugin->GetISAVect()) ){
        return false;
      }
    }

    //-- Inst Formats
    if( Node["InstFormats"] ){
      const YAML::Node& InstFormatNodes = Node["InstFormats"];
      if( !ReadInstFormatYaml(InstFormatNodes,
                            NewPlugin->GetInstFormatVect(),
                            NewPlugin->GetISAVect(),
                            NewPlugin->GetRegClassVect()) ){
        return false;
      }
    }

    //-- Insts
    if( Node["Insts"] ){
      const YAML::Node& InstNodes = Node["Insts"];
      if( !ReadInstYaml(InstNodes,
                      NewPlugin->GetInstVect(),
                      NewPlugin->GetInstFormatVect(),
                      NewPlugin->GetISAVect()) ){
        return false;
      }
    }

    //-- PseudoInsts
    if( Node["PseudoInsts"] ){
      const YAML::Node& PInstNodes = Node["PseudoInsts"];
      if( !ReadPseudoInstYaml(PInstNodes,
                            NewPlugin->GetPseudoInstVect(),
                            NewPlugin->GetInstVect(),
                            NewPlugin->GetISAVect()) ){
        return false;
      }
    }

    //-- Caches
    if( Node["Caches"] ){
      const YAML::Node& CacheNodes = Node["Caches"];
      if( !ReadCacheYaml(CacheNodes,NewPlugin->GetCacheVect()) ){
        return false;
      }
    }

    //-- DataPaths
    if( Node["DataPaths"] ){
      const YAML::Node& DataPathNodes = Node["DataPaths"];
      if( !ReadDataPathYaml(DataPathNodes,NewPlugin->GetDataPathVect()) ){
        return false;
      }
    }

    //-- Cores
    if( Node["Cores"] ){
      const YAML::Node& CoreNodes = Node["Cores"];
      if( !ReadCoreYaml(CoreNodes,
                      NewPlugin->GetCoreVect(),
                      NewPlugin->GetCacheVect(),
                      NewPlugin->GetISAVect(),
                      NewPlugin->GetRegClassVect(),
                      NewPlugin->GetExtVect()) ){
        return false;
      }
    }

    //-- Spads
    if( Node["Scratchpads"] ){
      const YAML::Node& SpadNodes = Node["Scratchpads"];
      if( !ReadSpadYaml(SpadNodes,NewPlugin->GetSpadVect()) ){
        return false;
      }
    }

    //-- Memory Controllers
    if( Node["MemoryControllers"] ){
      const YAML::Node& MCtrlNodes = Node["MemoryControllers"];
      if( !ReadMCtrlYaml(MCtrlNodes,NewPlugin->GetMCtrlVect()) ){
        return false;
      }
    }

    //-- Exts
    if( Node["Extensions"] ){
      const YAML::Node& ExtNodes = Node["Extensions"];
      if( !ReadExtYaml(ExtNodes,NewPlugin->GetExtVect()) ){
        return false;
      }
    }

    //-- VtP nodes
    if( Node["VTPControllers"] ){
      const YAML::Node& VTPNodes = Node["VTPControllers"];
      if( !ReadVTPYaml(VTPNodes,NewPlugin->GetVTPVect()) ){
        return false;
      }
    }

    //-- Socs
    if( Node["Socs"] ){
      const YAML::Node& SocNodes = Node["Socs"];
      if( !ReadSocYaml(SocNodes,NewPlugin->GetSocVect(),NewPlugin->GetCoreVect()) ){
        return false;
      }
    }

    //-- Comms
    if( Node["Comms"] ){
      const YAML::Node& CommNodes = Node["Comms"];
      if( !ReadCommYaml(CommNodes,
                    NewPlugin->GetSocVect(),
                    NewPlugin->GetCoreVect(),
                    NewPlugin->GetCacheVect(),
                    NewPlugin->GetISAVect(),
                    NewPlugin->GetInstFormatVect(),
                    NewPlugin->GetInstVect(),
                    NewPlugin->GetPseudoInstVect(),
                    NewPlugin->GetRegClassVect(),
                    NewPlugin->GetRegVect(),
                    NewPlugin->GetCommVect(),
                    NewPlugin->GetSpadVect(),
                    NewPlugin->GetMCtrlVect(),
                    NewPlugin->GetVTPVect(),
                    NewPlugin->GetExtVect(),
                    Plugins) ){
        return false;
      }
    }
  }
  return true;
}

bool CoreGenYaml::ReadExtYaml(const YAML::Node& ExtNodes,
                              std::vector<CoreGenExt *> &Exts){
  for( unsigned i=0; i<ExtNodes.size(); i++ ){
    const YAML::Node& Node = ExtNodes[i];
    if( !CheckValidNode(Node,"Extension") ){
      PrintParserError(ExtNodes,
                       "Extension",
                       "Extensions");
      return false;
    }
    std::string Name = Node["Extension"].as<std::string>();

    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }

    std::string Type = Node["Type"].as<std::string>();
    std::transform(Type.begin(),Type.end(),Type.begin(),::tolower);
    CGExtType FullType;
    if( Type == "template" ){
      FullType = CGExtTemplate;
    }else if( Type == "module" ){
      FullType = CGExtModule;
    }else if( Type == "comm" ){
      FullType = CGExtComm;
    }else if( Type == "unknown" ){
      FullType = CGExtUnk;
    }else{
      // unknown type
      Errno->SetError(CGERR_ERROR,"Error in recognizing extension node type : " + Type );
      return false;
    }

    // create a new extension node
    CoreGenExt *E = new CoreGenExt( Name, Errno );
    if( !E->SetType(FullType) ){
      Errno->SetError(CGERR_ERROR,"Failed to set the type for Extension=" +
                                  Name + " to Type=" + Type);
      return false;
    }

    if( CheckValidNode(Node,"Notes") ){
      E->SetNotes( Node["Notes"].as<std::string>() );
    }

    //-- Registers
    if( Node["Registers"] ){
      const YAML::Node& RegNodes = Node["Registers"];
      if( !ReadRegisterYaml(RegNodes,E->GetRegVect()) ){
        Errno->SetError(CGERR_ERROR, "Error in parsing YAML Node for registers" );
        return false;
      }
    }


    //-- Register Classes
    if( Node["RegClasses"] ){
      const YAML::Node& RegClassNodes = Node["RegClasses"];
      if( !ReadRegisterClassYaml(RegClassNodes,
                               E->GetRegClassVect(),
                               E->GetRegVect()) ){
        return false;
      }
    }

    //-- ISAs
    if( Node["ISAs"] ){
      const YAML::Node& ISANodes = Node["ISAs"];
      if( !ReadISAYaml(ISANodes,E->GetISAVect()) ){
        return false;
      }
    }

    //-- Inst Formats
    if( Node["InstFormats"] ){
      const YAML::Node& InstFormatNodes = Node["InstFormats"];
      if( !ReadInstFormatYaml(InstFormatNodes,
                            E->GetInstFormatVect(),
                            E->GetISAVect(),
                            E->GetRegClassVect()) ){
        return false;
      }
    }

    //-- Insts
    if( Node["Insts"] ){
      const YAML::Node& InstNodes = Node["Insts"];
      if( !ReadInstYaml(InstNodes,
                      E->GetInstVect(),
                      E->GetInstFormatVect(),
                      E->GetISAVect()) ){
        return false;
      }
    }

    //-- PseudoInsts
    if( Node["PseudoInsts"] ){
      const YAML::Node& PInstNodes = Node["PseudoInsts"];
      if( !ReadPseudoInstYaml(PInstNodes,
                            E->GetPseudoInstVect(),
                            E->GetInstVect(),
                            E->GetISAVect()) ){
        return false;
      }
    }

    //-- Caches
    if( Node["Caches"] ){
      const YAML::Node& CacheNodes = Node["Caches"];
      if( !ReadCacheYaml(CacheNodes,E->GetCacheVect()) ){
        return false;
      }
    }

    //-- Cores
    if( Node["Cores"] ){
      const YAML::Node& CoreNodes = Node["Cores"];
      if( !ReadCoreYaml(CoreNodes,
                      E->GetCoreVect(),
                      E->GetCacheVect(),
                      E->GetISAVect(),
                      E->GetRegClassVect(),
                      E->GetExtVect()) ){
        return false;
      }
    }

    //-- Spads
    if( Node["Scratchpads"] ){
      const YAML::Node& SpadNodes = Node["Scratchpads"];
      if( !ReadSpadYaml(SpadNodes,E->GetSpadVect()) ){
        return false;
      }
    }

    //-- Memory Controllers
    if( Node["MemoryControllers"] ){
      const YAML::Node& MCtrlNodes = Node["MemoryControllers"];
      if( !ReadMCtrlYaml(MCtrlNodes,E->GetMCtrlVect()) ){
        return false;
      }
    }

    //-- VTP Controllers
    if( Node["VTPControllers"] ){
      const YAML::Node& VTPNodes = Node["VTPControllers"];
      if( !ReadVTPYaml(VTPNodes,E->GetVTPVect()) ){
        return false;
      }
    }

    //-- Exts
    if( Node["Extensions"] ){
      const YAML::Node& ExtNodes = Node["Extensions"];
      if( !ReadExtYaml(ExtNodes,E->GetExtVect()) ){
        return false;
      }
    }

    //-- Comms
    if( Node["Comms"] ){
      const YAML::Node& CommNodes = Node["Comms"];
      // we define two blank vectors of nodes
      // because extensions do not have plugins, SoCs
      std::vector<CoreGenPlugin *> Plugins;   // temporary fix
      std::vector<CoreGenSoC *> TSoCVect;     // temporary fix
      if( !ReadCommYaml(CommNodes,
                    TSoCVect,
                    E->GetCoreVect(),
                    E->GetCacheVect(),
                    E->GetISAVect(),
                    E->GetInstFormatVect(),
                    E->GetInstVect(),
                    E->GetPseudoInstVect(),
                    E->GetRegClassVect(),
                    E->GetRegVect(),
                    E->GetCommVect(),
                    E->GetSpadVect(),
                    E->GetMCtrlVect(),
                    E->GetVTPVect(),
                    E->GetExtVect(),
                    Plugins) ){
        return false;
      }
    }

    //-- Read Node RTL
    if( Node["RTL"] ){
      E->SetRTL( Node["RTL"].as<std::string>());
    }
    if( Node["RTLFile"] ){
      E->SetRTLFile( Node["RTLFile"].as<std::string>());
    }
    if( CheckValidNode(Node,"RTLType") ){
      if( !E->SetRTLType( StrToCGRTL(Node["RTLType"].as<std::string>()) ) ){
        return false;
      }
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(E),
                    std::vector<CoreGenNode *>(Exts.begin(),Exts.end()) ) ){
      return false;
    }
    E->UpdateChildNodes();
    Exts.push_back(E);

  }// end ExtNodes

  return true;
}

bool CoreGenYaml::ReadCommYaml( const YAML::Node& CommNodes,
                     std::vector<CoreGenSoC *>  &Socs,
                     std::vector<CoreGenCore *>  &Cores,
                     std::vector<CoreGenCache *>  &Caches,
                     std::vector<CoreGenISA *>  &ISAs,
                     std::vector<CoreGenInstFormat *>  &Formats,
                     std::vector<CoreGenInst *>  &Insts,
                     std::vector<CoreGenPseudoInst *>  &PInsts,
                     std::vector<CoreGenRegClass *>  &RegClasses,
                     std::vector<CoreGenReg *>  &Regs,
                     std::vector<CoreGenComm *> &Comms,
                     std::vector<CoreGenSpad *> &Spads,
                     std::vector<CoreGenMCtrl *> &MCtrls,
                     std::vector<CoreGenVTP *> &VTPs,
                     std::vector<CoreGenExt *>  &Exts,
                     std::vector<CoreGenPlugin *> &Plugins ){

  for( unsigned i=0; i<CommNodes.size(); i++ ){
    const YAML::Node& Node = CommNodes[i];
    if( !CheckValidNode(Node,"Comm") ){
      PrintParserError(CommNodes,
                       "Comm",
                       "Comms");
      return false;
    }
    std::string Name = Node["Comm"].as<std::string>();

    if( !IsValidName(Name) ){
      Errno->SetError(CGERR_ERROR, "Invalid IR Node Name: " + Name );
      return false;
    }

    // type
    std::string Type = Node["Type"].as<std::string>();
    CGCommType FullType = CGCommUnk;
    if( Type == "P2P" ){
      FullType = CGCommP2P;
    }else if( Type == "Bus" ){
      FullType = CGCommBus;
    }else if( Type == "NOC" ){
      FullType = CGCommNoc;
    }

    // width
    unsigned Width = 0;
    if( Node["Width"] ){
      try{
        Width = Node["Width"].as<unsigned>();
      }catch(YAML::BadConversion& e){
        Errno->SetError(CGERR_ERROR, "Error in parsing comm Width: "
                        + std::string(e.what()));
        return false;
      }
    }

    CoreGenComm *Comm = new CoreGenComm(Name,FullType,Width,Errno);
    if( Comm == nullptr ){
      Errno->SetError(CGERR_ERROR,
                      "Error creating new communication channel for "
                      + Name + " at line " + std::to_string(GetLineNum(Node)));
      return false;
    }

    // endpoints
    if( Node["Endpoints"] ){
      const YAML::Node& ENode = Node["Endpoints"];
      if( ENode.size() == 0 ){
        Errno->SetError(CGERR_WARN,
                        "Warning: No endpoints defined for communication channel " +
                        Name + " at line " + std::to_string(GetLineNum(ENode)) );
        // We don't error out, but record a warning
      }

      // walk all the endpoints
      for( unsigned j=0; j<ENode.size(); j++ ){
        std::string ENodeName = ENode[j].as<std::string>();
        CoreGenNode *EndPoint = NameToNode(ENodeName,
                                           Socs,
                                           Cores,
                                           Caches,
                                           ISAs,
                                           Formats,
                                           Insts,
                                           PInsts,
                                           RegClasses,
                                           Regs,
                                           Comms,
                                           Spads,
                                           MCtrls,
                                           VTPs,
                                           Exts,
                                           Plugins);
        if( !EndPoint ){
          Errno->SetError(CGERR_ERROR,
                          "Error: could not convert endpoint name " +
                          ENodeName + " to node pointer" );
          return false;
        }
        if( !Comm->InsertEndpoint( EndPoint ) ){
          Errno->SetError(CGERR_ERROR,
                          "Error: could not insert endpoint " +
                          EndPoint->GetName() + " into communication channel " +
                          Name );
          return false;
        }
      }// endpoint loop
    }// endpoint handler

    // check for plugin override
    if( CheckValidNode(Node,"Override") ){
      std::string ONode = Node["Override"].as<std::string>();
      CoreGenNode *PNode = CheckOverridePlugin(ONode);
      if( PNode == nullptr ){
        Errno->SetError(CGERR_ERROR, "Invalid override node " + ONode + " for node=" + Name );
        return false;
      }
      Comm->SetOverriddenNode(PNode);
    }

    if( CheckValidNode(Node,"Notes") ){
      Comm->SetNotes( Node["Notes"].as<std::string>() );
    }

    if( IsDuplicate(Node,
                    static_cast<CoreGenNode *>(Comm),
                    std::vector<CoreGenNode *>(Comms.begin(),Comms.end()) ) ){
      return false;
    }
    Comms.push_back(Comm);
  }

  return true;
}

bool CoreGenYaml::ReadYaml(  std::vector<CoreGenSoC *>  &Socs,
                             std::vector<CoreGenCore *>  &Cores,
                             std::vector<CoreGenCache *>  &Caches,
                             std::vector<CoreGenISA *>  &ISAs,
                             std::vector<CoreGenInstFormat *>  &Formats,
                             std::vector<CoreGenInst *>  &Insts,
                             std::vector<CoreGenPseudoInst *>  &PInsts,
                             std::vector<CoreGenRegClass *>  &RegClasses,
                             std::vector<CoreGenReg *>  &Regs,
                             std::vector<CoreGenComm *> &Comms,
                             std::vector<CoreGenSpad *> &Spads,
                             std::vector<CoreGenMCtrl *> &MCtrls,
                             std::vector<CoreGenVTP *> &VTPs,
                             std::vector<CoreGenVLIWStage *> &Stages,
                             std::vector<CoreGenExt *>  &Exts,
                             std::vector<CoreGenDataPath *> &DataPaths,
                             std::vector<CoreGenPlugin *> &Plugins ){
  // sanity check
  if( FileName.length() == 0 ){
    Errno->SetError(CGERR_ERROR, "YAML input file name is null" );
    return false;
  }

  PluginPtr = &Plugins;

  // load the config file
  YAML::Node IR;
  std::ifstream fin(FileName.c_str());
  try{
    IR = YAML::Load(fin);
  }catch(YAML::ParserException& e){
    Errno->SetError(CGERR_ERROR, "Error in parsing Yaml IR file: "
                    + std::string(e.what()));
    return false;
  }
  if( IR.IsNull() ){
    Errno->SetError(CGERR_ERROR, "Error in resolving Yaml IR file" );
    return false;
  }

  //-- Project
  const YAML::Node& ProjNodes = IR["ProjectInfo"];
  if( CheckValidNode(IR,"ProjectInfo") ){
    if( !ReadProjYaml(ProjNodes) ){
      fin.close();
      return false;
    }
  }

  //-- Plugins
  const YAML::Node& PluginNodes = IR["Plugins"];
  if( !ReadPluginYaml(PluginNodes,Plugins) ){
    fin.close();
    return false;
  }

  //-- Registers
  const YAML::Node& RegNodes = IR["Registers"];
  if( !ReadRegisterYaml(RegNodes,Regs) ){
    fin.close();
    return false;
  }

  //-- Register Classes
  const YAML::Node& RegClassNodes = IR["RegClasses"];
  if( !ReadRegisterClassYaml(RegClassNodes,RegClasses,Regs) ){
    fin.close();
    return false;
  }

  //-- ISAs
  const YAML::Node& ISANodes = IR["ISAs"];
  if( !ReadISAYaml(ISANodes,ISAs) ){
    fin.close();
    return false;
  }

  //-- Inst Formats
  const YAML::Node& InstFormatNodes = IR["InstFormats"];
  if( !ReadInstFormatYaml(InstFormatNodes,Formats,ISAs,RegClasses) ){
    fin.close();
    return false;
  }

  //-- Read VLIW Stages
  const YAML::Node& VLIWStageNodes = IR["VLIWStages"];
  if( !ReadVLIWStageYaml(VLIWStageNodes,
                         Stages,
                         Formats) ){
    fin.close();
    return false;
  }

  //-- Insts
  const YAML::Node& InstNodes = IR["Insts"];
  if( !ReadInstYaml(InstNodes,Insts,Formats,ISAs ) ){
    fin.close();
    return false;
  }

  //-- PseudoInsts
  const YAML::Node& PInstNodes = IR["PseudoInsts"];
  if( !ReadPseudoInstYaml(PInstNodes,PInsts,Insts,ISAs) ){
    fin.close();
    return false;
  }

  //-- Caches
  const YAML::Node& CacheNodes = IR["Caches"];
  if( !ReadCacheYaml(CacheNodes,Caches) ){
    fin.close();
    return false;
  }

  //-- DataPaths
  const YAML::Node& DatapathNodes = IR["DataPaths"];
  if( !ReadDataPathYaml(DatapathNodes,DataPaths) ){
    fin.close();
    return false;
  }

  //-- Exts
  const YAML::Node& ExtNodes = IR["Extensions"];
  if( !ReadExtYaml(ExtNodes,Exts) ){
    fin.close();
    return false;
  }

  //-- Cores
  const YAML::Node& CoreNodes = IR["Cores"];
  if( !ReadCoreYaml(CoreNodes,Cores,Caches,ISAs,RegClasses,Exts) ){
    fin.close();
    return false;
  }

  //-- Socs
  const YAML::Node& SocNodes = IR["Socs"];
  if( !ReadSocYaml(SocNodes,Socs,Cores) ){
    fin.close();
    return false;
  }

  //-- Spads
  const YAML::Node& SpadNodes = IR["Scratchpads"];
  if( !ReadSpadYaml(SpadNodes,Spads) ){
    fin.close();
    return false;
  }

  //-- Memory Controllers
  const YAML::Node& MCtrlNodes = IR["MemoryControllers"];
  if( !ReadMCtrlYaml(MCtrlNodes,MCtrls) ){
    fin.close();
    return false;
  }

  // -- Virtual to Physical Units
  const YAML::Node& VTPNodes = IR["VTPControllers"];
  if( !ReadVTPYaml(VTPNodes,VTPs) ){
    fin.close();
    return false;
  }

  //-- Comms: read these last in order to resolve all the names
  const YAML::Node& CommNodes = IR["Comms"];
  if( !ReadCommYaml(CommNodes,
                    Socs,
                    Cores,
                    Caches,
                    ISAs,
                    Formats,
                    Insts,
                    PInsts,
                    RegClasses,
                    Regs,
                    Comms,
                    Spads,
                    MCtrls,
                    VTPs,
                    Exts,
                    Plugins) ){
    fin.close();
    return false;
  }

  fin.close();

  return true;
}

CoreGenNode *CoreGenYaml::CheckOverridePlugin(std::string Name){
  for( unsigned i=0; i<(*PluginPtr).size(); i++ ){
    if( (*PluginPtr)[i]->GetPluginName() == Name ){
      return static_cast<CoreGenNode *>((*PluginPtr)[i]);
    }
  }
  return nullptr;
}

// EOF
