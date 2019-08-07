//
// _SpecDoc_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenBackend/Passes/Data/SpecDoc.h"

SpecDoc::SpecDoc(std::ostream *O,
                     CoreGenDAG *D,
                     CoreGenErrno *E)
  : CoreGenPass(CGPassData,3,"SpecDoc",
                "Generates a LaTeX Spec Document",false,O,D,E){
}

SpecDoc::~SpecDoc(){
}

std::string SpecDoc::EscapeUnderscore( std::string Str ){
  std::string New;
  for( unsigned i=0; i<Str.length(); i++ ){
    if( Str[i] == '_' ){
      New += '\\';
    }
    New += Str[i];
  }
  return New;
}

std::string SpecDoc::FixUnderscore( std::string Str ){
  std::string New;
  for( unsigned i=0; i<Str.length(); i++ ){
    if( Str[i] == '_' ){
      //New += '\\';
      New += "\\string";
    }
    New += Str[i];
  }
  return New;
}

bool SpecDoc::WriteMakefile() {
  // build the output file name
  std::string OFile = this->GetOutputPath() + "/Makefile";
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

bool SpecDoc::WriteOverviewTex(CoreGenDAG *DAG, std::ofstream &ofs ){
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% OVERVIEW" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\section{Overview}" << std::endl;
  ofs << "\\label{sec:Overview}" << std::endl;

  ofs << "This specification document was auto-generated by the System Architect " << std::endl
      << " CoreGen infrastructure.  Any changes to this file will be automatically " << std::endl
      << " overwritten by CoreGen." << std::endl;

  ofs << std::endl << std::endl;

  return true;
}

bool SpecDoc::WriteRegisterClassTex(CoreGenDAG *DAG, std::ofstream &ofs ){
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% REGISTER CLASSES" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\section{Register Classes}" << std::endl;
  ofs << "\\label{sec:RegisterClasses}" << std::endl;

  for( unsigned i=0; i<DAG->GetDimSize(); i++ ){
    CoreGenNode *N = DAG->FindNodeByIndex(i);
    if( N->GetType() == CGRegC ){
      CoreGenRegClass *RC = static_cast<CoreGenRegClass *>(N);
      ofs << std::endl << std::endl;
      ofs << "\\subsection{Register Class: " << EscapeUnderscore(RC->GetName()) << "}" << std::endl;
      ofs << "\\label{sec:" << FixUnderscore(RC->GetName()) << "}" << std::endl;

      ofs << "The following table represents the registers configured in the "
          << EscapeUnderscore(RC->GetName()) << " register class." << std::endl;

      // tabulate all the info on the registers
      ofs << std::endl << std::endl;
      ofs << "\\begin{longtable}[h]{c c c c c c c c}" << std::endl;
      ofs << "\\hline" << std::endl;
      ofs << "\\textbf{Name} & \\textbf{Index} & \\textbf{Width} & "
          << "\\textbf{Attrs} & \\textbf{SIMD} & \\textbf{SIMD Width} & "
          << "\\textbf{PseudoName} & \\textbf{FixedValue}\\\\" << std::endl;
      ofs << "\\hline" << std::endl;

      for( unsigned j=0; j<RC->GetNumReg(); j++ ){
        CoreGenReg *REG = RC->GetReg(j);

        std::string Attrs;
        bool OrVal = false;
        if( REG->IsRWAttr() ){
          Attrs+= "RW";
          OrVal = true;
        }
        if( REG->IsROAttr() ){
          if( OrVal ){
            Attrs+=":";
          }
          Attrs+="RO";
          OrVal = true;
        }
        if( REG->IsCSRAttr() ){
          if( OrVal ){
            Attrs+=":";
          }
          Attrs+="CSR";
          OrVal = true;
        }
        if( REG->IsAMSAttr() ){
          if( OrVal ){
            Attrs+=":";
          }
          Attrs+="AMS";
          OrVal = true;
        }
        if( REG->IsTUSAttr() ){
          if( OrVal ){
            Attrs+=":";
          }
          Attrs+="TUS";
          OrVal = true;
        }
        if( REG->IsPCAttr() ){
          if( OrVal ){
            Attrs+=":";
          }
          Attrs+="PC";
          OrVal = true;
        }
        if( REG->IsShared() ){
          if( OrVal ){
            Attrs+=":";
          }
          Attrs+="SH";
          OrVal = true;
        }

        ofs << EscapeUnderscore(REG->GetName())   << " & "  << std::hex << "0x" << REG->GetIndex() << std::dec
            << " & "
            << REG->GetWidth()  << " & "  << Attrs            << " & ";
        if( REG->IsSIMD() ){
          ofs << "Y & " << REG->GetSIMDWidth() << " & ";
        }else{
          ofs << "N & 0 & ";
        }
        if( REG->GetPseudoName().length() > 0 ){
          ofs << EscapeUnderscore(REG->GetPseudoName()) << " & ";
        }else{
          ofs << "NA & ";
        }
        if( REG->IsFixedValue() ){
          //ofs << "Y & ";
          std::vector<uint64_t> Vals;
          REG->GetFixedVals(Vals);

          // TODO: this is wrong
          for( signed k=Vals.size(); k>=0; k-- ){
            ofs << Vals[k];
          }
          ofs << "\\\\" << std::endl;
        }else{
          ofs << " NA \\\\" << std::endl;
        }
      }

      ofs << "\\caption{" << EscapeUnderscore(RC->GetName()) << " Registers}" << std::endl;
      ofs << "\\label{tab:" << FixUnderscore(RC->GetName()) << "Registers}" << std::endl;
      ofs << "\\end{longtable}" << std::endl;
    }
  }

  // count subregs
  unsigned count = 0;
  for( unsigned i=0; i<DAG->GetDimSize(); i++ ){
    CoreGenNode *N = DAG->FindNodeByIndex(i);
    if( N->GetType() == CGRegC ){
      CoreGenRegClass *RC = static_cast<CoreGenRegClass *>(N);
      for( unsigned j=0; j<RC->GetNumReg(); j++ ){
        count += RC->GetReg(j)->GetNumSubRegs();
      }
    }
  }

  // if we have at least one subreg, print them
  for( unsigned i=0; i<DAG->GetDimSize(); i++ ){
    CoreGenNode *N = DAG->FindNodeByIndex(i);
    if( N->GetType() == CGRegC ){
      CoreGenRegClass *RC = static_cast<CoreGenRegClass *>(N);
      unsigned count = 0;
      for( unsigned j=0; j<RC->GetNumReg(); j++ ){
        count += RC->GetReg(j)->GetNumSubRegs();
      }
      if( count > 0 ){
        ofs << std::endl << std::endl;
        ofs << "\\subsection{ " << EscapeUnderscore(RC->GetName()) << " Register Fields" << "}" << std::endl;
        ofs << "\\label{sec:" << FixUnderscore(RC->GetName()) << "RegisterFields}" << std::endl;
        for( unsigned j=0; j<RC->GetNumReg(); j++ ){
          CoreGenReg *REG = RC->GetReg(j);
          if( REG->GetNumSubRegs() > 0 ){
            ofs << std::endl << std::endl;
            ofs << "\\subsubsection{" << EscapeUnderscore(REG->GetName()) << " SubRegister Fields"
                << "}" << std::endl;
            ofs << "\\label{sec:" << FixUnderscore(RC->GetName())
                << FixUnderscore(REG->GetName())
                << "SubRegs}" << std::endl;

            ofs << "The following table represents the subregister fields configured in the "
                << EscapeUnderscore(REG->GetName()) << " register." << std::endl;


            ofs << std::endl << std::endl;
            ofs << "\\begin{longtable}[h]{c c c}" << std::endl;
            ofs << "\\hline" << std::endl;
            ofs << "\\textbf{Name} & \\textbf{StartBit} & \\textbf{EndBit}\\\\"
                << std::endl;
            ofs << "\\hline" << std::endl;

            for( unsigned k=0; k<REG->GetNumSubRegs(); k++ ){
              std::string SRName;
              unsigned StartBit;
              unsigned EndBit;
              if( !REG->GetSubReg( k, SRName, StartBit, EndBit ) ){
                return false;
              }
              ofs << EscapeUnderscore(SRName) << " & " << StartBit << " & " << EndBit << "\\\\"
                  << std::endl;
            }

            ofs << "\\caption{" << EscapeUnderscore(RC->GetName())
                << ":" << EscapeUnderscore(REG->GetName())
                << " Sub Registers}" << std::endl;
            ofs << "\\label{tab:" << FixUnderscore(RC->GetName())
                << FixUnderscore(REG->GetName())
                << "SubRegs}" << std::endl;
            ofs << "\\end{longtable}" << std::endl;
          }
        }
      }
    }
  }

  return true;
}

std::string SpecDoc::GenerateInstFormatHeader(CoreGenInstFormat *IF){
  std::string Header;
  double base = 0.18;
  for( signed i=(IF->GetNumFields()-1); i>=0; i-- ){
    unsigned Width = IF->GetFieldWidth(IF->GetFieldName(i));
    if( Width == 0 ){
      Header+=" ";
    }else{
      double FieldWidth = ((IF->GetEndBit(IF->GetFieldName(i)) -
       IF->GetStartBit(IF->GetFieldName(i)))+1)*base;
      Header = Header + "p{" + std::to_string(FieldWidth) + "in}";
    }

    if( i!=0 ){
      Header += "@{}";
    }
  }
  return Header;
}

bool SpecDoc::WriteInstFormatDotFile( CoreGenInstFormat *IF ){
  if( !IF )
    return false;

  std::cout << "building dot file for " << IF->GetName() << std::endl;

  std::string OFile = this->GetOutputPath() + "/" + CGRemoveDot(IF->GetName()) + ".dot";
  bool PrevFile = false;

  if( CGFileExists(OFile) ){
    PrevFile = true;
    CGFileCopy( OFile,
                OFile + ".back" );
    CGDeleteFile(OFile);
  }

  std::ofstream dofs;
  dofs.open(OFile.c_str(), std::ofstream::out | std::ofstream::app);
  if( !dofs.is_open() )
    return false;

  dofs << "digraph struct {" << std::endl;
  dofs << "\tnode[shape=record];" << std::endl;
  unsigned NumFields = IF->GetNumFields();
  for( signed j=NumFields-1; j>=0; j-- ){
    if( j==(NumFields-1) ){
      dofs << "struct1 [label=\"";
    }else{
      dofs << "|";
    }
    dofs << IF->GetFieldName(j) << "\\n["
        << std::to_string(IF->GetEndBit(IF->GetFieldName(j))) << ":"
        << std::to_string(IF->GetStartBit(IF->GetFieldName(j))) << "]";
  }
  dofs << "\"];" << std::endl;
  dofs << "}" << std::endl;

  dofs.close();

  if( PrevFile ){
    CGDeleteFile(OFile + ".back");
  }
  return true;
}

bool SpecDoc::WriteInstFormatTex(CoreGenDAG *DAG, std::ofstream &ofs ){
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% INSTRUCTION FORMATS" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\section{Instruction Formats}" << std::endl;
  ofs << "\\label{sec:InstructionFormats}" << std::endl;

  for( unsigned i=0; i<DAG->GetDimSize(); i++ ){
    CoreGenNode *N = DAG->FindNodeByIndex(i);
    if( N->GetType() == CGInstF ){
      CoreGenInstFormat *IF = static_cast<CoreGenInstFormat *>(N);

      ofs << "\\subsection{InstructionFormat: " << EscapeUnderscore(IF->GetName()) << "}" << std::endl;
      ofs << "\\label{sec:" << FixUnderscore(IF->GetName()) << "}" << std::endl;
      ofs << std::endl << std::endl;

      ofs << "The following diagrams represent the " << EscapeUnderscore(IF->GetName())
          << " instruction format from the " << EscapeUnderscore(IF->GetISA()->GetName())
          << " instruction set architecture." << std::endl;

      // write out the dot file for the target inst format
      WriteInstFormatDotFile(IF);

      std::string FigFile = CGRemoveDot(IF->GetName()) + ".pdf";

      ofs << "\\vspace{0.25in}" << std::endl;
      ofs << "\\begin{figure}[H]" << std::endl;
      ofs << "\\begin{center}" << std::endl;
      ofs << "\\includegraphics[width=6.5in]{" << FigFile << "}" << std::endl;
      ofs << "\\caption{" << EscapeUnderscore(IF->GetName()) << "}" << std::endl;
      ofs << "\\label{fig:" << FixUnderscore(IF->GetName()) << "}" << std::endl;
      ofs << "\\end{center}" << std::endl;
      ofs << "\\end{figure}" << std::endl;

      ofs << std::endl << std::endl;

      // print all the field details
      ofs << "\\begin{table}[h]" << std::endl;
      ofs << "\\begin{center}" << std::endl;
      ofs << "\\caption{" << EscapeUnderscore(IF->GetName()) << " Fields}" << std::endl;
      ofs << "\\label{tab:" << FixUnderscore(IF->GetName()) << "Fields}" << std::endl;
      ofs << "\\begin{tabular}{|c | c | c | c | c | c|}" << std::endl;
      ofs << "\\hline" << std::endl;
      ofs << "\\textbf{Name} & \\textbf{EndBit} & \\textbf{StartBit} & "
          << "\\textbf{FieldType} & \\textbf{RegClass} & \\textbf{Mandatory?}\\\\"
          << std::endl;
      ofs << "\\hline" << std::endl;
      ofs << "\\hline" << std::endl;
      for( signed j=(IF->GetNumFields()-1); j>=0; --j ){
        ofs << IF->GetFieldName(j) << " & ";
        ofs << IF->GetEndBit(IF->GetFieldName(j)) << " & ";
        ofs << IF->GetStartBit(IF->GetFieldName(j)) << " & ";
        ofs << IF->CGInstFieldToStr(IF->GetFieldType(IF->GetFieldName(j)))
            << " & ";
        if( IF->GetFieldType(IF->GetFieldName(j)) == CoreGenInstFormat::CGInstReg){
          ofs << EscapeUnderscore(IF->GetFieldRegClass(IF->GetFieldName(j))->GetName())
              << " & ";
        }else{
          ofs << "N/A & ";
        }
        if( IF->GetMandatoryFlag(IF->GetFieldName(j)) ){
          ofs << " Yes \\\\" << std::endl;
        }else{
          ofs << " No \\\\" << std::endl;
        }
        ofs << "\\hline" << std::endl;
      }

      ofs << "\\end{tabular}" << std::endl;
      ofs << "\\end{center}" << std::endl;
      ofs << "\\end{table}" << std::endl;

      ofs << std::endl << std::endl;
    }
  }

  return true;
}

bool SpecDoc::WriteInstTex(CoreGenDAG *DAG, std::ofstream &ofs ){
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% INSTRUCTIONS " << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\section{Instructions}" << std::endl;
  ofs << "\\label{sec:Instructions}" << std::endl;

  for( unsigned i=0; i<DAG->GetDimSize(); i++ ){
    CoreGenNode *N = DAG->FindNodeByIndex(i);
    if( N->GetType() == CGInst ){
      CoreGenInst *INST = static_cast<CoreGenInst *>(N);

      ofs << "\\subsection{" << FixUnderscore(INST->GetName()) << "}" << std::endl;
      ofs << "\\label{sec:" << FixUnderscore(INST->GetName()) << "}" << std::endl;
      ofs << "The " << EscapeUnderscore(INST->GetName()) << " instruction belongs to the "
          << EscapeUnderscore(INST->GetISA()->GetName()) << " instruction set.  It has the following features:"
          << std::endl;

      // print instruction mnemonic
      if( INST->IsSyntax() ){
        std::string Syntax = INST->GetSyntax();
        Syntax.erase(std::remove(Syntax.begin(), Syntax.end(), '%'), Syntax.end());
        Syntax.erase(std::remove(Syntax.begin(), Syntax.end(), '$'), Syntax.end());
        ofs << "\\textbf{Assembly Mnemonic} : " << EscapeUnderscore(Syntax) << std::endl;
      }

      // print an instruction table
      ofs << "\\begin{center}" << std::endl;
      ofs << "\\begin{longtable}{c c c}" << std::endl;
      ofs << "\\caption{" << EscapeUnderscore(INST->GetName()) << " Information} ";
      ofs << "\\label{tab:" << FixUnderscore(INST->GetName()) << "INFO}\\\\" << std::endl;
      ofs << "\\hline" << std::endl;
      ofs << "\\textbf{General Info} & &\\\\" << std::endl;
      ofs << "\\hline" << std::endl;
      ofs << "Name & " << EscapeUnderscore(INST->GetName()) << " &\\\\" << std::endl;
      ofs << "ISA & " << EscapeUnderscore(INST->GetISA()->GetName()) << " &\\\\" << std::endl;
      ofs << "Instruction Format & " << EscapeUnderscore(INST->GetFormat()->GetName()) << " &\\\\" << std::endl;
      ofs << "\\hline" << std::endl;

      ofs << "\\textbf{Encoding Info} & &\\\\" << std::endl;
      ofs << "\\textit{Field} & \\textit{Width(bits)} & \\textit{Value}\\\\" << std::endl;
      for( unsigned j=0; j<INST->GetNumEncodings(); j++ ){
        CoreGenEncoding *E = INST->GetEncoding(j);
        ofs << E->GetField() << " & "
            << E->GetLength() << " & "
            << std::hex << "0x" << E->GetEncoding() << std::dec << "\\\\" << std::endl;
      }

      ofs << "\\hline" << std::endl;
      ofs << "\\end{longtable}" << std::endl;
      ofs << "\\end{center}" << std::endl;

      ofs << std::endl << std::endl;

      // print the stonecutter source
      if( INST->IsImpl() ){
        ofs << "The instruction is described by the following StoneCutter source code:"
            << std::endl;
        ofs << std::endl;
        ofs << "\\vspace{0.125in}" << std::endl;
        ofs << "\\begin{lstlisting}[frame=single,style=base,caption={"
            << EscapeUnderscore(INST->GetName()) << " StoneCutter Implementation"
            << "},captionpos=b,label={lis:"
            << EscapeUnderscore(INST->GetName())
            << "}]"
            << std::endl;
        ofs << INST->GetImpl() << std::endl;
        ofs << "\\end{lstlisting}" << std::endl;
        ofs << std::endl << std::endl;
      }
    }
  }

  return true;
}

bool SpecDoc::WritePseudoInstTex(CoreGenDAG *DAG, std::ofstream &ofs ){
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% PSEUDO INSTRUCTIONS" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\section{Pseudo Instructions}" << std::endl;
  ofs << "\\label{sec:PseudoInstructions}" << std::endl;

  for( unsigned i=0; i<DAG->GetDimSize(); i++ ){
    CoreGenNode *N = DAG->FindNodeByIndex(i);
    if( N->GetType() == CGPInst ){
      CoreGenPseudoInst *INST = static_cast<CoreGenPseudoInst *>(N);

      ofs << "\\subsection{" << FixUnderscore(INST->GetName()) << "}" << std::endl;
      ofs << "\\label{sec:" << FixUnderscore(INST->GetName()) << "}" << std::endl;
      ofs << "The " << EscapeUnderscore(INST->GetName()) << " pseudo instruction belongs to the "
          << EscapeUnderscore(INST->GetISA()->GetName()) << " instruction set.  It is associated with the "
          << EscapeUnderscore(INST->GetInst()->GetName()) << " instruction.  It has the following features:"
          << std::endl;

      // print an instruction table
      ofs << "\\begin{center}" << std::endl;
      ofs << "\\begin{longtable}{c c c}" << std::endl;
      ofs << "\\caption{" << EscapeUnderscore(INST->GetName()) << " Information} ";
      ofs << "\\label{tab:" << FixUnderscore(INST->GetName()) << "INFO}\\\\" << std::endl;
      ofs << "\\hline" << std::endl;
      ofs << "\\textbf{General Info} & &\\\\" << std::endl;
      ofs << "\\hline" << std::endl;

      ofs << "Name & " << EscapeUnderscore(INST->GetName()) << " &\\\\" << std::endl;
      ofs << "ISA & " << EscapeUnderscore(INST->GetISA()->GetName()) << " &\\\\" << std::endl;
      ofs << "Instruction & " << EscapeUnderscore(INST->GetInst()->GetName())
          << "[Section \\ref{sec:" << EscapeUnderscore(INST->GetInst()->GetName()) << "}] &\\\\" << std::endl;
      ofs << "Instruction Format & " << EscapeUnderscore(INST->GetInst()->GetFormat()->GetName()) << " &\\\\" << std::endl;
      ofs << "\\hline" << std::endl;

      ofs << "\\textbf{Encoding Info} & &\\\\" << std::endl;
      ofs << "\\textit{Field} & \\textit{Width(bits)} & \\textit{Value}\\\\" << std::endl;
      for( unsigned j=0; j<INST->GetNumEncodings(); j++ ){
        CoreGenEncoding *E = INST->GetEncoding(j);
        ofs << E->GetField() << " & "
            << E->GetLength() << " & "
            << std::hex << "0x" << E->GetEncoding() << std::dec << "\\\\" << std::endl;
      }


      ofs << "\\hline" << std::endl;
      ofs << "\\end{longtable}" << std::endl;
      ofs << "\\end{center}" << std::endl;

      ofs << std::endl << std::endl;
    }
  }

  return true;
}

bool SpecDoc::WriteInstTableTex(CoreGenDAG *DAG, std::ofstream &ofs ){
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% INSTRUCTION TABLE" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\clearpage" << std::endl;
  ofs << "\\section{Instruction Table}" << std::endl;
  ofs << "\\label{sec:InstructionTable}" << std::endl;

  return true;
}

bool SpecDoc::WriteTexfile(CoreGenDAG *DAG) {
  // build the output file name
  std::string OFile = this->GetOutputPath() + "/spec.tex";
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
  ofs << "\% System Architect CoreGen Auto-generated Specification Document" << std::endl;
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

  ofs << "\\newcommand{\\note}[1]{{\\bf [ NOTE: #1 ]}}" << std::endl;

  // tabuluar column widths for instruction formats
  ofs << "\\newcommand{\\instbit}[1]{\\mbox{\\scriptsize #1}}" << std::endl;
  ofs << "\\newcommand{\\instbitrange}[2]{~\\instbit{#1} \\hfill \\instbit{#2}~}" << std::endl;
  ofs << "\\newcommand{\\reglabel}[1]{\\hfill {\\tt #1}\\hfill\\ }" << std::endl;
  ofs << "\\newcommand{\\wiri}{\\textbf{WIRI}}" << std::endl;
  ofs << "\\newcommand{\\wpri}{\\textbf{WPRI}}" << std::endl;
  ofs << "\\newcommand{\\wlrl}{\\textbf{WLRL}}" << std::endl;
  ofs << "\\newcommand{\\warl}{\\textbf{WARL}}" << std::endl;
  ofs << "\\newcommand\\purl[1]{\\protect\\url{#1}}" << std::endl;


  ofs << std::endl << std::endl;

  ofs << "\\setlength{\\parindent}{1em}" << std::endl;
  ofs << "\\setlength{\\parskip}{1em}" << std::endl;
  ofs << std::endl << std::endl;
  ofs << "\\lstdefinestyle{base}{" << std::endl;
  ofs << "\tlanguage=C++," << std::endl;
  ofs << "\tnumbers=left," << std::endl;
  ofs << "\tstepnumber=1," << std::endl;
  ofs << "\temptylines=1," << std::endl;
  ofs << "\tbreaklines=true," << std::endl;
  ofs << "\tbasicstyle=\\ttfamily\\color{black}," << std::endl;
  ofs << "\tmoredelim=**[is][\\color{red}]{@}{@}," << std::endl;
  ofs << "}" << std::endl;

  ofs << std::endl << std::endl;

  ofs << "\\hypersetup{" << std::endl;
  ofs << "\tcolorlinks=true," << std::endl;
  ofs << "\tlinktoc=all," << std::endl;
  ofs << "\tlinkcolor=black," << std::endl;
  ofs << "\tcitecolor=black," << std::endl;
  ofs << "\turlcolor=black" << std::endl;
  ofs << "}" << std::endl;

  ofs << std::endl << std::endl;

  ofs << "\\setlength\\parindent{0pt}" << std::endl;
  ofs << "\\renewcommand{\\labelenumi}{\\alph{enumi}.}" << std::endl;

  ofs << std::endl << std::endl;

  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% DOCUMENT LAYOUT INFORMATION" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\\pagestyle{fancy}" << std::endl;
  ofs << "\\lhead{}" << std::endl;
  ofs << "\\chead{SPECIFICATION}" << std::endl;
  ofs << "\\rhead{}" << std::endl;
  ofs << "\\lfoot{" << CGCurrentDateTime() << "}" << std::endl;
  ofs << "\\cfoot{SPECIFICATION}" << std::endl;
  ofs << "\\rfoot{\\thepage}" << std::endl;

  ofs << std::endl << std::endl;

  ofs << "\% ---------------------------------------------------------------" << std::endl;
  ofs << "\% DOCUMENT INFORMATION" << std::endl;
  ofs << "\% ---------------------------------------------------------------" << std::endl;

  ofs << "\\title{Instruction Set Architecture Specification}" << std::endl;
  ofs << "\\author{Auto-Generated by System Architect CoreGen}" << std::endl;
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
  ofs << "Authors: AutoGenerated by System Architect CoreGen \\\\" << std::endl;
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
  ofs << "\\listoffigures" << std::endl;
  ofs << "\\lstlistoflistings" << std::endl;
  ofs << "\\listoftables" << std::endl;

  bool rtn = true;

  //-- write the overview
  if( !WriteOverviewTex( DAG, ofs ) ){
    rtn = false;
  }

  //-- write the register classes
  if( !WriteRegisterClassTex( DAG, ofs ) ){
    rtn = false;
  }

  //-- writer the instruction formats
  if( !WriteInstFormatTex( DAG, ofs ) ){
    rtn = false;
  }

  //-- write the instructions
  if( !WriteInstTex( DAG, ofs ) ){
    rtn = false;
  }

  //-- write the pseudo instructions
  if( !WritePseudoInstTex( DAG, ofs ) ){
    rtn = false;
  }

  //-- write the instruction table
  if( !WriteInstTableTex( DAG, ofs ) ){
    rtn = false;
  }

  ofs << "\\clearpage" << std::endl;
  ofs << "\\end{document}" << std::endl;

  ofs.close();

  if( PrevFile ){
    CGDeleteFile(OFile + ".back");
  }
  return rtn;
}

bool SpecDoc::Execute(){

  // check the output path
  if( this->GetOutputPath().length() == 0 ){
    // requires an output path to be set
    WriteMsg("Error: output path is not set for " +
             this->GetName() );
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Output path is not set " );
    return false;
  }

  // Get the correct DAG Level 1
  CoreGenDAG *D1 = DAG->GetDAGFromLevel(this->GetLevel());
  if( D1 == nullptr ){
    WriteMsg( "Error obtaining DAG Level " + std::to_string(this->GetLevel()));
    Errno->SetError( CGERR_ERROR, this->GetName() +
                     " : Error obtaining DAG Level " +
                     std::to_string(this->GetLevel()));
    return false;
  }

  // Write the makefile
  if( !WriteMakefile() ){
    return false;
  }

  // Write the tex file
  if( !WriteTexfile(D1) ){
    return false;
  }

  return true;
}

// EOF
