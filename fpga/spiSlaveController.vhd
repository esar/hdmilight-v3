
-- commands:
--   0x02: Write: | addr_msb | addr_lsb | data | ...
--   0x03: Read: | addr_msb | addr_lsb | dummy | data | ...
--   0xa0: Read config from flash: | flash_slot |
--   0xa1: Write config to flash:  | flash_slot |
--   0xa2: Read flash transfer status: | bit0: 0=idle, 1=transfer in progress |


library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;


entity spiSlaveController is
	port (
		clk : in std_logic;

		spi_clk  : in std_logic;
		spi_cs   : in std_logic;
		spi_mosi : in std_logic;
		spi_miso : out std_logic;

		config_data_in   : in  std_logic_vector( 7 downto 0);
		config_data_out  : out std_logic_vector( 7 downto 0);
		config_data_addr : out std_logic_vector(15 downto 0);
		config_data_we   : out std_logic;
		
		flash_spi_clk  : out std_logic;
		flash_spi_cs   : out std_logic;
		flash_spi_mosi : out std_logic;
		flash_spi_miso : in  std_logic
	);
end spiSlaveController;


architecture Behavioral of spiSlaveController is
	type StateType is (
		STATE_NONE, 
		STATE_CMD, 
		STATE_READ_ADDR_MSB, STATE_READ_ADDR_LSB, STATE_READ_DUMMY, STATE_READ_BYTE,
		STATE_WRITE_ADDR_MSB, STATE_WRITE_ADDR_LSB, STATE_WRITE_BYTE,
		STATE_FLASH_SAVE_ADDR,
		STATE_FLASH_RESTORE_ADDR,
		STATE_FLASH_DMA
	);
	
	type SrcType is (
		TX_SRC_NONE, TX_SRC_CONFIG, TX_SRC_FLASH_TRANSFER_STATUS
	);
	
	constant CMD_READ          : std_logic_vector(7 downto 0) := "00000001";
	constant CMD_WRITE         : std_logic_vector(7 downto 0) := "00000010";
	constant CMD_FLASH_RESTORE : std_logic_vector(7 downto 0) := "10100000";
	constant CMD_FLASH_SAVE    : std_logic_vector(7 downto 0) := "10100001";
	constant CMD_FLASH_STATUS  : std_logic_vector(7 downto 0) := "10100010";

	signal state     : StateType := STATE_CMD;
	signal tx_src    : SrcType;
	
	signal rx_data   : std_logic_vector(7 downto 0);
	signal tx_data   : std_logic_vector(7 downto 0);

	signal byte_ready      : std_logic;
	signal byte_ready_sync : std_logic_vector(2 downto 0);

	signal step            : std_logic;

	signal addr         : std_logic_vector(15 downto 0);
	
	signal we            : std_logic;
	
	signal addr_lsb_load : std_logic;
	signal addr_msb_load : std_logic;
	signal addr_inc      : std_logic;
	
	signal flash_dma_start_addr : std_logic_vector(23 downto 0);
	signal flash_dma_start      : std_logic := '0';
	signal flash_dma_busy       : std_logic;
	signal flash_dma_we         : std_logic;
	signal flash_dma_addr       : std_logic_vector(15 downto 0);
	signal flash_dma_data_out   : std_logic_vector(7 downto 0);
