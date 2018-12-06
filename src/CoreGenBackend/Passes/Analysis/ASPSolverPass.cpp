//
// _ASPSolverPass_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/ASPSolverPass.h"

ASPSolverPass::ASPSolverPass(std::ostream *O,
                       CoreGenDAG *D,
                       CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,3,"ASPSolverPass",
                "Executes ASP solver input against L3 IR",
                true, O,D,E) {
}

ASPSolverPass::~ASPSolverPass(){
}

void ASPSolverPass::SetFiles(std::vector<std::string> F){
  Files = F;
}

bool ASPSolverPass::Execute(){
  // Get the correct DAG level: 3
  CoreGenDAG *D3 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D3 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  if( this->Files.size() == 0 ){
    WriteMsg( "Error: no input ASP rules given" );
    Errno->SetError( CGERR_STATUS, this->GetName() +
                     " : no ASP rules given" );
    return false;
  }

  // this string contains the Path of the target
  // ASP rule set(s)
  std::string ASPPath = this->GetInStr();
  std::string ASPDagFile = ASPPath + "/aspdag.lp";

  std::ofstream out(ASPDagFile);

  // this string contains the file name of your target
  // ASP rule set
  //std::string ASPFile = this->GetInStr();

  // return value
  bool rtn = true;

  // execute the ASP solver
  // TODO

  //Build DAG in ASP
  for(unsigned i = 0; i < D3->GetDimSize(); i++){
    CoreGenNode *N = static_cast<CoreGenNode *>(D3->FindNodeByIndex(i));
    if(N->GetASP().length() > 0){
      out << N->GetASP();
    }
  }
  out.close();

  for(unsigned i = 0; i < Files.size(); i++){
    double StartT = CGGetWallTime();
    std::string cmd = "clingo " + ASPDagFile + " " + ASPPath + "/" + Files[i] + " > /dev/null";
    double EndT = CGGetWallTime();
    std::cout << "  " << Files[i];
    CGPrintDots( Files[i].length() + 2, 30 );
    std::cout << EndT - StartT;
    CGPrintDots( CGDoubleToStr(EndT - StartT).length(), 30 );
    if (system(cmd.c_str()) == 7680){
      std::cout << "PASSED" << std::endl;
    }
    else{
      std::cout << "FAILED" << std::endl;
      rtn = false;
    }
  }
/*
  std::string cmd = "clingo aspdag.lp " + ASPFile;
  if (system(cmd.c_str()) == 7680){
    rtn = true;
  }
  else{
    rtn = false;
  }
*/
  return rtn;
}

// EOF
