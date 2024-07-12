CXX=g++
CXXFLAGS=-Wall -g
DIR=./src
OBJ=main.o fft.o
all: fft.exe
fft.exe: $(OBJ)
	$(CXX) $(CXXFLAGS) -o fft fft.o main.o
main.o: $(DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c $(DIR)/main.cpp
fft.o: $(DIR)/fft.cpp
	$(CXX) $(CXXFLAGS) -c $(DIR)/fft.cpp
clean:
	rm -rf *.o *.exe imagen_comprimida.imgc imagen_descomprimida.img 
