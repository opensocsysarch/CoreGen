//
// _CoreGenChiselCodegen_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/CoreGenChiselCodegen.h"

CoreGenChiselCodegen::CoreGenChiselCodegen( CoreGenNode *T,
                                            CoreGenProj *P,
                                            std::string R,
                                            CoreGenErrno *E )
  : Top(T), Proj(P), ChiselRoot(R), Errno(E) {
}

CoreGenChiselCodegen::~CoreGenChiselCodegen(){
}

CoreGenNode *CoreGenChiselCodegen::GetRegClassISANode(CoreGenNode *N){
  // walks the node structure looking for ISAs that utilize the target
  // register class
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGCore ){
      // found a core, check its register classes
      CoreGenCore *C = static_cast<CoreGenCore *>(Top->GetChild(i));
      for( unsigned j=0; j<C->GetNumRegClass(); j++ ){
        if( C->GetRegClass(j)->GetName() == N->GetName() ){
          return static_cast<CoreGenNode *>(C->GetISA());
        }
      }
    }
  }

  // if we arrive here, then something went wrong
  return nullptr;
}

bool CoreGenChiselCodegen::ExecSocCodegen(CoreGenNode *N){
  std::string FullPath = ChiselRoot + "/top/soc.chisel";
  std::string Package = Proj->GetProjName();

  SocCG *CG = new SocCG(N,Proj,Package,FullPath,true,Errno);
  bool rtn = true;
  if( !CG->Execute() ){
    rtn = false;
  }

  delete CG;

  return rtn;
}

bool CoreGenChiselCodegen::ExecSpadCodegen(CoreGenNode *N){
  std::string FullPath = ChiselRoot + "/common/" +
                         CGRemoveDot(N->GetName()) + ".chisel";
  std::string Package = "Common";
  bool Common = true;

  SpadCG *CG = new SpadCG(N,Proj,Package,FullPath,Common,Errno);
  bool rtn = true;
  if( !CG->Execute() ){
    rtn = false;
  }

  delete CG;

  return rtn;
}

bool CoreGenChiselCodegen::ExecDataPathCodegen(CoreGenNode *N){

  std::string Package = Proj->GetProjName();
  std::string FullPath = ChiselRoot;

  FullPath += ("/" + N->GetName());

  if( !CGMkDirP(FullPath) ){
    Errno->SetError(CGERR_ERROR, "Could not construct DataPath directory: "
                      + FullPath );
    return false;
  }

  FullPath += "/DataPath.chisel";

  DPathCG *CG = new DPathCG(N,Proj,Package,FullPath,false,Errno);
  bool rtn = true;
  if( !CG->Execute() ){
    rtn = false;
  }

  delete CG;

  return rtn;
    
}
bool CoreGenChiselCodegen::ExecISACodegen(CoreGenNode *N){
  // build a vector of all the instructions contained within the ISA
  std::vector<CoreGenInst *> Insts;
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGInst ){
      CoreGenInst *Inst = static_cast<CoreGenInst *>(Top->GetChild(i));
      if( Inst->GetISA()->GetName() == N->GetName() ){
        Insts.push_back(Inst);
      }
    }
  }

  // now that we have the full set of instructions associated with this
  // ISA, we need to figure out how many unique instruction formats
  // there are and initiate code generation for each format individually
  std::vector<CoreGenInstFormat *> IF;
  for( unsigned i=0; i<Insts.size(); i++ ){
    IF.push_back( Insts[i]->GetFormat() );
  }

  // filter out the unique instruction formats
  std::sort(IF.begin(),IF.end());
  IF.erase(std::unique(IF.begin(),IF.end()),IF.end());

  std::string Package = Proj->GetProjName();
  std::string FullPath = ChiselRoot;

  FullPath += ("/" + N->GetName());

  if( !CGMkDirP(FullPath) ){
    Errno->SetError(CGERR_ERROR, "Could not construct ISA directory: "
                      + FullPath );
    return false;
  }

  FullPath += "/instructions.chisel";

  ISACG *CG = new ISACG(N,Proj,Package,FullPath,false,Errno);
  bool rtn = true;
  if( !CG->Execute(Insts,IF) ){
    rtn = false;
  }

  delete CG;

  return rtn;
}

