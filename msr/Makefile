# Intelligent-acoustics msr Makefile 

CC=$(CROSS_PREFIX)gcc
CPP=$(CROSS_PREFIX)g++
AR=$(CROSS_PREFIX)ar

CC_FLAG = -Wall -O2

CC_FLAG += -lpthread

INC = -I../common/

MSR_TARGET := libmsr.a
OBJ = msr.o

.PHONY : clean all

all: $(MSR_TARGET)

$(MSR_TARGET):$(OBJ)
	$(AR) rcs $(MSR_TARGET) $(OBJ)

		

%.o : %.cpp
	$(CPP) $(CC_FLAG) $(INC) -c $< -o $@
	
clean:
	@rm -f $(MSR_TARGET)
	@rm -f $(OBJ)
	@rm -f *.a