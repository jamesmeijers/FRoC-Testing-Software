#compile all files
vlog -reportprogress 300 -work work constants.sv
vlog -reportprogress 300 -work work Controller.sv
vlog -reportprogress 300 -work work controller.v
vlog -reportprogress 300 -work work counter_testing.v
vlog -reportprogress 300 -work work DUT.sv
vlog -reportprogress 300 -work work top.v
vlog -reportprogress 300 -work work Interface.sv
vlog -reportprogress 300 -work work shift_reg.v
vlog -reportprogress 300 -work work test_top.sv

#set up the simulation
vsim -t 1ps -L altera_ver -L cycloneive_ver test_top -c

#run until $stop
run -a

#quit modelsim
exit

