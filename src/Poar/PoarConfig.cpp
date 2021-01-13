//
// _PoarConfig_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/PoarConfig.h"

//
// Contains an array of ConfigEntry structures to define
// the parsing and storage of the configuration values
//
// The format of the struct is [in order]:
// - ConfigType Type: defines the enumerated type of the entry
// - std::string Name: defines the string name of the type (used for parsing)
// - std::string Accum: accumulator object
// - double DefaultVal: the default value if we don't find the value in the parsed config
// - double Value: the value to use (parsed or default) in the calculation
// - double Result: the resulting accumulated value (Width * Value)
//
//
PoarConfig::ConfigEntry PoarConfig::Entries[] = {
  // -- POWER --
  {PoarConfig::POWER_REGBIT,   PoarConfig::PoarPower, PoarConfig::PoarCG, "POWER_REGBIT",   "CoreGenReg",   0.1, 0., 0.},
  {PoarConfig::POWER_DPATHBIT, PoarConfig::PoarPower, PoarConfig::PoarCG, "POWER_DPATHBIT", "CoreGenDPath", 0.1, 0., 0.},
  {PoarConfig::POWER_CPATHBIT, PoarConfig::PoarPower, PoarConfig::PoarCG, "POWER_CPATHBIT", "CoreGenCPath", 0.1, 0., 0.},
  {PoarConfig::POWER_CACHEBIT, PoarConfig::PoarPower, PoarConfig::PoarCG, "POWER_CACHEBIT", "CoreGenCache", 0.1, 0., 0.},
  {PoarConfig::POWER_SPADBIT,  PoarConfig::PoarPower, PoarConfig::PoarCG, "POWER_SPADBIT",  "CoreGenSpad",  0.1, 0., 0.},
  {PoarConfig::POWER_ROMBIT,   PoarConfig::PoarPower, PoarConfig::PoarCG, "POWER_ROMBIT",   "CoreGenROM",   0.1, 0., 0.},
  {PoarConfig::POWER_ALUREGBIT,PoarConfig::PoarPower, PoarConfig::PoarSM, "POWER_ALUREGBIT","ALUReg",       0.1, 0., 0.},
  {PoarConfig::POWER_ALU,      PoarConfig::PoarPower, PoarConfig::PoarSM, "POWER_ALU",      "ALU",          0.1, 0., 0.},
  {PoarConfig::POWER_ALUDPATH, PoarConfig::PoarPower, PoarConfig::PoarSM, "POWER_ALUDPATH", "ALUDPath",     0.1, 0., 0.},
  {PoarConfig::POWER_ALUCPATH, PoarConfig::PoarPower, PoarConfig::PoarSM, "POWER_ALUCPATH", "ALUCPath",     0.1, 0., 0.},

  // -- AREA --
  {PoarConfig::AREA_REGBIT,    PoarConfig::PoarArea, PoarConfig::PoarCG, "AREA_REGBIT",    "CoreGenReg",    1.0, 0., 0.},
  {PoarConfig::AREA_DPATHBIT,  PoarConfig::PoarArea, PoarConfig::PoarCG, "AREA_DPATHBIT",  "CoreGenDPath",  1.0, 0., 0.},
  {PoarConfig::AREA_CPATHBIT,  PoarConfig::PoarArea, PoarConfig::PoarCG, "AREA_CPATHBIT",  "CoreGenCPath",  1.0, 0., 0.},
  {PoarConfig::AREA_CACHEBIT,  PoarConfig::PoarArea, PoarConfig::PoarCG, "AREA_CACHEBIT",  "CoreGenCache",  1.0, 0., 0.},
  {PoarConfig::AREA_SPADBIT,   PoarConfig::PoarArea, PoarConfig::PoarCG, "AREA_SPADBIT",   "CoreGenSpad",   1.0, 0., 0.},
  {PoarConfig::AREA_ROMBIT,    PoarConfig::PoarArea, PoarConfig::PoarCG, "AREA_ROMBIT",    "CoreGenROM",    1.0, 0., 0.},
  {PoarConfig::AREA_ALUREGBIT, PoarConfig::PoarArea, PoarConfig::PoarSM, "AREA_ALUREGBIT", "ALUReg",        0.1, 0., 0.},
  {PoarConfig::AREA_ALU,       PoarConfig::PoarArea, PoarConfig::PoarSM, "AREA_ALU",       "ALU",           0.1, 0., 0.},
  {PoarConfig::AREA_ALUDPATH,  PoarConfig::PoarArea, PoarConfig::PoarSM, "AREA_ALUDPATH",  "ALUDPath",      0.1, 0., 0.},
  {PoarConfig::AREA_ALUCPATH,  PoarConfig::PoarArea, PoarConfig::PoarSM, "AREA_ALUCPATH",  "ALUCPath",      0.1, 0., 0.},

  // -- FINAL ENTRY --
  {PoarConfig::UNK_ENTRY, PoarConfig::PoarPower, PoarConfig::PoarCG, "NULL", "NULL", 0., 0., 0.} //-- this must remain the last entry
};


