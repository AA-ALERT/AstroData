
UTILS := $(HOME)/src/utils

CC := g++
CFLAGS := -std=c++11 -Wall
ifneq ($(DEBUG), 1)
	CFLAGS += -O3 -g0
else
	CFLAGS += -O0 -g3
endif


all: ColorMap.o Observation.o

ColorMap.o: include/ColorMap.hpp src/ColorMap.cpp
	$(CC) -o bin/ColorMap.o -c src/ColorMap.cpp -I"include" $(CFLAGS)

Observation.o: $(UTILS)/include/utils.hpp include/Observation.hpp src/Observation.cpp
	$(CC) -o bin/Observation.o -c src/Observation.cpp -I"include" -I"$(UTILS)/include" $(CFLAGS)

clean:
	rm bin/*.o

