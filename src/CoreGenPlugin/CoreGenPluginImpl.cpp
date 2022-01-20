//
// _CoreGenPluginImpl_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenPlugin/CoreGenPluginImpl.h"

CoreGenPluginImpl::CoreGenPluginImpl(std::string N,
                                     CGPluginType T,
                                     bool HDL,
                                     bool LLVM,
                                     unsigned MaV,
                                     unsigned MiV,
                                     unsigned PaV,
                                     CoreGenEnv *EV,
                                     CoreGenErrno *E)
  : Name(N), Type(T), NumFeatures(0), HDLCodegen(HDL), LLVMCodegen(LLVM),
    MajorVersion(MaV), MinorVersion(MiV), PatchVersion(PaV),
    Env(EV), Errno(E),Top(new CoreGenNode(CGTop,Errno)) {
}

CoreGenPluginImpl::~CoreGenPluginImpl(){

  this->PurgeNodes();

  if( Top ){
    delete Top;
  }
}

bool CoreGenPluginImpl::IsCommentLine(std::string line){
  if( (line[0] == '/') && (line[1] == '/') ){
    // c++, verilog, chisel comment
    return true;
  }else if( (line[0] == '/') && (line[1] == '*') ){
    // c, c++, verilog comment
    return true;
  }else if( line[0] == '#' ){
    // shell script comment
    return true;
  }
  return false;
}

bool CoreGenPluginImpl::CodegenChiselImport(std::string File,
                                            std::vector<std::string> Imports){
  // Stage 1: copy the target file to a temporary
  std::string TmpFile = File + ".tmp";

  if( !CGFileCopy( File, TmpFile ) ){
    Errno->SetError(CGERR_ERROR,
                    this->GetName() + ":Could not copy file to temporary: " + File );
    return false;
  }

  // Stage 2: open the TmpFile
  std::ifstream InFile(TmpFile);
  if( !InFile.is_open() ){
    Errno->SetError(CGERR_ERROR,
                    this->GetName() + ":Failed to open temporary source file: " + TmpFile );
    return false;
  }

  // Stage 3: walk the file and find the line number of the last import statement
  std::string line;
  unsigned lineNum = 0;
  unsigned foundLine = 0;
  while( std::getline(InFile,line) ){
    if( !IsCommentLine(line) ){
      // not a comment line, look for the keyword
      std::size_t found = line.find("import");
      if( found != std::string::npos ){
        foundLine = lineNum;
      }
    }
    lineNum++;
  }

  // Stage 4: reset the input file pointer
  InFile.seekg(std::ios_base::beg);

  // Stage 5: Delete the original file
  if( !CGDeleteFile(File) ){
    Errno->SetError(CGERR_ERROR,
                    this->GetName() + ":Failed to remove original source file: " + File );
    InFile.close();
    CGDeleteFile(TmpFile);
    return false;
  }

  // Stage 6: open the original file
  std::ofstream OutStream;
  OutStream.open(File,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR,
                    this->GetName() + ":Failed to open target source file: " + File );
    InFile.close();
    CGDeleteFile(TmpFile);
    return false;
  }

  // Stage 7: iterate across the TmpFile and copy until we hit "foundLine"
  //          then insert the vector of import statements and continue copying
  lineNum = 0;
  while( std::getline(InFile,line) ){
    if( lineNum == foundLine ){
      // output the last import statement
      OutStream << line;

      // output the set of imports from the user
      for( unsigned i=0; i<Imports.size(); i++ ){
        OutStream << Imports[i] << std::endl;
      }
    }else{
      OutStream << line;
    }
    lineNum++;
  }

  // Stage 8: close all the files and delete the tmp
  InFile.close();
  OutStream.close();
  CGDeleteFile(TmpFile);

  return true;
}

