//
// _PoarIO_cpp_
//
// Copyright (C) 2017-2022 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/Poar/PoarIO.h"

PoarIO::PoarIO(PoarConfig *Config, std::string File)
  : PConfig(Config), OutFile(File){
}

PoarIO::~PoarIO(){
}

double PoarIO::GetTotalPower(){
  double result = 0.;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarPower) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      result += CE.AdjustedResult;
    }
  }
  return result;
}

double PoarIO::GetTotalUnadjustedPower(){
  double result = 0.;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarPower) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      result += CE.Result;
    }
  }
  return result;
}

double PoarIO::GetTotalArea(){
  double result = 0.;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarArea) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      result += CE.AdjustedResult;
    }
  }
  return result;
}

double PoarIO::GetTotalUnadjustedArea(){
  double result = 0.;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarArea) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      result += CE.Result;
    }
  }
  return result;
}

bool PoarIO::WriteText(){
  double TotalPower = GetTotalPower();
  double TotalArea  = GetTotalArea();

  // -- power
  std::cout << "Power Values" << std::endl;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarPower) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      std::cout << " - " << CE.Name;
      CGPrintSpace(CE.Name.length(),20);
      std::cout << CE.AdjustedResult << " mW" << std::endl;
    }
  }
  std::cout << "-----------------------------------------------------------------" << std::endl;

  // -- area
  std::cout << "Area Values" << std::endl;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarArea) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      std::cout << " - " << CE.Name;
      CGPrintSpace(CE.Name.length(),20);
      std::cout << CE.AdjustedResult << " gates" << std::endl;
    }
  }
  std::cout << "-----------------------------------------------------------------" << std::endl;

  // -- print the totals
  std::cout << "Summary" << std::endl;
  std::cout << "TOTAL POWER = " << TotalPower << " mW" << std::endl;
  std::cout << "TOTAL AREA  = " << TotalArea << " gates" << std::endl;

  return true;
}

bool PoarIO::WriteYaml(){
  if( OutFile.length() == 0 ){
    return false;
  }
  double TotalPower = GetTotalPower();
  double TotalArea  = GetTotalArea();

  // open the output file
  std::ofstream OutYaml(OutFile.c_str());
  YAML::Emitter out(OutYaml);
  out << YAML::BeginMap;

  // write out the total summary
  out << YAML::Key << "Summary";
  out << YAML::BeginSeq;
  out << YAML::BeginMap << YAML::Key << "TotalArea"
      << YAML::Value << TotalArea << YAML::EndMap;
  out << YAML::BeginMap << YAML::Key << "TotalPower"
      << YAML::Value << TotalPower << YAML::EndMap;
  out << YAML::EndSeq;

  // write out the power values
  out << YAML::Key << "Area";
  out << YAML::BeginSeq;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarArea) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      out << YAML::BeginMap;
      out << YAML::Key << CE.Name << YAML::Value << CE.AdjustedResult;
      out << YAML::EndMap;
    }
  }
  out << YAML::EndSeq;

  // write out the area values
  out << YAML::Key << "Power";
  out << YAML::BeginSeq;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarPower) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      out << YAML::BeginMap;
      out << YAML::Key << CE.Name << YAML::Value << CE.AdjustedResult;
      out << YAML::EndMap;
    }
  }
  out << YAML::EndSeq;

  // close the file
  out << YAML::EndMap;
  OutYaml.close();

  return true;
}

bool PoarIO::WriteXML(){
  if( OutFile.length() == 0 ){
    return false;
  }
  double TotalPower = GetTotalPower();
  double TotalArea  = GetTotalArea();

  // open the file
  std::ofstream Out(OutFile.c_str());
  Out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
  Out << "<POAR>" << std::endl;
  Out << "\t<Summary>" << std::endl;
  Out << "\t\t<TotalArea>" << TotalArea << "</TotalArea>" << std::endl;
  Out << "\t\t<TotalPower>" << TotalPower << "</TotalPower>" << std::endl;
  Out << "\t</Summary>" << std::endl;
  Out << "\t<Area>" << std::endl;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarArea) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      Out << "\t\t<" << CE.Name << ">"
          << CE.AdjustedResult
          << "</" << CE.Name << ">" << std::endl;
    }
  }
  Out << "\t</Area>" << std::endl;
  Out << "\t<Power>" << std::endl;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarPower) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      Out << "\t\t<" << CE.Name << ">"
          << CE.AdjustedResult
          << "</" << CE.Name << ">" << std::endl;
    }
  }
  Out << "\t</Power>" << std::endl;
  Out << "</POAR>" << std::endl;

  // close the file
  Out.close();
  return true;
}

