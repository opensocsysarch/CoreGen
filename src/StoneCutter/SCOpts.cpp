//
// _SCOpts_cpp_
//
// Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/StoneCutter/SCOpts.h"

// ------------------------------------------------- CONSTRUCTOR
SCOpts::SCOpts(SCMsg *M)
  : argc(0), argv(NULL),
  isKeep(false), isParse(true), isIR(true),
  isOptimize(true), isChisel(true), isCG(false), isVerbose(false),
  isDisable(false), isEnable(false), isListPass(false), isListSCPass(false),
  isSigMap(false), isPassRun(false), isPerf(false),
  isSCDisable(false),isSCEnable(false),
  Msgs(M) {}

SCOpts::SCOpts(SCMsg *M, int A, char **C)
  : argc(A), argv(C),
  isKeep(false), isParse(true), isIR(true),
  isOptimize(true), isChisel(true), isCG(false), isVerbose(false),
  isDisable(false), isEnable(false), isListPass(false), isListSCPass(false),
  isSigMap(false), isPassRun(false), isPerf(false),
  isSCDisable(false),isSCEnable(false),
  Msgs(M) {}

// ------------------------------------------------- DESTRUCTOR
SCOpts::~SCOpts(){}

// ------------------------------------------------- FIND_DASH
bool SCOpts::FindDash(const std::string &s){
  auto pos = s.find('-');
  if( pos == std::string::npos ){
    return false;
  }
  return true;
}

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

// ------------------------------------------------- SCREMOVEDOT
std::string SCOpts::SCRemoveDot( std::string in ){
  std::string out = in;
  out.erase(std::remove(out.begin(),out.end(),'.'),out.end());
  return out;
}
// ------------------------------------------------- GETISANAME
std::string SCOpts::GetISANameFromPath(){
  std::string FilePath = FileList[0];
#ifdef _WIN32
  char sep = '\\';
#else
  char sep = '/';
#endif
  std::size_t dotPos = FilePath.rfind('.');
  std::size_t sepPos = FilePath.rfind(sep);
  bool withExtension = false;

  if( sepPos != std::string::npos ){
    return SCRemoveDot(FilePath.substr( sepPos + 1,
                            FilePath.size() - (withExtension || dotPos != std::string::npos ? 1 : dotPos) ));
  }
  return "";
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
    }else if( (s=="-s") || (s=="-sigmap") || (s=="--sigmap") ){
      if( i+1 > (argc-1) ){
        Msgs->PrintMsg(L_ERROR, "--sigmap requires an argument");
        return false;
      }
      std::string F(argv[i+1]);
      SigMap = F;
      isSigMap = true;
      i++;
    }else if( (s=="-a") || (s=="-package") || (s=="--package") ){
      if( i+1 > (argc-1) ){
        Msgs->PrintMsg(L_ERROR, "--package requires an argument");
        return false;
      }
      std::string F(argv[i+1]);
      Package = F;
      i++;
    }else if( (s=="-i") || (s=="-isa") || (s=="--isa") ){
      if( i+1 > (argc-1) ){
        Msgs->PrintMsg(L_ERROR, "--isa requires an argument");
        return false;
      }
      std::string F(argv[i+1]);
      ISA = F;
      i++;
    }else if( (s=="-k") || (s=="-keep") || (s=="--keep") ){
      isKeep = true;
    }else if( (s=="-P") || (s=="-perf") || (s=="--perf") ){
      isPerf = true;
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
    }else if( s=="--list-sc-passes"){
      isListSCPass = true;
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
    }else if( s=="--enable-sc-pass"){
      if( i+1 > (argc-1) ){
        Msgs->PrintMsg(L_ERROR, "--enable-sc-pass requires an argument");
        return false;
      }
      std::string P(argv[i+1]);
      std::vector<std::string> tmpV1 = ParsePasses(P);
      EnableSCPass.insert(EnableSCPass.end(),tmpV1.begin(),tmpV1.end());
      if( tmpV1.size() > 0 ){
        isSCEnable = true;
        isSCDisable = false;
      }else{
        Msgs->PrintMsg(L_ERROR, "--enable-sc-pass requires an argument");
        return false;
      }
      i++;
    }else if( s=="--disable-sc-pass"){
      if( i+1 > (argc-1) ){
        Msgs->PrintMsg(L_ERROR, "--disable-sc-pass requires an argument");
        return false;
      }
      std::string P(argv[i+1]);
      std::vector<std::string> tmpV2 = ParsePasses(P);
      DisableSCPass.insert(DisableSCPass.end(),tmpV2.begin(),tmpV2.end());
      if( tmpV2.size() > 0 ){
        isSCEnable = false;
        isSCDisable = true;
      }else{
        Msgs->PrintMsg(L_ERROR, "--disable-sc-pass requires an argument");
        return false;
      }
      i++;
    }else{
      if( FindDash(s) ){
        Msgs->PrintMsg(L_ERROR, "Unknown argument: " + s );
        return false;
      }
      // assume this is a file space member
      FileList.push_back(s);
    }
  }

  // is raw parsing is disabled, enable the codegen
  if( !isParse ){
    isCG = true;
  }

  if( (!*isHelp) && (FileList.size() == 0) && (!isListPass) && (!isListSCPass) ){
    Msgs->PrintMsg( L_ERROR, "No input files found" );
    return false;
  }

  if(*isHelp){
    return true;
  }

  // derive the ISA if it wasn't specified
  if( (!isListPass) && (!isListSCPass) && (ISA.length() == 0) ){
    ISA = GetISANameFromPath();
  }

  return true;
}

