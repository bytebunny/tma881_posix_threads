CC=gcc
CFLAGS = -O0 -Wall -pthread
LIBS =  -larb -lflint -lmpfr -lgmp -lpthread
# Directory to keep object files:
ODIR = obj
IDIR = include

_DEPS = newtonlib.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

.PHONY: all
all: newton

# Rule to generate object files:
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -flto -c -o $@ $< $(CFLAGS) -I$(IDIR)

newton: $(ODIR)/newton.o $(ODIR)/newtonlib.o
	$(CC) -flto -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean # Avoid conflict with a file of the same name
clean:
	rm -rvf $(ODIR)/*.o newton attractor.ppm convergence.ppm 
