//
// _DOT_TEST31_CPP_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
// 
//

#include <iostream>
#include "CoreGen/CoreGenBackend/CoreGenBackend.h"

std::string PROJNAME = "TEST31";
std::string PROJROOT = "./";
std::string ARCHROOT = "./";
std::string PROJYAML = "TEST31.yaml";

unsigned ITER=16;

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

  // add an ISA
  CoreGenISA *ISA = CG->InsertISA( PROJNAME + ".isa" );

  // add a core
  CoreGenCore *Core = CG->InsertCore( PROJNAME + ".core1", ISA );
  CoreGenCore *Core2 = CG->InsertCore( PROJNAME + ".core2", ISA );

  // add a cache
  CoreGenCache *Cache1 = CG->InsertCache( PROJNAME + ".core1.L1.cache", 1, 1 );
  if( !Core->InsertCache( Cache1 ) ){
    std::cout << "ERROR : FAILED TO INSERT CACHE INTO CORE:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }
  CoreGenCache *Cache2 = CG->InsertCache( PROJNAME + ".core2.L1.cache", 1, 1 );
  if( !Core2->InsertCache(Cache2) ){
    std::cout << "ERROR : FAILED TO INSERT CACHE INTO CORE:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  CoreGenCache *Cache3 = CG->InsertCache( PROJNAME + ".L2.cache", 2, 8 );
  if( !Cache1->SetChildCache( Cache3 ) ){
    std::cout << "ERROR : FAILED TO SET CHILD CACHE LEVEL:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }
  if( !Cache2->SetChildCache( Cache3 ) ){
    std::cout << "ERROR : FAILED TO SET CHILD CACHE LEVEL:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // add a register class
  CoreGenRegClass *RC = CG->InsertRegClass( PROJNAME + ".regclass" );
  CoreGenRegClass *RCC = CG->InsertRegClass( PROJNAME + ".csrregclass" );

  // insert the regclass into the core
  if( !Core->InsertRegClass( RC ) ){
    std::cout << "ERROR : FAILED TO INSERT REGCLASS INTO CORE:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }
  if( !Core->InsertRegClass( RCC ) ){
    std::cout << "ERROR : FAILED TO INSERT REGCLASS INTO CORE:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
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
  if( !SoC->InsertCore( Core ) ){
    std::cout << "ERROR : FAILED TO INSERT CORE INTO SOC:" <<
      CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
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
  CoreGenInst *Inst = CG->InsertInst( PROJNAME + ".inst1",
                                      ISA,
                                      IF );

  // set the field encodings
  if( !Inst->SetEncoding( "opcode", 0x01 ) ){
    std::cout << "ERROR : FAILED TO SET opcode ENCODING FOR INSTRUCTION: " <<
      Inst->GetName() << " : " << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // write the yaml
  if( !CG->WriteIR( PROJYAML ) ){
    std::cout << "ERROR : FAILED TO WRITE YAML IR:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // build the dag
  if( !CG->BuildDAG() ){
    std::cout << "ERROR : FAILED TO BUILD THE DAG:" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  // export the DOT file
  if( !CG->ExportDOT(PROJNAME+".dot") ){
    std::cout << "ERROR : FAILED TO EXPORT THE DOT OUTPUT" << CG->GetErrStr() << std::endl;
    delete CG;
    return -1;
  }

  delete CG;
  return 0;
}
