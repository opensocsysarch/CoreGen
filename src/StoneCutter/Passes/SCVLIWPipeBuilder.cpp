//
// _SCVLIWPipeBuilder_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/Passes/SCVLIWPipeBuilder.h"
#include "CoreGen/StoneCutter/SCMsg.h"

SCVLIWPipeBuilder::SCVLIWPipeBuilder(Module *TM,
                     SCOpts *O,
                     SCMsg *M)
  : SCPass("VLIWPipeBuilder","",TM,O,M) {
}

SCVLIWPipeBuilder::~SCVLIWPipeBuilder(){
}

bool SCVLIWPipeBuilder::Execute(){
  if( !TheModule ){
    this->PrintMsg( L_ERROR, "LLVM IR Module is null" );
    return false;
  }

#ifdef 0
  // TODO: Not sure if InstVect should use the same as PipeBuilder... seems like a BundleParser that would look at the sigmap may be more appropriate
  VLIWPipeVect = SigMap->GetVLIWPipeVect();
  // BundleVect = SigMap->GetBundleVect();
  // InstVect = ParseBundles();

  // Enable SubPasses (TODO: VLIW Specific?)
  if( !EnableSubPasses() ) {
    this->PrintMsg( L_ERROR, "Could not initialize VLIW sub-passes");
    return false;
  }
  // Initalize the attribute map (TODO: This would remain the same process just different attributes... I think)
  if( !InitAttrs() ){
    this->PrintMsg( L_ERROR, "Could not initialize attributes" );
    return false;
  }

  // TODO: Would there be a matrix? VLIW Payload won't have crack & decode yet
  // allocate the matrix
  if( !AllocMat() ){
    this->PrintMsg( L_ERROR, "Could not allocate matrix" );
    return false;
  }

  // build the pipeline matrix (TODO: This should be replaced by a bundle scheduler... is In/Out the only constraints a user can specify?)
  if( !BuildMat() ){
    this->PrintMsg( L_ERROR, "Could not build matrix representation of pipeline" );
    FreeMat();
    return false;
  }

  // execute optimizations (TODO: We should be able to use this as is w/ different passes enabled of course)
  if( !Optimize() ){
    this->PrintMsg( L_ERROR, "Encountered errors executing the the optimization phases" );
    FreeMat();
    return false;
  }

  // write everything back to the sigmap (TODO: This should be able to be used as well, just maybe specifying which pipeline to write back to?)
  if( !WriteSigMap() ){
    this->PrintMsg( L_ERROR, "Could not write signal map for the prescribed pipeline" );
    FreeMat();
    return false;
  }

  // free the matrix
  if( !FreeMat() ){
    this->PrintMsg( L_ERROR, "Could not free adjacency matrix" );
    return false;
  }

#endif
  return true;
}

// EOF