begin
	slave : entity work.spiSlave port map(
		spi_clk, spi_cs, spi_mosi, spi_miso, rx_data, tx_data, byte_ready
	);
	
	flashdma : entity work.flashDMAController port map(
		clk, 
		flash_dma_start_addr, 
		(others => '0'),
		"1000000000000000",
		'0',
		flash_dma_start,
		flash_dma_busy,
		flash_dma_we,
		flash_dma_addr,
		config_data_in,
		flash_dma_data_out,
		flash_spi_clk,
		flash_spi_cs,
		flash_spi_mosi,
		flash_spi_miso
	);


	-- It's safe to process the new rx byte, and safe to load a new tx byte, on the falling edge of tx_busy. 
	-- So we'll synchronise that to the main clock, and use it to step the state machine
	process(clk)
	begin
		if rising_edge(clk) then
			byte_ready_sync <= byte_ready_sync(1 downto 0) & byte_ready;
		end if;
	end process;
	step <= '1' when byte_ready_sync(2) = '0' and byte_ready_sync(1) = '1' else '0';
	

	process(clk)
	begin
		if rising_edge(clk) then
			if addr_msb_load = '1' then
				addr(15 downto 8) <= rx_data;
			elsif addr_lsb_load = '1' then
				addr(7 downto 0) <= rx_data;
			elsif addr_inc = '1' then
				addr <= std_logic_vector(unsigned(addr) + 1);
			end if;
		end if;
	end process;

	process(spi_cs, clk)
	begin
		if spi_cs = '1' then
			state <= STATE_CMD;
			tx_src <= TX_SRC_NONE;
		elsif rising_edge(clk) then
			we <= '0';
			addr_msb_load <= '0';
			addr_lsb_load <= '0';
			addr_inc <= '0';
			flash_dma_start <= '0';

			if step = '1' then
				case state is
					when STATE_CMD =>
						case rx_data is
							when CMD_READ =>
								state <= STATE_READ_ADDR_MSB;
							when CMD_WRITE =>
								state <= STATE_WRITE_ADDR_MSB;
							when CMD_FLASH_SAVE =>
								state <= STATE_FLASH_SAVE_ADDR;
							when CMD_FLASH_RESTORE =>
								state <= STATE_FLASH_RESTORE_ADDR;
								--state <= STATE_FLASH_DMA;
							when CMD_FLASH_STATUS =>
								tx_src <= TX_SRC_FLASH_TRANSFER_STATUS;
								state <= STATE_NONE;
							when others =>
								state <= STATE_NONE;
						end case;
						
					when STATE_READ_ADDR_MSB =>
						addr_msb_load <= '1';
						state <= STATE_READ_ADDR_LSB;
					when STATE_READ_ADDR_LSB =>
						addr_lsb_load <= '1';
						tx_src <= TX_SRC_CONFIG;
						state <= STATE_READ_DUMMY;
					when STATE_READ_DUMMY =>
						addr_inc <= '1';
						state <= STATE_READ_BYTE;
					when STATE_READ_BYTE =>
						addr_inc <= '1';
						
					when STATE_WRITE_ADDR_MSB =>
						addr_msb_load <= '1';
						state <= STATE_WRITE_ADDR_LSB;
					when STATE_WRITE_ADDR_LSB =>
						addr_lsb_load <= '1';
						state <= STATE_WRITE_BYTE;
					when STATE_WRITE_BYTE =>
						we <= '1';
						addr_inc <= '1';

					when STATE_FLASH_SAVE_ADDR =>
						addr_lsb_load <= '1';
						state <= STATE_NONE;

					when STATE_FLASH_RESTORE_ADDR =>
						addr_lsb_load <= '1';
						state <= STATE_FLASH_DMA;

					when STATE_FLASH_DMA =>
						-- flash address = 0x60000 + (slot * 32768)
						flash_dma_start_addr <= std_logic_vector(16#60000# + unsigned("0" & addr(7 downto 0) & "000000000000000"));
						flash_dma_start <= '1';
						state <= STATE_NONE;
					
					when STATE_NONE =>
				end case;
			end if;
		end if;
	end process;

	tx_data <= config_data_in               when tx_src = TX_SRC_CONFIG else
	           "0000000" & flash_dma_busy   when tx_src = TX_SRC_FLASH_TRANSFER_STATUS else
	           "00000000";

	config_data_addr <= addr                when flash_dma_busy = '0' else
	                    flash_dma_addr;
							  
	config_data_out <=  rx_data             when flash_dma_busy = '0' else
	                    flash_dma_data_out;
							  
	config_data_we <=   we                  when flash_dma_busy = '0' else
	                    flash_dma_we;
end Behavioral;
