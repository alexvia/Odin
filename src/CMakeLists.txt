include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/extern/glm)
include_directories(${CMAKE_SOURCE_DIR}/extern/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/extern/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/extern/tinyxml2)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

file(GLOB sources Odin/*.cpp Util/*.cpp *cpp)
file(GLOB tinyxml2sources ${CMAKE_SOURCE_DIR}/extern/tinyxml2/*.cpp)

add_executable(Odin ${sources} ${tinyxml2sources})

target_link_libraries(Odin opengl32 glfw ${GLFW_LIBRARIES})
target_link_libraries(Odin glad)