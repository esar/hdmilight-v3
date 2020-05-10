.PHONY: tools tools_clean firmware firmware_clean config config_clean fpga_program fpga_flash fpga_flash_design fpga_flash_config

all: firmware config firmware_flash fpga_flash

clean: firmware_clean config_clean
	rm -f hdmilight.bit


firmware:
	make -C firmware size

firmware_flash:
	make -C firmware flash

firmware_clean:
	make -C firmware clean

config:
	make -C config

config_clean:
	make -C config clean

fpga_program:
	openocd -f hdmilight-v3-openocd.cfg -c "init; targets xc6s.proxy; xc6s_program xc6s.tap; pld load 0 fpga/HdmilightTop.bit; exit"

fpga_flash: fpga_flash_design fpga_flash_config

fpga_flash_design:
	openocd -f hdmilight-v3-openocd.cfg -c "init; targets xc6s.proxy; jtagspi_init 0 bscan_spi_xc6slx9.bit; jtagspi_program fpga/HdmilightTop.bin 0; xc6s_program xc6s.tap; shutdown"

fpga_flash_config:
	openocd -f hdmilight-v3-openocd.cfg -c "init; targets xc6s.proxy; jtagspi_init 0 bscan_spi_xc6slx9.bit; jtagspi_program config/merged.bin 0x60000; xc6s_program xc6s.tap; shutdown"
