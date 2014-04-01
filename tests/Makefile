
debug		:= 0
include		../Makefile.inc


all: clean readSIGPROC

readSIGPROC: readSIGPROC.cpp
	$(CC) -o $(PROJ_BASE)/bin/readSIGPROCTest readSIGPROC.cpp $(UTILS_SRC) $(CFLAGS)

clean:
	rm -f $(PROJ_BASE)/bin/readSIGPROCTest
