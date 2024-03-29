//
// _DHDTGraph_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/DHDT/DHDTGraph.h"

std::vector<SCIntrin *> DHDTGraph::Intrins = {
  static_cast<SCIntrin *>(new SCMax()),
  static_cast<SCIntrin *>(new SCMin()),
  static_cast<SCIntrin *>(new SCLoad()),
  static_cast<SCIntrin *>(new SCStore()),
  static_cast<SCIntrin *>(new SCLoadElem()),
  static_cast<SCIntrin *>(new SCStoreElem()),
  static_cast<SCIntrin *>(new SCNot()),
  static_cast<SCIntrin *>(new SCNeg()),
  static_cast<SCIntrin *>(new SCReverse()),
  static_cast<SCIntrin *>(new SCPopcount()),
  static_cast<SCIntrin *>(new SCClz()),
  static_cast<SCIntrin *>(new SCCtz()),
  static_cast<SCIntrin *>(new SCSext()),
  static_cast<SCIntrin *>(new SCZext()),
  static_cast<SCIntrin *>(new SCRotateL()),
  static_cast<SCIntrin *>(new SCRotateR()),
  static_cast<SCIntrin *>(new SCMaj()),
  static_cast<SCIntrin *>(new SCDoz()),
  static_cast<SCIntrin *>(new SCCompress()),
  static_cast<SCIntrin *>(new SCCompressM()),
  static_cast<SCIntrin *>(new SCInsertS()),
  static_cast<SCIntrin *>(new SCInsertZ()),
  static_cast<SCIntrin *>(new SCExtractS()),
  static_cast<SCIntrin *>(new SCExtractZ()),
  static_cast<SCIntrin *>(new SCMerge()),
  static_cast<SCIntrin *>(new SCConcat()),
  static_cast<SCIntrin *>(new SCLss()),
  static_cast<SCIntrin *>(new SCFence()),
  static_cast<SCIntrin *>(new SCBsel()),
  static_cast<SCIntrin *>(new SCNop()),
  static_cast<SCIntrin *>(new SCIn()),
  static_cast<SCIntrin *>(new SCOut())
};

DHDTGraph::DHDTGraph(DHDTConfig &Config, bool Verbose)
  : Verbose(Verbose), Config(Config) {}

DHDTGraph::~DHDTGraph(){
  for( unsigned i=0; i<Nodes.size(); i++ ){
    delete Nodes[i];
  }
  for( unsigned i=0; i<Links.size(); i++ ){
    delete Links[i];
  }
  Nodes.clear();
  Links.clear();
  Mod.reset();
}

bool DHDTGraph::ReadLLVMIR(std::string IR){
  if( IR.length() == 0 ){
    return false;
  }

  Mod = parseIRFile(IR.c_str(), Err, Context);
  if( !Mod ){
    std::cout << "Error parsing LLVM IR File: " << IR << std::endl;
    Err.print(IR.c_str(), errs());
    return false;
  }

  return true;
}

bool DHDTGraph::ReadCoreGenIR(std::string IR){
  if( !CG.ReadIR(IR) ){
    std::cout << "Error parsing CoreGen IR File: " << IR << std::endl;
    return false;
  }
  return true;
}

bool DHDTGraph::ReadIR(std::string LLVMIR, std::string CoreGenIR){
  if( LLVMIR.length() == 0 ){
    std::cout << "Error reading LLVM IR file: file is null : "
              << LLVMIR << std::endl;
    return false;
  }else if( CoreGenIR.length() == 0 ){
    std::cout << "Error reading CoreGen IR file: file is null : "
              << CoreGenIR << std::endl;
    return false;
  }

  if( !ReadLLVMIR(LLVMIR) ){
    return false;
  }

  if( !ReadCoreGenIR(CoreGenIR) ){
    return false;
  }

  return true;
}

DHDTNode *DHDTGraph::GetFuncNode(std::string N){
  if( N.length() == 0 ){
    return nullptr;
  }

  auto it = NodeMap.find(N);
  if( it != NodeMap.end() )
    return it->second;
  return nullptr;
}

DHDTNode *DHDTGraph::GetLatestNode(){
  if( Nodes.size() == 0 )
    return nullptr;

  return Nodes[Nodes.size()-1];
}

DHDTLink *DHDTGraph::GetLatestLink(){
  if( Links.size() == 0 )
    return nullptr;

  return Links[Links.size()-1];
}

bool DHDTGraph::IsTopVLIWStage(Function &Func){
  for( auto &BB : Func.getBasicBlockList() ){
    for( auto &Inst : BB.getInstList() ){
      if( auto *CInst = dyn_cast<CallInst>(&Inst) ){
        if( CInst->getCalledFunction()->getName().str() == "IN" ){
          // if we encounter an "IN" intrinsic, then this
          // stage has inputs from a previous stage
          return false;
        }
      }
    }
  }
  return true;
}

