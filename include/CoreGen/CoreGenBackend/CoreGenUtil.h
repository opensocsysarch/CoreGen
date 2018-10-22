//
// _CoreGenUtil_h_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREGENUTIL_H_
#define _COREGENUTIL_H_

#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#define STRINGIZE_INSTALL_PREFIX(CGIPREFIX) #CGIPREFIX
#define _COREGEN_INSTALL_PREFIX_ STRINGIZE_INSTALL_PREFIX(CGIPREFIX)

/// CoreGenUtil: Retrieve the build date
inline std::string CGBuildDate(){
  const char *__CoreGenBuildDate = __DATE__;
  return std::string(__CoreGenBuildDate);
}

/// CoreGenUtil: Retrieve the build time
inline std::string CGBuildTime(){
  const char *__CoreGenBuildTime = __TIME__;
  return std::string(__CoreGenBuildTime);
}

/// CoreGenUtil: Retrieve the current date+time
inline std::string CGCurrentDateTime(){
  time_t now = time(0);
  std::string DT(ctime(&now));
  return DT;
}

/// CoreGenUtil: Create the directory
inline bool CGMkDir(const std::string& dir){
#if defined(_WIN32)
  if( _mkdir(dir.c_str()) != 0 ){
    return false;
  }
  return true;
#else
  if( mkdir(dir.c_str(), 0777) != 0 ){
    return false;
  }
  return true;
#endif
}

/// CoreGenUtil: Check to see if the file exists
inline bool CGFileExists(const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

/// CoreGenUtil: Copy a source to a destination
inline bool CGFileCopy(const std::string& src,
                       const std::string& dest ){
  std::ifstream  Fsrc(src, std::ios::binary);
  std::ofstream  Fdst(dest, std::ios::binary);

  Fdst << Fsrc.rdbuf();
  return true;
}

/// CoreGenUtil: Delete the target file
inline bool CGDeleteFile(const std::string& name){
  if( remove(name.c_str()) != 0 ){
    return false;
  }else{
    return true;
  }
}

/// CoreGenUtil: Record the current wallclock time
inline double CGGetWallTime(){
  struct timeval tp;
  struct timezone tzp;

  gettimeofday( &tp, &tzp );
  return ( (double) tp.tv_sec + ((double) tp.tv_usec * 1.e-6) );
}

/// CoreGenUtil: Print a specific number of spaces for formatted printing
inline void CGPrintSpace( unsigned Buf, unsigned NDots ){
  for( unsigned i=0; i<(NDots-Buf); i++ ){
    std::cout << " ";
  }
}

/// CoreGenUtil: Print a specific number of spaces for formatted printing
inline std::string CGPrintSpaceStr( unsigned Buf, unsigned NDots ){
  std::string DOTS;
  for( unsigned i=0; i<(NDots-Buf); i++ ){
    DOTS = DOTS + " ";
  }
  return DOTS;
}

/// CoreGenUtil: Print a specific number of dots for formatted printing
inline void CGPrintDots( unsigned Buf, unsigned NDots ){
  for( unsigned i=0; i<(NDots-Buf); i++ ){
    std::cout << ".";
  }
}

/// CoreGenUtil: Print a specific number of dots for formatted printing
inline std::string CGPrintDotStr( unsigned Buf, unsigned NDots ){
  std::string DOTS;
  for( unsigned i=0; i<(NDots-Buf); i++ ){
    DOTS = DOTS + ".";
  }
  return DOTS;
}

/// CoreGenUtil: Convert double to string in pure format
inline std::string CGDoubleToStr( double d ){
  std::stringstream s;
  s << d;
  return s.str();
}

/// CoreGenUtil: Return the default installation location
inline std::string CGInstallPrefix(){
  return _COREGEN_INSTALL_PREFIX_;
}

#if defined(__clang__) || defined (__GNUC__)
# define ATTRIBUTE_NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
#else
# define ATTRIBUTE_NO_SANITIZE_ADDRESS
#endif

#endif

// EOF
