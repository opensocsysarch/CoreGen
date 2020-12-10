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

PoarIO::PoarIO(PoarConfig *Config, std::string File)
  : PConfig(Config), OutFile(File){
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
      std::cout << " - " << CE.Name;
      CGPrintSpace(CE.Name.length(),20);
      std::cout << CE.Result << " metric" << std::endl;
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
      std::cout << " - " << CE.Name;
      CGPrintSpace(CE.Name.length(),20);
      std::cout << CE.Result << " gates" << std::endl;
    }
  }
  std::cout << "-----------------------------------------------------------------" << std::endl;

  // -- print the totals
  std::cout << "Summary" << std::endl;
  std::cout << "TOTAL POWER = " << TotalPower << " metric" << std::endl;
  std::cout << "TOTAL AREA  = " << TotalArea << " gates" << std::endl;

  return true;
}

bool PoarIO::WriteYaml(){
  if( OutFile.length() == 0 ){
    return false;
  }
  double TotalPower = GetTotalPower();
  double TotalArea  = GetTotalArea();

  // open the output file
  std::ofstream OutYaml(OutFile.c_str());
  YAML::Emitter out(OutYaml);
  out << YAML::BeginMap;

  // write out the total summary
  out << YAML::Key << "Summary";
  out << YAML::BeginSeq;
  out << YAML::BeginMap << YAML::Key << "TotalPower"
      << YAML::Value << TotalPower << YAML::EndMap;
  out << YAML::BeginMap << YAML::Key << "TotalArea"
      << YAML::Value << TotalArea << YAML::EndMap;
  out << YAML::EndSeq;

  // write out the power values
  out << YAML::Key << "Area";
  out << YAML::BeginSeq;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarArea) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      out << YAML::BeginMap;
      out << YAML::Key << CE.Name << YAML::Value << CE.Result;
      out << YAML::EndMap;
    }
  }
  out << YAML::EndSeq;

  // write out the area values
  out << YAML::Key << "Power";
  out << YAML::BeginSeq;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarPower) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      out << YAML::BeginMap;
      out << YAML::Key << CE.Name << YAML::Value << CE.Result;
      out << YAML::EndMap;
    }
  }
  out << YAML::EndSeq;

  // close the file
  out << YAML::EndMap;
  OutYaml.close();

  return true;
}

bool PoarIO::WriteLatex(){
  if( OutFile.length() == 0 ){
    return false;
  }
  double TotalPower = GetTotalPower();
  double TotalArea  = GetTotalArea();
  return true;
}

bool PoarIO::WriteXML(){
  if( OutFile.length() == 0 ){
    return false;
  }
  double TotalPower = GetTotalPower();
  double TotalArea  = GetTotalArea();
  return true;
}

// EOF
