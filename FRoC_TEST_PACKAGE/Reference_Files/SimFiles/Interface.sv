`include "constants.sv"

//
//Interface goes between the DUT and the controller
//It is used to transport all the internal signals
//from FRoC's circuit that are needed for testing
//


interface signal_bus_if;
	//The current state of FRoC	
	logic [0:(`STATE_LENGTH - 1)] state;
	//All the LUT outputs of FRoC's circuits	
	logic intermediates[`NUMBER_OF_INTERMEDIATES];
	//All the sink outputs of FRoC's circuits
	logic sinks[`NUMBER_OF_SINKS];

endinterface: signal_bus_if
