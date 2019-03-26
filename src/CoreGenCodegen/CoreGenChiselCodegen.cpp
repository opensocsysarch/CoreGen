//
// _CoreGenChiselCodegen_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
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

bool CoreGenChiselCodegen::ExecRegClassCodegen(CoreGenNode *N){

  // examine the attributes and determine how/where to
  // place the chisel output
  std::string FullPath = ChiselRoot;
  std::string Package;
  bool Common = false;
  if( N->HasAttr(AttrISAReg) ){
    // shared across cores, put it in the common directory
    FullPath += "/common/" + CGRemoveDot(N->GetName()) + ".chisel";
    //Package = "opensocsysarch."+Proj->GetProjName()+".util";
    Package = "Common";
    Common = true;
  }else{
    // remains within a ISA, put it in the ISA-specific dir
    CoreGenNode *IN = GetRegClassISANode(N);
    if( IN == nullptr ){
      Errno->SetError(CGERR_ERROR, "No ISA containing register class: " +
                      N->GetName() );
      return false;
    }

    Package = Proj->GetProjName();
#if 0
    Package = "opensocsysarch."+
              Proj->GetProjName()+"."+
              IN->GetName()+"."+
              N->GetName();
#endif
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

bool CoreGenChiselCodegen::Execute(){
  // walk all the nodes and codegen each node individually
  bool rtn = true;

  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    // codegen the i'th node
    switch( Top->GetChild(i)->GetType() ){
    case CGSoc:
      break;
    case CGCore:
      break;
    case CGInstF:
      break;
    case CGInst:
      break;
    case CGPInst:
      break;
    case CGRegC:
      if( !ExecRegClassCodegen(Top->GetChild(i)) ){
        rtn = false;
      }
      break;
    case CGReg:
      break;
    case CGISA:
      break;
    case CGCache:
      break;
    case CGEnc:
      break;
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
  return rtn;
}

// EOF
