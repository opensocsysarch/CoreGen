//
// _CoreGenMCtrl_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenMCtrl.h"

CoreGenMCtrl::CoreGenMCtrl(std::string N, CoreGenErrno *E)
  : CoreGenNode(CGMCtrl,N,E), InputPorts(0), Order(CGWeak) {
}

CoreGenMCtrl::CoreGenMCtrl(std::string N, CoreGenErrno *E, unsigned P)
  : CoreGenNode(CGMCtrl,N,E), InputPorts(P), Order(CGWeak) {
}

CoreGenMCtrl::CoreGenMCtrl(std::string N, CoreGenErrno *E, unsigned P, CGMemOrder O)
  : CoreGenNode(CGMCtrl,N,E), InputPorts(P), Order(O) {
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

bool CoreGenMCtrl::SetMemOrder(CGMemOrder O){
  Order = O;
  return true;
}

// EOF
