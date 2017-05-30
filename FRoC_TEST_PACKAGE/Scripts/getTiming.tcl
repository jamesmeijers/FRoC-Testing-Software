#set test_directory /home/jmeijers/Documents/FRoC_TEST_PACKAGE/
set test_directory [lindex $argv 0]
append project $test_directory "FRoC_Project/top.qpf"
append out_file $test_directory "FRoC_timing"
#set my_project [lindex $argv 0]

project_open -force $project -revision top
#project_open $project -revision top
create_timing_netlist

report_timing -from_clock { CLK } -to_clock { CLK } -setup -npaths 2500 -detail path_only -show_routing -panel_name {Setup: CLK} -file $out_file -multi_corner

project_close