bool PoarIO::WriteLatexMakefile(){
  // build the output file name
  std::string OFile = OutFile + "/Makefile";
  bool PrevFile = false;

  if( CGFileExists(OFile) ){
    PrevFile = true;
    CGFileCopy( OFile,
                OFile + ".back" );
    CGDeleteFile(OFile);
  }

  std::ofstream ofs;
  ofs.open(OFile.c_str(), std::ofstream::out | std::ofstream::app);

  ofs << "#!/bin/sh" << std::endl;
  ofs << "#" << std::endl;
  ofs << "# Spec Document Makefile" << std::endl;
  ofs << "# Auto-generated by the System Architect CoreGen SpecDoc Data Pass" << std::endl;
  ofs << "# " << CGCurrentDateTime() << std::endl;
  ofs << "#" << std::endl;
  ofs << "DOC = spec" << std::endl;
  ofs << "TEX = $(wildcard *.tex)" << std::endl;
  ofs << "FIGS = $(wildcard figures/*.pdf figures/*.png figures/*.jpg)" << std::endl;
  ofs << std::endl;
  ofs << ".PHONY: all clean" << std::endl;
  ofs << std::endl;
  ofs << "\%.pdf : *.dot" << std::endl;
  ofs << "\tdot -Tpdf -o $@ $<" << std::endl;
  ofs << "all: $(addsuffix .pdf, $(basename $(wildcard *.dot))) $(DOC).pdf" << std::endl;
  ofs << "$(DOC).pdf: $(TEXT) $(FIGS)" << std::endl;
  ofs << "\techo $(FIGS)" << std::endl;
  ofs << "\tpdflatex -halt-on-error $(DOC)" << std::endl;
  ofs << "\tpdflatex -halt-on-error $(DOC)" << std::endl;
  ofs << "\tpdflatex -halt-on-error $(DOC)" << std::endl;
  ofs << "clean:" << std::endl;
  ofs << "\trm -f *.aux *.bbl *.blg *.log *.lof *.lol *.lot *.out *.toc *.pdf $(DOC).pdf" << std::endl;

  ofs.close();

  if( PrevFile ){
    CGDeleteFile(OFile + ".back");
  }
  return true;
}

std::string PoarIO::EscapeUnderscore(std::string Str){
  std::string New;
  for( unsigned i=0; i<Str.length(); i++ ){
    if( Str[i] == '_' ){
      New += '\\';
    }
    New += Str[i];
  }
  return New;
}

