# Intelligent-acoustics msr Makefile 

CC=$(CROSS_PREFIX)gcc
AR=$(CROSS_PREFIX)ar

MSR := $(wildcard *.c)
OBJ = $(MSR:%.c=%.o)


INC := -I../common/
INC += -I/usr/local/snd/include 

LIB := -L/usr/local/snd/lib -lsndfile


MSR_TARGET := libmsr.a
.PHONY : clean all

all: $(MSR_TARGET)

$(MSR_TARGET):$(OBJ)
	$(AR) rcs $(MSR_TARGET) $(OBJ)

.SUFFIXES: .o .c
.c.o:
	$(CC)  $(INC)  $(LIB) -c $*.c -o $*.o
	
clean:
	@rm -f $(MSR_TARGET)
	@rm -f $(OBJ)
	@rm -f *.a
