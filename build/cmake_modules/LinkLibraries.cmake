if (MSYS OR WIN32 OR MINGW OR CYGWIN)
  set(LIBS_REQUIRED ${LIBS_REQUIRED}
    -lws2_32
    -lwsock32
    )
else()
  set(LIBS_REQUIRED ${LIBS_REQUIRED}
    -lpthread
    -ldl
    )
endif()

