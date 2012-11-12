
debug		:= 0
include		../Makefile.inc


all: clean printSIGPROC printLOFA printLOFARChannels
 
printSIGPROC: printSIGPROC.cpp
	rm -f $(PROJ_BASE)/bin/printSIGPROC
	$(LINKER) -o $(PROJ_BASE)/bin/printSIGPROC printSIGPROC.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) 

printLOFAR: printLOFAR.cpp
	rm -f $(PROJ_BASE)/bin/printLOFAR
	$(LINKER) -o $(PROJ_BASE)/bin/printLOFAR printLOFAR.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) 

printLOFARChannels: printLOFARChannels.cpp
	LDFLAGS += -lX11
	rm -r $(PROJ_BASE)/bin/printLOFARChannels
	$(LINKER) -o $(PROJ_BASE)/bin/printLOFARChannels printLOFARChannels.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) 

clean:
	rm -f $(PROJ_BASE)/bin/printSIGPROC
	rm -f $(PROJ_BASE)/bin/printLOFAR
	rm -f $(PROJ_BASE)/bin/printLOFARChannels

