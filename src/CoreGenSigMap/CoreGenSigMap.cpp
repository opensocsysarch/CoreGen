//
// _CoreGenSigMap_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenSigMap/CoreGenSigMap.h"

CoreGenSigMap::CoreGenSigMap(){
}

CoreGenSigMap::~CoreGenSigMap(){
  Signals.clear();
}

bool CoreGenSigMap::InsertSignal( SCSig *S ){
  if( S == nullptr )
    return false;

  Signals.push_back( S );
  return true;
}

SCSig *CoreGenSigMap::GetSignal( unsigned Idx ){
  if( Idx > (Signals.size()-1) )
    return nullptr;

  return Signals[Idx];
}

bool CoreGenSigMap::WriteSigMap( std::string File ){
  if( File.length() == 0 )
    return false;
  if( Signals.size() == 0 )
    return false;

  // open the file
  std::ofstream OutYaml(File.c_str());

  // open the yaml emitter
  YAML::Emitter out(OutYaml);

  // open the yaml stream
  out << YAML::BeginMap;

  // write the opt-level signals
  if( !WriteTopLevelSignals(&out) ){
    OutYaml.close();
    return false;
  }

  // write the individual instruction signals
  if( !WriteInstSignals(&out) ){
    OutYaml.close();
    return false;
  }

  // end the YAML stream
  out << YAML::EndMap;

  // close the file
  OutYaml.close();

  return true;
}

bool CoreGenSigMap::ReadSigMap( std::string File ){
  return true;
}

bool CoreGenSigMap::WriteInstSignals(YAML::Emitter *out){
  if( out == nullptr )
    return false;

  *out << YAML::Key << "Instructions" << YAML::BeginSeq;

  // walk all the signals and derive the names
  std::vector<std::string> SigNames;
  for( unsigned i=0; i<Signals.size(); i++ ){
    if( SigNames.size() > 0 ){
      bool found = false;
      for( unsigned j=0; j<SigNames.size(); j++ ){
        if( Signals[i]->GetInst() == SigNames[j] )
          found = true;
      }
      if( !found )
        SigNames.push_back(Signals[i]->GetInst());
    }else{
      SigNames.push_back(Signals[i]->GetInst());
    }
  }

  // walk all the signal names, derive all the signals and write them out
  for( unsigned i=0; i<SigNames.size(); i++ ){
    std::vector<SCSig *> Sigs;

    // retrieve all the signals for the target instruction
    for( unsigned j=0; j<Signals.size(); j++ ){
      if( Signals[j]->GetInst() == SigNames[i] )
        Sigs.push_back(Signals[j]);
    }

    // make the vector unique
    std::vector<SCSig *> CSigs;

    // build a unique vector of all the local instruction signals
    for( unsigned j=0; j<Sigs.size(); j++ ){
      if( CSigs.size() > 0 ){
        bool found = false;
        for( unsigned k=0; k<CSigs.size(); k++ ){
          if( Sigs[j]->GetType() == CSigs[k]->GetType() )
            found = true;
        }
        if( !found )
          CSigs.push_back(Sigs[j]);
      }else{
        CSigs.push_back(Sigs[j]);
      }
    }

    // write out the instruction
    *out << YAML::BeginMap << YAML::Key << "Inst" << YAML::Value << SigNames[i];
    for( unsigned j=0; j<CSigs.size(); j++ ){
      *out << YAML::Key << "Signal" << YAML::Value << CSigs[j]->SigTypeToStr();
    }
    *out << YAML::EndMap;

  }

  *out << YAML::EndSeq;

  return true;
}

bool CoreGenSigMap::WriteTopLevelSignals(YAML::Emitter *out){
  if( out == nullptr )
    return false;

  // make a copy of the signal vector
  std::vector<SCSig *> Sigs;

  // build a unique vector of all the signals
  for( unsigned i=0; i<Signals.size(); i++ ){
    if( Sigs.size() > 0 ){
      bool found = false;
      for( unsigned j=0; j<Sigs.size(); j++ ){
        if( Signals[i]->GetType() == Sigs[j]->GetType() )
          found = true;
      }
      if( !found )
        Sigs.push_back(Signals[i]);
    }else{
      Sigs.push_back(Signals[i]);
    }
  }

  // write each signal out
  *out << YAML::Key << "SignalTop" << YAML::BeginSeq << YAML::BeginMap;
  for( unsigned i=0; i<Sigs.size(); i++ ){
    *out << YAML::Key << "Signal" << YAML::Value << Sigs[i]->SigTypeToStr();
  }
  *out << YAML::EndMap << YAML::EndSeq;

  return true;
}

// EOF
