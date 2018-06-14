add_definitions(-DASIO_STANDALONE -DASIO_HAS_STD_CHRONO)

# OS specify
if (MSYS OR WIN32 OR MINGW OR CYGWIN)
  add_definitions(-D__USE_W32_SOCKETS)
  add_definitions(-D_WIN32_WINNT=0x0501)
endif()

# Compile Mode
if (NOT (CMAKE_BUILD_TYPE STREQUAL Release))
  add_definitions(-DBILLING_DEBUG)
endif()
message(STATUS "Compiling in ${CMAKE_BUILD_TYPE} Mode")

