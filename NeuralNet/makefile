# Read the comments for compiling with macOS

CXX = g++ 
FLAGS = -Wall -O2 -g
CFLAGS = -Wall -O2 -g -c
LIBS = -lm 

TARGET = reconnaissance_pastille

all: $(TARGET)

$(TARGET): main.o fonctions.o 
	$(CXX) -o $@ $(FLAGS) $^ $(LIBS)

main.o: main.cpp NNdefinitions.h fonctions.h 
	$(CXX) $(CFLAGS) $<

fonctions.o: fonctions.cpp NNdefinitions.h fonctions.h
	$(CXX) $(CFLAGS) $<


zip:
	tar -cvzf $(TARGET).tgz *.[ch] *.pdf makefile

clean:
	rm -f *~ *.o

deepclean: clean
	rm -f $(TARGET) 
