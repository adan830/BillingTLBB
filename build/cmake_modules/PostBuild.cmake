# Gen compile_commands.json
if (${CMAKE_EXPORT_COMPILE_COMMANDS} STREQUAL ON)
  find_program(COMPDB compdb)
  if (COMPDB)
    add_custom_command(TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND
      ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
      )
    add_custom_command(TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND
      ${COMPDB} -p ${CMAKE_CURRENT_BINARY_DIR}/ list > ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
      )
  else()
    message(AUTHOR_WARNING "'compdb' not found")
  endif()
endif()

