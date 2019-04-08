//
// _RegIdxPass_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Analysis/RegIdxPass.h"

RegIdxPass::RegIdxPass(std::ostream *O,
                       CoreGenDAG *D,
                       CoreGenErrno *E)
  : CoreGenPass(CGPassAnalysis,1,"RegIdxPass",
                "Identifies potential register index collisions within a Register Class",
                false,O,D,E) {
}

RegIdxPass::~RegIdxPass(){
}

bool RegIdxPass::Execute(){
  /*
  std::ofstream mystream("aspdag.lp", std::ios::app);
  mystream << ":- regClassReg(X, Y)," << std::endl
           << "regClassReg(X, Z)," << std::endl
           << "regIndex(Y, W)," << std::endl
           << "regIndex(Z, W)," << std::endl
           << "Y != Z." << std::endl;
  mystream.close();
  if (system("clingo aspdag.lp") == 7680){
    return true;
  }
  else{
    return false;
  }

  mystream.open("out.txt", std::ios::app);
  mystream << system("clingo test.lp");
  FILE *pipe = popen("clingo aspdag.lp", "r");
  char buffer;
  int count = 0;
  fseek(pipe, 64, SEEK_SET);
  while (!feof(pipe)){
    count++;
    buffer = fgetc(pipe);
    if (buffer == 'U'){
      mystream << count;
    }
    mystream << buffer;
  }
  pclose(pipe);
  mystream.close();
  */
  ////////////////////////////////////////////////////////////
  // Get the correct DAG level: 1
  CoreGenDAG *D1 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D1 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  bool rtn = true;
  int NewIdx = -1;

  // walk all the nodes, pull out the register class nodes
  // and examine all the child register indices
  for(unsigned i=0; i<D1->GetDimSize(); i++ ){
    CoreGenRegClass *RC = static_cast<CoreGenRegClass *>(D1->FindNodeByIndex(i));
    if( RC->GetType() == CGRegC ){
      // found a register class, walk all its children
      std::vector<int> Idx;
      for( unsigned j=0; j<RC->GetNumReg(); j++ ){
        NewIdx = RC->GetReg(j)->GetIndex();
        // check against all the recorded indices
        for( unsigned k=0; k<Idx.size(); k++ ){
          if( NewIdx == Idx[k] ){
            // collision occurred
            WriteMsg( "Identified a register collision at RegisterClass=" +
                      RC->GetName() + "; Register=" + RC->GetReg(j)->GetName() +
                      " ;Index=" + std::to_string(NewIdx) );
            rtn = false;
          }
        }
        // no collision found, record the index
        Idx.push_back(NewIdx);
      }
    }
  }

  return rtn;
  
}

// EOF
