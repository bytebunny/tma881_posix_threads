CC := gcc
CFLAGS := -std=c11 -O2 -Wall -pthread -flto -march=native 
LIBS := -lpthread

OBJS := main.o newtonlib.o compute_block.o write_block.o

.PHONY: all clean
all: newton 

# Rule to generate object files:
newton: $(OBJS) 
	$(CC) -o $@ $(CFLAGS) $(OBJS)

$(OBJS) : helper.h

clean:
	-rm -f *.o *.ppm newton 