DHDTConfig::ConfigEntry DHDTGraph::LLVMInstToConfigType(unsigned Inst){
  switch(Inst){
  case Instruction::Add:
    return Config.GetEntryByType(DHDTConfig::ALUADD);
    break;
  case Instruction::FAdd:
    return Config.GetEntryByType(DHDTConfig::ALUFADD);
    break;
  case Instruction::Sub:
    return Config.GetEntryByType(DHDTConfig::ALUSUB);
    break;
  case Instruction::FSub:
    return Config.GetEntryByType(DHDTConfig::ALUFSUB);
    break;
  case Instruction::Mul:
    return Config.GetEntryByType(DHDTConfig::ALUMUL);
    break;
  case Instruction::FMul:
    return Config.GetEntryByType(DHDTConfig::ALUFMUL);
    break;
  case Instruction::UDiv:
    return Config.GetEntryByType(DHDTConfig::ALUUDIV);
    break;
  case Instruction::SDiv:
    return Config.GetEntryByType(DHDTConfig::ALUSDIV);
    break;
  case Instruction::FDiv:
    return Config.GetEntryByType(DHDTConfig::ALUFDIV);
    break;
  case Instruction::URem:
    return Config.GetEntryByType(DHDTConfig::ALUUREM);
    break;
  case Instruction::SRem:
    return Config.GetEntryByType(DHDTConfig::ALUSREM);
    break;
  case Instruction::FRem:
    return Config.GetEntryByType(DHDTConfig::ALUFREM);
    break;
  case Instruction::Shl:
    return Config.GetEntryByType(DHDTConfig::ALUSHL);
    break;
  case Instruction::LShr:
    return Config.GetEntryByType(DHDTConfig::ALULSHR);
    break;
  case Instruction::AShr:
    return Config.GetEntryByType(DHDTConfig::ALUASHR);
    break;
  case Instruction::And:
    return Config.GetEntryByType(DHDTConfig::ALUAND);
    break;
  case Instruction::Or:
    return Config.GetEntryByType(DHDTConfig::ALUOR);
    break;
  case Instruction::Xor:
    return Config.GetEntryByType(DHDTConfig::ALUXOR);
    break;
  case Instruction::Load:
    return Config.GetEntryByType(DHDTConfig::REGLOAD);
    break;
  case Instruction::Store:
    return Config.GetEntryByType(DHDTConfig::REGSTORE);
    break;
  case Instruction::ICmp:
  case Instruction::FCmp:
    return Config.GetEntryByType(DHDTConfig::ALUMUX);
    break;
  case Instruction::Call:
  default:
    return Config.GetEntryByType(DHDTConfig::UNK_ENTRY);
  }
}

DHDTConfig::ConfigEntry DHDTGraph::IntrinToConfigType(std::string Intrin){
  if( Intrin == "MAX" ){
    return Config.GetEntryByType(DHDTConfig::ALUMAX);
  }else if( Intrin == "MIN" ){
    return Config.GetEntryByType(DHDTConfig::ALUMIN);
  }else if( Intrin == "NOT" ){
    return Config.GetEntryByType(DHDTConfig::ALUNOT);
  }else if( Intrin == "NEG" ){
    return Config.GetEntryByType(DHDTConfig::ALUNEG);
  }else if( Intrin == "REVERSE" ){
    return Config.GetEntryByType(DHDTConfig::ALUREV);
  }else if( Intrin == "POPCOUNT" ){
    return Config.GetEntryByType(DHDTConfig::ALUPOPC);
  }else if( Intrin == "CLZ" ){
    return Config.GetEntryByType(DHDTConfig::ALUCLZ);
  }else if( Intrin == "CTZ" ){
    return Config.GetEntryByType(DHDTConfig::ALUCTZ);
  }else if( Intrin == "SEXT" ){
    return Config.GetEntryByType(DHDTConfig::ALUSEXT);
  }else if( Intrin == "ZEXT" ){
    return Config.GetEntryByType(DHDTConfig::ALUZEXT);
  }else if( Intrin == "ROTATEL" ){
    return Config.GetEntryByType(DHDTConfig::ALUROTL);
  }else if( Intrin == "ROTATER" ){
    return Config.GetEntryByType(DHDTConfig::ALUROTR);
  }else if( Intrin == "MAJ" ){
    return Config.GetEntryByType(DHDTConfig::ALUMAJ);
  }else if( Intrin == "DOZ" ){
    return Config.GetEntryByType(DHDTConfig::ALUDOZ);
  }else if( Intrin == "COMPRESS" ){
    return Config.GetEntryByType(DHDTConfig::ALUCOMP);
  }else if( Intrin == "COMPRESSM" ){
    return Config.GetEntryByType(DHDTConfig::ALUCOMPM);
  }else if( Intrin == "INSERTS" ){
    return Config.GetEntryByType(DHDTConfig::ALUINSERTS);
  }else if( Intrin == "INSERTZ" ){
    return Config.GetEntryByType(DHDTConfig::ALUINSERTZ);
  }else if( Intrin == "EXTRACTS" ){
    return Config.GetEntryByType(DHDTConfig::ALUEXTRS);
  }else if( Intrin == "EXTRACTZ" ){
    return Config.GetEntryByType(DHDTConfig::ALUEXTRZ);
  }else if( Intrin == "MERGE" ){
    return Config.GetEntryByType(DHDTConfig::ALUMERGE);
  }else if( Intrin == "CONCAT" ){
    return Config.GetEntryByType(DHDTConfig::ALUCONCAT);
  }else if( Intrin == "LSS" ){
    return Config.GetEntryByType(DHDTConfig::ALULSS);
  }else if( Intrin == "FENCE" ){
    return Config.GetEntryByType(DHDTConfig::ALUFENCE);
  }else if( Intrin == "BSEL" ){
    return Config.GetEntryByType(DHDTConfig::ALUBSEL);
  }else if( Intrin == "NOP" ){
    return Config.GetEntryByType(DHDTConfig::ALUNOP);
  }else{
    return Config.GetEntryByType(DHDTConfig::UNK_ENTRY);
  }
}

