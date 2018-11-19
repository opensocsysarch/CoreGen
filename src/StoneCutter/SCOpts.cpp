//
// _SCOpts_cpp_
//
// Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCOpts.h"

// ------------------------------------------------- CONSTRUCTOR
SCOpts::SCOpts(SCMsg *M, int A, char **C)
  : argc(A), argv(C),
  isKeep(false), isParse(true), isIR(true),
  isOptimize(true), isChisel(true), isCG(false), isVerbose(false),
  Msgs(M) {}

// ------------------------------------------------- DESTRUCTOR
SCOpts::~SCOpts(){}

// ------------------------------------------------- PARSEOPTS
bool SCOpts::ParseOpts(bool *isHelp){

  // sanity check the args
  if( argc == 1 ){
    Msgs->PrintMsg( L_ERROR,  "No input files found" );
    return false;
  }

  // walk the args
  for( int i=1; i<argc; i++ ){
    // grab the string and tokenize it
    std::string s(argv[i]);

    if( (s=="-h") || (s=="-help") || (s=="--help") ){
      *isHelp = true;
      this->PrintHelp();
      return true;
    }else if( (s=="-p") || (s=="-parse") || (s=="--parse") ){
      isParse = true;   // enable parsing
      isCG = false;     // disable object files
      isChisel = false; // disable chisel
    }else if( (s=="-c") || (s=="-chisel") || (s=="--chisel") ){
      isChisel = true;
    }else if( (s=="-o") || (s=="-object") || (s=="--object") ){
      isCG = true;
    }else if( (s=="-k") || (s=="-keep") || (s=="--keep") ){
      isKeep = true;
    }else if( (s=="-D") || (s=="-disable-chisel") || (s=="--disable-chisel") ){
      isChisel = false;
    }else if( (s=="-O") || (s=="-optimize") || (s=="--optimize") ){
      isOptimize = true;
    }else if( (s=="-N") || (s=="-no-optimize") || (s=="--no-optimize") ) {
      isOptimize = false;
    }else if( (s=="-v") || (s=="-verbose") || (s=="--verbose") ) {
      isVerbose = true;
    }else if( (s=="-V") || (s=="-version") || (s=="--version") ) {
      this->PrintVersion();
      *isHelp = true;
    }else{
      // assume this is a file space member
      FileList.push_back(s);
    }
  }

  // is raw parsing is disabled, enable the codegen
  if( !isParse ){
    isCG = true;
  }

  if( !*isHelp && (FileList.size() == 0) ){
    Msgs->PrintMsg( L_ERROR, "No input files found" );
    return false;
  }

  return true;
}

// ------------------------------------------------- GetInputFile
std::string SCOpts::GetInputFile( unsigned F ){
  if( F > (FileList.size()-1) ){
    return "";
  }
  return FileList[F];
}

// ------------------------------------------------- PRINTVERSION
void SCOpts::PrintVersion(){
  Msgs->PrintRawMsg("Version " + std::to_string(SC_MAJOR_VERSION)
                    + "." + std::to_string(SC_MINOR_VERSION)
                    + "." + std::to_string(SC_PATCH_VERSION));
}

// ------------------------------------------------- PRINTHELP
void SCOpts::PrintHelp(){
  Msgs->PrintRawMsg("sccomp [Options] /path/to/input.sc");
  Msgs->PrintRawMsg("Options:");
  Msgs->PrintRawMsg("     -h|-help|--help                     : Print the help menu");
  Msgs->PrintRawMsg("     -k|-keep|--keep                     : Keep intermediate files");
  Msgs->PrintRawMsg("     -c|-chisel|--chisel                 : Generate Chisel output (default=on");
  Msgs->PrintRawMsg("     -p|-parse|--parse                   : Parse but do not compile");
  Msgs->PrintRawMsg("     -o|-object|--object                 : Generate target object file [*.o]");
  Msgs->PrintRawMsg("     -V|-version|--version               : Print the version info");
  Msgs->PrintRawMsg(" ");
  Msgs->PrintRawMsg("Execution Options:");
  Msgs->PrintRawMsg("     -O|-optimize|--optimize             : Execute the optimizer" );
  Msgs->PrintRawMsg("     -N|-no-optimize|--no-optimize       : Do not execute the optimizer" );
  Msgs->PrintRawMsg("     -D|-disable-chisel|--disable-chisel : Disables Chisel output" );
  Msgs->PrintRawMsg("     -v|-verbose|--verbose               : Enable verbosity");
}

// EOF
