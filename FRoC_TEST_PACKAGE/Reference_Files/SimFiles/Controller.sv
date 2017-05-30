`include "constants.sv"

module sim_controller(signal_bus_if bus, input CLK, input reset, input done, input start, input error);
	
	integer out_file; //file to which output is written
	int numCycles;    //current cycle (starts from 0 after reset)
	reg fileOpen;     //high if file is open
	reg reset_phase;  //high if FRoC circuit is in a reset phase
	
	always@(posedge CLK) begin
		if(reset) begin
			fileOpen = 1'b0; //reset only happens once in test, at the beginning
		end
		else if(start) begin
			if(~fileOpen) begin
				//open the file
				$display("Opening file \n");
				out_file = $fopen("output.txt","w");
				//Write out the number of sinks and intermediates at the top of the file so the parser has easy access to the values
				$fwrite(out_file, "Sinks: %d\nIntermediates:%d\n",`NUMBER_OF_SINKS, `NUMBER_OF_INTERMEDIATES);
				//reset cycle count
				numCycles = 0;
				fileOpen = 1'b1;
			end
		end
		else if(done) begin
				if(fileOpen) begin
				$display("Done, closing file \n");
				//close the file
				$fclose(out_file);
				fileOpen = 1'b0;
			end
		end
		else begin
			//Write the current cycle to the file and increment
			$fwrite(out_file, "Cycle: %d\n", numCycles);
			numCycles = numCycles + 1;
			//Write if an error has been reported
			$fwrite(out_file, "Error: %d\n", error);
			//determine if in the reset phase by comparing the state to the minimum reset value 
			//(all states above that value are reset states). Print the reset value found
			reset_phase = bus.state > `MIN_RESET_VALUE;
			$fwrite(out_file, "Reset: %d\n", reset_phase);
			//print out all the sinks' current values
			$fwrite(out_file, "Sinks: \n");
			for (int i = 0; i < `NUMBER_OF_SINKS; i += 1) begin
				$fwrite(out_file, "%d\n", bus.sinks[i]);
			end
			//print out all the lut output current values
			$fwrite(out_file, "Intermediates: \n");
			
			for (int i = 0; i < `NUMBER_OF_INTERMEDIATES; i += 1) begin
				$fwrite(out_file, "%d\n", bus.intermediates[i]);
			end
		end
	
	
	end
	
endmodule