bool DHDTGraph::AddFunctionNode(Function &Func, unsigned &Stage){
  // retrieve the metadata and determine if this function
  // is a vliw pipe stage or a standard instruction
  AttributeSet AttrSet = Func.getAttributes().getFnAttributes();
  DHDTNode *Node = nullptr;
  DHDTLink *Link = nullptr;

  if( AttrSet.hasAttribute("instformat") ){
    // standard instruction
    Node = new DHDTNode(Func.getName().str(),
                        DNodeInst);
    Nodes.push_back(Node);
    Link = new DHDTLink(Node->GetName()+".link",
                        LNodeScalar,
                        1,
                        Node);
    Links.push_back(Link);
    Nodes[0]->InsertLink(Link);
    NodeMap.insert(std::pair<std::string,DHDTNode *>(Node->GetName(),Node));
    return true;
  }else if( AttrSet.hasAttribute("vliw") ){
    if( AttrSet.getAttribute("vliw").getValueAsString().str() == "true" ){
      if( AttrSet.hasAttribute("vliwStage") ){
        Stage = std::stoul(AttrSet.getAttribute("vliwStage").getValueAsString().str());
      }
      // vliw pipe stage
      Node = new DHDTNode(Func.getName().str(),
                          DNodeVLIWPipe);
      Nodes.push_back(Node);

      // insert a link+node pair if the stage is the top-level
      // all other link connectivity will be handled via IN/OUT
      // intrinsics
      if( IsTopVLIWStage(Func) ){
        Link = new DHDTLink(Node->GetName()+".link",
                            LNodeCtrlIn,
                            1,
                            Node);
        Links.push_back(Link);
        Nodes[0]->InsertLink(Link);
      }

      NodeMap.insert(std::pair<std::string,DHDTNode *>(Node->GetName(),Node));

      return true;
    }
  }

  return false;
}

unsigned DHDTGraph::GetStoreWidth(Instruction &Inst){
  DataLayout* DL = new DataLayout("e-m:e-i64:64-f80:128-n8:16:32:64-S128");
  auto *SInst = dyn_cast<StoreInst>(&Inst);
  Value *LV = SInst->getPointerOperand();
  PointerType *PT = cast<PointerType>(LV->getType());
  unsigned Width = DL->getTypeStoreSize(PT->getPointerElementType()) * 8;
  delete DL;
  return Width;
}

bool DHDTGraph::AddCallNode(Instruction &Inst,
                            DHDTNode *CurNode){
  auto *CInst = dyn_cast<CallInst>(&Inst);
  DHDTNode *Node = nullptr;
  DHDTLink *Link = nullptr;
  Value *LHS = nullptr;

  if( CInst->getCalledFunction()->getName().str() == "LOADELEM" ){
    // Load element from memory
    if( CInst->hasName() ){
      LHS = cast<Value>(&Inst);
      Node = new DHDTNode("LOADELEM",
                          DNodeMemRead);
      Link = new DHDTLink(CurNode->GetName()+".next",
                          LNodeScalar,
                          LHS->getType()->getIntegerBitWidth(),
                          Node);
      Nodes.push_back(Node);
      Links.push_back(Link);
      CurNode->InsertLink(Link);
    }else{
      Node = new DHDTNode("LOADELEM",
                          DNodeMemRead);
      Link = new DHDTLink(CurNode->GetName()+".next",
                          LNodeScalar,
                          Node);
      Nodes.push_back(Node);
      Links.push_back(Link);
      CurNode->InsertLink(Link);
    }
    return true;
  }else if( CInst->getCalledFunction()->getName().str() == "STOREELEM" ){
    // Store element to memory
    unsigned Width = 0;
    if( CInst->getNumArgOperands() == 3 ){
      Value *V = CInst->getOperand(2); // retrieve the SIZE field
      if( V->getType()->isIntegerTy() )
        Width = V->getType()->getIntegerBitWidth();
    }
    Node = new DHDTNode("STOREELEM",
                        DNodeMemWrite);
    Link = new DHDTLink(CurNode->GetName()+".next",
                        LNodeScalar,
                        Width,
                        Node);
    Nodes.push_back(Node);
    Links.push_back(Link);
    CurNode->InsertLink(Link);
    return true;
  }

  // handle the rest of the intrinsics
  bool Found = false;
  for( unsigned i=0; i<Intrins.size(); i++ ){
    if( CInst->getCalledFunction()->getName().str() ==
        Intrins[i]->GetKeyword() ){
      Found = true;
    }
  }

  if( !Found ){
    std::cout << "Error : no intrinsic found for : "
              << CInst->getCalledFunction()->getName().str()
              << std::endl;
    return false;
  }

  // Build a new call node
  unsigned Width = 0;
  if( CInst->getNumArgOperands() > 0 ){
    Value *V = CInst->getOperand(1); // retrieve the SIZE field
    if( V->getType()->isIntegerTy() )
      Width = V->getType()->getIntegerBitWidth();
  }
  Node = new DHDTNode(CInst->getCalledFunction()->getName().str(),
                      DNodeArith);
  Link = new DHDTLink(CurNode->GetName()+".next",
                      LNodeScalar,
                      Width,
                      Node);
  Nodes.push_back(Node);
  Links.push_back(Link);
  CurNode->InsertLink(Link);
  CurNode->SetEntry(
    IntrinToConfigType(CInst->getCalledFunction()->getName().str()));

  return true;
}

