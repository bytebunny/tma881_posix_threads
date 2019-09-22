CC=gcc
CFLAGS = -O0 -Wall
LIBS = -larb -lflint -lmpfr -lgmp -lpthread
# Directory to keep object files:
ODIR = obj
IDIR = include

# _DEPS = file.h
# DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

.PHONY: all
all: newton

# Rule to generate object files:
$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) #-I$(IDIR)

newton: $(ODIR)/newton.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean # Avoid conflict with a file of the same name
clean:
	rm -f $(ODIR)/*.o newton
