# Gen compile_commands.json
if (${CMAKE_EXPORT_COMPILE_COMMANDS} STREQUAL ON)
  find_program(COMPDB compdb)
  if (COMPDB)
    add_custom_target(compile_commands
      SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
      )
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
      COMMAND
      ${COMPDB} -p ${CMAKE_CURRENT_BINARY_DIR}/ list > ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
      )
  else()
    message(AUTHOR_WARNING "'compdb' not found")
  endif()
endif()

# Copy res
add_custom_command(TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E
  copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/deps/res/ $<TARGET_FILE_DIR:${PROJECT_NAME}>
  )