unsigned DHDTGraph::GetValueWidth(Value *Op){
  Type *TmpType = Op->getType();
  if( TmpType->isVoidTy() ){
    return 64;
  }else if( TmpType->isHalfTy() ){
    return 16;
  }else if( TmpType->isFloatTy() ){
    return 32;
  }else if( TmpType->isDoubleTy() ){
    return 64;
  }else if( TmpType->isFP128Ty() ){
    return 128;
  }else if( TmpType->isIntegerTy() ){
    return TmpType->getIntegerBitWidth();
  }else if( TmpType->isPointerTy() ){
    return 64;
  }
  return 1;
}

bool DHDTGraph::AddInstNode(Function &Func,
                            Instruction &Inst,
                            DHDTNode *CurNode){
  DHDTNode *Node = nullptr;
  DHDTLink *Link = nullptr;
  Value *LHS = nullptr;

  switch(Inst.getOpcode()){
  case Instruction::Add:
  case Instruction::FAdd:
  case Instruction::Sub:
  case Instruction::FSub:
  case Instruction::Mul:
  case Instruction::FMul:
  case Instruction::UDiv:
  case Instruction::SDiv:
  case Instruction::FDiv:
  case Instruction::URem:
  case Instruction::SRem:
  case Instruction::FRem:
  case Instruction::Shl:
  case Instruction::LShr:
  case Instruction::AShr:
  case Instruction::And:
  case Instruction::Or:
  case Instruction::Xor:
    LHS = cast<Value>(&Inst);
    Node = new DHDTNode(std::string(Inst.getOpcodeName()),
                        DNodeArith,
                        LHS->getType()->getIntegerBitWidth());
    Link = new DHDTLink(CurNode->GetName()+".next",
                        LNodeScalar,
                        1,
                        Node);
    Nodes.push_back(Node);
    Links.push_back(Link);
    CurNode->InsertLink(Link);
    CurNode->SetEntry(LLVMInstToConfigType(Inst.getOpcode()));
    break;
  case Instruction::Load:
    LHS = cast<Value>(&Inst);
    Node = new DHDTNode(std::string(Inst.getOpcodeName()),
                        DNodeRegRead,
                        LHS->getType()->getIntegerBitWidth());
    Link = new DHDTLink(CurNode->GetName()+".next",
                        LNodeScalar,
                        LHS->getType()->getIntegerBitWidth(),
                        Node);
    Nodes.push_back(Node);
    Links.push_back(Link);
    CurNode->InsertLink(Link);
    CurNode->SetEntry(LLVMInstToConfigType(Inst.getOpcode()));
    break;
  case Instruction::Store:
    Node = new DHDTNode(std::string(Inst.getOpcodeName()),
                        DNodeRegWrite,
                        GetValueWidth(Inst.getOperand(0)));
    Link = new DHDTLink(CurNode->GetName()+".next",
                        LNodeScalar,
                        GetStoreWidth(Inst),
                        Node);
    Nodes.push_back(Node);
    Links.push_back(Link);
    CurNode->InsertLink(Link);
    CurNode->SetEntry(LLVMInstToConfigType(Inst.getOpcode()));
    break;
  case Instruction::ICmp:
  case Instruction::FCmp:
    Node = new DHDTNode(std::string(Inst.getOpcodeName()),
                        DNodeMux,
                        GetValueWidth(Inst.getOperand(0)));
    Link = new DHDTLink(CurNode->GetName()+".next",
                        LNodeScalar,
                        1,
                        Node);
    Nodes.push_back(Node);
    Links.push_back(Link);
    CurNode->InsertLink(Link);
    CurNode->SetEntry(LLVMInstToConfigType(Inst.getOpcode()));
    break;
  case Instruction::Call:
    if( !AddCallNode(Inst,CurNode) ){
      return false;
    }
    break;
  case Instruction::Select:
  case Instruction::Br:
    break;
  case Instruction::ZExt:
  case Instruction::SExt:
  case Instruction::FPToUI :
  case Instruction::FPToSI :
  case Instruction::UIToFP :
  case Instruction::SIToFP :
  case Instruction::FPTrunc :
  case Instruction::FPExt :
  case Instruction::BitCast :
  case Instruction::PtrToInt :
  case Instruction::IntToPtr :
  case Instruction::ExtractElement :
  case Instruction::InsertElement :
  case Instruction::ExtractValue :
  case Instruction::InsertValue :
  case Instruction::Trunc :
  case Instruction::Fence :
  case Instruction::AtomicCmpXchg :
  case Instruction::AtomicRMW :
  case Instruction::ShuffleVector :
  case Instruction::AddrSpaceCast :
  case Instruction::CleanupPad :
  case Instruction::CatchPad :
  case Instruction::GetElementPtr :
  case Instruction::Alloca :
  case Instruction::Ret :
  case Instruction::PHI :
  case Instruction::UserOp1 :
  case Instruction::UserOp2 :
  case Instruction::VAArg :
  case Instruction::LandingPad :
  default:
    // ignore these instructions
    return true;
  }
  return true;
}