// ------------------------------------------------- PurgeInputFiles
bool SCOpts::PurgeInputFiles(){
  FileList.clear();
  return true;
}

// ------------------------------------------------- SetInputFile
bool SCOpts::SetInputFile( std::string I ){
  FileList.push_back( I );
  return 0;
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
  Msgs->PrintRawMsg("Copyright (C) 2017-2020 Tactical Computing Laboratories, LLC");
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
  Msgs->PrintRawMsg("     -c|-chisel|--chisel                 : Generate Chisel output [default=on]");
  Msgs->PrintRawMsg("     -p|-parse|--parse                   : Parse but do not compile");
  Msgs->PrintRawMsg("     -f|-outfile|--outfile /path/to/out  : Set the output file name");
  Msgs->PrintRawMsg("     -s|-sigmap|--sigmap /path/to/sigmap : Generates a signal map");
  Msgs->PrintRawMsg("     -o|-object|--object                 : Generate target object file [*.o]");
  Msgs->PrintRawMsg("     -V|-version|--version               : Print the version info");
  Msgs->PrintRawMsg(" ");
  Msgs->PrintRawMsg("Execution Options:");
  Msgs->PrintRawMsg("     -O|-optimize|--optimize             : Execute the optimizer [default=on]" );
  Msgs->PrintRawMsg("     -N|-no-optimize|--no-optimize       : Do not execute the optimizer" );
  Msgs->PrintRawMsg("     -D|-disable-chisel|--disable-chisel : Disables Chisel output" );
  Msgs->PrintRawMsg("     -P|-perf|--perf                     : Prints performance info" );
  Msgs->PrintRawMsg("     -v|-verbose|--verbose               : Enable verbosity");
  Msgs->PrintRawMsg(" ");
  Msgs->PrintRawMsg("Optimization Pass Options:");
  Msgs->PrintRawMsg("     --list-passes                       : Lists all the LLVM passes");
  Msgs->PrintRawMsg("     --list-sc-passes                    : Lists all the StoneCutter passes");
  Msgs->PrintRawMsg("     --enable-pass \"PASS1,PASS2\"         : Enables individual LLVM passes");
  Msgs->PrintRawMsg("     --disable-pass \"PASS1,PASS2\"        : Disables individual LLVM passes");
  Msgs->PrintRawMsg("     --enable-sc-pass \"PASS1,PASS2\"      : Enables individual StoneCutter passes");
  Msgs->PrintRawMsg("     --disable-sc-pass \"PASS1,PASS2\"     : Disables individual StoneCutter passes");
  Msgs->PrintRawMsg("     --sc-pass-opts \"PASS1:OPTS\"         : Set StoneCutter pass-specific optiosn");
  Msgs->PrintRawMsg(" ");
  Msgs->PrintRawMsg("Chisel Output Options:");
  Msgs->PrintRawMsg("     -a|-package|--package PACKAGE       : Sets the Chisel package name");
  Msgs->PrintRawMsg("     -i|-isa|--isa ISA                   : Sets the Chisel ISA name");
}

// EOF
