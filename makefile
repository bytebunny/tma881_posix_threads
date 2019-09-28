CC=gcc
CFLAGS = -O0 -Wall -pthread
LIBS =  -larb -lflint -lmpfr -lgmp -lpthread
# Directory to keep object files:
ODIR = obj
IDIR = -I/home/hpcuser029/local_flint/include -I/home/hpcuser029/local_arb/include
LDIR = -L/home/hpcuser029/local_flint/lib -L/home/hpcuser029/local_arb/lib

# _DEPS = file.h
# DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

.PHONY: all
all: newton

# Rule to generate object files:
$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

newton: $(ODIR)/newton.o
	$(CC) -o $@ $^ $(CFLAGS) $(IDIR) $(LDIR) $(LIBS)


.PHONY: clean # Avoid conflict with a file of the same name
clean:
	rm -rvf $(ODIR)/*.o newton attractor.ppm convergence.ppm 
