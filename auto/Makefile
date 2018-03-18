# Intelligent-acoustics auto Makefile 

CC=$(CROSS_PREFIX)gcc
AR=$(CROSS_PREFIX)ar

AUTO := $(wildcard *.c)
OBJ  := $(AUTO:%.c=%.o)

CC_FLAG += -lpthread


AUTO_TARGET := libauto.a
.PHONY : clean all

all: $(AUTO_TARGET)

$(AUTO_TARGET):$(OBJ)
	$(AR) rcs $(AUTO_TARGET) $(OBJ)

	
.SUFFIXES: .o .c	

.c.o:
	$(CC) $(CC_FLAG) -c $*.c -o $*.o
	
clean:
	@rm -f $(AUTO_TARGET)
	@rm -f $(OBJ)
	@rm -f *.a