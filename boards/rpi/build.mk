LOCAL_DIR := $(GET_LOCAL_DIR)

CFLAGS += -I$(LOCAL_DIR)

SRCS += $(LOCAL_DIR)/board.c

include $(TOPDIR)/platform/linux/build.mk
