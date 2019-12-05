//
// _CoreGenArchive_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenArchive/CoreGenArchive.h"

CoreGenArchive::CoreGenArchive( std::string Base )
  : BaseDir(Base) {
}

CoreGenArchive::~CoreGenArchive(){
  ArchFiles.clear();
  Arch.clear();
}

bool CoreGenArchive::CheckValidNode(const YAML::Node Node,
                                    std::string Token){
  if( !Node[Token] || Node[Token].IsNull() )
    return false;
  return true;
}

void CoreGenArchive::PrintParserError(const YAML::Node Node,
                                      std::string Name,
                                      std::string Token ){
  if( Token.length() > 0 ){
    Error = "Error in parsing archive entry node at "
                    + Name + ":" + Token + " near line "
                    + std::to_string(Node.Mark().line);
  }else{
    Error = "Error in parsing archive entry node at "
                    + Name + " near line "
                    + std::to_string(Node.Mark().line);
  }
}

bool CoreGenArchive::StrToSrcType( std::string Input,
                                   CGASrcType &ST ){
  if( Input == "UNKNOWN" ){
    ST = CGA_SRC_UNK;
    return true;
  }else if( Input == "ZIP" ){
    ST = CGA_SRC_ZIP;
    return true;
  }else if( Input == "TGZ" ){
    ST = CGA_SRC_TGZ;
    return true;
  }else if( Input == "GIT" ){
    ST = CGA_SRC_GIT;
    return true;
  }

  Error = "Unknown src type: " + Input;

  return false;
}

bool CoreGenArchive::StrToType( std::string Input,
                                CGAEntryType &T ){
  if( Input == "UNKNOWN" ){
    T = CGA_UNK;
    return true;
  }else if( Input == "COMPILER" ){
    T = CGA_COMPILER;
    return true;
  }else if( Input == "RTL" ){
    T = CGA_RTL;
    return true;
  }else if( Input == "PLUGIN" ){
    T = CGA_PLUGIN;
    return true;
  }

  Error = "Unknown type: " + Input;

  return false;
}

bool CoreGenArchive::ReadEntries(const YAML::Node& Entries){
  // check to see how many entries there are
  // if there are no entries, its ok, just return true
  if( Entries.size() == 0 )
    return true;

  for( unsigned i=0; i<Entries.size(); i++ ){
    const YAML::Node& Node = Entries[i];

    // entry name
    if( !CheckValidNode(Node,"Entry") ){
      PrintParserError(Entries,"Entries","Entry");
      return false;
    }

    std::string Name = Node["Entry"].as<std::string>();

    // directory
    if( !CheckValidNode(Node,"Directory") ){
      PrintParserError(Entries,"Entry","Directory");
      return false;
    }

    std::string Dir = Node["Directory"].as<std::string>();

    // src type
    if( !CheckValidNode(Node,"SrcType") ){
      PrintParserError(Entries,"Entry","SrcType");
      return false;
    }

    CGASrcType SrcType = CGA_SRC_UNK;
    if( !StrToSrcType(Node["SrcType"].as<std::string>(), SrcType) ){
      return false;
    }

    // type
    if( !CheckValidNode( Node,"Type") ){
      PrintParserError(Entries,"Entry","Type");
      return false;
    }

    CGAEntryType Type = CGA_UNK;
    if( !StrToType(Node["Type"].as<std::string>(), Type) ){
      return false;
    }

    // version
    if( !CheckValidNode( Node,"Version") ){
      PrintParserError(Entries,"Entry","Version");
      return false;
    }

    std::string Version = Node["Version"].as<std::string>();

    // postscript [optional]
    std::string PS;
    if( CheckValidNode( Node,"Postscript") ){
      PS = Node["Postscript"].as<std::string>();
    }

    // latest [optional]
    bool Latest = false;
    if( CheckValidNode( Node,"Latest") ){
      Latest = Node["Latest"].as<bool>();
    }

    // url [optional, unless SrcType == GIT ]
    std::string URL;
    if( CheckValidNode( Node, "URL") ){
      URL = Node["URL"].as<std::string>();
    }else if( SrcType == CGA_SRC_GIT ){
      // git entries must have a url
      Error = "Git entries must specify a URL";
      return false;
    }

    // read everything, create a new entry
    Arch.push_back(new CoreGenArchEntry(Name,Dir,PS,Version,
                                        URL,Latest,Type,SrcType));
  }

  return true;
}

bool CoreGenArchive::ReadArchYaml(std::string Yaml){
  if( Yaml.length() == 0 ){
    Error = "Yaml file is null";
    return false;
  }

  YAML::Node ArchInput;
  std::ifstream fin(Yaml.c_str());
  try{
    ArchInput = YAML::Load(fin);
  }catch(YAML::ParserException& e){
    Error = "Yaml parser failed: caught exception";
    return false;
  }

  if( ArchInput.IsNull() ){
    Error = "Archive file is empty";
    fin.close();
  }

  const YAML::Node& Entries = ArchInput["Entries"];
  if( CheckValidNode(ArchInput,"Entries") ){
    if( !ReadEntries(Entries) )
      return false;
  }

  fin.close();
  return true;
}

bool CoreGenArchive::ReadYaml( std::string Yaml ){
  // search for an existing entry
  auto it = std::find(ArchFiles.begin(),ArchFiles.end(),Yaml);
  if( it != ArchFiles.end() ){
    Error = "Yaml entry has already been read: " + Yaml;
    return false;
  }

  // Read the yaml file
  if( !ReadArchYaml(Yaml) ){
    return false;
  }
  ArchFiles.push_back(Yaml);
  return true;
}

