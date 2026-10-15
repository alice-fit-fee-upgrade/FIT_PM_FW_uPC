`timescale 1ns/1ps
module IBUFDS #(parameter DIFF_TERM="TRUE")(input I,IB,output O);assign O=I;endmodule
module BUFG(input I,output O);assign O=I;endmodule
module OBUFT(input I,T,output O);assign O=T?1'bz:I;endmodule
module eth_jtag(input [511:0] report,output reg [15:0] command=2);endmodule
module pm12_mcu_spi(input SCKI,SPI_CS,MOSII,output MISOI,output [15:0] scratch_out,writes_out);
 assign MISOI=0;assign scratch_out=0;assign writes_out=0;
endmodule
module tb;
reg clk=0;always #4 clk=~clk;
reg [2:0] lines=7;wire miso;
pm12_analyzer #(.CAPTURE_CYCLES(10000)) dut(clk,~clk,lines[0],lines[1],lines[2],1'b0,1'b0,1'b1,miso);
task cycles(input integer n);repeat(n) @(negedge clk);endtask
integer i;reg [31:0] last;
initial begin
 cycles(10);dut.scan.command=16'h8000;cycles(10);
 if(!dut.active || dut.count!=1 || dut.events[0]!=={3'b111,32'b0}) $fatal(1,"arm/initial sample");
 for(i=0;i<10;i=i+1) begin lines[1]=~lines[1];cycles(10);end
 if(dut.count!=11) $fatal(1,"lost changes");
 last=0;
 for(i=1;i<11;i=i+1) begin
  if(dut.events[i][31:0]<=last || dut.events[i][34:32]!==((i%2)?3'b101:3'b111)) $fatal(1,"bad timestamp/levels");
  last=dut.events[i][31:0];
 end
 cycles(10000);
 if(dut.active || dut.count!=11 || dut.elapsed!=10000) $fatal(1,"timeout");
 dut.scan.command=16'h000a;cycles(10);
 if(!dut.active || dut.count!=1) $fatal(1,"rearm");
 for(i=0;i<2050;i=i+1) begin lines[0]=~lines[0];cycles(4);end
 if(dut.active || dut.count!=2048) $fatal(1,"full stop");
 dut.scan.command=16'h0011;cycles(10);
 if(dut.selected!==dut.events[17]) $fatal(1,"indexed readback");
 if(dut.report[31:0]!==32'h504d5a03 || dut.report[87:72]!=2048 || dut.report[162:128]!==dut.events[17]) $fatal(1,"report layout");
 $display("PASS arm/change timestamps/timeout/rearm/full/indexed report");$finish;
end
endmodule
