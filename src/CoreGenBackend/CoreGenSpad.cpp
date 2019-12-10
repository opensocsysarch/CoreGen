//
// _CoreGenSpad_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/CoreGenSpad.h"

CoreGenSpad::CoreGenSpad(std::string N, CoreGenErrno *E)
  : CoreGenNode(CGSpad,N,E), MemSize(0), RqstPorts(0), RspPorts(0),
    StartAddr(0x00ull) {
}

CoreGenSpad::CoreGenSpad(std::string N, CoreGenErrno *E,
                         unsigned MSize, unsigned RqPorts, unsigned RsPorts)
  : CoreGenNode(CGSpad,N,E), MemSize(MSize), RqstPorts(RqPorts), RspPorts(RsPorts) {
}

CoreGenSpad::~CoreGenSpad(){
}

bool CoreGenSpad::SetMemSize( unsigned M ){
  MemSize = M;
  return true;
}

bool CoreGenSpad::SetRqstPorts( unsigned P ){
  RqstPorts = P;
  return true;
}

bool CoreGenSpad::SetRspPorts( unsigned P ){
  RspPorts = P;
  return true;
}

bool CoreGenSpad::SetStartAddr( uint64_t Addr ){
  StartAddr = Addr;
  return true;
}

unsigned CoreGenSpad::GetMemSize(){
  return MemSize;
}

unsigned CoreGenSpad::GetRqstPorts(){
  return RqstPorts;
}

unsigned CoreGenSpad::GetRspPorts(){
  return RspPorts;
}

uint64_t CoreGenSpad::GetStartAddr(){
  return StartAddr;
}

// EOF
