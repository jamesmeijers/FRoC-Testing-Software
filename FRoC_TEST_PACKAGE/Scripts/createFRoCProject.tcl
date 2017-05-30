


#set device
set_global_assignment -name DEVICE EP4CE115F29C7


#add Verilog files
set_global_assignment -name VERILOG_FILE controller.v
set_global_assignment -name VERILOG_FILE shift_reg.v
set_global_assignment -name VERILOG_FILE counter_testing.v
set_global_assignment -name VERILOG_FILE top.v
set_global_assignment -name VERILOG_FILE t_ff.v
set_global_assignment -name VERILOG_FILE mux2to1.v

#other settings
set_global_assignment -name PROJECT_OUTPUT_DIRECTORY output_files
set_global_assignment -name MIN_CORE_JUNCTION_TEMP 0
set_global_assignment -name MAX_CORE_JUNCTION_TEMP 85
set_global_assignment -name ERROR_CHECK_FREQUENCY_DIVISOR 1
set_global_assignment -name NOMINAL_CORE_SUPPLY_VOLTAGE 1.2V



set_global_assignment -name EDA_SIMULATION_TOOL "ModelSim-Altera (Verilog)"
set_global_assignment -name EDA_OUTPUT_DATA_FORMAT "VERILOG HDL" -section_id eda_simulation


