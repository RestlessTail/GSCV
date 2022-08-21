cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Release)
project(GSCV VERSION 0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH /home/shichengge/Qt/6.2.4/gcc_64/)
set(CMAKE_CXX_FLAGS_PROFILE "-Ibuild/GSCV_autogen/include")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES
    language/zh_CN.ts
    language/en_US.ts
)

set(RESOURCE_FILES
    shaders.qrc
)
QT6_ADD_RESOURCES(QRC_FILES, ${RESOURCE_FILES})

file(GLOB USER_SOURCES_FILES "*.cpp")
file(GLOB USER_HEADER_FILES "*.h")
file(GLOB USER_UI_FILES "*.ui")

set(PROJECT_SOURCES
        ${USER_SOURCES_FILES}
        ${USER_HEADER_FILES}
        ${USER_UI_FILES}
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GSCV
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GSCV APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(GSCV SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GSCV
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(GSCV PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
find_package(Qt6 REQUIRED COMPONENTS OpenGL)
target_link_libraries(GSCV PRIVATE Qt6::OpenGL)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)
target_link_libraries(GSCV PRIVATE Qt6::OpenGLWidgets)
#Linux users
target_link_libraries(GSCV PRIVATE z)
#Windows users
#target_link_libraries(GSCV PRIVATE ${CMAKE_SOURCE_DIR}/libs/libzlibstatic.a)

set_target_properties(GSCV PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GSCV)
endif()
