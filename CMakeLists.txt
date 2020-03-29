cmake_minimum_required(VERSION 3.15.2)

project(simplegl)

set(CMAKE_BUILD_TYPE "Release") 
set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include(GNUInstallDirs)


# Disable in-source builds
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
FATAL: In-source build is not allowed. Create a separate directory for build files.
")
endif()

# Create library target
add_library(simplegl SHARED src/simplegl.cc)

# Add include dirs
target_include_directories(simplegl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

# Link dependencies

# GLEW
find_package(GLEW REQUIRED)
target_link_libraries(simplegl PRIVATE GLEW::GLEW)

# SDL2
find_package(SDL2 REQUIRED)
target_include_directories(simplegl PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(simplegl PRIVATE ${SDL2_LIBRARIES})

# TEST
add_executable(test src/test_opengl.cc)
target_link_libraries(test PRIVATE GLEW::GLEW)
target_include_directories(test PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(test PRIVATE ${SDL2_LIBRARIES})

# Configure installation
install(TARGETS simplegl EXPORT SimpleGLConfig
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Not using this one because could not figure out how to get cmake that uses this
# library to search in the simplegl folder, it would only search in the CMAKE_INSTALL_INCLUDEDIR
# folder, but the other cmakelists did not have access to that variable
#install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/simplegl)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT SimpleGLConfig DESTINATION share/SimpleGL/cmake)

export(TARGETS simplegl FILE SimpleGLConfig.cmake)
