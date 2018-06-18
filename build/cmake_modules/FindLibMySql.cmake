# - Try to find MySQL / MySQL Embedded library
# Find the MySQL includes and client library
# This module defines
#  MYSQL_INCLUDE_DIR, where to find mysql.h
#  LIB_MYSQL_CLIENT, the libraries needed to use MySQL.
#  MYSQL_LIB_DIR, path to the LIB_MYSQL_CLIENT
#  MYSQL_EMBEDDED_LIBRARIES, the libraries needed to use MySQL Embedded.
#  MYSQL_EMBEDDED_LIB_DIR, path to the MYSQL_EMBEDDED_LIBRARIES
#  MYSQL_FOUND, If false, do not try to use MySQL.
#  MYSQL_EMBEDDED_FOUND, If false, do not try to use MySQL Embedded.

# Copyright (c) 2006-2008, Jarosław Staniek <staniek@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

include(CheckCXXSourceCompiles)

if(WIN32)
  # set(CMAKE_FIND_LIBRARY_SUFFIXES .lib ${CMAKE_FIND_LIBRARY_SUFFIXES})
  find_path(MYSQL_INCLUDE_DIR mysql.h
    PATHS
    $ENV{MYSQL_INCLUDE_DIR}
    $ENV{MYSQL_DIR}/include
    $ENV{ProgramFiles}/MySQL/*/include
    $ENV{SystemDrive}/MySQL/*/include
    $ENV{ProgramW6432}/MySQL/*/include
    )
elseif(MINGW OR MSYS)
  # set(CMAKE_FIND_LIBRARY_SUFFIXES .lib ${CMAKE_FIND_LIBRARY_SUFFIXES})
else()
  # set(CMAKE_FIND_LIBRARY_SUFFIXES .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
  find_path(MYSQL_INCLUDE_DIR mysql.h
    PATHS
    $ENV{MYSQL_INCLUDE_DIR}
    $ENV{MYSQL_DIR}/include
    /usr/local/mysql/include
    /opt/mysql/mysql/include
    PATH_SUFFIXES
    mysql
    )
endif()

if(WIN32)
  if (${CMAKE_BUILD_TYPE})
    string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)
  endif()

  # path suffix for debug/release mode
  # binary_dist: mysql binary distribution
  # build_dist: custom build
  if(CMAKE_BUILD_TYPE_TOLOWER MATCHES "debug")
    set(binary_dist debug)
    set(build_dist Debug)
  else(CMAKE_BUILD_TYPE_TOLOWER MATCHES "debug")
    ADD_DEFINITIONS(-DDBUG_OFF)
    set(binary_dist opt)
    set(build_dist Release)
  endif(CMAKE_BUILD_TYPE_TOLOWER MATCHES "debug")

  #   find_library(LIB_MYSQL_CLIENT NAMES mysqlclient
  set(MYSQL_LIB_PATHS
    $ENV{MYSQL_DIR}/lib/${binary_dist}
    $ENV{MYSQL_DIR}/libmysql/${build_dist}
    $ENV{MYSQL_DIR}/client/${build_dist}
    $ENV{ProgramFiles}/MySQL/*/lib/${binary_dist}
    $ENV{SystemDrive}/MySQL/*/lib/${binary_dist}
    $ENV{MYSQL_DIR}/lib/opt
    $ENV{MYSQL_DIR}/client/release
    $ENV{ProgramFiles}/MySQL/*/lib/opt
    $ENV{SystemDrive}/MySQL/*/lib/opt
    $ENV{ProgramW6432}/MySQL/*/lib
    )
  find_library(LIB_MYSQL_CLIENT NAMES libmysql
    PATHS
    ${MYSQL_LIB_PATHS}
    )
