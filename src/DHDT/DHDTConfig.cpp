//
// _DHDTConfig_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/DHDT/DHDTConfig.h"


//
// Contains an array of ConfigEntry structures to define
// the parsing and storage of the configuration values
//
// The format of the struct is [in order]:
// - ConfigType Type
// - std::string Name
// - double DefaultVal
// - double Value
//
DHDTConfig::ConfigEntry DHDTConfig::Entries[] = {

  // -- generic components --
  {DHDTConfig::REGBIT,      "REGBIT",     0.1,  0.},
  {DHDTConfig::DPATHBIT,    "DPATHBIT",   0.1,  0.},
  {DHDTConfig::CPATHBIT,    "CPATHBIT",   0.1,  0.},
  {DHDTConfig::SPADBIT,     "SPADBIT",    0.1,  0.},
  {DHDTConfig::ROMBIT,      "ROMBIT",     0.1,  0.},

  // -- ALU control/ata components --
  {DHDTConfig::ALUREGBIT,   "ALUREGBIT",  0.1,  0.},
  {DHDTConfig::ALUDPATH,    "ALUDPATH",   0.1,  0.},
  {DHDTConfig::ALUCPATH,    "ALUCPATH",   0.1,  0.},

  // -- ALU ops --
  {DHDTConfig::ALUADD,      "ALUADD",     0.1,  0.},
  {DHDTConfig::ALUFADD,     "ALUFADD",    0.1,  0.},
  {DHDTConfig::ALUSUB,      "ALUSUB",     0.1,  0.},
  {DHDTConfig::ALUFSUB,     "ALUFSUB",    0.1,  0.},
  {DHDTConfig::ALUMUL,      "ALUMUL",     0.1,  0.},
  {DHDTConfig::ALUFMUL,     "ALUFMUL",    0.1,  0.},
  {DHDTConfig::ALUUDIV,     "ALUUDIV",    0.1,  0.},
  {DHDTConfig::ALUSDIV,     "ALUSDIV",    0.1,  0.},
  {DHDTConfig::ALUFDIV,     "ALUFDIV",    0.1,  0.},
  {DHDTConfig::ALUUREM,     "ALUUREM",    0.1,  0.},
  {DHDTConfig::ALUSREM,     "ALUSREM",    0.1,  0.},
  {DHDTConfig::ALUFREM,     "ALUFREM",    0.1,  0.},
  {DHDTConfig::ALUSHL,      "ALUSHL",     0.1,  0.},
  {DHDTConfig::ALULSHR,     "ALULSHR",    0.1,  0.},
  {DHDTConfig::ALUASHR,     "ALUASHR",    0.1,  0.},
  {DHDTConfig::ALUAND,      "ALUAND",     0.1,  0.},
  {DHDTConfig::ALUOR,       "ALUOR",      0.1,  0.},
  {DHDTConfig::ALUXOR,      "ALUXOR",     0.1,  0.},
  {DHDTConfig::ALUICMP,     "ALUICMP",    0.1,  0.},
  {DHDTConfig::ALUFCMP,     "ALUFCMP",    0.1,  0.},
  {DHDTConfig::ALUMUX,      "ALUMUX",     0.1,  0.},

  // -- ALU intrinsic ops --
  {DHDTConfig::ALUMAX,      "ALUMAX",     0.1,  0.},
  {DHDTConfig::ALUMIN,      "ALUMIN",     0.1,  0.},
  {DHDTConfig::ALUNOT,      "ALUNOT",     0.1,  0.},
  {DHDTConfig::ALUNEG,      "ALUNEG",     0.1,  0.},
  {DHDTConfig::ALUREV,      "ALUREV",     0.1,  0.},
  {DHDTConfig::ALUPOPC,     "ALUPOPC",    0.1,  0.},
  {DHDTConfig::ALUCLZ,      "ALUCLZ",     0.1,  0.},
  {DHDTConfig::ALUCTZ,      "ALUCTZ",     0.1,  0.},
  {DHDTConfig::ALUSEXT,     "ALUSEXT",    0.1,  0.},
  {DHDTConfig::ALUZEXT,     "ALUZEXT",    0.1,  0.},
  {DHDTConfig::ALUROTL,     "ALUROTL",    0.1,  0.},
  {DHDTConfig::ALUROTR,     "ALUROTR",    0.1,  0.},
  {DHDTConfig::ALUMAJ,      "ALUMAJ",     0.1,  0.},
  {DHDTConfig::ALUDOZ,      "ALUDOZ",     0.1,  0.},
  {DHDTConfig::ALUCOMP,     "ALUCOMP",    0.1,  0.},
  {DHDTConfig::ALUCOMPM,    "ALUCOMPM",   0.1,  0.},
  {DHDTConfig::ALUINSERTS,  "ALUINSERTS", 0.1,  0.},
  {DHDTConfig::ALUINSERTZ,  "ALUINSERTZ", 0.1,  0.},
  {DHDTConfig::ALUEXTRS,    "ALUEXTRS",   0.1,  0.},
  {DHDTConfig::ALUEXTRZ,    "ALUEXTRZ",   0.1,  0.},
  {DHDTConfig::ALUMERGE,    "ALUMERGE",   0.1,  0.},
  {DHDTConfig::ALUCONCAT,   "ALUCONCAT",  0.1,  0.},
  {DHDTConfig::ALULSS,      "ALULSS",     0.1,  0.},
  {DHDTConfig::ALUFENCE,    "ALUFENCE",   0.1,  0.},
  {DHDTConfig::ALUBSEL,     "ALUBSEL",    0.1,  0.},
  {DHDTConfig::ALUNOP,      "ALUNOP",     0.1,  0.},

  // -- Register ops --
  {DHDTConfig::REGLOAD,     "REGLOAD",    0.1,  0.},
  {DHDTConfig::REGSTORE,    "REGSTORE",   0.1,  0.},

  // -- Memory ops --
  {DHDTConfig::MEMLOAD,     "MEMLOAD",    0.1,  0.},
  {DHDTConfig::MEMSTORE,    "MEMSTORE",   0.1,  0.},

  // -- FINAL ENTRY --
  {DHDTConfig::UNK_ENTRY,   "UNK",        0.1,  0.}
};

