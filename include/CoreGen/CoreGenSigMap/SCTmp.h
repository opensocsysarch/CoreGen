//
// _SCTmp_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class SCTmp
 *
 * \ingroup CoreGenSigMap
 *
 * \brief CoreGen ALU Temporary Register
 *
 */

#ifndef _SCTMP_H_
#define _SCTMP_H_

#include <string>
#include <vector>

class SCTmp{
private:
  std::string Name;         ///< SCTmp: Name of the temporary
  unsigned Width;           ///< SCTmp: Width of the register in bits

  std::vector<std::pair<std::string,std::string>> InstMap;  ///< SCTmp: Vector mapping instruction and IR pairs

public:
  /// Default constructor
  SCTmp( std::string Name, unsigned Width,
         std::string Inst, std::string IRName );

  /// Default destructor
  ~SCTmp();

  /// Insert a new instruction:IR pair
  bool InsertTmpPair( std::string Inst, std::string IRName );

  /// Retrieve the width
  unsigned GetWidth() { return Width; }

  /// Retrieve the temporary register name
  std::string GetName() { return Name; }

  /// Retrieve the number of temporary variable mappings
  unsigned GetNumMappings() { return InstMap.size(); }

  /// Retrieve the target map at the desired index
  std::pair<std::string,std::string> GetMap(unsigned i);

  /// Retrieve a vector of all the IRNames for a given instruction
  std::vector<std::string> GetInstIRNames(std::string Inst);

  /// Retrieve a vector of all the Inst names for a given IRName
  std::vector<std::string> GetIRInstNames(std::string IRName);

  /// Determine if the target instruction name has a mapping to this temp
  bool IsInstFound(std::string Inst);

  /// Determine if the target IR name has a mapping to this temp
  bool IsIRFound(std::string IRName);

  /// Determine if the Inst:IRName pair is found
  bool IsPairFound(std::string Inst, std::string IRName);
};

#endif

// EOF
