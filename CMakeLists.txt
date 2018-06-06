cmake_minimum_required(VERSION 2.8)

project("Billing")

# Declare variables {
set(INC_DIRS)
set(SRC_FILES)
set(LIBS_REQUIRED)
# }

# Load Modules {
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/cmake_modules")
include(IncludeDirectories)
include(SourceFiles)
include(CompilerConfig)
include(BuildConfig)
include(LinkLibraries)
# }

include_directories(${PROJECT_NAME} PRIVATE ${INC_DIRS})

# add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
# add_library(${PROJECT_NAME} ${SRC_FILES})
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBS_REQUIRED})

# PostBuild {
include(PostBuild)
# }

