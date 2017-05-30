`timescale 1 ns / 100 ps

`include "constants.sv"

//
//This test_top module instantiates the DUT, the bus, and the controller
//It then runs the simulation for the number of cycles defined in constants.sv
//


module test_top;
	localparam CLK_PERIOD = 5;
	reg CLK;
	reg reset;
	reg start_test;
	reg error;
	reg f;
	reg done;
	integer cycle;
	 
	//Instantiate the neccessary modules
	signal_bus_if bus();
	DUT DUT1(.CLK(CLK), .reset(reset), .start_test(start_test), .error(error), .fuck(f), .bus(bus));
	sim_controller control(.bus(bus), .CLK(CLK), .reset(reset), .done(done), .start(start_test), .error(error));

	//Clock control
	always begin
		CLK = 1'b1;
		#(CLK_PERIOD/2);
		CLK = 1'b0;
		#(CLK_PERIOD/2);
	end

	initial begin
		//start the simulation
		$display("Starting Simulation\n");
		done = 0;
		cycle = 0;
		//reset FRoC's circuit
		reset = 1'b1;
		$display("About to wait for reset \n");
		#20 reset = 1'b0;
		//signal to start the test
		start_test = 1'b1;
		$display("Reset done, about to start test \n");
		#20 start_test = 1'b0;
		$display("Test Started! \n");
	end

	always @(posedge CLK) 
		if(reset) 
			cycle <= 0;
		//if the final cycle is reached, signal to controller to close the file and end the test
		else if(cycle == `MAX_CYCLE) begin
			$display("Ending Simulation \n");
			done = 1;
			#20 done = 0;
			$stop;
			end
		else cycle = cycle + 1;


endmodule
