module counter_testing 
# (parameter L = 6)
(CLK,clr,timerReached);
input CLK;
input clr;
output timerReached;

reg [L-1:0]count;

always @(posedge CLK)
begin
	if(clr)
		count <= 0;
	else
		count <= count+1'b1;
end

assign timerReached = & count;
endmodule