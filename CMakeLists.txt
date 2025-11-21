cmake_minimum_required(VERSION 3.10)
project(NBody)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function -O3")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wno-unused-function -O3 -funroll-loops -std=gnu11")

option(BUILD_PARALLEL "Build parallel simulation, only one of BUILD_PARALLEL of BUILD_MPI should be ON" OFF)
if (BUILD_PARALLEL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNBODY_PARALLEL=1")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNBODY_PARALLEL=1")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNBODY_PARALLEL=0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNBODY_PARALLEL=0")
endif()

option(BUILD_MPI "Build distributed simulation, only one of BUILD_PARALLEL of BUILD_MPI should be ON" OFF)
if (BUILD_MPI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNBODY_MPI=1")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNBODY_MPI=1")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNBODY_MPI=0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNBODY_MPI=0")
endif()

option(BUILD_VISUALIZER "Build the OpenGL visualizer")
if (BUILD_VISUALIZER)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNBODY_SIM_WITH_RENDERER=0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNBODY_SIM_WITH_RENDERER=0")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNBODY_SIM_WITH_RENDERER=1")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNBODY_SIM_WITH_RENDERER=1")
endif()


include_directories(include)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# include GLFW
add_subdirectory(include/glfw)

# include GLM
add_subdirectory(include/glm)

# include GLAD
include_directories(include/glad/include)

set(SOURCE_FILES
        src/NBodyMain.cpp
        src/NBodyForces.c
        src/NBodyHashedOctree.c
        src/NBodyInit.c
        src/NBodyKeys.c
        src/NBodyOctree.c
        src/NBodySimulation.cpp
        include/glad/src/glad.c)


add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# linking GLFW + OGL
target_link_libraries(${CMAKE_PROJECT_NAME} glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
