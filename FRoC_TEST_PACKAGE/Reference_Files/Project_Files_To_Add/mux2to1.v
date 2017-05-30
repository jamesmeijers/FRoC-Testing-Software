module mux2to1 (in0, in1, s, out);

input in0,in1, s;
output out;

assign out = (~s&in0)|(s&in1);

endmodule