bool CoreGenChiselCodegen::ExecRegClassCodegen(CoreGenNode *N){

  // examine the attributes and determine how/where to
  // place the chisel output
  std::string FullPath = ChiselRoot;
  std::string Package;
  bool Common = false;
  if( N->HasAttr(AttrISAReg) ){
    // shared across cores, put it in the common directory
    FullPath += "/common/" + CGRemoveDot(N->GetName()) + ".chisel";
    Package = "Common";
    Common = true;
  }else{
    // remains within an ISA, put it in the ISA-specific dir
    CoreGenNode *IN = GetRegClassISANode(N);
    if( IN == nullptr ){
      Errno->SetError(CGERR_ERROR, "No ISA containing register class: " +
                      N->GetName() );
      return false;
    }

    Package = Proj->GetProjName();
    FullPath += ("/" + IN->GetName());

    if( !CGMkDirP(FullPath) ){
      Errno->SetError(CGERR_ERROR, "Could not construct ISA directory: "
                      + FullPath );
      return false;
    }

    FullPath += ("/" + CGRemoveDot(N->GetName()) + ".chisel");
  }

  RegClassCG *CG = new RegClassCG(N,Proj,Package,FullPath,Common,Errno);
  bool rtn = true;
  if( !CG->Execute() ){
    rtn = false;
  }

  delete CG;

  return rtn;
}

bool CoreGenChiselCodegen::ExecPluginCodegen(CoreGenNode *N){
  std::string FullPath = ChiselRoot + "/top/"
                                    + CGRemoveDot(N->GetName())
                                    + ".chisel";
  std::string Package = Proj->GetProjName();

  PluginCG *CG = new PluginCG(N,Proj,Package,FullPath,true,Errno);
  bool rtn = true;
  if( !CG->Execute() ){
    rtn = false;
  }

  delete CG;
  return rtn;
}

bool CoreGenChiselCodegen::WriteCoreConfig(std::ofstream &O){
  if( !O.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not write core config info" );
    return false;
  }

  std::vector<CoreGenNode *> CNodes;

  // walk all the top-level nodes and find all the register classes
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGCore )
      CNodes.push_back(Top->GetChild(i));
  }

  // made the vector unique
  std::sort(CNodes.begin(),CNodes.end());
  CNodes.erase(std::unique(CNodes.begin(),CNodes.end()),CNodes.end());

  // for each core, output the relevant configuration data
  for( unsigned i=0; i<CNodes.size(); i++ ){
    CoreGenCore *Core = static_cast<CoreGenCore *>(CNodes[i]);
    O << "\tval " << CGRemoveDot(Core->GetName()) << "_startaddr = \"h80000000\".U" << std::endl;
  }

  return true;
}

bool CoreGenChiselCodegen::WriteRegClassConfig(std::ofstream &O){
  if( !O.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not write register class config info" );
    return false;
  }

  std::vector<CoreGenNode *> RCNodes;

  // walk all the top-level nodes and find all the register classes
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->GetType() == CGRegC )
      RCNodes.push_back(Top->GetChild(i));
  }

  // made the vector unique
  std::sort(RCNodes.begin(),RCNodes.end());
  RCNodes.erase(std::unique(RCNodes.begin(),RCNodes.end()),RCNodes.end());

  // for each register class node, output the relevant configuration data
  for( unsigned i=0; i<RCNodes.size(); i++ ){
    CoreGenRegClass *RC = static_cast<CoreGenRegClass *>(RCNodes[i]);
    O << "\tval " << CGRemoveDot(RC->GetName()) << "_numregs = " << RC->GetNumReg() << std::endl;
    O << "\tval " << CGRemoveDot(RC->GetName()) << "_maxwidth = " << RC->GetMaxWidth() << std::endl;
  }

  return true;
}

bool CoreGenChiselCodegen::GenerateConfig(){
  std::string ConfigFile = ChiselRoot + "/common/configurations.scala";
  std::ofstream MOutFile;
  MOutFile.open(ConfigFile,std::ios::trunc);
  if( !MOutFile.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open common configuration file: " + ConfigFile );
    return false;
  }

  MOutFile << "//-- common/configurations.scala" << std::endl << std::endl;
  MOutFile << "package Common" << std::endl
           << "{" << std::endl
           << "import chisel3._" << std::endl
           << "import chisel3.util._" << std::endl << std::endl
           << "case class " << CGRemoveDot(Proj->GetProjName()) << "Configuration()" << std::endl
           << "{" << std::endl;

  // write out the config parameters
  if( !WriteRegClassConfig(MOutFile) ){
    MOutFile.close();
    return false;
  }

  // write out the core config parameters for reset
  if( !WriteCoreConfig(MOutFile) ){
    MOutFile.close();
    return false;
  }

  MOutFile << "}" << std::endl
           << "}" << std::endl;

  MOutFile.close();
  return true;
}

