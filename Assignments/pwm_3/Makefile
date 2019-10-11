# Tiva Makefile
#######################################
# user configuration:
#######################################
# TARGET: name of the output file
TARGET = project
# MCU: part number to build for
MCU = TM4C123GH6PM
# SOURCES: list of input source sources
SOURCES = main.c startup_gcc.c
# OUTDIR: directory to use for output
OUTDIR = build

# LD_SCRIPT: linker script
LD_SCRIPT = $(MCU).lds

# define flags
CFLAGS = -g -mthumb -march=armv7e-m+fp -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -O0 -ffunction-sections -fdata-sections -MD -std=c99 -Wall
CFLAGS += -pedantic -DPART_$(MCU) -c -I../..  
CFLAGS += -DTARGET_IS_TM4C123_RB1 -D DEBUG
LDFLAGS = -mthumb -march=armv7e-m+fp -mfloat-abi=hard -Wl,-Map=build/$(TARGET).map  -Wl,-T $(LD_SCRIPT) --entry ResetISR -Wl,--gc-sections -Wl,--start-group $(LIBS) -Wl,--end-group   

#######################################
# end of user configuration
#######################################
#
#######################################
# binaries
#######################################
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
RM      = rm -f
MKDIR	= mkdir -p
#######################################

# list of object files, placed in the build directory regardless of source path
OBJECTS = $(addprefix $(OUTDIR)/,$(notdir $(SOURCES:.c=.o)))

# default: build bin
all: $(OUTDIR)/$(TARGET).bin

$(OUTDIR)/%.o: src/%.c | $(OUTDIR)
	$(CC) -o $@ $^ $(CFLAGS)

$(OUTDIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS) 

$(OUTDIR)/$(TARGET).bin: $(OUTDIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# create the output directory
$(OUTDIR):
	$(MKDIR) $(OUTDIR)

clean:
	-$(RM) $(OUTDIR)/*

.PHONY: all clean
