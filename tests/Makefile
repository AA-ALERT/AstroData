
debug		:= 0
include		../Makefile.inc


all: clean readSIGPROC

readSIGPROC: readSIGPROC.cpp
	$(CC) -o $(PROJ_BASE)/bin/readSIGPROCTest readSIGPROC.cpp $(INCLUDES) $(UTILS_SRC) $(CFLAGS)

clean:
	rm -f $(PROJ_BASE)/bin/readSIGPROCTest