bool CoreGenChiselCodegen::GenerateDriver(CoreGenNode *SocNode){
  std::string DriverFile = ChiselRoot + "/top/top.scala";
  std::ofstream MOutFile;
  MOutFile.open(DriverFile,std::ios::trunc);
  if( !MOutFile.is_open() ){
    Errno->SetError(CGERR_ERROR, "Could not open top level scala file: " + DriverFile );
    return false;
  }

  MOutFile << "//-- top/top.scala " << std::endl
           << "package " << Proj->GetProjName() << std::endl << std::endl
           << "import chisel3._" << std::endl
           << "import chisel3.util_" << std::endl
           << "import Common._" << std::endl
           << "import scala.collection.mutable.ArrayBuffer" << std::endl
           << "import scala.collection.mutable.HashMap" << std::endl << std::endl;

  MOutFile << "class Top extends Module" << std::endl
           << "{" << std::endl
           << "\tval io = IO(new Bundle{" << std::endl
           << "\t\tval success = Output(Bool())" << std::endl
           << "\t})" << std::endl << std::endl
           << "\timplicit val proj_conf = "
           << CGRemoveDot(Proj->GetProjName()) << "Configuration()" << std::endl;

  // if there is a SoC in the design, instantiate it
  if( SocNode != nullptr )
    MOutFile << "\tval soc = Module(new " << CGRemoveDot(SocNode->GetName()) << ")" << std::endl;

  // TODO: instantiate clocks and debug

  MOutFile << "}" << std::endl;

  MOutFile << "object elaborate {" << std::endl
           << "\tdef main(args: Array[String]): Unit = {" << std::endl
           << "\t\tchisel3.Driver.execute(args, () => new Top)" << std::endl
           << "\t}" << std::endl
           << "}" << std::endl;

  return true;
}

bool CoreGenChiselCodegen::Execute(){
  // walk all the nodes and codegen each node individually
  bool rtn = true;

  // generate the top-level config
  if( !GenerateConfig() ){
    return false;
  }

  std::cout << "Generating Chisel... " << std::endl;
  CoreGenNode *SocNode = nullptr;

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    if( Top->GetChild(i)->IsOverridden() ){
      if( !ExecPluginCodegen(Top->GetChild(i)) )
          rtn = false;
    }

    // codegen the i'th node
    switch( Top->GetChild(i)->GetType() ){
    case CGSoc:
      if( !ExecSocCodegen(Top->GetChild(i)) ){
        rtn = false;
      }
      SocNode = Top->GetChild(i);
      break;
    case CGCore:
      break;
#if 0
    case CGInstF:
      break;
#endif
    case CGInst:
      break;
#if 0
    case CGPInst:
      break;
#endif
    case CGRegC:
      if( !ExecRegClassCodegen(Top->GetChild(i)) ){
        rtn = false;
      }
      break;
#if 0
    case CGReg:
      break;
#endif
    case CGISA:
      if( !ExecISACodegen(Top->GetChild(i)) ){
        rtn = false;
      }
      break;
    case CGCache:
      break;
    case CGDPath:
      std::cout << "Generating Datapath... " << std::endl;
        if( !ExecDataPathCodegen(Top->GetChild(i) )){
         rtn = false;
        }
      break;
#if 0
    case CGEnc:
      break;
#endif
    case CGExt:
      break;
    case CGComm:
      break;
    case CGSpad:
      if( !ExecSpadCodegen(Top->GetChild(i)) ){
        rtn = false;
      }
      break;
    case CGMCtrl:
      break;
    case CGVTP:
      break;
    case CGPlugin:
      break;
    default:
      break;
    }
  }

  // generate the scala driver
  if( !GenerateDriver(SocNode) ){
    rtn = false;
  }

  return rtn;
}

// EOF
