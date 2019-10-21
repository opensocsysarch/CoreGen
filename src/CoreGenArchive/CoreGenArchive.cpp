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

bool CoreGenArchive::Init( unsigned Entry ){
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
