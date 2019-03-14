LOCAL_DIR := $(GET_LOCAL_DIR)

CFLAGS += -I$(LOCAL_DIR)

SRCS += $(LOCAL_DIR)/sx127x.c \
		$(LOCAL_DIR)/internal.c \
		$(LOCAL_DIR)/lora.c
