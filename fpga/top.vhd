----------------------------------------------------------------------------------
--
-- Copyright (C) 2019 Stephen Robinson
--
-- This file is part of HDMI-Light
--
-- HDMI-Light is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 2 of the License, or
-- (at your option) any later version.
--
-- HDMI-Light is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
-- 
-- You should have received a copy of the GNU General Public License
-- along with this code (see the file names COPING).  
-- If not, see <http://www.gnu.org/licenses/>.
--
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity HdmilightTop is
	port 
	(
		-- 32MHz oscillator input
		CLK : in std_logic;

		-- ADV7611 Video 
		ADV_LLC : in std_logic;
		ADV_P :   in std_logic_vector(23 downto 0);
		ADV_HS :  in std_logic;
		ADV_VS :  in std_logic;
		ADV_DE :  in std_logic;

		-- ADV7611 I2S Audio
		ADV_AP    : in std_logic;
		ADV_SCLK  : in std_logic;
		ADV_LRCLK : in std_logic;
		ADV_MCLK  : in std_logic;

		-- AV7611 I2C Configuration
		ADV_SCL : inout std_logic;
		ADV_SDA : inout std_logic;

		ADV_INT1 : in std_logic;

		ADV_RST : inout std_logic;  -- must be inout for tristate 'Z' to work

		MCU_I2C_SCL : inout std_logic;
		MCU_I2C_SDA : inout std_logic;

		MCU_SPI_CLK  : in std_logic;
		MCU_SPI_MISO : inout std_logic;
		MCU_SPI_MOSI : in std_logic;
		MCU_SPI_CS   : in std_logic;

		LED_OUT : out std_logic_vector(7 downto 0);
		LED_CLK : out std_logic_vector(7 downto 0);

		FLASH_CK : out std_logic;
		FLASH_CS : out std_logic;
		FLASH_SI : out std_logic;
		FLASH_SO : in  std_logic;

		MCU_PD2 : inout std_logic;
		MCU_PB5 : inout std_logic;
		MCU_PB8 : inout std_logic;

		VIVEPOS_SENSOR : in std_logic_vector(3 downto 0);

		PAPIX_POS : in std_logic_vector(1 downto 0);
		PAPIX_RGB : out std_logic_vector(1 downto 0);

		IN5V : in std_logic_vector(1 downto 0);
		OUT5V : out std_logic_vector(1 downto 0);

		GPIO : inout std_logic_vector(7 downto 0);
		
		ROUTE_UNDER : inout std_logic_vector(5 downto 0)
	);
end HdmilightTop;

architecture Behavioral of HdmilightTop is

-----------------------------------------------
-- Component Definitions
-----------------------------------------------
component DCM32to16
	port(
		CLKIN_IN        : in std_logic;          
		CLKFX_OUT       : out std_logic;
		CLKIN_IBUFG_OUT : out std_logic;
		CLK0_OUT        : out std_logic
	);
end component;


-----------------------------------------------
-- Signals
-----------------------------------------------

signal RST: std_logic:= '1';
signal RST_COUNT: std_logic_vector(1 downto 0):="00";

signal CLK16: std_logic;

signal vidclk : std_logic;
signal viddata_r : std_logic_vector(7 downto 0);
signal viddata_g : std_logic_vector(7 downto 0);
signal viddata_b : std_logic_vector(7 downto 0);
signal hblank : std_logic_vector(1 downto 0);
signal vblank : std_logic_vector(1 downto 0);
signal dataenable : std_logic_vector(1 downto 0);

signal AMBILIGHT_CFG_WE   : std_logic;
signal AMBILIGHT_CFG_ADDR : std_logic_vector(15 downto 0);
signal AMBILIGHT_CFG_DIN  : std_logic_vector(7 downto 0);
signal AMBILIGHT_CFG_DOUT : std_logic_vector(7 downto 0);

signal driverOutput : std_logic_vector(7 downto 0);

signal INT_CLEAR  : std_logic_vector(7 downto 0);
signal INT_FORMAT : std_logic;

signal interruptOut : std_logic;

begin

