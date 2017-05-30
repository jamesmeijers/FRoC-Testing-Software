module t_ff(reset,out,clk);
input  reset, clk;
output out;

reg temp;

always @ (posedge clk) begin
	if (reset)
		temp <= 1'b0;
	else
		temp <= ~temp;
end

assign out = temp;

endmodule