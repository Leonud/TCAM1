namespace import tcl::mathfunc::*
namespace import tcl::mathop::*

set n_requests 0

for {set i 0} {$i < [llength $argv]} {incr i} {
 switch [lindex $argv $i] {
  -w {incr i; set width [lindex $argv $i]}
  -s {incr i; set tcam_size [lindex $argv $i]}
  -seed {incr i; set seed [lindex $argv $i]}
  -genreq {incr i; set n_requests [lindex $argv $i]}
 }
}

if {![info exists width] || ![info exists tcam_size]} {
 puts stderr {usage: tcam_gen_data.tcl [-genreq <n_requests>] [-seed <rnd_seed>] -w <width> -s <tcam_size>}
 exit 1
}

if {[info exists seed]} {
 srand $seed
}

proc gen_hexstr {w} {
 global width
 set s ""
 for {set j 0} {$j < $width} {incr j 8} {
  if {$width - $j < 8} {
   set mask [expr {(0xff00 >> ($width-$j)) & 0xff}]
  } else {
   set mask 0xff
  }
  set byte [expr {int(rand()*255) & $mask}]
  append s [format %02X $byte]
 }
 return $s
}

set hexstr_size [expr {(($width-1)/8+1)*2}]

if {!$n_requests} {
 for {set i 0} {$i < $tcam_size} {incr i} {
  set match [gen_hexstr $width]
  set dont_care [gen_hexstr $width]
  set match [expr 0x$match & ~0x$dont_care]
  set match [format %0${hexstr_size}X $match]
  puts $match
  puts $dont_care
 }
 exit 0
}

# replicate tcam
for {set i 0} {$i < $tcam_size} {incr i} {
 set tcam($i,match) [gen_hexstr $width]
 set tcam($i,dont_care) [gen_hexstr $width]
}

for {set i 0} {$i < $n_requests} {incr i} {
 set shoud_match [int [* [rand] $tcam_size 1.5]]
 set s [gen_hexstr $width]
 if {$shoud_match >= $tcam_size} {
  # just random data
  puts $s
 } else {
  #puts "should match $shoud_match"
  set s [| 0x$tcam($shoud_match,match) [& 0x$s 0x$tcam($shoud_match,dont_care)]]
  puts [format %0${hexstr_size}X $s]
 }
}

