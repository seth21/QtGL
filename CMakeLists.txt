cmake_minimum_required(VERSION 3.10)

# your project name
project("CVSCODE")

# find includes in the corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# run moc automatically
set(CMAKE_AUTOMOC ON)
# if you're using Widgets and editing .ui forms in Designer
set(CMAKE_AUTOUIC ON)

message("Looking for Qt...")
# Qt modules (https://doc.qt.io/qt-5/qtmodules.html) you're using in your application
find_package(Qt5 REQUIRED Widgets)
find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
if (${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message("Couldn't find Qt")
endif()

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

# resources
file(GLOB files "resources/*")
foreach(file ${files})
	get_filename_component(name ${file} NAME)
	message(${name})
	configure_file(${file} resources/${name} COPYONLY)
endforeach()

# shaders
file(GLOB files "shaders/*")
foreach(file ${files})
	get_filename_component(name ${file} NAME)
	configure_file(${file} shaders/${name} COPYONLY)
endforeach()

# also tell CMake to link with the libraries from the line 14
# otherwise it'll think that you wanted to find those just for fun and won't link them
target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    Qt5::Widgets
    Qt5::OpenGL
    )