FORMAT         := ihex
MCU            := atmega32u4
F_CPU          := 16000000
LED_BRIGHTNESS := 0.5
DEBOUNCE_TIME  := 20

TARGET := firmware
SRC    := $(wildcard *.c)
OBJ     = $(SRC:%.c=%.o)

CFLAGS := -mmcu=$(MCU)
CFLAGS += -DF_CPU=$(F_CPU)
CFLAGS += -DMAKEFILE_LED_BRIGHTNESS=$(LED_BRIGHTNESS)
CFLAGS += -DMAKEFILE_DEBOUNCE_TIME=$(DEBOUNCE_TIME)
CFLAGS += -std=gnu99 -Os
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -fpack-struct -fshort-enums
CFLAGS += -ffunction-sections -fdata-sections

LDFLAGS := -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += -Wl,--relax
LDFLAGS += -Wl,--gc-sections

GENDEPFLAGS := -MMD -MP -MF $@.dep

CC      := avr-gcc
OBJCOPY := avr-objcopy
SIZE    := avr-size

.PHONY: all clean load

all: $(TARGET).hex $(TARGET).eep
	@echo
	@$(SIZE) --target=$(FORMAT) $(TARGET).hex
	@echo

load: all
	teensy_loader_cli -w --mcu TEENSY2 $(TARGET).hex

clean:
	git clean -dX -f

%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom -R .fuse -R .lock -R .signature $< $@

%.eep: %.elf
	-$(OBJCOPY) -O $(FORMAT) -j .eeprom \
		--set-section-flags=.eeprom="alloc,load" \
		--change-section-lma .eeprom=0 \
		--no-change-warnings $< $@ || exit 0

%.elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ --output $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(GENDEPFLAGS) $< -o $@

-include $(OBJ:%=%.dep)
