
INSTALL_ROOT ?= $(HOME)
INCLUDES := -I"include" -I"$(INSTALL_ROOT)/include"

CC := g++
CFLAGS := -std=c++11 -Wall

ifdef DEBUG
	CFLAGS += -O0 -g3
else
	CFLAGS += -O3 -g0
endif

ifdef LOFAR
	CFLAGS += -DHAVE_HDF5
	INCLUDES += -I"$(HDF5INCLUDE)"
endif
ifdef PSRDADA
	CFLAGS += -DHAVE_PSRDADA
	INCLUDES += -I"$(PSRDADA)/src"
endif

all: bin/Observation.o bin/Platform.o bin/ReadData.o
	-@mkdir -p lib
	$(CC) -o lib/libAstroData.so -shared -Wl,-soname,libAstroData.so bin/ReadData.o bin/Observation.o bin/Platform.o $(CFLAGS)

bin/ReadData.o: include/ReadData.hpp src/ReadData.cpp
	-@mkdir -p bin
	$(CC) -o bin/ReadData.o -c -fpic src/ReadData.cpp $(INCLUDES) $(CFLAGS)

bin/Observation.o: include/Observation.hpp src/Observation.cpp
	-@mkdir -p bin
	$(CC) -o bin/Observation.o -c -fpic src/Observation.cpp $(INCLUDES) $(CFLAGS)

bin/Platform.o: include/Platform.hpp src/Platform.cpp
	-@mkdir -p bin
	$(CC) -o bin/Platform.o -c -fpic src/Platform.cpp $(INCLUDES) $(CFLAGS)

clean:
	-@rm bin/*.o
	-@rm lib/*

install: all
	-@mkdir -p $(INSTALL_ROOT)/include
	-@cp include/* $(INSTALL_ROOT)/include
	-@mkdir -p $(INSTALL_ROOT)/lib
	-@cp lib/* $(INSTALL_ROOT)/lib
