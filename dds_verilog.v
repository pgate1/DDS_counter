module dds_verilog (
	rst, clk, enable
);
	input rst, clk;
	output enable;

	parameter COUNT_WIDTH = 4;
	parameter ADD_NUM = 4'd3;
	parameter MAX_NUM = 4'd5;

	wire [COUNT_WIDTH-1:0] add;
	wire [COUNT_WIDTH-1:0] max;
	reg  [COUNT_WIDTH-1:0] count;
	wire [COUNT_WIDTH-1:0] sa;

	assign add = ADD_NUM;
	assign max = MAX_NUM;

	assign sa = count - max;

	always @(posedge clk) begin
		if(rst)
			count <= 0;
		else if(sa[COUNT_WIDTH-1]) // count < max
			count <= count + add;
		else
			count <= sa + add;
	end

	assign enable = ~sa[COUNT_WIDTH-1];

endmodule
