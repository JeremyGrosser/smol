LOCAL_DIR := $(GET_LOCAL_DIR)

CFLAGS += -DNRF51 \
		  -I$(LOCAL_DIR)

SRCS += $(LOCAL_DIR)/board.c \
		$(LOCAL_DIR)/console.c

LDSCRIPT ?= $(TOPDIR)/third_party/nrfx/mdk/nrf51_xxaa.ld

include $(TOPDIR)/platform/nrf51/build.mk
