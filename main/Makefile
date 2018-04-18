TARGET = intel_acou_alpu

CROSS_PREFIX := arm-cortexa9-linux-gnueabihf-

CC 	= $(CROSS_PREFIX)gcc
CPP = $(CROSS_PREFIX)g++

CC_FLAG += -Wall -O2
CC_FLAG += -fPIC -DPIC -DCONFIG_DEBUG_LEVEL=255

COMMON_INC_ROOT = ../common
#SND_INC_ROOT = /usr/local/arm/snd/include

AUTO_LIB_ROOT = ../auto
MSR_LIB_ROOT = ../msr
#SND_LIB_ROOT = /usr/local/arm/snd/lib

INC := -I$(COMMON_INC_ROOT)
INC += -I/usr/local/arm/python3.5/include/python3.5m

LIB := -L$(AUTO_LIB_ROOT)
LIB += -L$(MSR_LIB_ROOT)
LIB += -L/usr/local/arm/python3.5/lib -lpython3.5m
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
