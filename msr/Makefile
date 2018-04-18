# Intelligent-acoustics msr Makefile

CROSS_PREFIX := arm-cortexa9-linux-gnueabihf-

CC=$(CROSS_PREFIX)gcc
AR=$(CROSS_PREFIX)ar

MSR := $(wildcard *.c)
OBJ = $(MSR:%.c=%.o)


CC_FLAG += -lpthread

INC := -I../common/
INC := -I/usr/local/arm/python3.5/include/python3.5m


LIB := -L/usr/local/arm/python3.5/lib -lpython3.5m


MSR_TARGET := libmsr.a
.PHONY : clean all

all: $(MSR_TARGET)

$(MSR_TARGET):$(OBJ)
	$(AR) rcs $(MSR_TARGET) $(OBJ)

.SUFFIXES: .o .c
.c.o:
	$(CC)  $(INC)  $(CC_FLAG) $(LIB) -c $*.c -o $*.o

clean:
	@rm -f $(MSR_TARGET)
	@rm -f $(OBJ)
	@rm -f *.a
