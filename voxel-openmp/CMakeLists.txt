include_directories(../)
include_directories(./include)


 # dziala rekurencyjnie
file(GLOB_RECURSE voxel-omp_SOURCES "src/*.cpp")
file(GLOB_RECURSE voxel-omp_HEADERS "inc/*.h")


#find_package(OpenMP)
#if (OPENMP_FOUND)
#    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    include_directories(${OPENMP_INCLUDE_DIRS})
#    message("OPENMP ZNALEZIONO !!!!")
#endif()

    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(voxel-omp ${voxel-omp_SOURCES} ${voxel-omp_HEADERS})


if(UNIX)
    target_link_libraries(voxel-omp m) 
endif(UNIX) 



   