bool CoreGenPluginImpl::CodegenKeyword(std::string File,
                                       std::string Key,
                                       std::string Output){
  // Stage 1: copy the target file to a temporary
  std::string TmpFile = File + ".tmp";

  if( !CGFileCopy( File, TmpFile ) ){
    Errno->SetError(CGERR_ERROR,
                    this->GetName() + ":Could not copy file to temporary: " + File );
    return false;
  }

  // Stage 2: remove the original file
  if( !CGDeleteFile(File) ){
    Errno->SetError(CGERR_ERROR,
                    this->GetName() + ":Failed to remove original source file: " + File );
    return false;
  }

  // Stage 3: open the temporary file and the original file
  std::ofstream OutStream;
  OutStream.open(File,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR,
                    this->GetName() + ":Failed to open target source file: " + File );
    // copy the original back
    CGFileCopy( TmpFile, File );
    return false;
  }

  std::ifstream InFile(TmpFile);
  if( !InFile.is_open() ){
    Errno->SetError(CGERR_ERROR,
                    this->GetName() + ":Failed to open temporary source file: " + TmpFile );
    OutStream.close();
    CGFileCopy(TmpFile,File);
    return false;
  }

  // Stage 4: walk the file and initiate the codegen
  std::string line;
  while( std::getline(InFile,line) ){
    if( IsCommentLine(line) ){
      // found a comment line, look for the keyword
      std::size_t found = line.find(Key);
      if( found != std::string::npos ){
        OutStream << Output;
      }
    }else{
      // just output the line
      OutStream << line;
    }
  }

  // Stage 5: close the files
  InFile.close();
  OutStream.close();

  return true;
}

bool CoreGenPluginImpl::CodegenFile(std::string File,
                                    std::string Output){
  std::ofstream OutStream;
  OutStream.open(File,std::ios::trunc);
  if( !OutStream.is_open() ){
    Errno->SetError(CGERR_ERROR,
                    this->GetName() + ":Could not open plugin output file: " + File );
    return false;
  }
  OutStream << Output;
  OutStream.close();
  return true;
}

bool CoreGenPluginImpl::CopyPluginSrc(std::string Archive, std::string Path){
  if( !CGDirExists(Path.c_str()) ){
    if( !CGMkDirP(Path) ){
      Errno->SetError(CGERR_ERROR,
                      this->GetName() + ":Directory doesn't exist and could not be created: " + Path );
      return false;
    }
  }
  std::string UncStr = "cp -R " + Archive + "/* " + Path + "/";
  if( system( UncStr.c_str() ) != 0 ){
      Errno->SetError(CGERR_ERROR,
                      this->GetName() + ":Could not copy source from archive directory: " + Archive );
      return false;
  }
  return true;
}

CGPluginType CoreGenPluginImpl::GetPluginType(){
  return Type;
}

bool CoreGenPluginImpl::PurgeNodes(){
  for( unsigned i=0; i<Top->GetNumChild(); i++ ){
    Top->DeleteChild( Top->GetChild(i) );
  }
  if(Top->GetNumChild() > 0){
    for( unsigned i=0; i<Caches.size(); i++ ){
      delete Caches[i];
    }
    for( unsigned i=0; i<Cores.size(); i++ ){
      delete Cores[i];
    }
    for( unsigned i=0; i<Exts.size(); i++ ){
      delete Exts[i];
   }
    for( unsigned i=0; i<Insts.size(); i++ ){
      delete Insts[i];
   }
    for( unsigned i=0; i<PInsts.size(); i++ ){
      delete PInsts[i];
    }
    for( unsigned i=0; i<Formats.size(); i++ ){
      delete Formats[i];
   }
   for( unsigned i=0; i<Regs.size(); i++ ){
      delete Regs[i];
    }
    for( unsigned i=0; i<RegClasses.size(); i++ ){
      delete RegClasses[i];
    }
    for( unsigned i=0; i<Socs.size(); i++ ){
      delete Socs[i];
    }
    for( unsigned i=0; i<ISAs.size(); i++ ){
      delete ISAs[i];
    }
    for( unsigned i=0; i<Comms.size(); i++ ){
      delete Comms[i];
    }
    for( unsigned i=0; i<Spads.size(); i++ ){
      delete Spads[i];
    }
    for( unsigned i=0; i<MCtrls.size(); i++ ){
      delete MCtrls[i];
    }
    for( unsigned i=0; i<VTPs.size(); i++ ){
      delete VTPs[i];
    }
  }

  return true;
}

void CoreGenPluginImpl::GetVersion( unsigned *Major,
                                    unsigned *Minor,
                                    unsigned *Patch ){
  if( Major ){
    *Major = MajorVersion;
  }
  if( Minor ){
    *Minor = MinorVersion;
  }
  if( Patch ){
    *Patch = PatchVersion;
  }
}

bool CoreGenPluginImpl::InsertFeature(std::string Name,
                                      CGFeatureType Type,
                                      CGFeatureVal Value){

  FeatureTable.push_back({Name,Type,Value});
  NumFeatures = FeatureTable.size();
  return true;
}

CGFeatureType CoreGenPluginImpl::GetFeatureType( std::string Feature ){
  if( Feature.length() == 0 ){
    return CGFUnknown;
  }

  for( unsigned i=0; i<FeatureTable.size(); i++ ){
    if( FeatureTable[i].Name == Feature ){
      return FeatureTable[i].Type;
    }
  }

  return CGFUnknown;
}

