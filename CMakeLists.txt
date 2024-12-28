cmake_minimum_required(VERSION 3.10)
project(my_cpp_project)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check for JAVA_HOME
if(NOT DEFINED ENV{JAVA_HOME})
    message(FATAL_ERROR "JAVA_HOME environment variable is not set")
endif()

# Add MinHook subdirectory
add_subdirectory(external/minhook)

# Build shared library instead of executable
add_library(${PROJECT_NAME} SHARED
    src/main.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        "$ENV{JAVA_HOME}/include"
        "$ENV{JAVA_HOME}/include/win32"
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/minhook/include
)

# Define symbol export macro
target_compile_definitions(${PROJECT_NAME} PRIVATE MY_DLL_EXPORTS)

# Link against MinHook
target_link_libraries(${PROJECT_NAME} PRIVATE minhook) 