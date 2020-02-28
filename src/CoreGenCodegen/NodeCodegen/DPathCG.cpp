
//
// _DPathCG_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/NodeCodegen/DPathCG.h"

DPathCG::DPathCG( CoreGenNode *N, CoreGenProj *P,
                std::string Pack, std::string Path,
                bool Comm, CoreGenErrno *E )
  : CoreGenNodeCodegen(N,P,Pack,Path,Comm,E) {
}



bool DPathCG::GenSharedBus(std::ofstream &O){

  if( !O.is_open() )
    return false;
   O << "/* Reference Chisel: " << std::endl;
   /*
   // The Bus
   // (this is a bus-based RISCV implementation, so all data movement goes
   // across this wire)
   val bus = MuxCase(0.U, Array(
               (io.ctl.en_imm)                  -> imm(conf.xprlen-1,0),
               (io.ctl.en_alu)                  -> alu(conf.xprlen-1,0),
               (io.ctl.en_reg & ~io.ctl.reg_wr &
                 (io.ctl.reg_sel =/= RS_CR))     -> reg_rdata(conf.xprlen-1,0),
               (io.ctl.en_mem & ~io.ctl.mem_wr) -> io.mem.resp.bits.data(conf.xprlen-1,0),
               (io.ctl.en_reg & ~io.ctl.reg_wr &
                  (io.ctl.reg_sel === RS_CR))   -> csr_rdata
             ))

    */ 

    //For bus-based RISC implementations this function will generate the shared bus that
    //all uOp fubs use for communication.  Initially, we assume that:
    // 1) Each macro-op executes serially
    // 2) no registers between fubs on bus
    // 3) State machine that manages the control signals will be clocked, so that adds
    //      determinism / synchronous to the uOp data movement
    // 4) All RISC pipeline implementations have the same elements regardless of ISA 
    //
    // This will need to be updated as we add pipelining, multi-cycle fubs, etc
    

   return true; 

}


bool DPathCG::Execute( ){

  // open the output file
  std::ofstream OutFile;
  OutFile.open(Path,std::ios::trunc);
  if(!OutFile.is_open()){
    Errno->SetError(CGERR_ERROR, "Could not open chisel file for DataPath:" +
                    Path );
    return false;
  }

  // write the header
  OutFile << "// -- " << Node->GetName()
          << " Chisel Output" << std::endl << std::endl;

  // write the package info
  if( !WritePackageInfo(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write standard package info to file : " +
                    Path );
    OutFile.close();
    return false;
  }

  // import the chisel imports
  if( !WriteChiselImports(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write standard Chisel header to file : " + 
                    Path );
    OutFile.close();
    return false;
  }

  // write out the pipeline
  if( !GenSharedBus(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not Shared Bus block to file : " +
                    Path );
    OutFile.close();
    return false;
  }

  if( !WriteStdFooter(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write standard Chisel footer to file : " + 
                    Path );
    OutFile.close();
    return false;
  }

  // close the output file
  OutFile.close();

  return true;
}

// EOF
