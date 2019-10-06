CC=gcc
CFLAGS = -O2 -Wall -pthread -march=native
LIBS =  -lpthread
# Directory to keep object files:
ODIR = obj
IDIR = include

_DEPS = newtonlib.h compute_block.h write_block.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

.PHONY: all
all: newton

# Rule to generate object files:
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -flto -c -o $@ $< $(CFLAGS) -I$(IDIR)

newton: $(ODIR)/newton.o $(ODIR)/compute_block.o $(ODIR)/write_block.o 
	$(CC) -flto -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: test clean # Avoid conflict with a file of the same name
clean:
	rm -rvf $(ODIR)/*.o newton *.ppm newton.tar.gz extracted/ pictures/

test:
	tar -czvf newton.tar.gz *
	./check_submission.py newton.tar.gz
