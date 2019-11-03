--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   20:14:06 11/02/2019
-- Design Name:   
-- Module Name:   /home/stephen/projects/hardware/hdmilight-v3/fpga/test_spi.vhd
-- Project Name:  hdmilight
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: spiSlaveController
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY test_spi IS
END test_spi;
 
ARCHITECTURE behavior OF test_spi IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT spiSlaveController
    PORT(
         clk : IN  std_logic;
         spi_clk : IN  std_logic;
         spi_cs : IN  std_logic;
         spi_mosi : IN  std_logic;
         spi_miso : OUT  std_logic;
         config_data_in : IN  std_logic_vector(7 downto 0);
         config_data_out : OUT  std_logic_vector(7 downto 0);
         config_data_addr : OUT  std_logic_vector(15 downto 0);
         config_data_we : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal clk : std_logic := '0';
   signal spi_clk : std_logic := '0';
   signal spi_cs : std_logic := '0';
   signal spi_mosi : std_logic := '0';
   signal config_data_in : std_logic_vector(7 downto 0) := (others => '0');

 	--Outputs
   signal spi_miso : std_logic;
   signal config_data_out : std_logic_vector(7 downto 0);
   signal config_data_addr : std_logic_vector(15 downto 0);
   signal config_data_we : std_logic;

   -- Clock period definitions
   constant clk_period : time := 62.5 ns;    -- 16MHz
   constant spi_clk_period : time := 66.6 ns;  -- 15MHz
 
	signal input_byte : std_logic_vector(7 downto 0);
	
	shared variable i : integer := 0;
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: spiSlaveController PORT MAP (
          clk => clk,
          spi_clk => spi_clk,
          spi_cs => spi_cs,
          spi_mosi => spi_mosi,
          spi_miso => spi_miso,
          config_data_in => config_data_in,
          config_data_out => config_data_out,
          config_data_addr => config_data_addr,
          config_data_we => config_data_we
        );

   -- Clock process definitions
   clk_process :process
   begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
   end process;
 
   spi_clk_process :process
   begin
		spi_clk <= '0';
		wait for spi_clk_period/2;
		spi_clk <= '1';
		wait for spi_clk_period/2;
   end process;
 
	process(clk)
	begin
		if rising_edge(clk) then
			config_data_in <= "1010" & config_data_addr(3 downto 0);
		end if;
	end process;

   -- Stimulus process
   stim_proc: process
   begin
		spi_cs <= '1';
	
      -- hold reset state for 100 ns.
      wait for 100 ns;	

      wait for clk_period*10;

      -- insert stimulus here 
		
		input_byte <= "00000001";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;
		
		input_byte <= "00000000";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;
		
		input_byte <= "00000011";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;
		
		input_byte <= "00000000";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;
		
		input_byte <= "00000000";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;
		
		input_byte <= "00000000";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;
		
		input_byte <= "00000000";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;
		
		
		spi_cs <= '1';
		wait until spi_clk = '0';
		wait until spi_clk = '1';
		wait until spi_clk = '0';
		wait until spi_clk = '1';

		
		input_byte <= "00000010";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;
		
		input_byte <= "00000000";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;		
		
		input_byte <= "00000011";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;		
		
		input_byte <= "10100011";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;
		
		input_byte <= "10100100";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;
		
		input_byte <= "10100101";
		for i in 0 to 7 loop
			wait until spi_clk = '0';
			spi_cs <= '0';
			spi_mosi <= input_byte(7);
			input_byte <= input_byte(6 downto 0) & '0';
			wait until spi_clk = '1';
		end loop;		
		
		
		
		spi_cs <= '1';
		wait until spi_clk = '0';
		wait until spi_clk = '1';
		wait until spi_clk = '0';
		wait until spi_clk = '1';
		
      wait;
   end process;

END;