PoarConfig::PoarConfig(std::string C)
  : Config(C){
  if( Config.length() > 0 ){
    ReadConfig();
  }else{
    LoadDefaultValues();
  }

#if 0
  // debug statement
  PrintConfig();
#endif
}

PoarConfig::~PoarConfig(){
}

bool PoarConfig::SetResult(unsigned Entry, uint64_t Width){
  unsigned i=0;
  bool done = false;
  while( !done ){

    if( i == Entry ){
      Entries[i].Result = Entries[i].Value * (double)(Width);
      return true;
    }

    i++;
    if( Entries[i].Type == UNK_ENTRY )
      done = true;
  }
  return false;
}

PoarConfig::ConfigEntry PoarConfig::GetEntry(unsigned Entry){
  unsigned i=0;
  bool done = false;
  PoarConfig::ConfigEntry NullEntry = {PoarConfig::UNK_ENTRY,
                                       PoarConfig::PoarPower,
                                       PoarConfig::PoarCG,
                                       "NULL", "NULL", 0., 0., 0.};

  while( !done ){

    if( i == Entry ){
      return Entries[i];
    }

    i++;
    if( Entries[i].Type == UNK_ENTRY )
      return Entries[i];
  }
  return NullEntry;
}

unsigned PoarConfig::GetNumEntry(){
  unsigned NE = 0;
  unsigned i  = 0;
  bool done = false;
  while( !done ){
    i++;
    NE++;
    if( Entries[i].Type == UNK_ENTRY )
      return NE;
  }
  return NE;
}

void PoarConfig::PrintConfig(){
  unsigned i=0;
  bool done = false;

  while( !done ){

    std::cout << Entries[i].Name << " = " << Entries[i].Value << std::endl;

    i++;
    if( Entries[i].Type == UNK_ENTRY )
      done = true;
  }
}

void PoarConfig::ReadConfig(){
  YAML::Node MyConfig;
  std::ifstream fin(Config.c_str());
  try{
    MyConfig = YAML::Load(fin);
  }catch(YAML::ParserException& e){
    std::cout << "Error in parsing Yaml configuration file: "
              << Config << std::endl;
    fin.close();
    return ;
  }

  if( MyConfig.IsNull() ){
    std::cout << "Error : could not resolve Yaml configuration file: "
              << Config << std::endl;
    fin.close();
    return ;
  }

  const YAML::Node& CE = MyConfig["PoarConfig"];

  // main parsing loop
  unsigned i = 0;
  bool done = false;
  while( !done ){
    LoadDefaultValue(Entries[i].Type);
    for( unsigned j=0; j<CE.size(); j++ ){
      const YAML::Node& Node  = CE[j];
      if( Node[Entries[i].Name] ){
        Entries[i].Value = Node[Entries[i].Name].as<double>();
      }
    }

    i++;
    if( Entries[i].Type == UNK_ENTRY )
      done = true;
  }

  fin.close();
}

bool PoarConfig::LoadDefaultValue(ConfigType T){
  unsigned i = 0;
  bool done = false;
  while( !done ){
    if( Entries[i].Type == T ){
      Entries[i].Value = Entries[i].DefaultVal;
      return true;
    }

    i++;

    if( Entries[i].Type == UNK_ENTRY )
      done = true;
  }
  return false;
}

void PoarConfig::LoadDefaultValues(){
  // walk the configuration structure and load the default values
  unsigned i = 0;
  bool done = false;
  while( !done ){
    if( !LoadDefaultValue(Entries[i].Type) ){
      std::cout << "Error : could not load default value for entry="
                << Entries[i].Name << std::endl;
      return ;
    }

    i++;

    if( Entries[i].Type == UNK_ENTRY )
      done = true;
  }
}

// EOF
