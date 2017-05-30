create_clock -name {CLK} -period "200 MHz" [get_ports {CLK}]
set_max_delay -from [get_cells {PATH*NODE*}] -to [get_cells {control0|sinks_buff*}] 2.5 
set_max_delay -from [get_cells {control0|sinks_buff*}] -to [get_cells {control0|sinks_buff_buff*}] 2.5 
set_multicycle_path -from [get_cells {control0|*}] -through [get_nets {PATH*NODE*}] -setup -end 3
set_multicycle_path -from [get_cells {control0|*}] -through [get_nets {PATH*NODE*}] -hold -end 2

#set_multicycle_path -from [get_cells {control0|state*}] -to [get_cells {control0|controlSignals*}] -setup -end 3
#set_multicycle_path -from [get_cells {control0|state*}] -to [get_cells {control0|controlSignals*}] -hold -end 2