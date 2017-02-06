
SOURCE_ROOT ?= $(HOME)

UTILS := $(SOURCE_ROOT)/src/utils
HDF5 := $(SOURCE_ROOT)/src/hdf5
PSRDADA := $(SOURCE_ROOT)/src/psrdada

CC := g++
CFLAGS := -std=c++11 -Wall
ifneq ($(DEBUG), 1)
	CFLAGS += -O3 -g0
else
	CFLAGS += -O0 -g3
endif


all: bin/ColorMap.o bin/Observation.o bin/Platform.o bin/ReadData.o

bin/ReadData.o: include/ReadData.hpp src/ReadData.cpp
	-mkdir -p bin
	$(CC) -o bin/ReadData.o -c src/ReadData.cpp -I"include" -I"$(UTILS)/include" -I"$(HDF5)/include" -I"$(PSRDADA)/src" $(CFLAGS)

bin/ColorMap.o: include/ColorMap.hpp src/ColorMap.cpp
	-mkdir -p bin
	$(CC) -o bin/ColorMap.o -c src/ColorMap.cpp -I"include" $(CFLAGS)

bin/Observation.o: $(UTILS)/include/utils.hpp include/Observation.hpp src/Observation.cpp
	-mkdir -p bin
	$(CC) -o bin/Observation.o -c src/Observation.cpp -I"include" -I"$(UTILS)/include" $(CFLAGS)

bin/Platform.o: $(UTILS)/include/utils.hpp include/Platform.hpp src/Platform.cpp
	-mkdir -p bin
	$(CC) -o bin/Platform.o -c src/Platform.cpp -I"include" -I"$(UTILS)/include" $(CFLAGS)

clean:
	-@rm bin/*.o

