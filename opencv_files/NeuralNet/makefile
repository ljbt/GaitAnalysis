# Read the comments for compiling with macOS

CXX = g++ 
FLAGS = -Wall -O2 -g
CFLAGS = -Wall -O2 -g -c
LIBS = -lm -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_videoio


TARGET = reconnaissance_pastille

all: $(TARGET)

$(TARGET): mainLearning.o fonctions.o gestionModeles.o Mask.o imageProcessing.o
	$(CXX) -o $@ $(FLAGS) $^ $(LIBS)

mainLearning.o: mainLearning.cpp NNdefinitions.h
	$(CXX) $(CFLAGS) $<

fonctions.o: fonctions.cpp NNdefinitions.h
	$(CXX) $(CFLAGS) $<

gestionModeles.o: gestionModeles.cpp 
	$(CXX) $(CFLAGS) $<

Mask.o: Mask.cpp Mask.h
	$(CXX) $(CFLAGS) $<

imageProcessing.o: imageProcessing.cpp imageProcessing.h NNdefinitions.h
	$(CXX) $(CFLAGS) $<

zip:
	tar -cvzf $(TARGET).tgz *.[ch] *.pdf makefile

clean:
	rm -f *~ *.o

deepclean: clean
	rm -f $(TARGET) 
