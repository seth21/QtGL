cmake_minimum_required(VERSION 3.10)

# your project name
project("CVSCODE")

# find includes in the corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# run moc automatically
set(CMAKE_AUTOMOC ON)
# if you're using Widgets and editing .ui forms in Designer
set(CMAKE_AUTOUIC ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "Build type not specified: Use Release by default")
endif(NOT CMAKE_BUILD_TYPE)

message("Looking for Qt...")
# Qt modules (https://doc.qt.io/qt-5/qtmodules.html) you're using in your application
find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
if (${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message("Couldn't find Qt")
endif()

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

## your source files
# Add source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp)

# name of the .exe file, window flag and the list of things to compile
add_executable(${CMAKE_PROJECT_NAME} WIN32 ${HEADER_FILES} ${SOURCE_FILES})

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")


#######################################
# LOOK for the packages that we need! #
#######################################

# GLM
find_package(GLM REQUIRED)
message(STATUS "Found GLM at ${GLM_INCLUDE_DIR}")
if(GLM_FOUND)
	include_directories(${GLM_INCLUDE_DIR})
endif()

# ASSIMP
set(ASSIMP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(ASSIMP_LIBRARY ${CMAKE_SOURCE_DIR}/lib)
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP at ${ASSIMP_INCLUDE_DIR}")
if(ASSIMP_FOUND)
	include_directories(${ASSIMP_INCLUDE_DIR})
endif()

set(LIBS assimp)

# Copy dlls
if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/dlls"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

# also tell CMake to link with the libraries from the line 14
# otherwise it'll think that you wanted to find those just for fun and won't link them
target_link_libraries(
	${CMAKE_PROJECT_NAME}
	Qt5::Widgets
	Qt5::OpenGL
	${LIBS}
	)
#
# copy folder resources and its contents
file(GLOB files "resources/**/*")
foreach(file ${files})
	get_filename_component(name ${file} NAME)
	get_filename_component(folder ${file} PATH)
	string(REPLACE "${CMAKE_SOURCE_DIR}/" "" folder "${folder}")
	message(STATUS "path to copy = ${folder}/${name}")
	if (MSVC)
		configure_file(${file} "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${folder}/${name}" COPYONLY)
	else()
		configure_file(${file} "${CMAKE_CURRENT_BINARY_DIR}/${folder}/${name}" COPYONLY)
	endif()
endforeach()
