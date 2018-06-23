# Gen compile_commands.json
if (${CMAKE_EXPORT_COMPILE_COMMANDS} STREQUAL ON)
  find_program(COMPDB compdb)
  if (COMPDB)
    execute_process(COMMAND ${CMAKE_COMMAND} -E
      remove ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
      )
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
if (WIN32)
  add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    remove $<TARGET_FILE_DIR:${PROJECT_NAME}>/*.sh
    )
else()
  add_custom_command(TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    remove $<TARGET_FILE_DIR:${PROJECT_NAME}>/*.cmd
    )
endif()
set(BILLING_EE_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/deps/ee/deps/res/Config.ini)
if (EXISTS ${BILLING_EE_CONFIG_FILE})
  file(READ ${BILLING_EE_CONFIG_FILE} BILLING_EE_CONFIG_CONTENT)
  string(REGEX REPLACE "\n" ";" BILLING_EE_CONFIG_CONTENT_LIST "${BILLING_EE_CONFIG_CONTENT}")
  foreach (LINE ${BILLING_EE_CONFIG_CONTENT_LIST})
    add_custom_command(TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E
      echo ${LINE} >> $<TARGET_FILE_DIR:${PROJECT_NAME}>/Config.ini
      )
  endforeach()
endif()

