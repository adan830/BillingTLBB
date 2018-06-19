set(LIBS_REQUIRED ${LIBS_REQUIRED}
  dl
  pthread
  )

# OS specify
if (MSYS OR WIN32 OR MINGW OR CYGWIN)
  find_library(LIB_WS2_32 NAMES ws2_32)
  if (NOT LIB_WS2_32)
    message(FATAL_ERROR "Lib ws2_32 not found")
  endif()
  message(STATUS "Found winsock2_32 ${LIB_WS2_32}")
  set(LIBS_REQUIRED ${LIBS_REQUIRED}
    ${LIB_WS2_32}
    )
endif()

link_directories(${LIBS_LIBRARIES})

