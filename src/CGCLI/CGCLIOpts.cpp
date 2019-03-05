//
// _CGCLIOpts_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CGCLIOpts.h"

CGCLIOpts::CGCLIOpts( int argc, char **argv )
  : ParseSuccess(false),HelpSelected(false),ManualPasses(false),
    GroupPasses(false), DataPasses(false), AnalysisPasses(false),
    OptPasses(false), ListPasses(false), Verify(false), ExecPass(false),
    CGChisel(false), CGComp(false), CGVersion(false), CheckPlugins(false),
    ListSysPasses(false), ExecSysPass(false), ASPSolver(false),
    ProjName("UNKNOWN"){
  // setup default project root
  char PATH[FILENAME_MAX];
  if( getcwd(PATH,sizeof(PATH)) == NULL ){
    PATH[0] = '.';
    PATH[1] = '/';
  }
  ProjRoot = std::string(PATH);
  // parse the options
  ParseSuccess = ParseOpts(argc,argv);
}

CGCLIOpts::~CGCLIOpts(){
}

void CGCLIOpts::PrintOptions(){
  std::cout << " Usage: cgcli [OPTIONS]" << std::endl;
  std::cout << " Options:" << std::endl;
  std::cout << "\t-h|-help|--help                       : Print help menu" << std::endl;
  std::cout << "\t-v|-version|--version                 : Print version info" << std::endl;
  std::cout << "\t-cv|-coregenversion|--coregenversion  : Print the CoreGen version info" << std::endl;
  std::cout << "\t-i|-ir|--ir IRFile.yaml               : Specify the IR input" << std::endl;
  std::cout << "\t-o|-out|--out IRFile.yaml             : Specify the IR output" << std::endl;
  std::cout << "\t-d|-dot|--dot File.dot                : Write the DOT representation to File.dot" << std::endl;
  std::cout << std::endl;
  std::cout << " Execution Options:" << std::endl;
  std::cout << "\t--verify                              : Verify the IR and exit" << std::endl;
  std::cout << "\t--pass                                : Run some or all of the IR passes" << std::endl;
  std::cout << "\t--chisel                              : Run the Chisel codegen" << std::endl;
  std::cout << "\t--compiler                            : Run the compiler codegen" << std::endl;
  std::cout << std::endl;
  std::cout << " Project Options:" << std::endl;
  std::cout << "\t-p|-project|--project ProjectName     : Set the project name" << std::endl;
  std::cout << "\t-r|-root|--root /path/                : Set the project root" << std::endl;
  std::cout << "\t-a|-archive|--archive /path/          : Set the archive path" << std::endl;
  std::cout << std::endl;
  std::cout << " Plugin Options:" << std::endl;
  std::cout << "\t--check-plugin \"PLUGIN1.so,PLUGIN2.so\": Read features from the target plugin" << std::endl;
  std::cout << std::endl;
  std::cout << " Pass Options:" << std::endl;
  std::cout << "\t--enable-pass \"PASS1,PASS2,...\"       : Explicitly enable passes" << std::endl;
  std::cout << "\t--disable-pass \"PASS1,PASS2,...\"      : Explicitly disable passes" << std::endl;
  std::cout << "\t--enable-sys-pass \"PASS1:ARG1,PASS2\"  : Explicitly enable system pass" << std::endl;
  std::cout << "\t--disable-all-passes                  : Disable all the passes" << std::endl;
  std::cout << "\t--enable-data-passes                  : Enable all data passes" << std::endl;
  std::cout << "\t--enable-analysis-passes              : Enable all analysis passes" << std::endl;
  std::cout << "\t--enable-opt-passes                   : Enable all optimization passes" << std::endl;
  std::cout << "\t--list-passes                         : List all the passes and return" << std::endl;
  std::cout << "\t--list-sys-passes                     : List all the system passes" << std::endl;
  std::cout << "\t--asp \"ASP Text\"                      : Execute ASP solver with input" << std::endl;
  std::cout << std::endl;
}

bool CGCLIOpts::FileExists(std::string F){
  if (FILE *file = fopen(F.c_str(), "r")) {
    fclose(file);
    return true;
  } else {
    return false;
  }

  return false;
}

