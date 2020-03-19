cmake_minimum_required(VERSION 3.15.2)

project(simplegl)

set(CMAKE_BUILD_TYPE "Release") 
include(GNUInstallDirs)

add_library(simplegl SHARED src/simplegl.cc)
target_include_directories(simplegl
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src)

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
