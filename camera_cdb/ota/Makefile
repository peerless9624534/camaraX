CROSS_COMPILE = mips-linux-gnu-
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
CPLUSPLUS = $(CROSS_COMPILE)g++
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

LDFLAGS := -pthread -lm -lrt -lstdc++
LDFLAGS += -Wall -Wl,--gc-section

CFLAGS := -Wall -O2 -march=mips32r2

OBJ := ota.o

TARGET := ../out/ota

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
	$(STRIP) $(TARGET)

$(OBJS): %.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@ 

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJ)