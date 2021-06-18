
cmake_minimum_required(VERSION 3.10)
project(tinywebserver)

# compiler flags for debugging
SET(GCC_COVERAGE_COMPILE_FLAGS "-g") # -g builds a special debug version that works with GDB
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

# header files 
set(HEADER_FILES
  ./datalogging.h
  ./debug.h
  ./endpoint.h
  ./folderlist.h
  ./httpcodes.h
  ./main.h
  ./networkutil.h
#  ./relay.h
  ./server.h
  ./severconfig.h
  ./webpage.h
  ./webservice.h
)

# source files 
set(SOURCE_FILES
  ./datalogging.c
  ./endpoint.c
  ./folderlist.c
  ./main.c
#  ./relay.c
  ./server.c
  ./webpage.c
  ./networkutils.c
)

# make the exe
add_executable(tinywebserver
	${HEADER_FILES}
	${SOURCE_FILES}
)

