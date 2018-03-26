TARGET = intel_acou_alpu

CC 	= $(CROSS_PREFIX)gcc
CPP = $(CROSS_PREFIX)g++

CC_FLAG += -Wall -O2
CC_FLAG += -fPIC -DPIC -DCONFIG_DEBUG_LEVEL=255

COMMON_INC_ROOT = ../common

AUTO_LIB_ROOT = ../auto 
MSR_LIB_ROOT = ../msr 

INC = -I$(COMMON_INC_ROOT)
LIB := -L$(AUTO_LIB_ROOT) 
LIB += -L$(MSR_LIB_ROOT) 
LIB += -lpthread -lauto -lmsr


OBJ = main.o

all : $(TARGET)

$(TARGET) : $(OBJ)
	$(CPP) $(OBJ) $(INC) $(LIB) -o $@ 
	
%.o : %.c
	$(CC)  $(CC_FLAG) $(INC) $(LIB) -c $< -o $@ 
%.o : %.cpp
	$(CPP) $(CC_FLAG) $(INC) $(LIB) -c $< -o $@ 
	
clean :
	rm -f $(OBJ) $(TARGET)
	rm -f ../bin/$(TARGET)
install :
	cp $(TARGET) ../bin/$(TARGET)
