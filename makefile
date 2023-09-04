CXX=g++
CXXFLAGS=-Wall -g
all: fft.exe
fft.exe: fft.cpp
	$(CXX) $(CXXFLAGS) -o fft fft.cpp
clean:
	rm -rf *.o *.exe
