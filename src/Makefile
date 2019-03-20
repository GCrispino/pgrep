CC=g++
CFLAGS=-I$(IDIR) --std=c++11
IDIR=lib
_DEPS = DirInfo.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

ODIR=obj

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 