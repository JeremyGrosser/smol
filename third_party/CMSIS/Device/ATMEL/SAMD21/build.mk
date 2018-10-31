LOCAL_DIR := $(GET_LOCAL_DIR)

CFLAGS += -I$(LOCAL_DIR)/include \
		  -I$(LOCAL_DIR)/source

SRCS += $(LOCAL_DIR)/source/gcc/startup_samd21.c
