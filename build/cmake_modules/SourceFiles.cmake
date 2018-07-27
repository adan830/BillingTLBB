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
  if (BILLING_EE)
    add_definitions(-D__BILLING_ENTERPRISE_EDITION__)
    set(SRC_FILES ${SRC_FILES}
      deps/ee/src/net/packet/Routes.cpp
      deps/ee/src/net/packet/RoutesAntiClone.cpp
      )

    if (NOT BILLING_WITHOUT_ANTI_CLONE_IP)
      set(SRC_FILES ${SRC_FILES}
        deps/ee/src/database/models/IpAccounts.cpp
        )
    endif()
    if (NOT BILLING_WITHOUT_ANTI_CLONE_MAC)
      set(SRC_FILES ${SRC_FILES}
        deps/ee/src/database/models/MacAccounts.cpp
        )
    endif()
    if (NOT BILLING_WITHOUT_ANTI_CLONE_HARDWARE)
      set(SRC_FILES ${SRC_FILES}
        deps/ee/src/database/models/HardwareAccounts.cpp
        )
    endif()
  endif()
  add_definitions(-D__BILLING_WITH_AFTER_START__)
  set(SRC_FILES ${SRC_FILES}
    deps/ee/src/net/BillingSocket.cpp
    deps/ee/src/UtilsEE.cpp
    )
endif()

