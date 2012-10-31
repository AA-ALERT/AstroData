
PROJ_BASE	:= /home/alessio/src/AstroData

INCLUDES	:= -I"$(PROJ_BASE)/include" -I"$(OPENCL_INCLUDE)" -I"/home/alessio/src/utils/include" -I"/home/alessio/src/OpenCL/include"
LIBS		:= -L"$(OPENCL_LIB)"

ifneq ($(debug), 1)
	CFLAGS		:= -O3 -g0 -w -msse3 -fopenmp
else
	CFLAGS		:= -O0 -g3 -Wall -msse3 -fopenmp
endif

LDFLAGS         := -lm -lOpenCL -lhdf5 -lhdf5_cpp

CC		:= g++
LINKER		:= g++

UTILS_SRC	:= /home/alessio/src/utils/src/Timer.cpp /home/alessio/src/utils/src/utils.cpp

