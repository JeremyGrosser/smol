LOCAL_DIR := $(GET_LOCAL_DIR)

SRCS += $(LOCAL_DIR)/main.c

include $(TOPDIR)/driver/eeprom/build.mk
include $(TOPDIR)/driver/sx127x/build.mk
