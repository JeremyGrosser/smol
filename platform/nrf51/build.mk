LOCAL_DIR := $(GET_LOCAL_DIR)

TRIPLET := arm-none-eabi
CC := $(TRIPLET)-gcc
LD := $(CC)
OBJCOPY := $(TRIPLET)-objcopy
OBJDUMP := $(TRIPLET)-objdump

CFLAGS += -mcpu=cortex-m0 \
		  -mthumb \
          -I$(LOCAL_DIR)
LDFLAGS += --specs=nano.specs
#           --specs=nosys.specs

CMSIS := $(TOPDIR)/third_party/CMSIS

SRCS += $(LOCAL_DIR)/platform.c \
        $(LOCAL_DIR)/gpio.c \
        $(LOCAL_DIR)/uart.c \
		$(LOCAL_DIR)/i2c.c \
        $(LOCAL_DIR)/stubs.c

include $(CMSIS)/build.mk
include $(TOPDIR)/third_party/nrfx/build.mk
