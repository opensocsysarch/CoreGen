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
// - double DefaultVal: the default value if we don't find the value in the parsed config
// - double Value: the value to use (parsed or default) in the calculation
//
//
PoarConfig::ConfigEntry PoarConfig::Entries[] = {
  // -- POWER --
  {PoarConfig::POWER_REGBIT,    "POWER_REGBIT",   0.1, 0.},
  {PoarConfig::POWER_DPATHBIT,  "POWER_DPATHBIT", 0.1, 0.},
  {PoarConfig::POWER_CPATHBIT,  "POWER_CPATHBIT", 0.1, 0.},
  {PoarConfig::POWER_CACHEBIT,  "POWER_CACHEBIT", 0.1, 0.},
  {PoarConfig::POWER_SPADBIT,   "POWER_SPADBIT",  0.1, 0.},
  {PoarConfig::POWER_ROMBIT,    "POWER_ROMBIT",   0.1, 0.},

  // -- AREA --
  {PoarConfig::AREA_REGBIT,     "AREA_REGBIT",    1.0, 0.},
  {PoarConfig::AREA_DPATHBIT,   "AREA_DPATHBIT",  1.0, 0.},
  {PoarConfig::AREA_CPATHBIT,   "AREA_CPATHBIT",  1.0, 0.},
  {PoarConfig::AREA_CACHEBIT,   "AREA_CACHEBIT",  1.0, 0.},
  {PoarConfig::AREA_SPADBIT,    "AREA_SPADBIT",   1.0, 0.},
  {PoarConfig::AREA_ROMBIT,     "AREA_ROMBIT",    1.0, 0.},

  // -- FINAL ENTRY --
  {PoarConfig::UNK_ENTRY, "NULL", 0., 0.} //-- this must remain the last entry
};


PoarConfig::PoarConfig(std::string C)
  : Config(C){
  if( Config.length() > 0 ){
    ReadConfig();
  }else{
    LoadDefaultValues();
  }

  PrintConfig();
}

PoarConfig::~PoarConfig(){
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
