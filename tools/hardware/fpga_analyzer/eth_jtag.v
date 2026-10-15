module eth_jtag(input [511:0] report, output reg [15:0] command=16'h0002);
wire tck,reset,cap,shift,sel,tdi; wire tck2,reset2,cap2,shift2,sel2,tdi2,update2;
reg [511:0] sr=0; reg [15:0] cr=0;
BSCANE2 #(.JTAG_CHAIN(1)) read_scan(.TCK(tck),.RESET(reset),.CAPTURE(cap),.SHIFT(shift),.SEL(sel),.TDI(tdi),.TDO(sr[0]));
BSCANE2 #(.JTAG_CHAIN(2)) cmd_scan(.TCK(tck2),.RESET(reset2),.CAPTURE(cap2),.SHIFT(shift2),.SEL(sel2),.TDI(tdi2),.UPDATE(update2),.TDO(1'b0));
always @(posedge tck) if(sel && cap) sr<=report; else if(sel && shift) sr<={tdi,sr[511:1]};
always @(posedge tck2) if(sel2 && shift2) cr<={tdi2,cr[15:1]};
always @(negedge tck2) if(sel2 && update2) command<=cr;
endmodule