bool DHDTGraph::FindFuncMatch( DHDTNode *Parent,
                               std::vector<std::string> Outs,
                               std::vector<std::string> Types ){
  std::vector<std::string> Ins;
  std::vector<std::string> TmpOuts = Outs;
  bool Found = true;

  for( auto &Func : Mod->getFunctionList() ){
    if( !Func.isDeclaration() ){
      for( auto &BB : Func.getBasicBlockList() ){
        for( auto &Inst : BB.getInstList() ){
          if( auto *CInst = dyn_cast<CallInst>(&Inst) ){
            if( CInst->getCalledFunction()->getName().str() == "OUT" ){
              if( MDNode *N = Inst.getMetadata("vliw.arg")) {
                Ins.push_back(
                  cast<MDString>(N->getOperand(0))->getString().str() );
              }
            }
          }
        }

        if( Outs.size() == Ins.size() ){
          // run a full comparison
          Found = true;
          std::sort(Ins.begin(), Ins.end());
          std::sort(TmpOuts.begin(), TmpOuts.end());
          for( unsigned i=0; i<TmpOuts.size(); i++ ){
            if( TmpOuts[i] != Ins[i] ){
              Found = false;
            }
          }

          if( Found ){
            // we found a good candidate function target, output all the link edges
            for( unsigned i=0; i<Outs.size(); i++ ){
              if( Types[i] == "DATA" ){
                // data link
                DHDTLink *Link = new DHDTLink(Outs[i],
                                              LNodeDataOut,
                                              GetFuncNode(Func.getName().str()));
                Links.push_back(Link);
                Parent->InsertLink(Link);
              }else{
                // ctrl link
                DHDTLink *Link = new DHDTLink(Outs[i],
                                              LNodeCtrlOut,
                                             GetFuncNode(Func.getName().str()));
                Links.push_back(Link);
                Parent->InsertLink(Link);
              }
            }
            return true;
          }
        }
        Ins.clear();
      }// for &BB
    }// is !Declaration
  }

  std::cout << "Error : could not find matching VLIW stage for I/O map" << std::endl;
  return false;
}

bool DHDTGraph::InitLinkPower(){
  if( Links.size() == 0 ){
    std::cout << "Error : no links to initialize power values" << std::endl;
    return false;
  }

  for( unsigned i=0; i<Links.size(); i++ ){
    switch( Links[i]->GetType() ){
    case LNodeScalar:
      Links[i]->SetEntry(Config.GetEntryByType(DHDTConfig::ALUDPATH));
      break;
    case LNodeCtrlIn:
      Links[i]->SetEntry(Config.GetEntryByType(DHDTConfig::CPATHBIT));
      break;
    case LNodeCtrlOut:
      Links[i]->SetEntry(Config.GetEntryByType(DHDTConfig::CPATHBIT));
      break;
    case LNodeDataIn:
      Links[i]->SetEntry(Config.GetEntryByType(DHDTConfig::DPATHBIT));
      break;
    case LNodeDataOut:
      Links[i]->SetEntry(Config.GetEntryByType(DHDTConfig::DPATHBIT));
      break;
    default:
      std::cout << "Error : unknown link type: "
                << (unsigned)(Links[i]->GetType()) << std::endl;
      return false;
      break;
    }
  }

  return true;
}

bool DHDTGraph::HandleInOutIntrin(){
  std::vector<std::string> Outs;
  std::vector<std::string> Types;

  for( auto &Func : Mod->getFunctionList() ){
    if( !Func.isDeclaration() ){
      for( auto &BB : Func.getBasicBlockList() ){
        for( auto &Inst : BB.getInstList() ){
          if( auto *CInst = dyn_cast<CallInst>(&Inst) ){
            if( CInst->getCalledFunction()->getName().str() == "OUT" ){
              if( MDNode *N = Inst.getMetadata("vliw.arg")) {
                Outs.push_back(
                  cast<MDString>(N->getOperand(0))->getString().str() );
              }
              if( MDNode *N = Inst.getMetadata("vliw.type")) {
                Types.push_back(
                  cast<MDString>(N->getOperand(0))->getString().str() );
              }
            }
          }
        }
        // attempt to find a matching function
        if( Outs.size() > 0 ){
          if( !FindFuncMatch(GetFuncNode(Func.getName().str()), Outs,Types) )
            return false;
        }
        Outs.clear();
        Types.clear();
      }
    }
  }
  return true;
}

