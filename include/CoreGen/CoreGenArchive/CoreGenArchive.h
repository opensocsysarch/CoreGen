//
// _CoreGenArchive_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _COREGENARCHIVE_H_
#define _COREGENARCHIVE_H_

// Standard Headers
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sys/stat.h>
#include <sys/time.h>
#include <cerrno>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>
#include <vector>
#include <sstream>
#include <cstring>
#include <cassert>

// Zlib headers
#include "zlib.h"

// YAML headers
#include "yaml-cpp/yaml.h"

typedef enum{
  CGA_UNK       = 0,    ///< CGAEntryType: unknown archive type
  CGA_COMPILER  = 1,    ///< CGAEntryType: compiler archive entry
  CGA_RTL       = 2,    ///< CGAEntryType: RTL archive entry
  CGA_PLUGIN    = 3     ///< CGAEntryType: Plugin archive entry
}CGAEntryType;

typedef enum{
  CGA_SRC_UNK   = 0,    ///< CGASrcType: unknown src type
  CGA_SRC_ZIP   = 1,    ///< CGASrcType: ZIP source file
  CGA_SRC_TGZ   = 2,    ///< CGASrcType: TGZ source file
  CGA_SRC_GIT   = 3     ///< CGASrcType: GIT source URL
}CGASrcType;

/**
 * \class CoreGenArchEntry
 *
 * \ingroup CoreGenArchive
 *
 * \brief CoreGen Archive Library Entry
 *
 */
class CoreGenArchEntry{
private:
  std::string Name;         ///< CoreGenArchEntry: name of the entry
  std::string Directory;    ///< CoreGenArchEntry: directory location
  std::string Postscript;   ///< CoreGenArchEntry: post init script
  std::string Version;      ///< CoreGenArchEntry: version info
  std::string URL;          ///< CoreGenArchEntry: entry url

  bool Latest;              ///< CoreGenArchEntry: latest version of the entry

  CGAEntryType EntryType;   ///< CoreGenArchEntry: archive entry type
  CGASrcType SrcType;       ///< CoreGenArchEntry: archive source type

public:
  /// CoreGenArchEntry: default constructor
  CoreGenArchEntry( std::string N,
                    std::string Dir,
                    std::string Script,
                    std::string Ver,
                    std::string URL,
                    bool L,
                    CGAEntryType Type,
                    CGASrcType Src )
    : Name(N), Directory(Dir), Postscript(Script), Version(Ver),
      URL(URL), Latest(L), EntryType(Type), SrcType(Src) {}

  /// CoreGenArchEntry: default destructor
  ~CoreGenArchEntry() {}

  /// CoreGenArchEntry: retrieve the name of the entry
  std::string GetName() { return Name; }

  /// CoreGenArchEntry: retrieve the directory location
  std::string GetDirectory() { return Directory; }

  /// CoreGenArchEntry: retrieve the post init script
  std::string GetPostscript() { return Postscript; }

  /// CoreGenArchEntry: retrieve the version information
  std::string GetVersion() { return Version; }

  /// CoreGenArchEntry: retrieve the archive URL
  std::string GetURL() { return URL; }

  /// CoreGenArchEntry: is this entry the latest version?
  bool IsLatest() { return Latest; }

  /// CoreGenArchEntry: retrieve the entry type
  CGAEntryType GetEntryType() { return EntryType; }

  /// CoreGenArchEntry: retrieve the source type
  CGASrcType GetSrcType() { return SrcType; }

  /// CoreGenArchEntry: retrieve the entry type as a std::string
  std::string GetEntryTypeStr() {
    std::string Out;
    switch(EntryType){
    case CGA_COMPILER:
      Out = "COMPILER";
      break;
    case CGA_RTL:
      Out = "RTL";
      break;
    case CGA_PLUGIN:
      Out = "PLUGIN";
      break;
    case CGA_UNK:
    default:
      Out = "UNKNOWN";
      break;
    }
    return Out;
  }

  /// CoreGenArchEntry: retrieve the source type as a std::string
  std::string GetSrcTypeStr() {
    std::string Out;
    switch(SrcType){
    case CGA_SRC_ZIP:
      Out = "ZIP";
      break;
    case CGA_SRC_TGZ:
      Out = "TGZ";
      break;
    case CGA_SRC_GIT:
      Out = "GIT";
      break;
    case CGA_SRC_UNK:
    default:
      Out = "UNKNOWN";
      break;
    }
    return Out;
  }
};

/**
 * \class CoreGenArchive
 *
 * \ingroup CoreGenArchive
 *
 * \brief CoreGen Archive Library
 *
 */
class CoreGenArchive{
private:
  // private variables
  std::string BaseDir;                    ///< CoreGenArchive: base directory
  std::string Error;                      ///< CoreGenArchive: error string

  std::vector<std::string> ArchFiles;     ///< CoreGenArchive: Yaml input files
  std::vector<CoreGenArchEntry *> Arch;   ///< CoreGenArchive: archive entries

  // function variables
  /// CoreGenArchive: read the target yaml file
  bool ReadArchYaml(std::string Yaml);

  /// CoreGenArchive: checks the yaml node for valid state
  bool CheckValidNode(const YAML::Node Node,
                      std::string Token);

  /// CoreGenArchive: read the individual entries
  bool ReadEntries(const YAML::Node& Entries);

  /// CoreGenArchive: prints a yaml parser error
  void PrintParserError( const YAML::Node Node,
                         std::string Name,
                         std::string Token );

  /// CoreGenArchive: converts string to CGASrcType
  bool StrToSrcType( std::string Input, CGASrcType &ST );

  /// CoreGenArchive: converts string to CGAEntryType
  bool StrToType( std::string Input, CGAEntryType &T );

  /// CoreGenArchive: determines if the target entry has been initialized
  bool IsInit(CoreGenArchEntry *Entry);

  /// CoreGenArchive: determines if the directory exists
  bool CGADirExists(const char *path);

  /// CoreGenArchive: create a new directory
  bool CGAMkDir(const std::string& dir);

  /// CoreGenArchive: initialize zip archive
  bool InitZipArchive(CoreGenArchEntry *Entry);

  /// CoreGenArchive: initialize tgz archive
  bool InitTgzArchive(CoreGenArchEntry *Entry);

  /// CoreGenArchive: initialize git archive
  bool InitGitArchive(CoreGenArchEntry *Entry);

  /// CoreGenArchive: initialize generic archive
  bool InitUnkArchive(CoreGenArchEntry *Entry);

public:

  /// CoreGenArchive: default constructor
  CoreGenArchive(std::string Base);

  /// CoreGenArchive: default destructor
  ~CoreGenArchive();

  /// CoreGenArchive: retrieve the error string
  std::string GetErrStr() { return Error; }

  /// CoreGenArchive: read the target yaml file
  bool ReadYaml( std::string Yaml );

  /// CoreGenArchive: initialize the archive
  bool Init();

  /// CoreGenArchive: initialize the specific entry
  bool Init( unsigned Entry );

  /// CoreGenArchive: destroy the initialized archive
  bool Destroy();

  /// CoreGenArchive: destroy the specific entry
  bool Destroy( unsigned Entry );

  /// CoreGenArchive: execute the post-init script for the specific entry
  bool ExecPostscript( unsigned Entry );

  /// CoreGenArchive: retrieve the target archive entry
  CoreGenArchEntry *GetEntry( unsigned Entry );

  /// CoreGenArchive: retrieve the number of entries
  unsigned GetNumEntries() { return Arch.size(); }
};

#endif

// EOF
