CC := gcc
CFLAGS := -std=c11 -O2 -Wall -g -pthread
LIBS := -lpthread

OBJS := main.o newtonlib.o compute_block.o write_block.o

.PHONY: all clean
all: main 

# Rule to generate object files:
main: $(OBJS) 
	$(CC) $(CFLAGS) -flto $(OBJS)  -o $@ 

$(OBJS) : helper.h

clean:
	-rm -f *.o main 
