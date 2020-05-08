OBJS  = main.o         \
        clock.o        \
        uart.o         \
        printf.o       \
        i2c.o          \
        spi.o          \
        fixed_point.o  \
        cmd_area.o     \
        cmd_colour.o   \
        cmd_delay.o    \
        cmd_format.o   \
        cmd_gamma.o    \
        cmd_i2c.o      \
        cmd_keys.o     \
        cmd_output.o   \
        cmd_mem.o      \
        cmd_addr.o     \
        cmd_result.o   \
        cmd_status.o

OBJS += papix/pos_sensors.o     \
        papix/position.o

OBJS += system_stm32f3xx.o      \
        startup_stm32f303xe.o

CMSIS     = ../../STM32Cube_FW_F3_V1.10.0/Drivers/CMSIS/

CFLAGS += -c -fno-common -Os -g -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -std=gnu99
CFLAGS += -Wall -ffunction-sections -fdata-sections -fno-builtin
CFLAGS += -Wno-unused-function -ffreestanding
CFLAGS += -DSTM32F303xE
LFLAGS  = -Tstm32f303ret.ld -Wl,--gc-sections -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
#LFLAGS += -L/usr/lib/arm-none-eabi/newlib/thumb

INC     = -I$(CMSIS)/Include/
INC    += -I$(CMSIS)/Device/ST/STM32F3xx/Include/ -I./
CFLAGS += $(INC)


CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
AR      = arm-none-eabi-ar
AS      = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size


all: size flash

main.elf: $(OBJS)
	$(LD) $(LFLAGS) -o main.elf $(OBJS)

%.bin: %.elf
	$(OBJCOPY) --strip-unneeded -O binary $< $@


flash: main.bin
	sudo openocd -f ../hdmilight-v3-openocd.cfg -c "program $^ reset exit 0x08000000"

size: main.elf
	$(SIZE) $< 

clean:
	-rm -f $(OBJS) main.lst main.elf main.hex main.map main.bin main.list


.PHONY: all flash size clean distclean
