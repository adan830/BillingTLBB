set(LIBS_LIBRARIES
  )

set(LIBS_REQUIRED ${LIBS_REQUIRED}
  dl
  pthread
  )

# OS specify
if (MSYS OR WIN32 OR MINGW OR CYGWIN)
  set(LIBS_REQUIRED ${LIBS_REQUIRED}
    -lws2_32
    )
endif()

link_directories(${LIBS_LIBRARIES})

