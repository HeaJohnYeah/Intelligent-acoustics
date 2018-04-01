# Intelligent-acoustics msr Makefile 

CC=$(CROSS_PREFIX)gcc
AR=$(CROSS_PREFIX)ar

MSR := $(wildcard *.c)
OBJ = $(MSR:%.c=%.o)

CC_FLAG += -lpthread

INC = -I../common/

MSR_TARGET := libmsr.a
.PHONY : clean all

all: $(MSR_TARGET)

$(MSR_TARGET):$(OBJ)
	$(AR) rcs $(MSR_TARGET) $(OBJ)

.SUFFIXES: .o .c
.c.o:
	$(CC) $(CFLAGS) $(FLAGS_SRC) $(LDFLAGS) $(INC) -c $*.c -o $*.o
	
clean:
	@rm -f $(MSR_TARGET)
	@rm -f $(OBJ)
	@rm -f *.a