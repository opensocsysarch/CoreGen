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

  // return value
  bool rtn = true;

  // execute the ASP solver

  // Build DAG in ASP
  for(unsigned i = 0; i < D3->GetDimSize(); i++){
    CoreGenNode *N = static_cast<CoreGenNode *>(D3->FindNodeByIndex(i));
    if(N->GetASP().length() > 0){
      out << N->GetASP();
    }
  }
  out << "cacheIsParent(X, true) :- cacheParentOf(X, Y)." << std::endl;
  out << "cacheIsChild(X, true) :- cacheChildOf(X, Y)." << std::endl;
  out << "cacheIsParent(X, false) :- not cacheIsParent(X, true), cache(X)." << std::endl;
  out << "cacheIsChild(X, false) :- not cacheIsChild(X, true), cache(X)." << std::endl;
  out.close();

  // initialize clingo/gringo
#if defined(CLINGO_WITH_PYTHON)
  if (!clingo_register_python_()) {
    WriteMsg( std::string(clingo_error_message()) );
    return false;
  }
#elif defined(CLINGO_WITH_LUA)
  if (!clingo_register_lua_(nullptr)) {
    WriteMsg( std::string(clingo_error_message()) );
    return false;
  }
#else
  // no clingo configuration found
  WriteMsg( "No clingo solver configuration found: exiting" );
  return false;
#endif

  // setup the clingo args
  int argc = 3;
  char app[] = "clingo";
  char dot[] = ".";        // this is a temporary placeholder
  char *argv[] = {app,strdup(ASPDagFile.c_str()),dot,NULL};
  char *tmpname = strdup("/tmp/ASPTmpFileXXXXXX");
  mkstemp(tmpname);
  std::ofstream of(tmpname);
  for( unsigned i=0; i < Files.size(); i++ ){
    std::ifstream infile( Files[i] );
    of << infile.rdbuf();
    infile.close();
  }
  of.close();
  std::string LTmpFile(tmpname);

  argv[2] = strdup(LTmpFile.c_str());

  // execute clingo
  bool isSuccess = true;
  double StartT = CGGetWallTime();
  if( clingo_main_(argc, argv) != 0 ){
    isSuccess = false;
    rtn = false;
  }
  double EndT = CGGetWallTime();

  WriteMsg( CGPrintDotStr( LTmpFile.length(), 30 ) + LTmpFile );
  WriteMsg( CGPrintDotStr( CGDoubleToStr(EndT-StartT).length(), 30 ) +
            CGDoubleToStr(EndT-StartT) );

  if( isSuccess ){
    WriteMsg( CGPrintDotStr( 6, 30 ) + "PASSED" );
  }else{
    WriteMsg( CGPrintDotStr( 6, 30 ) + "FAILED" );
  }

  CGDeleteFile(LTmpFile);
  return rtn;
}

// EOF
