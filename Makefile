
SOURCE_ROOT ?= $(HOME)

UTILS := $(SOURCE_ROOT)/utils

CC := g++
CFLAGS := -std=c++11 -Wall
ifdef DEBUG
	CFLAGS += -O0 -g3
else
	CFLAGS += -O3 -g0
endif

ifdef LOFAR
	CFLAGS += -DHAVE_HDF5
	HDF5_INCLUDE ?= -I/usr/include
endif
ifdef PSRDADA
	CFLAGS += -DHAVE_PSRDADA
	INCLUDES += -I"$(PSRDADA)/src"
endif

all: bin/ColorMap.o bin/Observation.o bin/Platform.o bin/ReadData.o

bin/ReadData.o: include/ReadData.hpp src/ReadData.cpp
	-@mkdir -p bin
	$(CC) -o bin/ReadData.o -c src/ReadData.cpp -I"include" -I"$(UTILS)/include" $(HDF5_INCLUDE) $(INCLUDES) $(CFLAGS)

bin/ColorMap.o: include/ColorMap.hpp src/ColorMap.cpp
	-@mkdir -p bin
	$(CC) -o bin/ColorMap.o -c src/ColorMap.cpp -I"include" $(CFLAGS)

bin/Observation.o: $(UTILS)/include/utils.hpp include/Observation.hpp src/Observation.cpp
	-@mkdir -p bin
	$(CC) -o bin/Observation.o -c src/Observation.cpp -I"include" -I"$(UTILS)/include" $(CFLAGS)

bin/Platform.o: $(UTILS)/include/utils.hpp include/Platform.hpp src/Platform.cpp
	-@mkdir -p bin
	$(CC) -o bin/Platform.o -c src/Platform.cpp -I"include" -I"$(UTILS)/include" $(CFLAGS)

clean:
	-@rm bin/*.o

