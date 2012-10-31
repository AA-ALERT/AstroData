
debug		:= 0
include		../Makefile.inc


all: clean printSIGPROC printLOFAR
 
printSIGPROC: printSIGPROC.cpp
	$(LINKER) -o $(PROJ_BASE)/bin/printSIGPROC printSIGPROC.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) 

printLOFAR: printLOFAR.cpp
	$(LINKER) -o $(PROJ_BASE)/bin/printLOFAR printLOFAR.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) 

clean:
	rm -f $(PROJ_BASE)/bin/printSIGPROC
	rm -f $(PROJ_BASE)/bin/printLOFAR

