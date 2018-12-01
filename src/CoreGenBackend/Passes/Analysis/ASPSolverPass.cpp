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
  std::ofstream out("aspdag.lp");
  if( this->GetInStr().length() == 0 ){
    WriteMsg( "Error: no input ASP rule defined" );
    Errno->SetError( CGERR_STATUS, this->GetName() +
                     " : no ASP rule defined" );
    return false;
  }

  // this string contains the file name of your target
  // ASP rule set
  std::string ASPFile = this->GetInStr();

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

  std::string cmd = "clingo aspdag.lp " + ASPFile;
  if (system(cmd.c_str()) == 7680){
    rtn = true;
  }
  else{
    rtn = false;
  }

  return rtn;
}

// EOF
