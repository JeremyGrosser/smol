LOCAL_DIR := $(GET_LOCAL_DIR)

TRIPLET := arm-none-eabi
CC := $(TRIPLET)-gcc
LD := $(CC)
OBJCOPY := $(TRIPLET)-objcopy
OBJDUMP := $(TRIPLET)-objdump

CFLAGS += -march=armv6-m \
          -I$(LOCAL_DIR)
LDFLAGS += --specs=nosys.specs \
           --specs=nano.specs

CMSIS := $(TOPDIR)/third_party/CMSIS

SRCS += $(LOCAL_DIR)/platform.c \
        $(LOCAL_DIR)/gpio.c \
        $(LOCAL_DIR)/spi.c \
        $(LOCAL_DIR)/i2c.c \
        $(LOCAL_DIR)/uart.c \
        $(LOCAL_DIR)/stubs.c \
        $(LOCAL_DIR)/rtc.c \
        $(LOCAL_DIR)/sysclk.c
        #$(LOCAL_DIR)/timer.c

include $(CMSIS)/build.mk
include $(CMSIS)/Device/ATMEL/SAMD21/build.mk
