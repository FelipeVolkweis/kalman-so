cmake_minimum_required(VERSION 3.16.0)

# Setting project configs
project(SSLLogClient LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setting Qt-specific configs
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets Gui Core Network Concurrent REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# CMake policy: let AUTOMOC and AUTOUIC process header files that end with a .hh extension
cmake_policy(SET CMP0100 NEW)

# Setting `SRC_DIR` as `src` folder absolute path
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Generating protobuf files
execute_process(COMMAND sh protoc.sh WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Finding source and header files from disk
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${SRC_DIR}/*.c ${SRC_DIR}/*.cpp ${SRC_DIR}/*.cc)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${SRC_DIR}/*.h ${SRC_DIR}/*.hpp ${SRC_DIR}/*.hh)

# Adding the source, header and resource files to the target compilation
set(RESOURCE_FILES)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCE_FILES})

# Linking necessary libraries
target_link_libraries(
  ${PROJECT_NAME}
  Qt5::Widgets Qt5::Gui Qt5::Network Qt5::Core Qt5::Concurrent
  protobuf Eigen3::Eigen
)

# Adding `src` directory to the include path
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

# Ignore warnings from 3rdparty libraries
target_compile_options(${PROJECT_NAME} PRIVATE "-isystem" "${SRC_DIR}/3rdparty")
target_compile_options(${PROJECT_NAME} PRIVATE "-w" "-Wno-everything")