void CGCLIOpts::Split(const std::string& s, char delim,
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

std::string CGCLIOpts::GetSysPass(unsigned Pass){
  std::string nullstring = "";
  if( Pass > (EnableSysPass.size()-1) ){
    return nullstring;
  }
  std::tuple<std::string,bool,std::string> Tuple = EnableSysPass[Pass];
  return std::get<0>(Tuple);
}

std::string CGCLIOpts::GetSysPassArg(unsigned Pass){
  std::string nullstring = "";
  if( Pass > (EnableSysPass.size()-1) ){
    return nullstring;
  }
  std::tuple<std::string,bool,std::string> Tuple = EnableSysPass[Pass];
  return std::get<2>(Tuple);
}

bool CGCLIOpts::GetSysPassIsArg(unsigned Pass){
  if( Pass > (EnableSysPass.size()-1) ){
    return false;
  }
  std::tuple<std::string,bool,std::string> Tuple = EnableSysPass[Pass];
  return std::get<1>(Tuple);
}

bool CGCLIOpts::IsPassEnabled( std::string P ){
  if( !ManualPasses ){
    return true;
  }
  // examine the pass vectors
  if( EnablePass.size() == 0 ){
    for( unsigned i=0; i<DisablePass.size(); i++ ){
      if( DisablePass[i] == P ){
        return false;
      }
    }
    return true;
  }else{
    for( unsigned i=0; i<EnablePass.size(); i++ ){
      if( EnablePass[i] == P ){
        return true;
      }
    }
  }
  return false;
}

std::vector<std::string> CGCLIOpts::ParsePlugins( std::string P ){
  std::vector<std::string> V;

  // exit if the string is null
  if( P.length() == 0 ){
    std::cout << "Warning: No plugins found for plugin argument" << std::endl;
    return V;
  }

  std::string Str = P;  // local parsing string

  // break the string into tokens
  Split( Str, ',', V );

  return V;
}

std::vector<std::string> CGCLIOpts::ParsePasses( std::string P ){
  std::vector<std::string> V;

  // exit if the string is null
  if( P.length() == 0 ){
    std::cout << "Warning: No passes found for pass argument" << std::endl;
    return V;
  }

  std::string Str = P;  // local parsing string

  // break the string into tokens
  Split( Str, ',', V );

  return V;
}

bool CGCLIOpts::ParseSysPasses( std::string P ){
  std::vector<std::string> V;

  // exit if the string is null
  if( P.length() == 0 ){
    std::cout << "Warning: No passes found for system pass argument" << std::endl;
    return false;
  }

  std::string Str = P;

  // split the individual tokens into <PASS:ARG><PASS:ARG>
  Split( Str, ',', V );

  // now for each token, split into its pass:arg tuple
  for( unsigned i=0; i<V.size(); i++ ){
    std::vector<std::string> Tokens;
    std::string Token = V[i];
    Split( Token, ':', Tokens );
    if( Tokens.size() == 0 ){
      std::cout << "Warning: No passes found for system pass argument" << std::endl;
      return false;
    }else if( Tokens.size() == 1 ){
      EnableSysPass.push_back(
        std::tuple<std::string,bool,std::string>(Tokens[0],false," "));
    }else{
      EnableSysPass.push_back(
        std::tuple<std::string,bool,std::string>(Tokens[0],true,Tokens[1]));
    }
  }

  if( EnableSysPass.size() == 0 ){
    std::cout << "Warning: No passes found for system pass argument" << std::endl;
    return false;
  }
  return true;
}

void CGCLIOpts::SetDefaultPaths(){
  Archive = CGInstallPrefix() + "/archive";
}

bool CGCLIOpts::ParseOpts( int argc, char **argv ){

  // set the default path options
  SetDefaultPaths();

  // walk the options
  for( int i=1; i<argc; i++ ){
    std::string s(argv[i]);

    if( (s=="-h") || (s=="-help") || (s=="--help") ){
      // help
      HelpSelected = true;
      PrintOptions();
      return true;
    }else if( (s=="-v") || (s=="-version") || (s=="--version") ){
      // version
      HelpSelected=true;
      std::cout << "Copyright (C) 2017-2018 Tactical Computing Laboratories, LLC" << std::endl;
      std::cout << "CGCLI " << CGCLI_VERSION_MAJOR << "."
                            << CGCLI_VERSION_MINOR << "."
                            << CGCLI_VERSION_PATCH << std::endl;
      return true;
    }else if( (s=="-cv") || (s=="-coregenversion") || (s=="--coregenversion") ){
      // coregen version
      CGVersion = true;
    }else if( (s=="-i") || (s=="-ir") || (s=="--ir") ){
      // IR
      if( i+1 > (argc-1) ){
        std::cout << "Error : --ir requires an argument" << std::endl;
        return false;
      }
      std::string P(argv[i+1]);
      IRFile = P;
      if( !FileExists(IRFile) ){
        std::cout << "Error : IR file " << IRFile << " cannot be read" << std::endl;
        return false;
      }
      if( OutFile.length() == 0 ){
        // set the output = input
        OutFile = IRFile;
      }
      i++;
    }else if( (s=="-o") || (s=="-out") || (s=="--out") ){
      // Output file
      if( i+1 > (argc-1) ){
        std::cout << "Error : --out requires an argument" << std::endl;
        return false;
      }
      std::string P(argv[i+1]);
      OutFile = P;
      i++;
    }else if( (s=="-d") || (s=="-dot") || (s=="--dot") ){
      // Dot file
      if( i+1 > (argc-1) ){
        std::cout << "Error : --dot requires an argument" << std::endl;
        return false;
      }
      std::string P(argv[i+1]);
      DotFile = P;
      i++;
      Verify = true;  // we must verify the IR first
    }else if( (s=="-p") || (s=="-project") || (s=="--project") ){
      // project name
      if( i+1 > (argc-1) ){
        std::cout << "Error : --project requires an argument" << std::endl;
        return false;
      }
      std::string P(argv[i+1]);
      ProjName = P;
      i++;
    }else if( (s=="-r") || (s=="-root") || (s=="--root") ){
      // project name
      if( i+1 > (argc-1) ){
        std::cout << "Error : --root requires an argument" << std::endl;
        return false;
      }
      std::string P(argv[i+1]);
      ProjRoot = P;
      i++;
    }else if( (s=="-a") || (s=="-archive") || (s=="--archive") ){
      // project name
      if( i+1 > (argc-1) ){
        std::cout << "Error : --archive requires an argument" << std::endl;
        return false;
      }
      std::string P(argv[i+1]);
      Archive = P;
      i++;
    }else if( s=="--check-plugin" ){
      // check the target plugins
      if( i+1 > (argc-1) ){
        std::cout << "Error : --check-plugin requires an argument" << std::endl;
        return false;
      }
      // parse the plugin names
      std::string P(argv[i+1]);
      Plugins = ParsePlugins(P);
      if( Plugins.size() > 0 ){
        CheckPlugins = true;
      }else{
        std::cout << "Error : --check-plugin requires an argument" << std::endl;
        return false;
      }
      i++;
    }else if( s == "--enable-pass" ){
      // manual pass selection
      if( i+1 > (argc-1) ){
        std::cout << "Error : --enable-pass requires an argument" << std::endl;
        return false;
      }
      // parse the pass names
      std::string P(argv[i+1]);
      std::vector<std::string> tmpV1 = ParsePasses(P);
      EnablePass.insert(EnablePass.end(), tmpV1.begin(), tmpV1.end());
      if( tmpV1.size() > 0 ){
        ManualPasses = true;
      }else{
        std::cout << "Error : --enable-pass requires an argument" << std::endl;
        return false;
      }
      i++;
    }else if( s == "--disable-pass" ){
      // manual pass selection
      if( i+1 > (argc-1) ){
        std::cout << "Error : --disable-pass requires an argument" << std::endl;
        return false;
      }
      // parse the pass names
      std::string P(argv[i+1]);
      std::vector<std::string> tmpV2 = ParsePasses(P);
      DisablePass.insert(DisablePass.end(), tmpV2.begin(), tmpV2.end());
      if( tmpV2.size() > 0 ){
        ManualPasses = true;
      }else{
        std::cout << "Error : --disable-pass requires an argument" << std::endl;
        return false;
      }
      i++;
    }else if( s == "--enable-sys-pass" ){
      // system pass selection
      if( i+1 > (argc-1) ){
        std::cout << "Error : --enable-sys-pass requires an argument" << std::endl;
        return false;
      }
      // parse the pass names
      std::string P(argv[i+1]);
      if( !ParseSysPasses(P) ){
        return false;
      }
      ExecSysPass = true;
      Verify = true;
      i++;
    }else if( s == "--asp" ){
      // ASP pass selection
      if( i+1 > (argc-1) ){
        std::cout << "Error : --asp requires an argument" << std::endl;
        return false;
      }
      std::string P(argv[i+1]);
      ASP = P;
      if( ASP.length() == 0 ){
        std::cout << "Error : ASP string must be non null" << std::endl;
        return false;
      }
      ASPSolver = true;
      i++;
    }else if( s == "--disable-all-passes" ){
      ManualPasses = true;
      EnablePass.clear();
      // Per the logic in IsPassEnabled, pushing back a pass name
      // here that never matches any known passes will effectively
      // disable all passes.  This is only true if ManualPasses=true
      EnablePass.push_back(".");
    }else if( s=="--enable-data-passes"){
      GroupPasses = true;
      DataPasses = true;
    }else if( s=="--enable-analysis-passes"){
      GroupPasses = true;
      AnalysisPasses = true;
    }else if( s=="--enable-opt-passes"){
      GroupPasses = true;
      OptPasses = true;
    }else if( s=="--list-passes" ){
      ListPasses = true;
    }else if( s=="--list-sys-passes" ){
      ListSysPasses = true;
    }else if( s=="--verify" ){
      Verify = true;
    }else if( s=="--pass" ){
      ExecPass = true;
      Verify = true;
    }else if( s=="--opt" ){
      ExecPass = true;
      Verify = true;
    }else if( s == "--chisel" ){
      CGChisel = true;
      Verify = true;
    }else if( s == "--compiler" ){
      CGComp = true;
      Verify = true;
    }else{
      // parsing error
      std::cout << "Error : unknown option : " << s << std::endl;
      std::cout << "Use \"--help\" for the help menu" << std::endl;
      return false;
    }
  }

  if( !ExecPass && ((EnablePass.size()>0)||(DisablePass.size()>0)) ){
    std::cout << "Warning: individual passes selected, but passes will not be executed" 
      << std::endl;
    std::cout << "Add \"--pass\" to your command line arguments" << std::endl;
  }

  return true;
}

// Utilizes Levenshtein Edit Distance algorithm from:
// https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C++
// Based upon the original common Lisp implementation whereby only two of the columns
// in the matrix are utilized
unsigned CGCLIOpts::EditDistance( std::string &s1, std::string &s2 ){
  const std::size_t len1 = s1.size(), len2 = s2.size();
  std::vector<unsigned int> col(len2+1), prevCol(len2+1);

  for (unsigned int i = 0; i < prevCol.size(); i++){
    prevCol[i] = i;
  }

  for (unsigned int i = 0; i < len1; i++) {
    col[0] = i+1;
    for (unsigned int j = 0; j < len2; j++){
      // note that std::min({arg1, arg2, arg3}) works only in C++11,
      // for C++98 use std::min(std::min(arg1, arg2), arg3)
      col[j+1] = std::min({ prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (s1[i]==s2[j] ? 0 : 1) });
    }
    col.swap(prevCol);
  }

  return prevCol[len2];
}

// Returns the closest string in the known pass list
std::string CGCLIOpts::GetNearbyString(std::string &Input,
                                       std::vector<std::string> Passes){
  std::string RtnStr;
  unsigned LowVal;

  // walk all the pass values and find their edit distance from the input
  LowVal = EditDistance(Input,Passes[0]);
  RtnStr = Passes[0];
  for( unsigned i=1; i<Passes.size(); i++ ){
    unsigned tmp = EditDistance(Input,Passes[i]);
    if( tmp < LowVal ){
      LowVal = tmp;
      RtnStr = Passes[i];
    }
  }

  return RtnStr;
}

bool CGCLIOpts::CheckManualPassList(std::vector<std::string> Passes){

  bool rtn = true;

  // check the EnablePass vector
  for( unsigned i=0; i<EnablePass.size(); i++ ){
    std::vector<std::string>::iterator it;
    it = std::find(Passes.begin(),Passes.end(),EnablePass[i]);
    if( it == Passes.end() ){
      std::cout << "Unknown Enabled Pass: "
                << EnablePass[i]
                << ". Did you mean "
                << GetNearbyString(EnablePass[i],Passes)
                << "?"
                << std::endl;
      rtn = false;
    }
  }

  // check the DisablePass vector
  for( unsigned i=0; i<DisablePass.size(); i++ ){
    std::vector<std::string>::iterator it;
    it = std::find(Passes.begin(),Passes.end(),DisablePass[i]);
    if( it == Passes.end() ){
      std::cout << "Unknown Disabled Pass: "
                << DisablePass[i]
                << ". Did you mean "
                << GetNearbyString(EnablePass[i],Passes)
                << "?"
                << std::endl;
      rtn = false;
    }
  }

  return rtn;
}

// EOF
