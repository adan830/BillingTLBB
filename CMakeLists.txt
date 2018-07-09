# Copyright (c) Dark.Hades
# https://github.com/HadesD/TLBB.Billing

cmake_minimum_required(VERSION 2.8)

project("Billing")

# Declare variables {
set(INC_DIRS)
set(SRC_FILES)
set(LIBS_LIBRARIES)
set(LIBS_REQUIRED)
# }

# Load Modules {
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake_modules/GenCompileCommands.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake_modules/CompilerConfig.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake_modules/BuildConfig.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake_modules/FindLibMySql.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake_modules/LinkLibraries.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake_modules/Definitions.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake_modules/IncludeDirectories.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake_modules/SourceFiles.cmake)
# }

include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRS})

# add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
# add_library(${PROJECT_NAME} ${SRC_FILES})
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBS_REQUIRED})

# PostBuild {
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake_modules/PostBuild.cmake)
# }

