source /home/codex-hil/fit-testing-publish/diagnostics/pm12/i2c/common.tcl
pm12_open
proc report {} {
 scan_ir_hw_jtag 102 -tdi 3fffffffffffffffffffffffc2
 set raw [scan_dr_hw_jtag 524 -tdi 0]
 set r [expr {("0x$raw") + 0}]
 if {($r & 0xffffffff)!=0x504d5a03} {error "Analyzer identity mismatch"}
 return $r
}
proc command {v} {
 scan_ir_hw_jtag 102 -tdi 3fffffffffffffffffffffffc3
 scan_dr_hw_jtag 28 -tdi [format %07x $v]
}
set r [report]
set t0 [clock milliseconds];set c0 [expr {($r>>32)&0xffffffff}]
after 200
set r [report];set t1 [clock milliseconds];set c1 [expr {($r>>32)&0xffffffff}]
set rate [expr {(($c1-$c0)&0xffffffff)*1000.0/($t1-$t0)}]
puts "CLOCK $rate"
if {$rate<10000000 || $rate>200000000} {error "Unexpected sample clock $rate"}
command 32768
# Host emits a bounded console read after this marker, while this Tcl owns JTAG.
set marker [open $::env(PM12_PAYLOAD) w];puts $marker armed;close $marker
after [expr {int(ceil(120000000000.0/$rate))+500}]
set r [report]
if {($r>>88)&1} {error "Capture still active; clock below expected rate?"}
set n [expr {($r>>72)&65535}]
if {$n<1 || $n>2048} {error "Invalid event count $n"}
puts "HEADER [format %llx $r]"
for {set i 0} {$i<$n} {incr i} {
 command [expr {32768|$i}]
 runtest_hw_jtag -tck 10
 set r [report]
 if {(($r>>192)&2047)!=$i} {error "RAM address echo mismatch"}
 puts "EVENT $i [expr {($r>>128)&0xffffffff}] [expr {($r>>160)&7}]"
}
puts "CAPTURE_PASS $n"
pm12_close
exit