bool PoarIO::WriteLatexTexfile(){
  // build the output file name
  double TotalPower = GetTotalPower();
  double TotalArea  = GetTotalArea();
  std::string OFile = OutFile + "/spec.tex";
  bool PrevFile = false;

  if( CGFileExists(OFile) ){
    PrevFile = true;
    CGFileCopy( OFile,
                OFile + ".back" );
    CGDeleteFile(OFile);
  }

  std::ofstream ofs;
  ofs.open(OFile.c_str(), std::ofstream::out | std::ofstream::app);

  // write out the header
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% spec.tex" << std::endl;
  ofs << "\% System Architect CoreGen Power/Area Speculation (POAR) Document" << std::endl;
  ofs << "\% " << CGCurrentDateTime() << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << std::endl << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% LATEX PACKAGES" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\documentclass{article}" << std::endl;
  ofs << "\\usepackage[margin=1.0in]{geometry}" << std::endl;
  ofs << "\\usepackage{graphicx}" << std::endl;
  ofs << "\\usepackage{amsmath}" << std::endl;
  ofs << "\\usepackage[utf8]{inputenc}" << std::endl;
  ofs << "\\usepackage[english]{babel}" << std::endl;
  ofs << "\\usepackage[parfill]{parskip}" << std::endl;
  ofs << "\\usepackage{array}" << std::endl;
  ofs << "\\usepackage{algorithm}" << std::endl;
  ofs << "\\usepackage{listings}" << std::endl;
  ofs << "\\usepackage{xcolor}" << std::endl;
  ofs << "\\usepackage{courier}" << std::endl;
  ofs << "\\usepackage{colortbl}" << std::endl;
  ofs << "\\usepackage{placeins}" << std::endl;
  ofs << "\\usepackage{longtable}" << std::endl;
  ofs << "\\usepackage{multirow}" << std::endl;
  ofs << "\\usepackage{float}" << std::endl;
  ofs << "\\usepackage{caption}" << std::endl;
  ofs << "\\usepackage{url}" << std::endl;
  ofs << "\\RequirePackage{epstopdf}" << std::endl;
  ofs << "\\RequirePackage{tabularx}" << std::endl;
  ofs << "\\RequirePackage{xstring}" << std::endl;
  ofs << "\\RequirePackage{hyperref}" << std::endl;
  ofs << "\\RequirePackage{fancyhdr}" << std::endl;

  ofs << std::endl << std::endl;

  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% DOCUMENT LAYOUT INFORMATION" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\pagestyle{fancy}" << std::endl;
  ofs << "\\lhead{}" << std::endl;
  ofs << "\\chead{POAR}" << std::endl;
  ofs << "\\rhead{}" << std::endl;
  ofs << "\\lfoot{" << CGCurrentDateTime() << "}" << std::endl;
  ofs << "\\cfoot{POAR}" << std::endl;
  ofs << "\\rfoot{\\thepage}" << std::endl;

  ofs << std::endl << std::endl;

  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% DOCUMENT INFORMATION" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;

  ofs << "\\title{Power and Area (POAR) Design Summary}" << std::endl;
  ofs << "\\author{Auto-Generated by System Architect Poar}" << std::endl;
  ofs << "\\date{}" << std::endl;

  ofs << "\\begin{document}" << std::endl;

  ofs << std::endl << std::endl;

  ofs << "\\maketitle" << std::endl;
  ofs << "\\thispagestyle{fancy}" << std::endl;

  ofs << std::endl << std::endl;

  ofs << "\\begin{center}" << std::endl;
  ofs << "\\begin{tabular}{l r}" << std::endl;
  ofs << "Date: & " << CGCurrentDateTime() << "\\\\" << std::endl;
  ofs << "Revision: 1.0 \\\\" << std::endl;
  ofs << "Authors: AutoGenerated by System Architect Poar \\\\" << std::endl;
  ofs << "\\end{tabular}" << std::endl;
  ofs << "\\end{center}" << std::endl;

  ofs << std::endl << std::endl;

  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% TABLE OF CONTENTS" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\tableofcontents" << std::endl;
  ofs << "\\clearpage" << std::endl;

  ofs << std::endl << std::endl;

  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% LISTINGS" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\listoftables" << std::endl;

  // write the summary
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% SUMMARY SECTION" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\section{Design Summary}" << std::endl;
  ofs << std::endl << std::endl;
  ofs << "\\begin{longtable}[h]{| c | c |}" << std::endl;
  ofs << "\\hline" << std::endl;
  ofs << "\\textbf{Total Area} & " << TotalArea << " gates\\\\" << std::endl;
  ofs << "\\hline" << std::endl;
  ofs << "\\textbf{Total Power} & " << TotalPower << " mW\\\\" << std::endl;
  ofs << "\\hline" << std::endl;
  ofs << "\\caption{Power and Area Summary}" << std::endl;
  ofs << "\\label{tab:POARSummary}" << std::endl;
  ofs << "\\end{longtable}" << std::endl;
  ofs << std::endl << std::endl;
  ofs << std::endl << std::endl;

  // write the area information
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% AREA SECTION" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\section{Design Area}" << std::endl;
  ofs << std::endl << std::endl;
  ofs << "\\begin{longtable}[h]{| c | c |}" << std::endl;
  ofs << "\\hline" << std::endl;
  ofs << "\\textbf{Component} & \\textbf{Value} \\\\" << std::endl;
  ofs << "\\hline" << std::endl;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarArea) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      ofs << "\\texttt{" << EscapeUnderscore(CE.Name) << "} & " << CE.AdjustedResult << " gates \\\\" << std::endl;
      ofs << "\\hline" << std::endl;
    }
  }
  ofs << "\\hline" << std::endl;
  ofs << "\\caption{Component Area Summary}" << std::endl;
  ofs << "\\label{tab:POARAreaSummary}" << std::endl;
  ofs << "\\end{longtable}" << std::endl;
  ofs << std::endl << std::endl;
  ofs << std::endl << std::endl;

  // write the power information
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% POWER SECTION" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\section{Design Power}" << std::endl;
  ofs << std::endl << std::endl;
  ofs << "\\begin{longtable}[h]{| c | c |}" << std::endl;
  ofs << "\\hline" << std::endl;
  ofs << "\\textbf{Component} & \\textbf{Value} \\\\" << std::endl;
  ofs << "\\hline" << std::endl;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( (CE.VType == PoarConfig::PoarPower) &&
        (CE.Type  != PoarConfig::UNK_ENTRY) ){
      ofs << "\\texttt{" << EscapeUnderscore(CE.Name) << "} & " << CE.AdjustedResult << " mW \\\\" << std::endl;
      ofs << "\\hline" << std::endl;
    }
  }
  ofs << "\\caption{Component Power Summary}" << std::endl;
  ofs << "\\label{tab:POARPowerSummary}" << std::endl;
  ofs << "\\end{longtable}" << std::endl;
  ofs << std::endl << std::endl;
  ofs << std::endl << std::endl;

  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% DEVICE CONFIGURATION SUMMARY" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\section{Device Configuration Summary}" << std::endl;
  ofs << std::endl << std::endl;
  ofs << "\\begin{longtable}[h]{| c | c | c |}" << std::endl;
  ofs << "\\hline" << std::endl;
  ofs << "\\textbf{Component} & \\textbf{Type} & \\textbf{Unit Value} \\\\" << std::endl;
  ofs << "\\hline" << std::endl;
  for( unsigned i=0; i<PConfig->GetNumEntry(); i++ ){
    // retrieve the i'th entry
    PoarConfig::ConfigEntry CE = PConfig->GetEntry(i);

    if( CE.Type  != PoarConfig::UNK_ENTRY ){
      if( CE.VType == PoarConfig::PoarPower ){
        ofs << "\\texttt{" << EscapeUnderscore(CE.Name) << "} & Power & " << CE.DefaultVal << " mW\\\\" << std::endl;
      }else{
        ofs << "\\texttt{" << EscapeUnderscore(CE.Name) << "} & Area & " << CE.DefaultVal << " gates\\\\" << std::endl;
      }
      ofs << "\\hline" << std::endl;
    }
  }
  ofs << "\\caption{Device Configuration Summary}" << std::endl;
  ofs << "\\label{tab:POARDeviceSummary}" << std::endl;
  ofs << "\\end{longtable}" << std::endl;
  ofs << std::endl << std::endl;
  ofs << std::endl << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\end{document}" << std::endl;

  ofs.close();

  if( PrevFile ){
    CGDeleteFile(OFile + ".back");
  }

  return true;
}

bool PoarIO::WriteLatex(){
  if( OutFile.length() == 0 ){
    return false;
  }

  // create the directory if it doesn't exist
  if( !CGDirExists(OutFile.c_str()) ){
    if( !CGMkDirP(OutFile) )
      return false;
  }

  // write the makefile
  if( !WriteLatexMakefile() )
    return false;

  // write the latex body
  if( !WriteLatexTexfile() )
    return false;

  return true;
}

// EOF
