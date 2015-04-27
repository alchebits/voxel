include_directories(../)
include_directories(./include)

 # dziala rekurencyjnie
file(GLOB_RECURSE voxel_SOURCES "src/*.cpp")
file(GLOB_RECURSE voxel_HEADERS "inc/*.h")

SET(HEADERS
  ${voxel_HEADERS}
)

add_executable(voxel ${voxel_SOURCES} ${voxel_HEADERS})

if(UNIX)
    target_link_libraries(voxel m) 
endif(UNIX) 


   