bool CoreGenArchive::Init(){
  for( unsigned i=0; i<Arch.size(); i++ ){
    if( !Init(i) ){
      return false;
    }
  }
  return true;
}

bool CoreGenArchive::CGADeleteFile(const std::string& name){
  if( remove(name.c_str()) != 0 ){
    return false;
  }else{
    return true;
  }
}

bool CoreGenArchive::CGAMkDir(const std::string& dir){
#if defined(_WIN32)
  if( _mkdir(dir.c_str()) != 0 ){
    return false;
  }
  return true;
#else
  if( mkdir(dir.c_str(), 0777) == -1 ){
    Error = "Could not create directory: " + dir;
    return false;
  }
  return true;
#endif
}

bool CoreGenArchive::CGADirExists(const char *path){
  struct stat info;

  if(stat( path, &info ) != 0)
    return false;
  else if(info.st_mode & S_IFDIR)
    return true;
  else
    return false;
}

#ifdef __cplusplus
extern "C"
{
#endif
static size_t WriteData(void *ptr, size_t size, size_t nmemb, void *stream){
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}
#ifdef __cplusplus
} // extern "C"
#endif

std::string CoreGenArchive::DownloadFile( std::string URL ){
  CURL *curl;
  CURLcode res;
  FILE *pagefile;
  srand(time(NULL));
  std::string TmpFile = BaseDir + "/tmp" + std::to_string(rand());

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  if( curl ){
    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);

    pagefile = fopen(TmpFile.c_str(),"wb");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK){
      Error = "Failed to download file from URL="+
              URL+" : "+
              std::string(curl_easy_strerror(res));
      CGADeleteFile(TmpFile);
      TmpFile = "";
    }
    fclose( pagefile );
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  return TmpFile;
}

bool CoreGenArchive::IsInit( CoreGenArchEntry *E ){

  return CGADirExists(GetFullPath(E).c_str());
}

std::string CoreGenArchive::GetFullPath(CoreGenArchEntry *E){
  std::string FullDir = BaseDir + "/" + E->GetDirectory();
  return FullDir;
}

bool CoreGenArchive::UncompressFile( std::string TmpFile ){
  std::string UncStr = "tar -zxvf " + TmpFile;
  if( system( UncStr.c_str() ) )
    return true;
  else{
    Error = "Failed to uncompress file " + TmpFile;
    return false;
  }
}

bool CoreGenArchive::InitCompressedArchive(CoreGenArchEntry *E){

  // check to see if the URL is valid
  if( E->GetURL().length() == 0 ){
    Error = "No URL present for archive entry " + E->GetName();
    return false;
  }

  // download the file
  std::string TmpFile = DownloadFile(E->GetURL());
  if( TmpFile.length() == 0 ){
    return false;
  }

  // uncompress it
  bool rtn = true;
  if( !UncompressFile(TmpFile) ){
    rtn = false;
  }

  // delete the tmp file
  CGADeleteFile(TmpFile);

  return rtn;
}

bool CoreGenArchive::InitGitArchive(CoreGenArchEntry *E){

  // check to see if the URL is valid
  if( E->GetURL().length() == 0 ){
    Error = "No URL present for archive entry " + E->GetName();
    return false;
  }

  // init the git2 library
  git_libgit2_init();

  // pull the repo
  bool rtn = true;
  git_repository *repo = NULL;
  const git_error *e;
  int error = git_clone(&repo,
                        E->GetURL().c_str(),
                        GetFullPath(E).c_str(),
                        NULL);
  if( error < 0 ){
    e = giterr_last();
    Error = std::string(e->message);
    rtn = false;
  }

  // shutdown the git2 library
  git_libgit2_shutdown();

  return rtn;
}

bool CoreGenArchive::InitUnkArchive(CoreGenArchEntry *E){
  std::string FullDir = BaseDir + "/" + E->GetDirectory();
  return CGAMkDir(FullDir);
}

bool CoreGenArchive::Init( unsigned Entry ){
  CoreGenArchEntry *E = GetEntry(Entry);
  if( !E ){
    return false;
  }

  // check to see if the entry is initialized
  if( IsInit(E) ){
    // previously initialized
    return true;
  }

  // initialize the entry
  switch( E->GetSrcType() ){
  case CGA_SRC_ZIP:
  case CGA_SRC_TGZ:
    return InitCompressedArchive(E);
    break;
  case CGA_SRC_GIT:
    return InitGitArchive(E);
    break;
  case CGA_SRC_UNK:
  default:
    return InitUnkArchive(E);
    break;
  }

  return true;
}

bool CoreGenArchive::Destroy(){
  for( unsigned i=0; i<Arch.size(); i++ ){
    if( !Destroy(i) ){
      return false;
    }
  }
  return true;
}

bool CoreGenArchive::Destroy( unsigned Entry ){
  return true;
}

bool CoreGenArchive::ExecPostscript( unsigned Entry ){
  return true;
}

CoreGenArchEntry *CoreGenArchive::GetEntry( unsigned Entry ){
  if( Entry > (Arch.size()-1) ){
    Error = "Entry out of bounds";
    return nullptr;
  }

  return Arch[Entry];
}

// EOF