DHDTConfig::DHDTConfig(std::string C)
  : Config(C){
  if( Config.length() > 0 ){
    ReadConfig();
  }else{
    LoadDefaultValues();
  }
}

DHDTConfig::~DHDTConfig(){
}

DHDTConfig::ConfigEntry DHDTConfig::GetEntryByType(DHDTConfig::ConfigType Type){
  unsigned i  = 0;
  bool done   = false;
  DHDTConfig::ConfigEntry NullEntry = {DHDTConfig::UNK_ENTRY,
                                       "UNK",
                                       0.,
                                       0.};
  while( !done ){
    if( Entries[i].Type == Type ){
      return Entries[i];
    }
    i++;
    if( Entries[i].Type == UNK_ENTRY )
      return Entries[i];
  }

  return NullEntry;
}

double DHDTConfig::GetValueByType(DHDTConfig::ConfigType Type){
  unsigned i  = 0;
  bool done   = false;
  DHDTConfig::ConfigEntry NullEntry = {DHDTConfig::UNK_ENTRY,
                                       "UNK",
                                       0.,
                                       0.};
  while( !done ){
    if( Entries[i].Type == Type ){
      return Entries[i].Value;
    }
    i++;
    if( Entries[i].Type == UNK_ENTRY )
      return Entries[i].Value;
  }

  return 0.;
}


DHDTConfig::ConfigEntry DHDTConfig::GetEntry(unsigned Entry){
  unsigned i  = 0;
  bool done   = false;
  DHDTConfig::ConfigEntry NullEntry = {DHDTConfig::UNK_ENTRY,
                                       "UNK",
                                       0.,
                                       0.};

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

unsigned DHDTConfig::GetNumEntry(){
  unsigned NE = 0;
  unsigned i  = 0;
  bool done   = false;
  while( !done ){
    i++;
    NE++;
    if( Entries[i].Type == UNK_ENTRY )
      return NE;
  }
  return NE;
}

void DHDTConfig::PrintConfig(){
  unsigned i  = 0;
  bool done   = false;

  while( !done ){
    std::cout << Entries[i].Name << " = " << Entries[i].Value << std::endl;
    i++;
    if( Entries[i].Type == UNK_ENTRY )
      done = true;
  }
}

void DHDTConfig::ReadConfig(){
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

  const YAML::Node& CE = MyConfig["DHDTConfig"];

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

bool DHDTConfig::LoadDefaultValue(ConfigType T){
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

void DHDTConfig::LoadDefaultValues(){
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