-----------------------------------------------
-- Instantiation
-----------------------------------------------

ambilight : entity work.ambilight port map(vidclk, viddata_r, viddata_g, viddata_b, hblank(1), vblank(1), dataenable(1),
                                           CLK16,
                                           AMBILIGHT_CFG_WE,
                                           AMBILIGHT_CFG_ADDR,
                                           AMBILIGHT_CFG_DIN,
                                           AMBILIGHT_CFG_DOUT,
                                           driverOutput,
                                           ADV_INT1,
                                           interruptOut);

spi : entity work.spiSlaveController port map(
	CLK16,
	MCU_SPI_CLK, MCU_SPI_CS, MCU_SPI_MOSI, MCU_SPI_MISO,
	AMBILIGHT_CFG_DOUT, AMBILIGHT_CFG_DIN, AMBILIGHT_CFG_ADDR, AMBILIGHT_CFG_WE,
	FLASH_CK, FLASH_CS, FLASH_SI, FLASH_SO
);

												  
Inst_DCM32to16: DCM32to16 PORT MAP(
	CLKIN_IN => CLK,
	CLKFX_OUT => CLK16,
	CLKIN_IBUFG_OUT => open,
	CLK0_OUT => open
);

-----------------------------------------------
-- Implementation
-----------------------------------------------

-- Reset Generator
process (CLK16)
begin
	if (rising_edge(CLK16)) then
		if (RST_COUNT = X"3") then
			RST <= '0';
		else
			RST_COUNT <= RST_COUNT + 1;
		end if;
	end if;
end process;


process(ADV_LLC)
begin
	if(rising_edge(ADV_LLC)) then
		viddata_r <= ADV_P(23 downto 16);
		viddata_g <= ADV_P(15 downto  8);
		viddata_b <= ADV_P( 7 downto  0);
	end if;
end process;

process(ADV_LLC)
begin
	if(rising_edge(ADV_LLC)) then
		hblank <= hblank(0) & (not ADV_HS);
		vblank <= vblank(0) & (not ADV_VS);
		dataenable <= dataenable(0) & ADV_DE;
	end if;
end process;

-----------------------------------------------
-- Combinatorial
----------------------------------------------- 

--MASTER_WE   <= '0' & DMA_WE when DMA_IN_PROGRESS = '1' else MCU_SRAM_WR;
--MASTER_ADDR <= DMA_ADDR when DMA_IN_PROGRESS = '1' else MCU_ADDR;
--MASTER_DIN  <= "00000000" & MCU_IO_DATA_READ when MCU_IO_RD = '1' else
--               SRAM_DOUT when MASTER_ADDR(15) = '0' else
--               "00000000" & AMBILIGHT_CFG_DOUT;
--MASTER_DOUT <= "00000000" & DMA_DOUT when DMA_IN_PROGRESS = '1' else MCU_DOUT;

--AMBILIGHT_CFG_DIN  <= MASTER_DOUT(7 downto 0);
--AMBILIGHT_CFG_ADDR <= '0' & MASTER_ADDR(14 downto 0);
--AMBILIGHT_CFG_WE   <= MASTER_WE(0) and MASTER_ADDR(15);

ADV_RST <= 'Z';
LED_OUT <= driverOutput;
LED_OUT <= (others => 'Z');
LED_CLK <= (others => 'Z');


OUT5V <= (others => '0');
PAPIX_RGB <= (others => '0');

ADV_SCL <= 'Z';
ADV_SDA <= 'Z';
MCU_I2C_SCL <= 'Z';
MCU_I2C_SDA <= 'Z';

MCU_PD2 <= interruptOut;
MCU_PB5 <= 'Z';
MCU_PB8 <= 'Z';

GPIO(0) <= MCU_SPI_CLK;
GPIO(1) <= MCU_SPI_CS;
GPIO(2) <= MCU_SPI_MOSI;
GPIO(3) <= MCU_SPI_MISO;
GPIO(7 downto 4) <= AMBILIGHT_CFG_DOUT(3 downto 0);

ROUTE_UNDER <= (others => 'Z');

vidclk <= ADV_LLC;


end Behavioral;
