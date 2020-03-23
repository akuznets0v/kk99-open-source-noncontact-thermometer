PROJECTNAME=pyrometer
MCU = attiny2313
PROGAMMER = usbasp

CC = avr-gcc
OBJCOPY = avr-objcopy
SIZE = avr-size

OBJDIR = build
SRCS = main.c SSNIndicator.c SMBusMaster.c MLXIRSensor.c

CFLAGS = -DF_CPU=1000000L -mmcu=$(MCU)
CFLAGS += -Os -Wall -Wextra -std=c99
CFLAGS += -Iinclude

VERSION?=debug
ifeq ($(VERSION), debug)
    CFLAGS += -DDEBUG
endif

vpath %.c src

ROOT=$(shell pwd)
OBJS = $(SRCS:.c=.o)

.PHONY: clean project

all: project

project: $(OBJDIR) $(OBJDIR)/$(PROJECTNAME).elf

$(OBJDIR)/$(PROJECTNAME).elf: $(SRCS)
	@echo "\033[32mCompile:"
	$(CC) $(CFLAGS) $^ -o $@
	@echo "\033[0m"
	@echo "\033[32mCreate objects:"
	$(OBJCOPY) -O ihex $(OBJDIR)/$(PROJECTNAME).elf $(OBJDIR)/$(PROJECTNAME).hex
	@echo "\033[0m"
	@echo "\033[34mObjects size:"
	$(SIZE) --mcu=$(MCU) -C $(OBJDIR)/$(PROJECTNAME).elf
	@echo "\033[0m"

$(OBJDIR):
	mkdir -p $@

clean:
	@echo "\033[31mRemove:"
	rm -rf $(OBJDIR)
	rm -f $(OBJDIR)/$(PROJECTNAME).elf
	rm -f $(OBJDIR)/$(PROJECTNAME).hex
	rm -f tags
	@echo "\033[0m"

program:
	sudo avrdude -p $(MCU) -c $(PROGAMMER) -U flash:w:$(ROOT)/$(OBJDIR)/$(PROJECTNAME).elf

ctags:
	ctags --languages=C -R .
