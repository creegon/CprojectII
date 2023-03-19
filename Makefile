CXX = g++ 
CFLAGS = -Wall -O3 
LDFLAGS = -fopenmp
CXXFLAGS = -Wall -O3 -mavx2 -mfma -msse4.1 -ffast-math
GMP_INCLUDE_PATH = ./gmp/include 
GMP_LIB_PATH = ./gmp/lib 
GMP_LIBS = -lgmpxx -lgmp 
OBJDIR = obj
SOURCE = dotVector.cpp 
OBJECTS = $(OBJDIR)/dotVector.o 
EXEC = exe 
OPENMP_LIBS = -fopenmp

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(OPENMP_LIBS) -L$(GMP_LIB_PATH) $(GMP_LIBS) -o $(EXEC)

$(OBJECTS): $(SOURCE)
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(OPENMP_LIBS) -I$(GMP_INCLUDE_PATH) -c $(SOURCE) -o $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(EXEC)

cleanobj:
	rm -f $(OBJECTS)

.PHONY: all clean cleanobj