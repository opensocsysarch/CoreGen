//
// _CoreGenUtil_h_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREGENUTIL_H_
#define _COREGENUTIL_H_

#include <sys/stat.h>
#include <sys/time.h>
#include <cerrno>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
#include <sstream>
#include <algorithm>

#define STRINGIZE_INSTALL_PREFIX(CGIPREFIX) #CGIPREFIX
#define _COREGEN_INSTALL_PREFIX_ STRINGIZE_INSTALL_PREFIX(CGIPREFIX)

#define QuoteCGIPrefix(CGIPREFIX) #CGIPREFIX
#define QuoteMacro(macro) QuoteCGIPrefix(macro)

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
  if( mkdir(dir.c_str(), 0777) == -1 ){
    return false;
  }
  return true;
#endif
}

/// CoreGenUtil: Check to see if a directory exists
inline bool CGDirExists(const char *path){
  struct stat info;

  if(stat( path, &info ) != 0)
    return false;
  else if(info.st_mode & S_IFDIR)
    return true;
  else
    return false;
}

/// CoreGenUtil: Recursively create directories
inline bool CGMkDirP(const std::string& dir){
  // build up a vector of directory names
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(dir);
  char delim = '/';
  while (std::getline(tokenStream, token, delim)){
    tokens.push_back(token);
  }

  // iteratively build the directory structure
  std::string newDir;
  for( unsigned i=0; i<tokens.size(); i++ ){
    newDir += (tokens[i] + "/");
    if( !CGDirExists(newDir.c_str()) ){
      // make a new directory
      if( !CGMkDir(newDir) ){
        return false;
      }
    }
  }

  return true;
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

/// CoreGenUtil: Recursively copy from the src directory to the target
inline bool CGCopyR(const std::string &src,
                    const std::string &target ){
  std::string UncStr = "cp -R " + src + "/* " + target + "/";
  if( system( UncStr.c_str() ) == 0 )
    return true;
  else{
    return false;
  }
}

/// CoreGenUtil: Delete the target directory
inline bool CGDeleteDir(const std::string& path){

  std::string UncStr = "rm -Rf " + path;
  if( system( UncStr.c_str() ) == 0 )
    return true;
  else{
    return false;
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

/// CoreGenUtil: Remove any dots (.) in the input string
inline std::string CGRemoveDot( std::string in ){
  std::string out = in;
  out.erase(std::remove(out.begin(),out.end(),'.'),out.end());
  return out;
}

/// CoreGenUtil: Return the default installation location
inline std::string CGInstallPrefix(){
  return QuoteMacro(CGIPREFIX);
}

#if defined(__clang__) || defined (__GNUC__)
# define ATTRIBUTE_NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
#else
# define ATTRIBUTE_NO_SANITIZE_ADDRESS
#endif

#endif

// EOF
