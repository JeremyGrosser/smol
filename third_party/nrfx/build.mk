LOCAL_DIR := $(GET_LOCAL_DIR)

CFLAGS += -I$(LOCAL_DIR)/mdk -L$(LOCAL_DIR)/mdk

SRCS += $(LOCAL_DIR)/mdk/gcc_startup_nrf51.S \
		$(LOCAL_DIR)/mdk/system_nrf51.c
