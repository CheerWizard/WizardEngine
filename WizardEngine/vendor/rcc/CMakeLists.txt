cmake_minimum_required(VERSION 3.12)

project(rcc)

if(UNIX AND NOT APPLE)
	set(BUILD_TYPE SHARED)
else()
	set(BUILD_TYPE STATIC)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

include(cmake/ProjectFiles.cmake)

#
# RuntimeCompiler
#
add_library(RuntimeCompiler ${BUILD_TYPE} ${RuntimeCompiler_SRCS})

#
# RuntimeObjectSystem
#
add_library(RuntimeObjectSystem ${BUILD_TYPE} ${RuntimeObjectSystem_SRCS})
target_link_libraries(RuntimeObjectSystem RuntimeCompiler)
if(UNIX)
	target_link_libraries(RuntimeObjectSystem dl)
endif()

#
# Make Install
#
install(DIRECTORY RuntimeObjectSystem DESTINATION ${CMAKE_INSTALL_PREFIX}/include/RuntimeObjectSystem
	FILES_MATCHING PATTERN "*.h")
install(DIRECTORY RuntimeCompiler DESTINATION ${CMAKE_INSTALL_PREFIX}/include/RuntimeCompiler
	FILES_MATCHING PATTERN "*.h")
install(TARGETS RuntimeObjectSystem RuntimeCompiler
	DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/)
