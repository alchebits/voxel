  cmake_minimum_required(VERSION 2.8)
  project(voxel)
	
# === Set the build type to release if not otherwise specified.
  if (NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE release)
  endif ()
	
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

  OPTION( BUILD_SHARED_LIBS "Set to OFF to build static libraries" OFF )

#  find_package (CUDA REQUIRED)
#  find_package (Threads REQUIRED)
 
 #if(UNIX)
   #SET(CMAKE_C_COMPILER gcc-4.9)
   #SET(CMAKE_CXX_COMPILER g++-4.9) 
   #set(CUDA_HOST_COMPILER  ${CMAKE_C_COMPILER})
 #endif(UNIX) 
  # 
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -O0 -g -Wall")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -O2 -Wall")
  #set(CMAKE_CXX_FLAGS "-Wall") # standardowe

  # === Set global project include directories.
  #  include_directories(${CUDA_INCLUDE_DIRS})

  file(COPY input/ DESTINATION ${voxel_BINARY_DIR}/input)
  file(COPY scripts/ DESTINATION ${voxel_BINARY_DIR}/scripts)

  add_subdirectory(voxel)
  #add_subdirectory(voxel-openmp) # to multiplatformize
  #add_subdirectory(voxel-mpi) # not created CMake for it yet, use console


