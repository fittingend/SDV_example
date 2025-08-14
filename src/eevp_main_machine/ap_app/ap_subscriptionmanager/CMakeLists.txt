CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

MESSAGE(STATUS "PARA_SDK : ${PARA_SDK}")

SET(INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
SET(GEN_MACHINE_DIR "${GEN_DIR}/gen/gen_ara/implementation/EEVP_Main_Machine")
SET(PARA_INC_DIR "${PARA_SDK}/include")

MESSAGE(STATUS "INSTALL_DIR : ${INSTALL_DIR}")
MESSAGE(STATUS "PARA_INC_DIR : ${PARA_INC_DIR}")

INCLUDE(${GEN_MACHINE_DIR}/cmake/ParaSdk.cmake)

SET(PARA_APP_NAME SubscriptionManager)
SET(PARA_APP_VERSION 1.0.0)
SET(PARA_APP_GEN_DIR "${GEN_MACHINE_DIR}/${PARA_APP_NAME}")
Para_App()

PROJECT(${PARA_APP_NAME}
    VERSION ${PARA_APP_VERSION}
    LANGUAGES CXX
)

INCLUDE_DIRECTORIES("./include")
INCLUDE_DIRECTORIES("./cloud/include")
INCLUDE_DIRECTORIES(${PARA_APP_GEN_DIR}/include)
INCLUDE_DIRECTORIES(${PARA_INC_DIR})

SET(CLOUD_SRC
    cloud/src/CloudClient.cpp
    cloud/src/CloudHelper.cpp
    cloud/src/rest_api.cpp
)

SET(SRC
    src/main.cpp
    src/OtaSubscriptionProxyImpl.cpp
    src/SubscriptionManagementSkeletonImpl.cpp
    src/SubscriptionManager.cpp
    ${CLOUD_SRC}
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC})

Para_Target(${PARA_APP_NAME}
    PARA_LIBS_PRIVATE core log exec com
    PARA_GEN_DIR ${PARA_APP_GEN_DIR}
    OUTPUT_NAME ${PARA_APP_NAME}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    PRIVATE
    event
    curl
    ${LIBS}
)

install(
    FILES cloud/files/config.json
    FILES cloud/files/subinfo.json
    DESTINATION /opt/para/SubscriptionManager/
)
