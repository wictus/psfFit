project(psffitting)

add_definitions(-std=gnu++0x)

list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)

find_package(ROOT REQUIRED COMPONENTS Hist Gpad Graf) 

include_directories(${ROOT_INCLUDE_DIRS})


add_library(PSF SHARED PSF/psf.cpp)
add_library(RAW SHARED RawSpectra/rawSpectra.cpp)

target_link_libraries(PSF ${ROOT_LIBRARIES})
target_link_libraries(RAW ${ROOT_LIBRARIES} PSF)
  
add_executable(main main.cpp)

target_link_libraries(main PSF RAW)
