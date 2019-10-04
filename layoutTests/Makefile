CC := gcc
CFLAGS := -std=c11 -O3 -Wall -pthread -flto
LIBS := -lpthread

OBJS := main.o newtonlib.o compute_block.o write_block.o

.PHONY: all clean test
all: newton 

# Rule to generate object files:
newton: $(OBJS) 
	$(CC) -o $@ $(CFLAGS) $(OBJS)

$(OBJS) : helper.h

clean:
	-rm -rvf *.o *.ppm newton newton.tar.gz extracted/ pictures/

test:
	tar -czvf newton.tar.gz *.c *.h Makefile
	./check_submission.py newton.tar.gz
