cmake_minimum_required(VERSION 3.8)
project(AstroData VERSION 4.0)
include(GNUInstallDirs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native -mtune=native")
if($ENV{LOFAR})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_HDF5")
endif()
if($ENV{PSRDADA})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_PSRDADA")
endif()

# libastrodata
set(LIBRARY_SOURCE
  src/Observation.cpp
  src/Platform.cpp
  src/ReadData.cpp
  src/SynthesizedBeams.cpp
)
set(LIBRARY_HEADER
  include/Generator.hpp
  include/Observation.hpp
  include/Platform.hpp
  include/ReadData.hpp
  include/SynthesizedBeams.hpp
)
add_library(astrodata SHARED ${LIBRARY_SOURCE} ${LIBRARY_HEADER})
set_target_properties(astrodata PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 1
  PUBLIC_HEADER "include/Generator.hpp;include/Observation.hpp;include/Platform.hpp;include/ReadData.hpp;include/SynthesizedBeams.hpp"
)
target_include_directories(astrodata PRIVATE include)

install(TARGETS astrodata
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Unit testing
enable_testing()
set(TEST_LINK_LIBRARIES gtest pthread isa_utils)
if($ENV{LOFAR})
  set(TEST_LINK_LIBRARIES ${TARGET_LINK_LIBRARIES} hdf5 hdf5_cpp z)
endif()
if($ENV{PSRDADA})
  set(TEST_LINK_LIBRARIES ${TEST_LINK_LIBRARIES} psrdada cudart)
endif()
## ReadDataTest
add_executable(ReadDataTest
  test/ReadDataTest.cpp
)
target_include_directories(ReadDataTest PRIVATE include)
target_link_libraries(ReadDataTest PRIVATE astrodata ${TEST_LINK_LIBRARIES})
add_test(NAME ReadDataTest COMMAND ReadDataTest -path ../test)
## SynthesizedBeamsTest
add_executable(SynthesizedBeamsTest
  test/SynthesizedBeamsTest.cpp
)
target_include_directories(SynthesizedBeamsTest PRIVATE include)
target_link_libraries(SynthesizedBeamsTest PRIVATE astrodata ${TEST_LINK_LIBRARIES})
add_test(NAME SynthesizedBeamsTest COMMAND SynthesizedBeamsTest -path ../test)