CGFeatureType CoreGenPluginImpl::GetFeatureType( unsigned Idx ){
  if( Idx > (FeatureTable.size()-1) ){
    return CGFUnknown;
  }
  return FeatureTable[Idx].Type;
}

std::string CoreGenPluginImpl::GetFeatureName( unsigned Idx ){
  if( Idx > (FeatureTable.size()-1) ){
    return "";
  }
  return FeatureTable[Idx].Name;
}

CGFeatureVal CoreGenPluginImpl::GetFeatureValue( std::string Feature ){
  CGFeatureVal Val;
  if( Feature.length() == 0 ){
    return Val;
  }

  for( unsigned i=0; i<FeatureTable.size(); i++ ){
    if( FeatureTable[i].Name == Feature ){
      return FeatureTable[i].Value;
    }
  }
  return Val;
}

CGFeatureVal CoreGenPluginImpl::GetFeatureValue( unsigned Idx ){
  CGFeatureVal Val;
  if( Idx > (FeatureTable.size()-1) ){
    return Val;
  }
  return FeatureTable[Idx].Value;
}

bool CoreGenPluginImpl::SetFeatureValue( std::string Feature,
                                         CGFeatureVal Val ){
  for( unsigned i=0; i<FeatureTable.size(); i++ ){
    if( FeatureTable[i].Name == Feature ){
      FeatureTable[i].Value = Val;
      return true;
    }
  }
  return false;
}

bool CoreGenPluginImpl::SetFeatureValue( unsigned Idx,
                                         CGFeatureVal Val ){
  if( Idx > (FeatureTable.size()-1) ){
    return false;
  }
  FeatureTable[Idx].Value = Val;
  return true;
}

