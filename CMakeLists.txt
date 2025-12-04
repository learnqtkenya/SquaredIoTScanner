cmake_minimum_required(VERSION 3.16)

project(SquaredIoTScanner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME SquaredIotScanner)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(${TARGET_NAME}
    main.cpp
)

qt_add_qml_module(${TARGET_NAME}
    URI SquaredIoTScanner
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        src/androidbarcodescanner.h
        src/androidbarcodescanner.cpp
)

target_include_directories(${TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(${TARGET_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${TARGET_NAME}
    PRIVATE Qt6::Quick
)

if(ANDROID)
    set_target_properties(${TARGET_NAME} PROPERTIES
        QT_ANDROID_VERSION_CODE 1
        QT_ANDROID_VERSION_NAME ${PROJECT_VERSION}
        QT_ANDROID_ABIS "arm64-v8a;armeabi-v7a"
        QT_ANDROID_BUILD_ALL_ABIS OFF
        QT_ANDROID_MIN_SDK_VERSION 26
        QT_ANDROID_TARGET_SDK_VERSION 35
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android/"
    )
    include(FetchContent)
    FetchContent_Declare(
        android_openssl
        DOWNLOAD_EXTRACT_TIMESTAMP true
        URL https://github.com/KDAB/android_openssl/archive/refs/heads/master.zip
    )
    FetchContent_MakeAvailable(android_openssl)
    include(${android_openssl_SOURCE_DIR}/android_openssl.cmake)
    add_android_openssl_libraries(${TARGET_NAME})

    target_link_options(${TARGET_NAME} PRIVATE "-Wl,-z,max-page-size=16384")
endif()

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
