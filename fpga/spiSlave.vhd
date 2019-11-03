library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;


entity spiSlave is
	port(
		spi_clk  : in  std_logic;
		spi_cs   : in  std_logic;
		spi_mosi : in  std_logic;
		spi_miso : out std_logic;

		rx_data  : out std_logic_vector(7 downto 0);
		tx_data  : in  std_logic_vector(7 downto 0);

		byte_ready : out std_logic
	);
end spiSlave;

architecture Behavioral of spiSlave is
	signal shiftout : std_logic_vector(7 downto 0);
	signal shiftin  : std_logic_vector(7 downto 0);
	signal bitcount : std_logic_vector(2 downto 0);
	signal first    : std_logic;
begin
	-- Count the bits in each byte
	process(spi_cs, spi_clk)
	begin
		if spi_cs = '1' then
			bitcount <= (others => '1');
		elsif rising_edge(spi_clk) then
			bitcount <= std_logic_vector(unsigned(bitcount) + 1);
		end if;
	end process;


	-- Shift in MOSI on rising edge of SPI clock
	process(spi_clk)
	begin
		if(rising_edge(spi_clk)) then
			shiftin <= shiftin(6 downto 0) & spi_mosi;
		end if;
	end process;


	-- Latch received data when we have a whole byte
	process(spi_clk)
	begin
		if rising_edge(spi_clk) then
			if bitcount = "111" then
				rx_data <= shiftin;
			end if;
		end if;
	end process;


	-- Shift out transmit data on falling edge, reloading with new data after every byte
	process(spi_clk)
	begin
		if falling_edge(spi_clk) then
			if bitcount = "111" then
				shiftout <= tx_data;
			else
				shiftout <= shiftout(6 downto 0) & '0';
			end if;
		end if;		
	end process;
	spi_miso <= shiftout(7);


	-- generate a flag to indicate the first byte of a transaction
	process(spi_cs, spi_clk)
	begin
		if spi_cs = '1' then
			first <= '1';
		elsif rising_edge(spi_clk) then
			if spi_cs = '0' and bitcount = "000" then
				first <= '0';
			end if;
		end if;
	end process;


	-- Signal that a byte is complete. 
	-- The user of this component should watch for the rising edge of this signal
	-- and read the rx_data and present new tx_data. The user must do this within
	-- 7 SPI clock cycles.
	-- The signal is 4 spi clocks long (1/2 a byte) to give the best chance of
	-- the other clock domain seeing the change in the signal.
	process(spi_cs, spi_clk)
	begin
		if spi_cs = '1' then
			byte_ready <= '0';
		elsif rising_edge(spi_clk) then
			if first = '0' and bitcount = "111" then
				byte_ready <= '1';
			elsif bitcount = "011" then
				byte_ready <= '0';
			end if;
		end if;
	end process;
end Behavioral;
