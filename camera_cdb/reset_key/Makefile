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

# 编译器在编译时的参数设置 -Wall显示所有警告， -O2优化等级 -g调试信息
CFLAGS := -Wall -O2 -march=mips32r2

#debug调试开关
CFLAGS += -DDEBUG -g 

BULD_ROOT := ..

OBJ := reset_key.o

TARGET := ../out/reset_key

$(TARGET): $(OBJ)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)
	$(STRIP) $(TARGET)

$(OBJS): %.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJ)