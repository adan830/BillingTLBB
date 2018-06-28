if (MSYS OR WIN32 OR MINGW OR CYGWIN)
  set(LIBS_REQUIRED ${LIBS_REQUIRED}
    ws2_32
    wsock32
    )
else()
  set(LIBS_REQUIRED ${LIBS_REQUIRED}
    pthread
    dl
    )
endif()

