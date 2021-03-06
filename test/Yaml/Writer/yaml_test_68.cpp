//
// _YAML_TEST68_CPP_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
// 
//

#include <iostream>
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

std::string PROJNAME = "TEST68";
std::string PROJROOT = "./";
std::string ARCHROOT = "./";
std::string PROJYAML = "TEST68.yaml";

unsigned ITER=68;
unsigned NUM_CORES=4;

int main( int argc, char **argv ){
  CoreGenBackend *CG = new CoreGenBackend(PROJNAME,
                                          PROJROOT,
                                          ARCHROOT);
  if( !CG ){
    std::cout << "ERROR : FAILED TO CREATE COREGEN OBJECT" << std::endl;
    return -1;
  }

  // add an SoC
  CoreGenSoC *SoC = CG->InsertSoC( PROJNAME + ".soc" );

  // add a scratchpad
  for( unsigned i=0; i<NUM_CORES; i++ ){
    CoreGenSpad *Spad = CG->InsertSpad( PROJNAME + "." + std::to_string(i) + ".spad",
                                        (i+1)*1024,
                                        1,
                                        1 );
    Spad->SetStartAddr(0x800000000 + ((i+1)*1024));
  }

  // add a memory controller
  std::vector<CoreGenMCtrl *> MCtrls;
  for( unsigned i=0; i<NUM_CORES; i++ ){
    MCtrls.push_back( CG->InsertMCtrl( PROJNAME + "." + std::to_string(i) + ".mctrl", 2 ) );
  }

  // add an ISA
  CoreGenISA *ISA = CG->InsertISA( PROJNAME + ".isa" );

  // add a core
  std::vector<CoreGenCore *> Cores;
  for( unsigned i=0; i<NUM_CORES; i++ ){
    Cores.push_back( CG->InsertCore(PROJNAME + ".core" + std::to_string(i), ISA) );
  }

  // add caches
  std::vector<CoreGenCache *> L1;
  for( unsigned i=0; i<NUM_CORES; i++ ){
    CoreGenCache *Cache1 = CG->InsertCache( PROJNAME + ".core"
                                            + std::to_string(i) + ".L1.cache", 1, 1 );
    L1.push_back(Cache1);
    if( !Cores[i]->InsertCache(Cache1) ){
      std::cout << "ERROR : FAILED TO INSERT CACHE INTO CORE:" <<
        CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  // Shared L2
  CoreGenCache *L2 = CG->InsertCache( PROJNAME + ".L2.cache", 2, 8 );
  for( unsigned i=0; i<NUM_CORES; i++ ){
    if( !L1[i]->SetChildCache( L2 ) ){
      std::cout << "ERROR : FAILED TO SET CHILD CACHE LEVEL:" <<
        CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  // add a register class
  CoreGenRegClass *RC = CG->InsertRegClass( PROJNAME + ".regclass" );
  CoreGenRegClass *RCC = CG->InsertRegClass( PROJNAME + ".csrregclass" );

  // insert the regclass into the cores
  for( unsigned i=0; i<NUM_CORES; i++ ){
    if( !Cores[i]->InsertRegClass( RC ) ){
      std::cout << "ERROR : FAILED TO INSERT REGCLASS INTO CORE:" <<
        CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
    if( !Cores[i]->InsertRegClass( RCC ) ){
      std::cout << "ERROR : FAILED TO INSERT REGCLASS INTO CORE:" <<
        CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  // add a register
  CoreGenReg *Reg[ITER];
  for(unsigned i=0; i<ITER; i++ ){
    Reg[i] = CG->InsertReg( PROJNAME + "." + std::to_string(i) + ".reg", i, 64 );
    Reg[i]->SetPseudoName( "PSEUDOREG."+std::to_string(i) );
    Reg[i]->SetAttrs(CoreGenReg::CGRegRW);

    // insert the reg into the reg class
    if( !RC->InsertReg( Reg[i] ) ){
      std::cout << "ERROR : FAILED TO INSERT REG INTO REGCLASS:" <<
        CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  CoreGenReg *CSR[ITER];
  for(unsigned i=0; i<ITER; i++ ){
    CSR[i] = CG->InsertReg( PROJNAME + "." + std::to_string(i) + ".csr", i, 64 );
    if( CSR[i] == nullptr ){
      std::cout << "ERROR : FAILED TO CREATE NEW REGISTER:" << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
    CSR[i]->SetPseudoName( "CSR."+std::to_string(i) );
    CSR[i]->SetAttrs(CoreGenReg::CGRegCSR);

    if( !RCC->InsertReg(CSR[i]) ){
      std::cout << "ERROR : FAILED TO INSERT REG INTO REGCLASS:" <<
        CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  // insert the core in the SoC
  for( unsigned i=0; i<NUM_CORES; i++ ){
    if( !SoC->InsertCore( Cores[i] ) ){
      std::cout << "ERROR : FAILED TO INSERT CORE INTO SOC:" <<
        CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  // create new instruction format
  CoreGenInstFormat *IF = CG->InsertInstFormat( PROJNAME + ".if", ISA );

  // insert some instruction fields
  //  RT     RA     RB   OPC
  // [31-24][23-16][15-8][7-0]
  if( !IF->InsertField( "opcode", 0, 7, CoreGenInstFormat::CGInstCode, true ) ){
    std::cout << "ERROR : FAILED TO INSERT opcode FIELD INTO FORMAT:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }
  if( !IF->InsertField( "RB", 8, 15, CoreGenInstFormat::CGInstReg, false ) ){
    std::cout << "ERROR : FAILED TO INSERT rb FIELD INTO FORMAT:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }
  if( !IF->InsertField( "RA", 16, 23, CoreGenInstFormat::CGInstReg, false ) ){
    std::cout << "ERROR : FAILED TO INSERT ra FIELD INTO FORMAT:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }
  if( !IF->InsertField( "RT", 24, 31, CoreGenInstFormat::CGInstReg, false ) ){
    std::cout << "ERROR : FAILED TO INSERT rt FIELD INTO FORMAT:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  if( !IF->InsertRegFieldMap("RB", RC) ){
    std::cout << "ERROR : FAILED TO SET rb FIELD REGISTER CLASS:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }
  if( !IF->InsertRegFieldMap("RA", RC) ){
    std::cout << "ERROR : FAILED TO SET ra FIELD REGISTER CLASS:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }
  if( !IF->InsertRegFieldMap("RT", RCC) ){
    std::cout << "ERROR : FAILED TO SET rt FIELD REGISTER CLASS:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // create a new instruction
  CoreGenInst *Inst[ITER];
  CoreGenPseudoInst *PInst[ITER];
  for( unsigned i=0; i<ITER; i++ ){
    Inst[i] = CG->InsertInst( PROJNAME + ".inst" + std::to_string(i),
                              ISA,
                              IF );
    PInst[i] = CG->InsertPseudoInst( PROJNAME + ".pinst" + std::to_string(i),
                                     Inst[i] );

    if( !Inst[i]->SetEncoding("opcode",i+1) ){
      std::cout << "ERROR : FAILED TO SET opcode ENCODING FOR INSTRUCTION: " <<
        Inst[i]->GetName() << " : " << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }

    if( !PInst[i]->SetEncoding("RA",i) ){
      std::cout << "ERROR : FAILED TO SET ra ENCODING FOR PSEUDO INSTRUCTION: " <<
        Inst[i]->GetName() << " : " << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  // insert a new Ext node
  CoreGenExt *E[NUM_CORES];
  for( unsigned i=0; i<NUM_CORES; i++ ){
    E[i] = CG->InsertExt( PROJNAME + ".ext" + std::to_string(i) );
    E[i]->SetRTLFile( E[i]->GetName() + ".v" );

    CoreGenRegClass *ERC = E[i]->InsertRegClass( PROJNAME +
                                              ".ext" + std::to_string(i) +
                                              ".regclass" );
    CoreGenISA *EISA = E[i]->InsertISA( PROJNAME + ".ext" + std::to_string(i) +
                                     ".isa" );
    CoreGenReg *EREG  = E[i]->InsertReg( PROJNAME + ".ext" + std::to_string(i) +
                                      ".reg", 0, 64 );
    if( !ERC->InsertReg(EREG ) ){
      std::cout << "ERROR : FAILED TO INSERT EXT REGISTER INTO EXT REGISTER CLASS : "
        << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  for( unsigned i=0; i<Cores.size(); i++ ){
    if( !Cores[i]->InsertExt( E[i] ) ){
      std::cout << "ERROR : FAILED TO INSERT EXTENSION FOR CORE " << i <<" : "
        << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  // insert a new VTP node
  CoreGenVTP *M = CG->InsertVTP( PROJNAME + ".vtp" );
  if( !M ){
    std::cout << "ERROR : FAILED TO INSERT NEW VIRTUAL TO PHYSICAL NODE "
              << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // insert a comm node
  CoreGenComm *COMM = CG->InsertComm( PROJNAME + ".comm" );
  if( !COMM ){
    std::cout << "ERROR : FAILED TO INSERT NEW COMM NODE "
              << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  if( !COMM->SetCommType(CGCommNoc) ){
    std::cout << "ERROR : FAILED TO SET COMM NODE TYPE "
              << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  if( !COMM->SetWidth(64) ){
    std::cout << "ERROR : FAILED TO SET COMM NODE WIDTH "
              << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  for( unsigned i=0; i<Cores.size(); i++ ){
    if( !COMM->InsertEndpoint( static_cast<CoreGenNode *>(Cores[i]) ) ){
      std::cout << "ERROR : FAILED TO INSERT COMM LINK FOR CORE " << i
                << " : " << CG->GetErrStr() << std::endl;
      delete CG;
      return -1;
    }
  }

  if( !COMM->InsertEndpoint( static_cast<CoreGenNode *>(L2) ) ){
    std::cout << "ERROR : FAILED TO INSERT L2 CACHE INTO COMM LINK "
              << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  if( !COMM->InsertEndpoint( static_cast<CoreGenNode *>(M) ) ){
    std::cout << "ERROR : FAILED TO INSERT VTP LAYER INTO COMM LINK "
              << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // write the yaml
  if( !CG->WriteIR( PROJYAML ) ){
    std::cout << "ERROR : FAILED TO WRITE YAML IR:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  delete CG;
  return 0;
}
