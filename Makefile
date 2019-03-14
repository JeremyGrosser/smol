TOPDIR=.

CPUS ?= 6
MAKEFLAGS += -j$(CPUS)

BOARD ?= lorawan
TARGET ?= smol-$(BOARD)

CFLAGS := -Os -Wall -std=c11 -ffreestanding -flto -g

SRCS :=

all: $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).lst

include make/macros.mk
include boards/$(BOARD)/build.mk
include lib/build.mk
include src/build.mk
include make/toolchain.mk
