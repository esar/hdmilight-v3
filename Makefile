DATA2MEM = /opt/Xilinx/14.4/ISE_DS/ISE/bin/lin64/data2mem


fpga_program:
	openocd -f hdmilight-v3-openocd.cfg -c "init; xc6s_program xc6s.tap; pld load 0 HdmilightTop.bit; exit"

fpga_flash:
	openocd -f hdmilight-v3-openocd.cfg -c "init; jtagspi_init 0 bcan_spi_xc6slx9.bit; jtagspi_program HdmilightTop.bit 0; xc6s_program xc6s.tap; shutdown"

v2_all:
	make -C tools
	make -C firmware
	make -C config
	avr-size --mcu=atmega16 -C firmware/image.elf
	$(DATA2MEM) -bm fpga/progmem_bd.bmm -bt fpga/HdmilightTop.bit -bd firmware/image.mem -o b hdmilight-preconfig.bit
	tools/bitmerge.py --pad=0x10000 hdmilight-preconfig.bit config/merged.bin hdmilight.bit

v2_clean:
	make -C tools clean
	make -C firmware clean
	make -C config clean
	rm -f hdmilight-preconfig.bit
	rm -f hdmilight.bit
