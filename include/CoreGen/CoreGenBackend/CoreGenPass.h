//
// _CoreGenPass_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

/**
 * \class CoreGenPass
 *
 * \ingroup CoreGen
 *
 * \brief CoreGen Pass Template Class
 *
 */

#ifndef _COREGENPASS_H_
#define _COREGENPASS_H_

#include <iostream>
#include <fstream>
#include <string>

#include "CoreGen/CoreGenBackend/CoreGenDAG.h"
#include "CoreGen/CoreGenBackend/CoreGenNode.h"
#include "CoreGen/CoreGenBackend/CoreGenErrno.h"
#include "CoreGen/CoreGenBackend/CoreGenUtil.h"

typedef enum{
  CGPassAnalysis  = 1,      ///< CGPassType: Analysis passes
  CGPassOpt       = 2,      ///< CGPassType: Optimization passes
  CGPassData      = 3,      ///< CGPassType: Data (printing) pass
  CGPassUnk       = 100     ///< CGPassType: Unknown pass type
}CGPassType;                ///< CoreGenPass: Pass types

class CoreGenPass {
private:
  CGPassType Type;          ///< CoreGenPass: Type of pass
  unsigned Level;           ///< CoreGenPass: Execution level
  CoreGenNode *TNode;       ///< CoreGenPass: Target node for pass inquiry
  bool ReqTarget;           ///< CoreGenPass: Does the pass require a target node?
  std::string OutPath;      ///< CoreGenPass: Output path for data
  std::string InStr;        ///< CoreGenPass: Input string
  std::string PassName;     ///< CoreGenPass: Pass name
  std::string Desc;         ///< CoreGenPass: Pass Description
  std::ostream *OutStream;  ///< CoreGenPass: Output stream

  CoreGenDAG *GetDAGLevel() {
    if( DAG == nullptr ){
      return nullptr;
    }
    return DAG->GetDAGFromLevel(this->GetLevel());
  }

  std::string TypeToStr(){
    switch(Type){
    case CGPassAnalysis:
      return "Analysis";
      break;
    case CGPassOpt:
      return "Optimization";
      break;
    case CGPassData:
      return "Data";
      break;
    case CGPassUnk:
      return "Unknown";
    default:
      break;
    }
    return "Unknown";
  }

protected:
  CoreGenDAG *DAG;          ///< CoreGenPass: DAG
  CoreGenErrno *Errno;      ///< CoreGenPass: Errno handler

public:

  /// Default constructor
  CoreGenPass(CGPassType T,
              unsigned L,
              std::string P,
              std::string De,
              bool RT,
              std::ostream *O,
              CoreGenDAG *D,
              CoreGenErrno *E) : Type(T), Level(L), TNode(nullptr), ReqTarget(RT),
                                 OutPath(""), InStr(""), PassName(P), Desc(De),
                                 OutStream(O), DAG(D),
                                 Errno(E) {}

  /// Default destructor
  ~CoreGenPass() {}

  /// Execute the pass
  virtual bool Execute() = 0;

  /// Set the target node for the inquiry
  bool SetTargetNode( CoreGenNode *N ){
    if( N ){ TNode = N; return true; } return false;
  }

  /// Queries to see whether the pass requires a target node
  bool IsTargetRequired(){
    return ReqTarget;
  }

  /// Sets the output path
  bool SetOutputPath(std::string OP){
    if( OP.length() > 0 ){
      OutPath = OP;
      return true;
    }
    return false;
  }

  /// Sets the input string
  bool SetInStr(std::string In){
    if( In.length() > 0 ){
      InStr = In;
      return true;
    }
    return false;
  }

  /// Retrieves the output path
  std::string GetOutputPath() { return OutPath; }

  /// Retrieves the input stirng
  std::string GetInStr() { return InStr; }

  /// Retrieve the type of pass
  CGPassType GetPassType() { return Type; }

  /// Retrieve the pass name
  std::string GetName() { return PassName; }

  /// Get the pass level
  unsigned GetLevel() { return Level; }

  /// Retrieve the target node pointer
  CoreGenNode *GetTargetNode() { return TNode; }

  /// Write a message to the status stream
  void WriteMsg( std::string Msg ){
    *OutStream << this->GetName() << " : " << Msg << std::endl;
  }

  /// Write the pass info to the status stream
  void PrintPassInfo(){
    *OutStream << this->GetName();
    CGPrintSpace( this->GetName().length(), 30 );
    *OutStream << this->TypeToStr();
    CGPrintSpace( this->TypeToStr().length(), 15 );
    *OutStream << Desc << std::endl;
  }

  /// Retrieve the pass description info
  std::string GetPassDesc() { return Desc; }
};

#endif

// EOF
