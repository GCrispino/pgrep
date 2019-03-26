CC=g++
CFLAGS=-I$(IDIR) --std=c++11
IDIR=lib
LIBS=-lpthread -lm
_DEPS = DirInfo.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

ODIR=obj

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: makedir pgrep

pgrep:  $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


debug: CFLAGS += -DDEBUG -g
debug: pgrep

makedir:
	mkdir -p obj
.PHONY: clean 
clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
