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

DHDTGraph::DHDTGraph(DHDTConfig &Config)
  : Config(Config) {}

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

  // get the set of signals
  // TODO: is this required?

  return true;
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
                        DNodeArith);
    Link = new DHDTLink(CurNode->GetName()+".next",
                        LNodeScalar,
                        LHS->getType()->getIntegerBitWidth(),
                        Node);
    Nodes.push_back(Node);
    Links.push_back(Link);
    CurNode->InsertLink(Link);
    break;
  case Instruction::Load:
    LHS = cast<Value>(&Inst);
    Node = new DHDTNode(std::string(Inst.getOpcodeName()),
                        DNodeRegRead);
    Link = new DHDTLink(CurNode->GetName()+".next",
                        LNodeScalar,
                        LHS->getType()->getIntegerBitWidth(),
                        Node);
    Nodes.push_back(Node);
    Links.push_back(Link);
    CurNode->InsertLink(Link);
    break;
  case Instruction::Store:
    Node = new DHDTNode(std::string(Inst.getOpcodeName()),
                        DNodeRegWrite);
    Link = new DHDTLink(CurNode->GetName()+".next",
                        LNodeScalar,
                        GetStoreWidth(Inst),
                        Node);
    Nodes.push_back(Node);
    Links.push_back(Link);
    CurNode->InsertLink(Link);
    break;
  case Instruction::ICmp:
  case Instruction::FCmp:
    Node = new DHDTNode(std::string(Inst.getOpcodeName()),
                        DNodeRegWrite);
    Link = new DHDTLink(CurNode->GetName()+".next",
                        LNodeScalar,
                        1,
                        Node);
    Nodes.push_back(Node);
    Links.push_back(Link);
    CurNode->InsertLink(Link);
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
          fs << "node" << i << "-> node" << k << ";" << std::endl;
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

bool DHDTGraph::HazardAnalysis(std::string InstFile,
                               std::string OutFile ){
  return true;
}

bool DHDTGraph::PowerAnalysis(std::string InstFile,
                              std::string PowerFile,
                              std::string OutFile ){
  return true;
}

// EOF
