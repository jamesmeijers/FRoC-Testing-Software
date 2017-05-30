module shift_reg 
# (parameter L = 10)
(CLK,in,out);

input CLK, in;
output out;

reg inter [L-1:0];

always @ (posedge CLK) begin
		inter[0] <= in;
end

genvar i;

generate 
	for (i=1;i<L;i=i+1) begin :shiftreg
		always @ (posedge CLK) begin
				inter[i] <= inter[i-1];
		end
	end
endgenerate
	
	
		
assign out = inter [L-1];
endmodule
	
