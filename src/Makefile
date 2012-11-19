
debug		:= 0
include		../Makefile.inc


all: clean printSIGPROC printLOFAR_GNUplot printLOFAR_Image printLOFARChannels
 
printSIGPROC: printSIGPROC.cpp
	$(LINKER) -o $(PROJ_BASE)/bin/printSIGPROC printSIGPROC.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) 

printLOFAR_GNUplot: printLOFAR_GNUplot.cpp
	$(LINKER) -o $(PROJ_BASE)/bin/printLOFAR_GNUplot printLOFAR_GNUplot.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) 

printLOFAR_Image: printLOFAR_Image.cpp
	$(LINKER) -o $(PROJ_BASE)/bin/printLOFAR_Image printLOFAR_Image.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) -lX11

printLOFARChannels: printLOFARChannels.cpp
	$(LINKER) -o $(PROJ_BASE)/bin/printLOFARChannels printLOFARChannels.cpp $(UTILS_SRC) $(INCLUDES) $(LIBS) $(CFLAGS) $(LDFLAGS) -lX11

clean:
	rm -f $(PROJ_BASE)/bin/printSIGPROC
	rm -f $(PROJ_BASE)/bin/printLOFAR_GNUplot
	rm -f $(PROJ_BASE)/bin/printLOFAR_Image
	rm -f $(PROJ_BASE)/bin/printLOFARChannels

