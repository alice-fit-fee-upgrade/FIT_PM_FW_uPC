// Passive, event-compressed logic capture. No I2C pin has an output driver.
// USER2: bit15 toggles arm, bits10:0 select event RAM address.
// USER1 report: magic32,tick32,lines8,count16,active8,elapsed32,
// event64 (timestamp32, lines3, padding29), selected address16.
module pm12_analyzer #(parameter CAPTURE_CYCLES=120000000)(
 input MCLK1_P,MCLK1_N,I2C_SDA,I2C_SCL,I2C_RESET_N,
 input SCK,MOSI,CS,output MISO
);
wire c,clk;
IBUFDS #(.DIFF_TERM("TRUE")) clock_input(.I(MCLK1_P),.IB(MCLK1_N),.O(c));
BUFG clock_global(.I(c),.O(clk));
(* ASYNC_REG="TRUE" *) reg [2:0] meta=7,lines=7;
wire [15:0] command;
(* ASYNC_REG="TRUE" *) reg [15:0] cmd_meta=2,cmd=2;
reg arm_seen=0,active=0;
reg [31:0] tick=0,elapsed=0;
reg [11:0] count=0;
reg [2:0] previous=7;
(* ram_style="block" *) reg [34:0] events[0:2047];
reg [34:0] selected=0;
wire arming=(cmd[15]!=arm_seen);
wire storing=arming || (active && lines!=previous);
wire [10:0] write_address=arming ? 11'b0 : count[10:0];
wire [34:0] write_data={lines,arming ? 32'b0 : elapsed+32'd1};
// Single uniform write port is required for block-RAM inference.
always @(posedge clk) begin
 if(storing) events[write_address] <= write_data;
 selected <= events[cmd[10:0]];
end
always @(posedge clk) begin
 meta <= {I2C_RESET_N,I2C_SCL,I2C_SDA}; lines <= meta;
 cmd_meta <= command; cmd <= cmd_meta;
 tick <= tick+1;
 if(arming) begin
  arm_seen <= cmd[15]; active<=1;elapsed<=0;count<=1;
  previous<=lines;
 end else if(active) begin
  elapsed<=elapsed+1;
  if(lines!=previous) begin
   previous<=lines;count<=count+1;
   if(count==2047) active<=0;
  end
  if(elapsed>=CAPTURE_CYCLES-1) active<=0;
 end
end
wire [15:0] scratch,writes;wire mi;
pm12_mcu_spi spi(.SCKI(SCK),.SPI_CS(CS),.MOSII(MOSI),.MISOI(mi),.scratch_out(scratch),.writes_out(writes));
OBUFT miso_driver(.I(mi),.T(CS),.O(MISO));
wire [511:0] report={304'b0,5'b0,cmd[10:0],29'b0,selected,elapsed,7'b0,active,4'b0,count,5'b0,lines,tick,32'h504d5a03};
eth_jtag scan(.report(report),.command(command));
endmodule
