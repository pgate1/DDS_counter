
`timescale 1ns/1ns

module dds_sim;

	reg reset, clk;
	wire enable;
/*
	DDS_5to3 ddsu (
		.p_reset(reset), .m_clock(clk),
		.enable(enable)
	);
*/

//	dds_verilog ddsu (
	dds_vhdl ddsu (
		.rst(reset), .clk(clk),
		.enable(enable)
	);


	always begin
	    clk = 0;
	#10 clk = 1;
	#10;
	end

	initial begin
	    reset = 0;
	#5  reset = 1;
	#10 reset = 0;
	#10;

//	#3580000
	#4610000

	//	$finish;
		;
	end

endmodule