bool CoreGenPluginImpl::DeleteNode( CoreGenNode *N ){
  //Top->DeleteChild( Top->GetChild(i) );
  // add all the subnodes
  for( unsigned i=0; i<Caches.size(); i++ ){
    if(static_cast<CoreGenNode *>(Caches[i]) == N){
      Caches.erase(Caches.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<Cores.size(); i++ ){
    if(static_cast<CoreGenNode *>(Cores[i]) == N){
      Cores.erase(Cores.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<Insts.size(); i++ ){
    if(static_cast<CoreGenNode *>(Insts[i]) == N){
      Insts.erase(Insts.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<PInsts.size(); i++ ){
    if(static_cast<CoreGenNode *>(PInsts[i]) == N){
      PInsts.erase(PInsts.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<Formats.size(); i++ ){
    if(static_cast<CoreGenNode *>(Formats[i]) == N){
      Formats.erase(Formats.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<Regs.size(); i++ ){
    if(static_cast<CoreGenNode *>(Regs[i]) == N){
      Regs.erase(Regs.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    if(static_cast<CoreGenNode *>(RegClasses[i]) == N){
      RegClasses.erase(RegClasses.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<Socs.size(); i++ ){
    if(static_cast<CoreGenNode *>(Socs[i]) == N){
      Socs.erase(Socs.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<ISAs.size(); i++ ){
    if(static_cast<CoreGenNode *>(ISAs[i]) == N){
      ISAs.erase(ISAs.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<Exts.size(); i++ ){
    if(static_cast<CoreGenNode *>(Exts[i]) == N){
      Exts.erase(Exts.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<Comms.size(); i++ ){
    if(static_cast<CoreGenNode *>(Comms[i]) == N){
      Comms.erase(Comms.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<Spads.size(); i++ ){
    if(static_cast<CoreGenNode *>(Spads[i]) == N){
      Spads.erase(Spads.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<MCtrls.size(); i++ ){
    if(static_cast<CoreGenNode *>(MCtrls[i]) == N){
      MCtrls.erase(MCtrls.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }
  for( unsigned i=0; i<VTPs.size(); i++ ){
    if(static_cast<CoreGenNode *>(VTPs[i]) == N){
      VTPs.erase(VTPs.begin()+i);
      Top->DeleteChild(N);
      return true;
    }
  }

  // just remove it from Top
  return Top->DeleteChild( N );
}

bool CoreGenPluginImpl::InsertNode( CoreGenNode *N ){
  if( !N ){
    return false;
  }
  Nodes.push_back(N);
  return true;
}

CoreGenCache *CoreGenPluginImpl::InsertCache(std::string Name,
                                          unsigned Sets,
                                          unsigned Ways){
  CoreGenCache *C = new CoreGenCache(Name,Sets,Ways,Errno);
  Caches.push_back(C);
  return C;
}

CoreGenCore *CoreGenPluginImpl::InsertCore(std::string Name,
                                        CoreGenISA *ISA ){
  CoreGenCore *C = new CoreGenCore(Name, ISA, Errno);
  Cores.push_back(C);
  return C;
}

CoreGenExt *CoreGenPluginImpl::InsertExt(std::string Name){
  CoreGenExt *E = new CoreGenExt(Name,Errno);
  Exts.push_back(E);
  return E;
}

CoreGenInst *CoreGenPluginImpl::InsertInst(std::string Name, CoreGenISA *ISA,
                                        CoreGenInstFormat *Format ){
  CoreGenInst *I = new CoreGenInst(Name,ISA,Format,Errno);
  Insts.push_back(I);
  return I;
}

CoreGenPseudoInst *CoreGenPluginImpl::InsertPseudoInst(std::string Name,
                                                    CoreGenInst *Inst ){
  CoreGenPseudoInst *P = new CoreGenPseudoInst(Name,Inst,Errno);
  PInsts.push_back(P);
  return P;
}

CoreGenInstFormat *CoreGenPluginImpl::InsertInstFormat(std::string Name,
                                                    CoreGenISA *ISA){
  CoreGenInstFormat *IF = new CoreGenInstFormat(Name,ISA,Errno);
  Formats.push_back(IF);
  return IF;
}

CoreGenReg *CoreGenPluginImpl::InsertReg(std::string Name,
                                      int RegIndex,
                                      int RegWidth ){
  CoreGenReg *R = new CoreGenReg(Name,RegIndex,RegWidth,Errno);
  Regs.push_back(R);
  return R;
}

CoreGenRegClass *CoreGenPluginImpl::InsertRegClass(std::string Name){
  CoreGenRegClass *RC = new CoreGenRegClass(Name,Errno);
  RegClasses.push_back(RC);
  return RC;
}

CoreGenSoC *CoreGenPluginImpl::InsertSoC(std::string Name){
  CoreGenSoC *S = new CoreGenSoC(Name,Errno);
  Socs.push_back(S);
  return S;
}

CoreGenISA *CoreGenPluginImpl::InsertISA(std::string Name){
  CoreGenISA *I = new CoreGenISA(Name,Errno);
  ISAs.push_back(I);
  return I;
}

CoreGenDataPath *CoreGenPluginImpl::InsertDataPath(std::string Name, std::string Style){
  CoreGenDataPath *D = new CoreGenDataPath(Name,Style, Errno);
  DataPaths.push_back(D);
  return D;
}

CoreGenComm *CoreGenPluginImpl::InsertComm(std::string Name){
  CoreGenComm *C = new CoreGenComm(Name,Errno);
  Comms.push_back(C);
  return C;
}

void CoreGenPluginImpl::InitFeatureValue(CGFeatureVal &Value){
  Value.UnsignedData = 0;
  Value.Uint32tData = 0x00;
  Value.Int32tData = 0x00;
  Value.Uint64tData = 0x00ull;
  Value.Int64tData = 0x00ll;
  Value.FloatData = 0.;
  Value.DoubleData = 0.;
  Value.StringData = "";
  Value.BoolData = false;
}

bool CoreGenPluginImpl::BuildDAG(){
  // purge all the children of Top
  Top->PurgeChildren();

  // add all the subnodes
  for( unsigned i=0; i<Caches.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Caches[i]));
  }
  for( unsigned i=0; i<Cores.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Cores[i]));
  }
  for( unsigned i=0; i<Insts.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Insts[i]));
  }
  for( unsigned i=0; i<PInsts.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(PInsts[i]));
  }
  for( unsigned i=0; i<Formats.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Formats[i]));
  }
  for( unsigned i=0; i<Regs.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Regs[i]));
  }
  for( unsigned i=0; i<RegClasses.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(RegClasses[i]));
  }
  for( unsigned i=0; i<Socs.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Socs[i]));
  }
  for( unsigned i=0; i<ISAs.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(ISAs[i]));
  }
  for( unsigned i=0; i<Exts.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Exts[i]));
  }
  for( unsigned i=0; i<Comms.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Comms[i]));
  }
  for( unsigned i=0; i<Spads.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(Spads[i]));
  }
  for( unsigned i=0; i<MCtrls.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(MCtrls[i]));
  }
  for( unsigned i=0; i<VTPs.size(); i++ ){
    Top->InsertChild(static_cast<CoreGenNode *>(VTPs[i]));
  }
  return true;
}

// EOF
