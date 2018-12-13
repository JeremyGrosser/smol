TOPDIR=.

CPUS ?= 6
MAKEFLAGS += -j$(CPUS)

BOARD ?= lorawan
TARGET ?= smol-$(BOARD)

CFLAGS := -O0 -g -Wall -std=c11 -ffreestanding
#CFLAGS := -O2 -Wall -std=c11 -ffreestanding

SRCS :=

all: $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).lst

include make/macros.mk
include boards/$(BOARD)/build.mk
include src/build.mk
include make/toolchain.mk