bool DHDTGraph::BuildLLVMGraph(){

  DHDTNode *CurNode = nullptr;

  // create a top-level crack + decode node
  Nodes.push_back(new DHDTNode("TOP", DNodeTop));
  CurNode = GetLatestNode();

  // stage 1:
  // build up all the function nodes first.
  // this allows us to map IN/OUT intrinsics later
  for( auto &Func : Mod->getFunctionList() ){
    if( !Func.isDeclaration() ){
      // Generate a correct function node
      unsigned Stage = 0;
      if( !AddFunctionNode(Func,Stage) ){
        std::cout << "Error building function node: "
                  << Func.getName().str() << std::endl;
        return false;
      }
    }
  }


  // stage 2:
  // walk all the instructions in each function
  for( auto &Func : Mod->getFunctionList() ){
    if( !Func.isDeclaration() ){
      CurNode = GetFuncNode(Func.getName().str());
      if( CurNode == nullptr ){
        std::cout << "Error retrieving function node vertex: "
                  << Func.getName().str() << std::endl;
      }

      // walk all the basic blocks in the function
      for( auto &BB : Func.getBasicBlockList() ){

        // walk all the instructions in the basic block
        for( auto &Inst : BB.getInstList() ){
          // build the node+link for the current instruction
          if( !AddInstNode(Func,Inst,CurNode) ){
            std::cout << "Error building instruction node: "
                      << Inst.getName().str() << std::endl;
            return false;
          }

          // reset the latest node
          CurNode = GetLatestNode();
        }
      }
    }
  }

  // stage 3:
  // handle all the in/out intrinsics for VLIW nodes
  if( !HandleInOutIntrin() ){
    return false;
  }

  // stage 4:
  // initialize all the link power values
  if( !InitLinkPower() ){
    return false;
  }

  return true;
}

bool DHDTGraph::BuildGraph(){

  // build the DAG
  if( !CG.BuildDAG() ){
    std::cout << "Error building CoreGen DAG: " << CG.GetErrStr() << std::endl;
    return false;
  }

  if( !BuildLLVMGraph() ){
    return false;
  }

  return true;
}

bool DHDTGraph::BuildDot(std::string DotFile){
  if( DotFile.length() == 0 ){
    return false;
  }

  // open the file
  std::fstream fs;
  fs.open(DotFile, std::fstream::out);
  if( !fs.is_open() ){
    std::cout << "Error : could not open output file: " << DotFile << std::endl;
    return false;
  }

  // write the header
  fs << "#" << std::endl;
  fs << "# Generate graph using \" dot -Tpdf thisfile > thisfile.pdf\"" << std::endl;
  fs << "#" << std::endl;
  fs << "digraph G{" << std::endl;

  // write the nodes
  for( unsigned i=0; i<Nodes.size(); i++ ){
    fs << "node" << i << " [label=\"" << Nodes[i]->GetName()
       << "\", shape=\"box\"];" << std::endl;
  }

  // write the links
  for( unsigned i=0; i<Nodes.size(); i++ ){
    std::vector<DHDTLink *> Links = Nodes[i]->GetLinks();
    for( unsigned j=0; j<Links.size(); j++ ){
      for( unsigned k=0; k<Nodes.size(); k++ ){
        if( Nodes[k] == Links[j]->GetTarget() ){
          // write the target
          fs << "node" << i << "-> node" << k << " [ label=\""
             << Links[j]->GetName() << "\"];" << std::endl;
        }
      }
    }
  }

  // write the footer
  fs << "}" << std::endl;

  // close the file
  fs.close();

  return true;
}

double DHDTGraph::AccumLinkPower(DHDTLink *Link){
  double Tmp = Link->GetEntry().Value*Link->GetWidth();
  Link->AccumPower(Tmp);
  return Tmp;
}

double DHDTGraph::AccumNodePower(DHDTNode *Node){
  double Tmp = Node->GetEntry().Value*Node->GetWidth();
  Node->AccumPower(Tmp);
  return Tmp;
}

double DHDTGraph::RecurseGraph(DHDTNode *Node){
  double IVal = 0.;

  IVal += AccumNodePower(Node);
  std::vector<DHDTLink *> Links = Node->GetLinks();
  for( unsigned i=0; i<Links.size(); i++ ){
    IVal += AccumLinkPower(Links[i]);
    IVal += RecurseGraph(Links[i]->GetTarget());
  }

  return IVal;
}

