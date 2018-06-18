set(SRC_FILES ${SRC_FILES}
  src/main.cpp

  src/Config.cpp
  src/Billing.cpp
  src/Utils.cpp
  src/Log.cpp
  src/Startup.cpp
  src/net/BillingSocket.cpp
  src/net/Session.cpp
  src/net/Packet.cpp
  src/net/packet/HexData.cpp
  src/net/packet/Routes.cpp
  src/database/Connector.cpp
  src/database/Model.cpp
  src/database/models/Account.cpp
  )

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/deps/ee/src)
  add_definitions(-D__BILLING_ENTERPRISE_EDITION__)
  set(SRC_FILES ${SRC_FILES}
    deps/ee/src/net/packet/Routes.cpp
    )
endif()

