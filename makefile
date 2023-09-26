CXX=g++
CXXFLAGS=-Wall -g
all: fft.exe
fft.exe: main.o fft.o
	$(CXX) -o fft fft.o main.o
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp
fft.o: fft.cpp
	$(CXX) $(CXXFLAGS) -c fft.cpp
clean:
	rm -rf *.o *.exe
