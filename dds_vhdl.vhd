library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity dds_vhdl is
	port(
		rst, clk : in std_logic;
		enable : out std_logic
	);
end dds_vhdl;

architecture RTL of dds_vhdl is

constant COUNT_WIDTH : integer := 4;
constant ADD_NUM : std_logic_vector(COUNT_WIDTH-1 downto 0)
	:= std_logic_vector(to_unsigned(3, COUNT_WIDTH));
constant MAX_NUM : std_logic_vector(COUNT_WIDTH-1 downto 0)
	:= std_logic_vector(to_unsigned(5, COUNT_WIDTH));

signal add   : std_logic_vector(COUNT_WIDTH-1 downto 0);
signal max   : std_logic_vector(COUNT_WIDTH-1 downto 0);
signal count : std_logic_vector(COUNT_WIDTH-1 downto 0);
signal sa    : std_logic_vector(COUNT_WIDTH-1 downto 0);

begin

	add <= ADD_NUM;
	max <= MAX_NUM;

	sa <= count - max;

	process(clk) begin
		if rising_edge(clk) then
			if rst='1' then
				count <= (others => '0');
			elsif sa(COUNT_WIDTH-1)='1' then -- count < max
				count <= count + add;
			else
				count <= count + add - max;
			end if;
		end if;
	end process;

	enable <= not sa(COUNT_WIDTH-1);

end RTL;
