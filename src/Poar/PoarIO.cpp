//
// _PoarIO_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/PoarIO.h"

PoarIO::PoarIO(PoarConfig *Config)
  : PConfig(Config){
}

PoarIO::~PoarIO(){
}

double PoarIO::GetTotalPower(){
  double result = 0.;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarPower) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      result += CE.Result;
    }
  }
  return result;
}

double PoarIO::GetTotalArea(){
  double result = 0.;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarArea) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      result += CE.Result;
    }
  }
  return result;
}

bool PoarIO::WriteText(){
  double TotalPower = GetTotalPower();
  double TotalArea  = GetTotalArea();

  // -- power
  std::cout << "Power Values" << std::endl;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarPower) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      std::cout << " - " << CE.Name << std::setw(20) << CE.Result << " metric" << std::endl;
    }
  }
  std::cout << "-----------------------------------------------------------------" << std::endl;

  // -- area
  std::cout << "Area Values" << std::endl;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarArea) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      std::cout << " - " << CE.Name << std::setw(20) << CE.Result << " gates" << std::endl;
    }
  }
  std::cout << "-----------------------------------------------------------------" << std::endl;

  // -- print the totals
  std::cout << "Summary" << std::endl;
  std::cout << "TOTAL POWER = " << std::setw(10) << TotalPower << " metric" << std::endl;
  std::cout << "TOTAL AREA  = " << std::setw(10) << TotalArea  << " gates" << std::endl;

  return true;
}

bool PoarIO::WriteYaml(){
  return true;
}

bool PoarIO::WriteLatex(){
  return true;
}

bool PoarIO::WriteXML(){
  return true;
}

// EOF
