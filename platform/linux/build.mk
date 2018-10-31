LOCAL_DIR := $(GET_LOCAL_DIR)

LD := $(CC)
OBJCOPY := objcopy
OBJDUMP := objdump

CFLAGS += -I$(LOCAL_DIR) \
          -D_POSIX_C_SOURCE=199309L

LDFLAGS += -lrt

SRCS += $(LOCAL_DIR)/platform.c \
        $(LOCAL_DIR)/gpio.c \
        $(LOCAL_DIR)/i2c.c \
        $(LOCAL_DIR)/spi.c \
        $(LOCAL_DIR)/uart.c \
        $(LOCAL_DIR)/rtc.c \
		$(LOCAL_DIR)/stubs.c
