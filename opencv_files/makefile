CXX = g++
RM = rm -f
FLAGS = -Wall  -g
CFLAGS = -Wall  -g -c
LDLIBS = -lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_videoio


TARGET = program

all: $(TARGET)

$(TARGET): main.o Mask.o imageProcessing.o statistics.o
	$(CXX) -o $@ $(FLAGS) $^ $(LDLIBS)


main.o: main.cpp Mask.h
	$(CXX) $(CFLAGS) $<

Mask.o: Mask.cpp Mask.h
	$(CXX) $(CFLAGS) $<

imageProcessing.o: imageProcessing.cpp imageProcessing.h
	$(CXX) $(CFLAGS) $<

statistics.o: statistics.cpp statistics.h
	$(CXX) $(CFLAGS) $<

clean:
	$(RM) *~ *.o

deepclean: clean
	$(RM) $(TARGET)
