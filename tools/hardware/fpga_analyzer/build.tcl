set here [file dirname [file normalize [info script]]]
cd $here
create_project -in_memory -part xc7k160tffg676-3
read_verilog $here/pm12_analyzer.v
read_verilog $here/eth_jtag.v
read_vhdl $here/pm12_mcu_spi.vhd
read_xdc $here/pins.xdc
synth_design -top pm12_analyzer -part xc7k160tffg676-3
opt_design
place_design
route_design
file mkdir build
report_drc -file build/drc.txt
report_timing_summary -file build/timing.txt
foreach corner {max min} {
 if {[llength [get_timing_paths -quiet -delay_type $corner -slack_lesser_than 0 -max_paths 1]]} {error "Timing failed $corner"}
}
write_checkpoint -force build/pm12_analyzer.dcp
write_bitstream -force build/pm12_analyzer.bit
exit
