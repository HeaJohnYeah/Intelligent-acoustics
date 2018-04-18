
CROSS_PREFIX := arm-cortexa9-linux-gnueabihf-

CC = $(CROSS_PREFIX)gcc

CC_FLAG += -Wall -O2

CGI := $(wildcard *.c)
OBJ := $(CGI:%.c=%.cgi)

all : $(OBJ)

%.cgi : %.c
	$(CC) $(CC_FLAG) $< -o $@
	
clean :
	rm -f *.cgi