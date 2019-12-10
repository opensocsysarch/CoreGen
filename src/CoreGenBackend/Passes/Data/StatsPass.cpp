//
// _StatsPass_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Data/StatsPass.h"

StatsPass::StatsPass(std::ostream *O,
                     CoreGenDAG *D,
                     CoreGenErrno *E)
  : CoreGenPass(CGPassData,3,"StatsPass",
                "Prints statistics for all DAG nodes",false,O,D,E),
    SocNode(0),CoreNode(0),InstFormatNode(0),InstNode(0),
    PseudoInstNode(0),RegClassNode(0),RegNode(0),
    ISANode(0),CacheNode(0),EncodingNode(0),
    ExtNode(0),CommNode(0),SpadNode(0),MCtrlNode(0),
    VTPNode(0),PluginNode(0){
}

StatsPass::~StatsPass(){
}

void StatsPass::RecordNodeType( CoreGenNode *N ){
  switch( N->GetType() ){
  case CGSoc:
    SocNode++;
    break;
  case CGCore:
    CoreNode++;
    break;
  case CGInstF:
    InstFormatNode++;
    break;
  case CGInst:
    InstNode++;
    break;
  case CGPInst:
    PseudoInstNode++;
    break;
  case CGRegC:
    RegClassNode++;
    break;
  case CGReg:
    RegNode++;
    break;
  case CGISA:
    ISANode++;
    break;
  case CGCache:
    CacheNode++;
    break;
  case CGEnc:
    EncodingNode++;
    break;
  case CGExt:
    ExtNode++;
    break;
  case CGComm:
    CommNode++;
    break;
  case CGSpad:
    SpadNode++;
    break;
  case CGMCtrl:
    MCtrlNode++;
    break;
  case CGPlugin:
    PluginNode++;
    break;
  case CGVTP:
    VTPNode++;
    break;
  case CGTop:
  default:
    // ignore these
    break;
  }
}

bool StatsPass::Execute(){
  // Get the correct DAG Level 3
  CoreGenDAG *D3 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D3 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  // walk all the nodes and print their stats
  WriteMsg( "==> IR Node\t:\t\tNum Children" );
  for( unsigned i=0; i<D3->GetDimSize(); i++ ){
    CoreGenNode *N = D3->FindNodeByIndex(i);
    WriteMsg( "\t" + N->GetName() + CGPrintDotStr(N->GetName().length(),30) +
              std::to_string(N->GetNumChild()) );
    RecordNodeType(N);
  }

  // print a summary of the nodes
  WriteMsg("==> Node Summary" );
  WriteMsg("==> SoC Nodes:\t\t" + std::to_string(SocNode));
  WriteMsg("==> Core Nodes:\t\t" + std::to_string(CoreNode));
  WriteMsg("==> Inst Format Nodes:\t" + std::to_string(InstFormatNode));
  WriteMsg("==> Inst Nodes:\t\t" + std::to_string(InstNode));
  WriteMsg("==> Pseudo Inst Nodes:\t" + std::to_string(PseudoInstNode));
  WriteMsg("==> RegClass Nodes:\t\t" + std::to_string(RegClassNode));
  WriteMsg("==> Register Nodes:\t\t" + std::to_string(RegNode));
  WriteMsg("==> ISA Nodes:\t\t" + std::to_string(ISANode));
  WriteMsg("==> Cache Nodes:\t\t" + std::to_string(CacheNode));
  WriteMsg("==> Encoding Nodes:\t\t" + std::to_string(EncodingNode));
  WriteMsg("==> Extension Nodes:\t" + std::to_string(ExtNode));
  WriteMsg("==> Comm Nodes:\t\t" + std::to_string(CommNode));
  WriteMsg("==> Spad Nodes:\t\t" + std::to_string(SpadNode));
  WriteMsg("==> MCtrl Nodes:\t\t" + std::to_string(MCtrlNode));
  WriteMsg("==> VTP Nodes:\t\t" + std::to_string(VTPNode));
  WriteMsg("==> Plugin Nodes:\t\t" + std::to_string(PluginNode));

  unsigned int Total = SocNode+CoreNode+InstNode+PseudoInstNode+
                       RegClassNode+RegNode+ISANode+CacheNode+
                       EncodingNode+ExtNode+ExtNode+SpadNode+
                       MCtrlNode+VTPNode+PluginNode;
  WriteMsg("==> TOTAL NODES:\t\t" + std::to_string(Total));


  return true;
}

// EOF
