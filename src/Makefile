
debug		:= 0
include		../Makefile.inc


all: clean printSIGPROC printLOFAR printLOFARChannels
 
printSIGPROC: printSIGPROC.cpp
	$(LINKER) -o $(PROJ_BASE)/bin/printSIGPROC printSIGPROC.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) 

printLOFAR: printLOFAR.cpp
	$(LINKER) -o $(PROJ_BASE)/bin/printLOFAR printLOFAR.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) 

printLOFARChannels: printLOFARChannels.cpp
	$(LINKER) -o $(PROJ_BASE)/bin/printLOFARChannels printLOFARChannels.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) -lX11

clean:
	rm -f $(PROJ_BASE)/bin/printSIGPROC
	rm -f $(PROJ_BASE)/bin/printLOFAR
	rm -f $(PROJ_BASE)/bin/printLOFARChannels