elseif(MINGW OR MSYS)
else(WIN32)
  # find_library(LIB_MYSQL_CLIENT NAMES mysqlclient
  set(MYSQL_LIB_PATHS
    $ENV{MYSQL_DIR}/libmysql_r/.libs
    $ENV{MYSQL_DIR}/lib
    $ENV{MYSQL_DIR}/lib/mysql
    /usr/local/mysql/lib
    /opt/mysql/mysql/lib
    $ENV{MYSQL_DIR}/libmysql_r/.libs
    $ENV{MYSQL_DIR}/lib
    $ENV{MYSQL_DIR}/lib/mysql
    /usr/local/mysql/lib
    /opt/mysql/mysql/lib
    PATH_SUFFIXES
    mysql
    )
  find_library(LIB_MYSQL_CLIENT NAMES mysqlclient
    PATHS
    ${MYSQL_LIB_PATHS}
    )
endif(WIN32)

# Copyright (c) Dark.Hades
# Find in this project
if (NOT MYSQL_INCLUDE_DIR)
  find_path(MYSQL_INCLUDE_DIR mysql.h
    PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/*
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/*/include
    )
endif()
if (NOT LIB_MYSQL_CLIENT)
  set(MYSQL_LIB_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/deps/*/lib
    )
  find_library(LIB_MYSQL_CLIENT NAMES mysqlclient
    PATHS
    ${MYSQL_LIB_PATHS}
    )
endif()
# Download lib
if (NOT MYSQL_INCLUDE_DIR OR NOT LIB_MYSQL_CLIENT)
  set(LIBMYSQL_DOWNLOAD_LINK)
  set(LIBMYSQL_DOWNLOAD_SAVE_FILE)
  if (WIN32)
  elseif (MINGW OR MSYS)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(MYSQL_DOWNLOAD_LINK
        https://downloads.mysql.com/archives/get/file/mysql-connector-c-6.1.11-winx64.zip
        )
    else()
      set(MYSQL_DOWNLOAD_LINK
        https://downloads.mysql.com/archives/get/file/mysql-connector-c-6.1.11-win32.zip
        )
    endif()
    set(LIBMYSQL_DOWNLOAD_SAVE_FILE
      /tmp/libmysqlclient.zip
      )
  else()
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(MYSQL_DOWNLOAD_LINK
        https://dev.mysql.com/get/Downloads/Connector-C/mysql-connector-c-6.1.11-linux-glibc2.12-x86_64.tar.gz
        )
    else()
      set(MYSQL_DOWNLOAD_LINK
        https://downloads.mysql.com/archives/get/file/mysql-connector-c-6.1.11-linux-glibc2.12-i686.tar.gz
        )
    endif()
    set(LIBMYSQL_DOWNLOAD_SAVE_FILE
      /tmp/libmysqlclient.tar.gz
      )
  endif()
  if (MYSQL_DOWNLOAD_LINK)
    file(DOWNLOAD
      ${MYSQL_DOWNLOAD_LINK}
      ${LIBMYSQL_DOWNLOAD_SAVE_FILE}
      SHOW_PROGRESS
      )
    if(EXISTS ${LIBMYSQL_DOWNLOAD_SAVE_FILE})
      message(STATUS "Extracting libmysqlclient to ${CMAKE_CURRENT_SOURCE_DIR}/deps")
      execute_process(COMMAND ${CMAKE_COMMAND} -E
        tar xvf ${LIBMYSQL_DOWNLOAD_SAVE_FILE} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps
        )
    endif()
  endif()
  # Find in this project Last time
  if (NOT MYSQL_INCLUDE_DIR)
    find_path(MYSQL_INCLUDE_DIR mysql.h
      PATHS
      ${CMAKE_CURRENT_SOURCE_DIR}/deps/*/include
      )
  endif()
  if (MYSQL_INCLUDE_DIR)
    message(STATUS "Found MYSQL_INCLUDE_DIR ${MYSQL_INCLUDE_DIR}")
  endif()
  if (NOT LIB_MYSQL_CLIENT)
    set(MYSQL_LIB_PATHS
      ${CMAKE_CURRENT_SOURCE_DIR}/deps/*/lib
      )
    if (WIN32)
    elseif (MINGW OR MSYS)
      message(STATUS "Finding libmysql")
      find_library(LIB_MYSQL_CLIENT NAMES libmysql
        PATHS
        ${MYSQL_LIB_PATHS}
        )
      message(STATUS "libmysql: ${LIB_MYSQL_CLIENT}")
    else()
      find_library(LIB_MYSQL_CLIENT NAMES mysqlclient
        PATHS
        ${MYSQL_LIB_PATHS}
        )
    endif()
  endif()
  if (NOT MYSQL_INCLUDE_DIR OR NOT LIB_MYSQL_CLIENT)
    message(FATAL_ERROR "Lib MySql Client not found")
  endif()
endif()
if (MYSQL_INCLUDE_DIR AND LIB_MYSQL_CLIENT)
  set(INC_DIRS ${INC_DIRS}
    ${MYSQL_INCLUDE_DIR}
    )
  set(LIBS_REQUIRED ${LIBS_REQUIRED}
    ${LIB_MYSQL_CLIENT}
    )
endif()
# End customize

find_library(MYSQL_EMBEDDED_LIBRARIES NAMES mysqld
  PATHS
  ${MYSQL_LIB_PATHS}
  )

if(LIB_MYSQL_CLIENT)
  get_filename_component(MYSQL_LIB_DIR ${LIB_MYSQL_CLIENT} PATH)
endif(LIB_MYSQL_CLIENT)

if(MYSQL_EMBEDDED_LIBRARIES)
  get_filename_component(MYSQL_EMBEDDED_LIB_DIR ${MYSQL_EMBEDDED_LIBRARIES} PATH)
endif(MYSQL_EMBEDDED_LIBRARIES)

set( CMAKE_REQUIRED_INCLUDES ${MYSQL_INCLUDE_DIR} )
set( CMAKE_REQUIRED_LIBRARIES ${MYSQL_EMBEDDED_LIBRARIES} )
check_cxx_source_compiles("#include <mysql.h>\nint main() { int i = MYSQL_OPT_USE_EMBEDDED_CONNECTION; }" HAVE_MYSQL_OPT_EMBEDDED_CONNECTION)

if(MYSQL_INCLUDE_DIR AND LIB_MYSQL_CLIENT)
  set(MYSQL_FOUND TRUE)
  message(STATUS "Found MySQL: ${MYSQL_INCLUDE_DIR}, ${LIB_MYSQL_CLIENT}")
else(MYSQL_INCLUDE_DIR AND LIB_MYSQL_CLIENT)
  set(MYSQL_FOUND FALSE)
  message(STATUS "MySQL not found.")
endif(MYSQL_INCLUDE_DIR AND LIB_MYSQL_CLIENT)

if(MYSQL_INCLUDE_DIR AND MYSQL_EMBEDDED_LIBRARIES AND HAVE_MYSQL_OPT_EMBEDDED_CONNECTION)
  set(MYSQL_EMBEDDED_FOUND TRUE)
  message(STATUS "Found MySQL Embedded: ${MYSQL_INCLUDE_DIR}, ${MYSQL_EMBEDDED_LIBRARIES}")
else(MYSQL_INCLUDE_DIR AND MYSQL_EMBEDDED_LIBRARIES AND HAVE_MYSQL_OPT_EMBEDDED_CONNECTION)
  set(MYSQL_EMBEDDED_FOUND FALSE)
  message(STATUS "MySQL Embedded not found.")
endif(MYSQL_INCLUDE_DIR AND MYSQL_EMBEDDED_LIBRARIES AND HAVE_MYSQL_OPT_EMBEDDED_CONNECTION)

mark_as_advanced(MYSQL_INCLUDE_DIR LIB_MYSQL_CLIENT MYSQL_EMBEDDED_LIBRARIES)

