LOCAL_DIR := $(GET_LOCAL_DIR)

CFLAGS += -D__SAMD21J18A__ \
		  -I$(LOCAL_DIR)

SRCS += $(LOCAL_DIR)/board.c

LDSCRIPT ?= $(CMSIS)/Device/ATMEL/SAMD21/source/gcc/samd21j18a_flash.ld

include $(TOPDIR)/platform/samd21/build.mk
include $(TOPDIR)/driver/max7219/build.mk
