//
// _SpadCG_cpp_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#include "CoreGen/CoreGenCodegen/NodeCodegen/SpadCG.h"

SpadCG::SpadCG( CoreGenNode *N, CoreGenProj *P,
                std::string Pack, std::string Path,
                bool Comm, CoreGenErrno *E )
  : CoreGenNodeCodegen(N,P,Pack,Path,Comm,E) {
}

SpadCG::~SpadCG(){
}

bool SpadCG::WriteSpad(std::ofstream &O ){
  // write the comment
  O << "//-- Common Scratchpad" << std::endl;

  O << "trait MemoryOpConstants" << std::endl
    << "{" << std::endl
    << "\tval MT_X  = 0.asUInt(3.W)" << std::endl
    << "\tval MT_B  = 1.asUInt(3.W)" << std::endl
    << "\tval MT_H  = 2.asUInt(3.W)" << std::endl
    << "\tval MT_W  = 3.asUInt(3.W)" << std::endl
    << "\tval MT_D  = 4.asUInt(3.W)" << std::endl
    << "\tval MT_BU = 5.asUInt(3.W)" << std::endl
    << "\tval MT_HU = 6.asUInt(3.W)" << std::endl
    << "\tval MT_WU = 7.asUInt(3.W)" << std::endl
    << "\tval M_X   = \"b0\".asUInt(1.W)" << std::endl
    << "\tval M_XRD = \"b0\".asUInt(1.W)" << std::endl
    << "\tval M_XWR = \"b1\".asUInt(1.W)" << std::endl
    << "\tval DPORT = 0" << std::endl
    << "\tval IPORT = 1" << std::endl
    << "}" << std::endl << std::endl;

  O << "class Rport(val addrWidth : Int,val dataWidth : Int) extends Bundle{" << std::endl
    << "\tval addr = Input(UInt(addrWidth.W))" << std::endl
    << "\tval data = Output(UInt(dataWidth.W)" << std::endl
    << "\toverride def cloneType = { new Rport(addrWidth,dataWidth).asInstanceOf[this.type] }" << std::endl
    << "}" << std::endl << std::endl;

  O << "class Wport(val addrWidth : Int,val dataWidth : Int) extends Bundle{" << std::endl
    << "\tval maskWidth = dataWidth/8" << std::endl
    << "\tval addr = Input(UInt(addrWidth.W))" << std::endl
    << "\tval data = Input(UInt(dataWidth.W))" << std::endl
    << "\tval mask = Input(UInt(maskWidth.W))" << std::endl
    << "\tval en = Input(Bool())" << std::endl
    << "\toverride def cloneType = { new Wport(addrWidth,dataWidth).asInstanceOf[this.type] }" << std::endl
    << "}" << std::endl << std::endl;

  O << "class d2h2i1(val addrWidth : Int) extends Bundle{" << std::endl
    << "\tval dataInstr = Vec(2,new  Rport(addrWidth,32))" << std::endl
    << "\tval hw = new  Wport(addrWidth,32)" << std::endl
    << "\tval dw = new  Wport(addrWidth,32)" << std::endl
    << "\tval hr = new  Rport(addrWidth,32)" << std::endl
    << "\tval clk = Input(Clock())" << std::endl
    << "}" << std::endl << std::endl;

  O << "class AsyncReadMem(val addrWidth : Int) extends BlackBox{" << std::endl
    << "val io = IO(new d2h2i1(addrWidth))" << std::endl
    << "}" << std::endl << std::endl;

  O << "class MemPortIo(data_width: Int)(implicit val conf: "
    << CGRemoveDot(Proj->GetProjName()) << "Configuration) extends Bundle{" << std::endl
    << "\tval req    = new DecoupledIO(new MemReq(data_width))" << std::endl
    << "\tval resp   = Flipped(new ValidIO(new MemResp(data_width)))" << std::endl
    << "\toverride def cloneType = { new MemPortIo(data_width).asInstanceOf[this.type] }" << std::endl
    << "}" << std::endl << std::endl;

  O << "class MemReq(data_width: Int)(implicit val conf: "
    << CGRemoveDot(Proj->GetProjName()) << "Configuration) extends Bundle{" << std::endl
    << "\tval addr = Output(UInt(conf.xprlen.W))" << std::endl
    << "\tval data = Output(UInt(data_width.W))" << std::endl
    << "\tval fcn  = Output(UInt(M_X.getWidth.W))" << std::endl
    << "\tval typ  = Output(UInt(MT_X.getWidth.W))" << std::endl
    << "\toverride def cloneType = { new MemReq(data_width).asInstanceOf[this.type] }" << std::endl
    << "}" << std::endl << std::endl;

  O << "class MemResp(data_width: Int) extends Bundle{" << std::endl
    << "\tval data = Output(UInt(data_width.W))" << std::endl
    << "\toverride def cloneType = { new MemResp(data_width).asInstanceOf[this.type] }" << std::endl
    << "}" << std::endl << std::endl;

  O << "class AsyncScratchPadMemory(num_core_ports: Int, num_bytes: Int = (1 << 21))"
    << "(implicit val conf: " << CGRemoveDot(Proj->GetProjName()) << "Configuration) extends Module{" << std::endl
    << "\tval io = IO(new Bundle" << std::endl
    << "\t{" << std::endl
    << "\t\tval core_ports = Vec(num_core_ports, Flipped(new MemPortIo(data_width = conf.xprlen)) )" << std::endl
    << "\t\tval debug_port = Flipped(new MemPortIo(data_width = 32))" << std::endl
    << "\t})" << std::endl
    << "\tval num_bytes_per_line = 8" << std::endl
    << "\tval num_lines = num_bytes / num_bytes_per_line" << std::endl
    << "\tprintln(\\n " << CGRemoveDot(Proj->GetProjName()) << ": Creating scratchpad memory of size: "
    << "\" + num_lines*num_bytes_per_line/1024 + \"kB\\n\")" << std::endl
    << "\tval async_data = Module(new AsyncReadMem(log2Ceil(num_bytes)))" << std::endl
    << "\tasync_data.io.clk := clock" << std::endl
    << "\tfor (i <- 0 until num_core_ports)" << std::endl
    << "\t{" << std::endl
    << "\t\tio.core_ports(i).resp.valid := io.core_ports(i).req.valid" << std::endl
    << "\t\tio.core_ports(i).req.ready := true.B" << std::endl
    << "\t\tasync_data.io.dataInstr(i).addr := io.core_ports(i).req.bits.addr" << std::endl
    << "\t}" << std::endl
    << std::endl
    << "\tval req_addri = io.core_ports(DPORT).req.bits.addr" << std::endl
    << std::endl
    << "\tval req_typi = io.core_ports(DPORT).req.bits.typ" << std::endl
    << "\tval resp_datai = async_data.io.dataInstr(DPORT).data" << std::endl
    << "\tio.core_ports(DPORT).resp.bits.data := MuxCase(resp_datai,Array(" << std::endl
    << "\t\t(req_typi === MT_B) -> Cat(Fill(24,resp_datai(7)),resp_datai(7,0))," << std::endl
    << "\t\t(req_typi === MT_H) -> Cat(Fill(16,resp_datai(15)),resp_datai(15,0))," << std::endl
    << "\t\t(req_typi === MT_BU) -> Cat(Fill(24,0.U),resp_datai(7,0))," << std::endl
    << "\t\t(req_typi === MT_HU) -> Cat(Fill(16,0.U),resp_datai(15,0))" << std::endl
    << "\t))" << std::endl
    << "\tasync_data.io.dw.en := Mux((io.core_ports(DPORT).req.bits.fcn === M_XWR),true.B,false.B)" << std::endl
    << "\twhen (io.core_ports(DPORT).req.valid && (io.core_ports(DPORT).req.bits.fcn === M_XWR))" << std::endl
    << "\t{" << std::endl
    << "\t\tasync_data.io.dw.data := io.core_ports(DPORT).req.bits.data << (req_addri(1,0) << 3)" << std::endl
    << "\t\tasync_data.io.dw.addr := Cat(req_addri(31,2),0.asUInt(2.W))" << std::endl
    << "\t\tasync_data.io.dw.mask := Mux(req_typi === MT_B,1.U << req_addri(1,0)," << std::endl
    << "\t\t\tMux(req_typi === MT_H,3.U << req_addri(1,0),15.U))" << std::endl
    << "\t}" << std::endl
    << std::endl
    << "\tif (num_core_ports == 2){" << std::endl
    << "\t\tio.core_ports(IPORT).resp.bits.data := async_data.io.dataInstr(IPORT).data" << std::endl
    << "\t}" << std::endl
    << std::endl
    << "\t// DEBUG PORTS" << std::endl
    << "\tio.debug_port.req.ready := true.B" << std::endl
    << "\tio.debug_port.resp.valid := io.debug_port.req.valid" << std::endl
    << "\tasync_data.io.hr.addr := io.debug_port.req.bits.addr" << std::endl
    << "\tio.debug_port.resp.bits.data := async_data.io.hr.data" << std::endl
    << "\tasync_data.io.hw.en := Mux((io.debug_port.req.bits.fcn === M_XWR),true.B,false.B)" << std::endl
    << "\twhen (io.debug_port.req.valid && io.debug_port.req.bits.fcn === M_XWR)" << std::endl
    << "\t{" << std::endl
    << "\t\tasync_data.io.hw.addr := io.debug_port.req.bits.addr" << std::endl
    << "\t\tasync_data.io.hw.data := io.debug_port.req.bits.data" << std::endl
    << "\t\tasync_data.io.hw.mask := 15.U" << std::endl
    << "\t}" << std::endl
    << "}" << std::endl << std::endl;

  O << "object GenVec" << std::endl
    << "{" << std::endl
    << "\tdef apply(din: UInt): Vec[UInt] =" << std::endl
    << "\t{" << std::endl
    << "\t\tval dout = Wire(Vec(8, UInt(8.W)))" << std::endl
    << "\t\tdout(0) := din(7,0)" << std::endl
    << "\t\tdout(1) := din(15,8)" << std::endl
    << "\t\tdout(2) := din(23,16)" << std::endl
    << "\t\tdout(3) := din(31,24)" << std::endl
    << "\t\tdout(4) := din(39,32)" << std::endl
    << "\t\tdout(5) := din(47,40)" << std::endl
    << "\t\tdout(6) := din(55,48)" << std::endl
    << "\t\tdout(7) := din(63,56)" << std::endl
    << "\t\treturn dout" << std::endl
    << "\t}" << std::endl
    << "}" << std::endl << std::endl;

  O << "object StoreDataGen" << std::endl
    << "{" << std::endl
    << "\tdef apply(din: Bits, typ: UInt, idx: UInt): Vec[UInt] =" << std::endl
    << "\t{" << std::endl
    << "\t\tval word = typ === MT_W" << std::endl
    << "\t\tval half = typ === MT_H" << std::endl
    << "\t\tval dout = Wire(Vec(8, UInt(8.W)))" << std::endl
    << "\t\tdout(idx) := din(7,0)" << std::endl
    << "\t\tdout(idx + 1.U) := Mux(half, din(15,8), 0.U)" << std::endl
    << "\t\tdout(idx + 2.U) := Mux(word, din(23,16), 0.U)" << std::endl
    << "\t\tdout(idx + 3.U) := Mux(word, din(31,24), 0.U)" << std::endl
    << "\t\treturn dout" << std::endl
    << "\t}" << std::endl
    << "}" << std::endl << std::endl;

  O << "object StoreMask" << std::endl
    << "{" << std::endl
    << "\tdef apply(sel: UInt, idx: UInt): UInt =" << std::endl
    << "\t{" << std::endl
    << "\t\tval word = sel === MT_W" << std::endl
    << "\t\tval half = sel === MT_H" << std::endl
    << "\t\tval wmask = Wire(UInt(8.W))" << std::endl
    << "\t\tval temp_byte = Wire(UInt(8.W))" << std::endl
    << "\t\tval temp_half = Wire(UInt(8.W))" << std::endl
    << "\t\tval temp_word = Wire(UInt(8.W))" << std::endl
    << "\t\ttemp_byte :=  1.U << idx" << std::endl
    << "\t\ttemp_half := 3.U << idx" << std::endl
    << "\t\ttemp_word := 15.U << idx" << std::endl
    << "\t\twmask := Mux(word, temp_word," << std::endl
    << "\t\t\tMux(half, temp_half, temp_byte))" << std::endl
    << "\t\treturn wmask" << std::endl
    << "\t}" << std::endl
    << "}" << std::endl << std::endl;

  O << "object LoadDataGen" << std::endl
    << "{" << std::endl
    << "\tdef apply(data: Vec[UInt], typ: UInt, idx: UInt) : UInt =" << std::endl
    << "\t{" << std::endl
    << "\t\tval word = (typ === MT_W) || (typ === MT_WU)" << std::endl
    << "\t\tval half = (typ === MT_H) || (typ === MT_HU)" << std::endl
    << "\t\tval dout_7_0 = Wire(UInt(8.W))" << std::endl
    << "\t\tval dout_15_8 = Wire(UInt(8.W))" << std::endl
    << "\t\tval dout_31_16 = Wire(UInt(16.W))" << std::endl
    << "\t\tdout_7_0 := data(idx)" << std::endl
    << "\t\tdout_15_8 := Mux(half, data(idx + 1.U), 0.U)" << std::endl
    << "\t\tdout_31_16 := Mux(word, Cat(data(idx + 3.U),data(idx + 2.U)), 0.U)" << std::endl
    << "\t\treturn Cat(dout_31_16,Cat(dout_15_8,dout_7_0))" << std::endl
    << "\t}" << std::endl
    << "}" << std::endl << std::endl;

  return true;
}

bool SpadCG::Execute(){

  // already generated this register file type
  if( CGFileExists(Path) )
    return true;

  // open the output file
  std::ofstream OutFile;
  OutFile.open(Path,std::ios::trunc);
  if(!OutFile.is_open()){
    Errno->SetError(CGERR_ERROR, "Could not open chisel file for scratchpad:" +
                    Path );
    return false;
  }

  // write the header
  OutFile << "// -- " << Node->GetName()
          << " Chisel Output" << std::endl << std::endl;

  // write the package info
  if( !WritePackageInfo(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write standard package info to file : " +
                    Path );
    OutFile.close();
    return false;
  }

  // import the chisel imports
  if( !WriteChiselImports(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write standard Chisel header to file : " + 
                    Path );
    OutFile.close();
    return false;
  }

  // write out the package block
  if( !WriteSpad(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write scratchpad block to file : " +
                    Path );
    OutFile.close();
    return false;
  }

  if( !WriteStdFooter(OutFile) ){
    Errno->SetError(CGERR_ERROR, "Could not write standard Chisel footer to file : " + 
                    Path );
    OutFile.close();
    return false;
  }

  // close the output file
  OutFile.close();

  return true;
}

// EOF
