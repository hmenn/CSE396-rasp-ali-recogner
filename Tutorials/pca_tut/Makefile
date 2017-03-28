CC = g++
CFLAGS 	= -c -Wall -O3  -std=c++11 -I ./include/

#Include Libs
#include OpenCV
CFLAGS 		+= $(shell pkg-config opencv --cflags)
LDFLAGS 	+= $(shell pkg-config opencv --libs)

SOURCES		= $(wildcard ./src/*.cpp)

OBJECTS		= $(SOURCES:.cpp=.o)

EXECUTABLE = pca_test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(CURDIR)/$< -o $@

clean:
	rm -rf ./src/*.o $(EXECUTABLE)
