
UTILS := $(HOME)/src/utils

CC := g++
CFLAGS := -std=c++11 -Wall
ifneq ($(DEBUG), 1)
	CFLAGS += -O3 -g0
else
	CFLAGS += -O0 -g3
endif


all: bin/ColorMap.o bin/Observation.o bin/Platform.o

bin/ColorMap.o: include/ColorMap.hpp src/ColorMap.cpp
	$(CC) -o bin/ColorMap.o -c src/ColorMap.cpp -I"include" $(CFLAGS)

bin/Observation.o: $(UTILS)/include/utils.hpp include/Observation.hpp src/Observation.cpp
	$(CC) -o bin/Observation.o -c src/Observation.cpp -I"include" -I"$(UTILS)/include" $(CFLAGS)

bin/Platform.o: $(UTILS)/include/utils.hpp include/Platform.hpp src/Platform.cpp
	$(CC) -o bin/Platform.o -c src/Platform.cpp -I"include" -I"$(UTILS)/include" $(CFLAGS)

clean:
	-@rm bin/*.o