bool DHDTGraph::ExecuteGraph(DHDTNode *Root, double &Power){
  if( Root == nullptr ){
    return false;
  }

  // this value will contain our per-instruction power values
  double IVal = 0.;

  std::vector<DHDTLink *> Links = Root->GetLinks();
  if( Root->GetType() == DNodeInst ){
    // instruction node
    // -- walk all the links and accumulate the power, then recurse
    for( unsigned i=0; i<Links.size(); i++ ){
      IVal += AccumLinkPower(Links[i]);
    }
    // choose the first link to recurse
    IVal += RecurseGraph(Links[0]->GetTarget());
  }else{
    // Top Node: vliw node walker
    // -- walk all the nodes descendent from Top and recurse
    for( unsigned i=0; i<Links.size(); i++ ){
      DHDTNode *TmpNode = Links[i]->GetTarget();
      std::vector<DHDTLink *> TmpLinks = TmpNode->GetLinks();
      std::vector<DHDTNode *> NodeVect;
      for( unsigned j=0; j<TmpLinks.size(); j++ ){
        IVal += AccumLinkPower(TmpLinks[j]);
        NodeVect.push_back(TmpLinks[j]->GetTarget());
      }

      // remove the duplicate target nodes
      std::sort(NodeVect.begin(), NodeVect.end());
      NodeVect.erase(std::unique(NodeVect.begin(), NodeVect.end()),
                     NodeVect.end());

      // recurse through the VLIW nodes
      for( unsigned j=0; j<NodeVect.size(); j++ ){
        IVal += RecurseGraph(NodeVect[j]);
      }
    }
  }

  Power = IVal;
  return true;
}

bool DHDTGraph::RecurseHazardGraph(DHDTNode *Node,
                                   std::vector<uint64_t> &Vect){

  Vect.push_back((uint64_t)(Node));
  std::vector<DHDTLink *> Links = Node->GetLinks();
  for( unsigned i=0; i<Links.size(); i++ ){
    Vect.push_back((uint64_t)(Links[i]));
    RecurseHazardGraph(Links[i]->GetTarget(),Vect);
  }

  return true;
}

bool DHDTGraph::ExecuteHazardGraph(DHDTNode *Root,
                                   std::vector<uint64_t> &Vect){
  if( Root == nullptr ){
    return false;
  }

  // add the top-level node
  if( Root->GetType() == DNodeInst ){
    // instruction node
    // -- walk all the links and accumulate the pointers, then recurse
    Vect.push_back((uint64_t)(Root));
    for( unsigned i=0; i<Links.size(); i++ ){
      Vect.push_back((uint64_t)(Links[i]));
    }
    if( !RecurseHazardGraph(Links[0]->GetTarget(),Vect) ){
      return false;
    }
  }else{
    // vliw node
    // -- walk all the nodes descendents from Top and recurse
    for( unsigned i=0; i<Links.size(); i++ ){
      DHDTNode *TmpNode = Links[i]->GetTarget();
      std::vector<DHDTLink *> TmpLinks = TmpNode->GetLinks();
      std::vector<DHDTNode *> NodeVect;
      for( unsigned j=0; j<TmpLinks.size(); j++ ){
        NodeVect.push_back(TmpLinks[j]->GetTarget());
      }

      // remove the duplicate target nodes
      std::sort(NodeVect.begin(), NodeVect.end());
      NodeVect.erase(std::unique(NodeVect.begin(), NodeVect.end()),
                     NodeVect.end());

      // recurse through the VLIW nodes
      for( unsigned j=0; j<NodeVect.size(); j++ ){
        if( !RecurseHazardGraph(NodeVect[j],Vect) ){
          return false;
        }
      }
    }
  }

  return true;
}

bool DHDTGraph::ExamineHazardResults(std::vector<std::vector<uint64_t>> HVect,
                                     std::vector<unsigned> LineNo,
                                     std::ofstream &Out){

  if( HVect.size() == 1 ){
    if( Out.is_open() ){
      Out << "No hazards present; only a single instruction" << std::endl;
    }else{
      std::cout << "No hazards present; only a single instruction" << std::endl;
    }
    return true;
  }

  for( unsigned i=1; i<HVect.size(); i++ ){
    // compare the i'th and i-1'th instructions
    unsigned Cur = i;
    unsigned Prev = i-1;

    for( unsigned j=0; j<HVect[Cur].size(); j++ ){
      if( (j+1) < HVect[Prev].size() ){
        // look at the [i][j+1] element
        if( HVect[Cur][j] == HVect[Prev][j+1] ){
          if( Out.is_open() ){
            Out << "Potential hazard found between instructions at Lines "
                << LineNo[Prev] << " and " << LineNo[Cur] << std::endl;
          }else{
            std::cout << "Potential hazard found between instructions at Lines "
                      << LineNo[Prev] << " and " << LineNo[Cur] << std::endl;
          }
        }
      } // else, don't examine the hazard
    }
  }

  return true;
}

