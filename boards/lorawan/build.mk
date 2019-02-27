LOCAL_DIR := $(GET_LOCAL_DIR)

CFLAGS += -D__SAMD21G18A__ \
		  -I$(LOCAL_DIR)

SRCS += $(LOCAL_DIR)/cdcserial.c \
		$(LOCAL_DIR)/board.c

LDSCRIPT ?= $(CMSIS)/Device/ATMEL/SAMD21/source/gcc/samd21g18a_flash.ld
#LDSCRIPT := $(LOCAL_DIR)/samd21g18a_arduino.ld

include $(TOPDIR)/platform/samd21/build.mk
include $(TOPDIR)/driver/eeprom/build.mk
include $(TOPDIR)/driver/sx127x/build.mk
