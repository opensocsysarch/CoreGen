//
// _CoreGenMCtrl_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenMCtrl.h"

CoreGenMCtrl::CoreGenMCtrl(std::string N, CoreGenErrno *E)
  : CoreGenNode(CGMCtrl,N,E), InputPorts(0) {
}

CoreGenMCtrl::CoreGenMCtrl(std::string N, CoreGenErrno *E, unsigned P)
  : CoreGenNode(CGMCtrl,N,E), InputPorts(P) {
}

CoreGenMCtrl::~CoreGenMCtrl(){
}

bool CoreGenMCtrl::SetNumInputPorts(unsigned P){
  InputPorts = P;
  return true;
}

unsigned CoreGenMCtrl::GetNumInputPorts(){
  return InputPorts;
}
