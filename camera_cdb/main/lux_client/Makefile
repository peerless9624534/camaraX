#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR = $(shell cd $(CURDIR)/../.. && /bin/pwd)
include base.mak
endif

#CURDIR ?= $(shell pwd)
#CC := $(CROSS_COMPILE)gcc
#===============================================================================
# local variable
#===============================================================================
SAMPLE_IMAGES := luxcmd

CFLAGS = -funsigned-char -freg-struct-return -fno-strict-aliasing -Wtrampolines -Wformat=2 -Wshadow -Wall -fpic -fpie -fno-common -Wfloat-equal -Wextra -fvisibility=hidden
CFLAGS += -D_FORTIFY_SOURCE=2 -O2 -pipe
#规范建议：在Debug版本中实施，Release版本不实施
#CFLAGS += -ftrapv

#CFLAGS += -I$(CURDIR)/../../securec
CFLAGS += -I$(CURDIR)/../lux_server/include
CFLAGS += -D_GNU_SOURCE

LDFLAGS := -Wl,-z,relro,-z,now -Wl,-z,noexecstack -pie -Wl,-Bsymbolic -rdynamic -Wl,--no-undefined
#LIBPATH += -L$(CURDIR)/../../securec

SAMPLE_OBJ := sample_cli.o \

DEPEND_LIBS := -lpthread
.PHONY: all clean
all:$(SAMPLE_IMAGES)

luxcmd: $(SAMPLE_OBJ)
	$(CC) -o $@ $^ $(LIBPATH) $(DEPEND_LIBS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	-@rm -f $(SAMPLE_IMAGES)
	-@rm -f $(SAMPLE_OBJ)
