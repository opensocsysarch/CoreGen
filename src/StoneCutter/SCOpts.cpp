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
  isDisable(false), isEnable(false), isListPass(false),
  Msgs(M) {}

// ------------------------------------------------- DESTRUCTOR
SCOpts::~SCOpts(){}

// ------------------------------------------------- SPLIT
void SCOpts::Split(const std::string& s, char delim,
                      std::vector<std::string>& v) {
  auto i = 0;
  auto pos = s.find(delim);
  if( pos == std::string::npos ){
    v.push_back(s.substr(i,s.length()));
  }
  while( pos != std::string::npos ){
    v.push_back(s.substr(i,pos-i));
    i = ++pos;
    pos = s.find(delim,pos);
    if( pos == std::string::npos ){
      v.push_back(s.substr(i,s.length()));
    }
  }
}

// ------------------------------------------------- PARSEPASSES
std::vector<std::string> SCOpts::ParsePasses( std::string P ){
  std::vector<std::string> V;

  // exit if the string is null
  if( P.length() == 0 ){
    Msgs->PrintMsg(L_WARN, "Warning: No passes found for pass argument");
    return V;
  }

  std::string Str = P;  // local parsing string

  // break the string into tokens
  Split( Str, ',', V );

  return V;
}

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
    }else if( (s=="-f") || (s=="-outfile") || (s=="--outfile") ){
      if( i+1 > (argc-1) ){
        Msgs->PrintMsg(L_ERROR, "--outfile requires an argument");
        return false;
      }
      std::string F(argv[i+1]);
      OutFile = F;
      i++;
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
    }else if( s=="--list-passes"){
      isListPass = true;
    }else if( s=="--enable-pass"){
      if( i+1 > (argc-1) ){
        Msgs->PrintMsg(L_ERROR, "--enable-pass requires an argument");
        return false;
      }
      std::string P(argv[i+1]);
      std::vector<std::string> tmpV1 = ParsePasses(P);
      EnablePass.insert(EnablePass.end(),tmpV1.begin(),tmpV1.end());
      if( tmpV1.size() > 0 ){
        isEnable = true;
        isDisable = false;
      }else{
        Msgs->PrintMsg(L_ERROR, "--enable-pass requires an argument");
        return false;
      }
      i++;
    }else if( s=="--disable-pass"){
      if( i+1 > (argc-1) ){
        Msgs->PrintMsg(L_ERROR, "--disable-pass requires an argument");
        return false;
      }
      std::string P(argv[i+1]);
      std::vector<std::string> tmpV2 = ParsePasses(P);
      DisablePass.insert(DisablePass.end(),tmpV2.begin(),tmpV2.end());
      if( tmpV2.size() > 0 ){
        isEnable = false;
        isDisable = true;
      }else{
        Msgs->PrintMsg(L_ERROR, "--disable-pass requires an argument");
        return false;
      }
      i++;
    }else{
      // assume this is a file space member
      FileList.push_back(s);
    }
  }

  // is raw parsing is disabled, enable the codegen
  if( !isParse ){
    isCG = true;
  }

  if( (!*isHelp) && (FileList.size() == 0) && (!isListPass) ){
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
  Msgs->PrintRawMsg("Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC");
  Msgs->PrintRawMsg("SCCOMP Version " + std::to_string(SC_MAJOR_VERSION)
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
  Msgs->PrintRawMsg("     -f|-outfile|--outfile /path/to/out  : Set the output file name");
  Msgs->PrintRawMsg("     -o|-object|--object                 : Generate target object file [*.o]");
  Msgs->PrintRawMsg("     -V|-version|--version               : Print the version info");
  Msgs->PrintRawMsg(" ");
  Msgs->PrintRawMsg("Execution Options:");
  Msgs->PrintRawMsg("     -O|-optimize|--optimize             : Execute the optimizer" );
  Msgs->PrintRawMsg("     -N|-no-optimize|--no-optimize       : Do not execute the optimizer" );
  Msgs->PrintRawMsg("     -D|-disable-chisel|--disable-chisel : Disables Chisel output" );
  Msgs->PrintRawMsg("     -v|-verbose|--verbose               : Enable verbosity");
  Msgs->PrintRawMsg(" ");
  Msgs->PrintRawMsg("Optimization Pass Options:");
  Msgs->PrintRawMsg("     --list-passes                       : Lists all the LLVM passes");
  Msgs->PrintRawMsg("     --enable-pass \"PASS1,PASS2\"         : Enables individual passes");
  Msgs->PrintRawMsg("     --disable-pass \"PASS1,PASS2\"        : Disables individual passes");
}

// EOF