bool DHDTGraph::HazardAnalysis(std::string InstFile,
                               std::string OutFile ){
  DHDTInst Stack(InstFile,CG);

  if( !Stack.IsOpen() ){
    std::cout << "Failed to open the instruction input file : " << InstFile << std::endl;
    return false;
  }

  std::ofstream Out;
  if( OutFile.length() > 0 ){
    Out.open(OutFile.c_str());
    if( !Out.is_open() ){
      std::cout << "Failed to open output file : " << OutFile << std::endl;
      return false;
    }
  }

  // hazard vector
  std::vector<std::vector<uint64_t>> HVect;
  std::vector<unsigned> LineNo;

  // execute the graph
  DInst *Inst = Stack.ReadInst();
  while( Inst ){
    // crack and decode the instruction
    std::string NodeName = Stack.CrackInst(Inst);
    if( NodeName.length() == 0 ){
      std::cout << "Failed to crack instruction payload = "
                << Inst->GetStr() << " at line number = "
                << Stack.GetLineNumber() << std::endl;
      return false;
    }

    // retrieve the top node and execute it
    if( NodeName == "__VLIW" ){
      // this is a vliw payload; execute all the top-level entry nodes
      std::vector<uint64_t> TVect;
      if( !ExecuteHazardGraph(GetTop(), TVect) ){
        std::cout << "Failed to execute hazard graph starting at node = "
                  << NodeName << std::endl;
        delete Inst;
        if( Out.is_open() )
          Out.close();
        return false;
      }
      HVect.push_back(TVect);
    }else{
      // this is a scalar node; find it and execute it
      auto pos = NodeMap.find(NodeName);
      if( pos != NodeMap.end() ){
        std::vector<uint64_t> TVect;
        if( !ExecuteHazardGraph(pos->second, TVect) ){
          std::cout << "Failed to execute hazard graph starting at node = "
                    << NodeName << std::endl;
          delete Inst;
          if( Out.is_open() )
            Out.close();
          return false;
        }
        HVect.push_back(TVect);
      }else{
        std::cout << "Failed to find graph node = " << NodeName << std::endl;
        delete Inst;
        if( Out.is_open() )
          Out.close();
        return false;
      }
    }

    // record the line number
    LineNo.push_back(Stack.GetLineNumber());

    // delete the instruction
    delete Inst;
    Inst = nullptr;

    // read the next instruction
    Inst = Stack.ReadInst();
  }

  // Examine the results
  if( !ExamineHazardResults(HVect, LineNo, Out) ){
    Out.close();
    return false;
  }

  if( Out.is_open() ){
    Out.close();
  }

  return true;
}

bool DHDTGraph::PowerAnalysis(std::string InstFile,
                              std::string PowerFile,
                              std::string OutFile ){
  DHDTInst Stack(InstFile,CG);

  if( !Stack.IsOpen() ){
    std::cout << "Failed to open the instruction input file : " << InstFile << std::endl;
    return false;
  }

  std::ofstream Out;
  if( OutFile.length() > 0 ){
    Out.open(OutFile.c_str());
    if( !Out.is_open() ){
      std::cout << "Failed to open output file : " << OutFile << std::endl;
      return false;
    }
  }

  DInst *Inst = Stack.ReadInst();
  double Power = 0.;
  while( Inst ){
    // crack and decode the instruction
    std::string NodeName = Stack.CrackInst(Inst);
    if( NodeName.length() == 0 ){
      std::cout << "Failed to crack instruction payload = "
                << Inst->GetStr() << " at line number = "
                << Stack.GetLineNumber() << std::endl;
      return false;
    }

    if( Verbose ){
      std::cout << "Processing instruction = " << NodeName << std::endl;
    }

    // retrieve the top node and execute it
    if( NodeName == "__VLIW" ){
      // this is a vliw payload; execute all the top-level entry nodes
      if( !ExecuteGraph(GetTop(), Power) ){
        std::cout << "Failed to execute power graph starting at node = "
                  << NodeName << std::endl;
        delete Inst;
        if( Out.is_open() )
          Out.close();
        return false;
      }
    }else{
      // this is a scalar node; find it and execute it
      auto pos = NodeMap.find(NodeName);
      if( pos != NodeMap.end() ){
        if( !ExecuteGraph(pos->second, Power) ){
          std::cout << "Failed to execute power graph starting at node = "
                    << NodeName << std::endl;
          delete Inst;
          if( Out.is_open() )
            Out.close();
          return false;
        }
      }else{
        std::cout << "Failed to find graph node = " << NodeName << std::endl;
        delete Inst;
        if( Out.is_open() )
          Out.close();
        return false;
      }
    }

    // print the individual power value
    if( Out.is_open() ){
      Out << Stack.GetLineNumber() << ","
          << "\"" << Inst->GetStr() << "\"" << ","
          << Power << std::endl;
    }else{
      std::cout << " - "
                << Stack.GetLineNumber() << " "
                << Inst->GetStr() << " "
                << Power << std::endl;
    }
    Power = 0.;

    // delete the instruction
    delete Inst;
    Inst = nullptr;

    // read the next instruction
    Inst = Stack.ReadInst();
  }

  // accumulate all the power values and print the total
  Power = 0.;
  for( unsigned i=0; i<Nodes.size(); i++ ){
    Power += Nodes[i]->GetTotalPower();
  }
  for( unsigned i=0; i<Links.size(); i++ ){
    Power += Links[i]->GetTotalPower();
  }

  if( Out.is_open() ){
    Out << "*,TOTAL_POWER," << Power << std::endl;
    Out.close();
  }else{
    std::cout << " ====== TOTAL POWER = " << Power << std::endl;
  }

  return true;
}

// EOF
