//
// _CoreGenPassMgr_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenPassMgr.h"

CoreGenPassMgr::CoreGenPassMgr( std::ostream *O,
                                CoreGenErrno *E,
                                CoreGenDAG *D,
                                CoreGenProj *P )
  : Stream(O), Errno(E), DAG(D), Proj(P){

  switch( Proj->GetProjType() ){
  case CGSocProj:
    InitSoCPasses(O);
    break;
  case CGModProj:
    InitModPasses(O);
    break;
  case CGExtProj:
    InitExtPasses(O);
    break;
  case CGNULLProj:
    InitSoCPasses(O);
    break;
  default:
    // don't initialize any passes
    break;
  }

  InitSysPasses(O);
}

CoreGenPassMgr::~CoreGenPassMgr(){
}

void CoreGenPassMgr::InitSoCPasses(std::ostream *O){
  // Analysis Passes
  Passes.push_back(static_cast<CoreGenPass *>(new StatsPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new MultSoCPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new ICacheCheckerPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new L1SharedPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegIdxPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegFieldPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new SpadSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CoreSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CommSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegClassSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CacheLevelPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new DanglingNodePass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new DanglingRegionPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new EncodingCollisionPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new MandatoryFieldPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new EncodingGapPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new PInstSafetyPass(O,DAG,Errno)));

  // Optimization Passes
  Passes.push_back(static_cast<CoreGenPass *>(new RegClassOpt(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CacheLevelOpt(O,DAG,Errno)));
}

void CoreGenPassMgr::InitModPasses(std::ostream *O){
  // Analysis Passes
  Passes.push_back(static_cast<CoreGenPass *>(new StatsPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new MultSoCPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new ICacheCheckerPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new L1SharedPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegIdxPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegFieldPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new SpadSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CoreSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CommSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegClassSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CacheLevelPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new DanglingNodePass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new DanglingRegionPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new EncodingCollisionPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new MandatoryFieldPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new EncodingGapPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new PInstSafetyPass(O,DAG,Errno)));

  // Optimization Passes
  Passes.push_back(static_cast<CoreGenPass *>(new RegClassOpt(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CacheLevelOpt(O,DAG,Errno)));
}

void CoreGenPassMgr::InitExtPasses(std::ostream *O){
  // Analysis Passes
  Passes.push_back(static_cast<CoreGenPass *>(new StatsPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new ICacheCheckerPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new L1SharedPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegIdxPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegFieldPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new SpadSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CoreSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CommSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new RegClassSafetyPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CacheLevelPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new DanglingNodePass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new DanglingRegionPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new EncodingCollisionPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new MandatoryFieldPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new EncodingGapPass(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new PInstSafetyPass(O,DAG,Errno)));

  // Optimization Passes
  Passes.push_back(static_cast<CoreGenPass *>(new RegClassOpt(O,DAG,Errno)));
  Passes.push_back(static_cast<CoreGenPass *>(new CacheLevelOpt(O,DAG,Errno)));
}

void CoreGenPassMgr::InitSysPasses(std::ostream *O){
  SysPasses.push_back(static_cast<CoreGenPass *>(new SafeDeletePass(O,DAG,Errno)));
  SysPasses.push_back(static_cast<CoreGenPass *>(new SpecDoc(O,DAG,Errno)));
}

std::vector<std::string> CoreGenPassMgr::GetPassNames(){
  std::vector<std::string> PName;

  std::vector<CoreGenPass *>::iterator it;
  for( it=Passes.begin(); it != Passes.end(); ++it ){
    CoreGenPass *P = (*it);
    PName.push_back(P->GetName());
  }

  return PName;
}

std::vector<std::string> CoreGenPassMgr::GetPassDescriptions(){
  std::vector<std::string> Descs;

  std::vector<CoreGenPass *>::iterator it;
  for( it=Passes.begin(); it != Passes.end(); ++it ){
    CoreGenPass *P = (*it);
    Descs.push_back(P->GetPassDesc());
  }

  return Descs;
}

bool CoreGenPassMgr::ExecuteSys( std::string Pass ){
  std::vector<CoreGenPass *>::iterator it;
  for( it=SysPasses.begin(); it != SysPasses.end(); ++it ){
    CoreGenPass *P = (*it);
    if( P->GetName() == Pass ){
      *Stream << std::endl << std::endl << "...Executing " << P->GetName() << std::endl;
      double StartT = CGGetWallTime();
      bool Rtn = P->Execute();
      double EndT = CGGetWallTime();
      if( Rtn ){
        *Stream << P->GetName();
        CGPrintDots( P->GetName().length(), 30 );
        *Stream << EndT - StartT;
        CGPrintDots( CGDoubleToStr(EndT - StartT).length(), 30 );
        *Stream << "PASSED" << std::endl;
      }else{
        *Stream << P->GetName();
        CGPrintDots( P->GetName().length(), 30 );
        *Stream << EndT - StartT;
        CGPrintDots( CGDoubleToStr(EndT - StartT).length(), 30 );
        *Stream << "FAILED" << std::endl;
      }
      return true;
    }
  }
  return false;
}

bool CoreGenPassMgr::SetPassOutputPath( std::string Pass, std::string Path ){
  std::vector<CoreGenPass *>::iterator it;
  for( it=SysPasses.begin(); it != SysPasses.end(); ++it ){
    CoreGenPass *P = (*it);
    if( P->GetName() == Pass ){
      return P->SetOutputPath(Path);
    }
  }
  return false;
}

bool CoreGenPassMgr::SetPassInputStr( std::string Pass, std::string Str ){
  std::vector<CoreGenPass *>::iterator it;
  for( it=SysPasses.begin(); it != SysPasses.end(); ++it ){
    CoreGenPass *P = (*it);
    if( P->GetName() == Pass ){
      return P->SetInStr(Str);
    }
  }
  return false;
}

bool CoreGenPassMgr::Execute( std::string Pass ){

  std::vector<CoreGenPass *>::iterator it;
  for( it=Passes.begin(); it != Passes.end(); ++it ){
    CoreGenPass *P = (*it);
    if( P->GetName() == Pass ){
      *Stream << std::endl << std::endl << "...Executing " << P->GetName() << std::endl;
      double StartT = CGGetWallTime();
      bool Rtn = P->Execute();
      double EndT = CGGetWallTime();
      if( Rtn ){
        *Stream << P->GetName();
        CGPrintDots( P->GetName().length(), 30 );
        *Stream << EndT - StartT;
        CGPrintDots( CGDoubleToStr(EndT - StartT).length(), 30 );
        *Stream << "PASSED" << std::endl;
      }else{
        *Stream << P->GetName();
        CGPrintDots( P->GetName().length(), 30 );
        *Stream << EndT - StartT;
        CGPrintDots( CGDoubleToStr(EndT - StartT).length(), 30 );
        *Stream << "FAILED" << std::endl;
      }
      return true;
    }
  }

  return false;
}

bool CoreGenPassMgr::Execute( CGPassType T ){
  std::string TStr;

  switch( T ){
  case CGPassAnalysis:
    TStr = "Analysis";
    break;
  case CGPassOpt:
    TStr = "Optimization";
    break;
  case CGPassData:
    TStr = "Data";
    break;
  case CGPassUnk:
  default:
    TStr = "UNKNOWN";
    break;
  }

  *Stream << "========================================================" << std::endl;
  *Stream << " Executing CoreGen Pass Type: " << TStr << std::endl;
  *Stream << "========================================================" << std::endl;
  *Stream << "PASS                          TIME (secs)                 PASS/FAIL" << std::endl;

  // this variable is setup to catch the case when users attempt to run
  // passes with no passes enabled
  bool IsEmptyRun = true;

  std::vector<CoreGenPass *>::iterator it;
  for( it=Passes.begin(); it != Passes.end(); ++it ){
    CoreGenPass *P = (*it);
    if( P->GetPassType() == T ){
      IsEmptyRun = false; // this is a non-empty run
      *Stream << std::endl << std::endl << "...Executing " << P->GetName() << std::endl;
      double StartT = CGGetWallTime();
      bool Rtn = P->Execute();
      double EndT = CGGetWallTime();
      if( Rtn ){
        *Stream << P->GetName();
        CGPrintDots( P->GetName().length(), 30 );
        *Stream << EndT - StartT;
        CGPrintDots( CGDoubleToStr(EndT - StartT).length(), 30 );
        *Stream << "PASSED" << std::endl;
      }else{
        *Stream << P->GetName();
        CGPrintDots( P->GetName().length(), 30 );
        *Stream << EndT - StartT;
        CGPrintDots( CGDoubleToStr(EndT - StartT).length(), 30 );
        *Stream << "FAILED" << std::endl;
      }
    }
  }

  if( IsEmptyRun ){
    *Stream << "FAILED : No passes enabled" << std::endl;
    return false;
  }

  return true;
}

bool CoreGenPassMgr::ExecuteSys(){
  *Stream << "========================================================" << std::endl;
  *Stream << " Executing CoreGen System Passes" << std::endl;
  *Stream << "========================================================" << std::endl;

  // Execute the passes
  std::vector<CoreGenPass *>::iterator it;
  std::vector<bool> Success;
  std::vector<double> StartT;
  std::vector<double> EndT;
  for( it=SysPasses.begin(); it != SysPasses.end(); ++it ){
    CoreGenPass *P = (*it);
    *Stream << std::endl << std::endl << "...Executing " << P->GetName() << std::endl;
    StartT.push_back(CGGetWallTime());
    Success.push_back(P->Execute());
    EndT.push_back(CGGetWallTime());
  }

  *Stream << "========================================================" << std::endl;
  *Stream << " CoreGen System Pass Summary" << std::endl;
  *Stream << "========================================================" << std::endl;
  *Stream << "PASS                          TIME (secs)                 PASS/FAIL" << std::endl;

  unsigned i=0;
  for( it=SysPasses.begin(); it != SysPasses.end(); ++it ){
    CoreGenPass *P = (*it);
    if( Success[i] ){
      *Stream << P->GetName();
      CGPrintDots( P->GetName().length(), 30 );
      *Stream << EndT[i] - StartT[i];
      CGPrintDots( CGDoubleToStr(EndT[i] - StartT[i]).length(), 30 );
      *Stream << "PASSED" << std::endl;
    }else{
      *Stream << P->GetName();
      CGPrintDots( P->GetName().length(), 30 );
      *Stream << EndT[i] - StartT[i];
      CGPrintDots( CGDoubleToStr(EndT[i] - StartT[i]).length(), 30 );
      *Stream << "FAILED" << std::endl;
    }
    i++;
  }

  return true;
}

bool CoreGenPassMgr::Execute(){
  *Stream << "========================================================" << std::endl;
  *Stream << " Executing CoreGen Passes" << std::endl;
  *Stream << "========================================================" << std::endl;

  // Execute the passes
  std::vector<CoreGenPass *>::iterator it;
  std::vector<bool> Success;
  std::vector<double> StartT;
  std::vector<double> EndT;
  for( it=Passes.begin(); it != Passes.end(); ++it ){
    CoreGenPass *P = (*it);
    *Stream << std::endl << std::endl << "...Executing " << P->GetName() << std::endl;
    StartT.push_back(CGGetWallTime());
    Success.push_back(P->Execute());
    EndT.push_back(CGGetWallTime());
  }

  *Stream << "========================================================" << std::endl;
  *Stream << " CoreGen Pass Summary" << std::endl;
  *Stream << "========================================================" << std::endl;
  *Stream << "PASS                          TIME (secs)                 PASS/FAIL" << std::endl;

  unsigned i=0;
  for( it=Passes.begin(); it != Passes.end(); ++it ){
    CoreGenPass *P = (*it);
    if( Success[i] ){
      *Stream << P->GetName();
      CGPrintDots( P->GetName().length(), 30 );
      *Stream << EndT[i] - StartT[i];
      CGPrintDots( CGDoubleToStr(EndT[i] - StartT[i]).length(), 30 );
      *Stream << "PASSED" << std::endl;
    }else{
      *Stream << P->GetName();
      CGPrintDots( P->GetName().length(), 30 );
      *Stream << EndT[i] - StartT[i];
      CGPrintDots( CGDoubleToStr(EndT[i] - StartT[i]).length(), 30 );
      *Stream << "FAILED" << std::endl;
    }
    i++;
  }

  return true;
}

bool CoreGenPassMgr::PrintPassInfo(){
  std::vector<CoreGenPass *>::iterator it;
  for( it=Passes.begin(); it != Passes.end(); ++it ){
    CoreGenPass *P = (*it);
    P->PrintPassInfo();
  }
  return true;
}

bool CoreGenPassMgr::PrintSysPassInfo(){
  std::vector<CoreGenPass *>::iterator it;
  for( it=SysPasses.begin(); it != SysPasses.end(); ++it ){
    CoreGenPass *P = (*it);
    P->PrintPassInfo();
  }
  return true;
}

// EOF
