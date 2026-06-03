cmake_minimum_required(VERSION 3.16)

project(TextEditor VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets LinguistTools)


find_path(QT_EASY_INCLUDE_DIR QtEasy/ PATHS /usr/local/include /usr/include)

find_library(QT_EASY_SHARED_LIB NAMES libQtEasy.so PATHS /usr/local/lib /usr/lib)


set(PROJECT_SOURCES 
    main.cpp 
)

add_executable(TextEditor ${PROJECT_SOURCES})

target_include_directories(TextEditor PRIVATE ${QT_EASY_INCLUDE_DIR})

target_link_libraries(TextEditor PRIVATE 
    Qt6::Core 
    Qt6::Widgets
    ${QT_EASY_SHARED_LIB}
)

qt_add_translations(TextEditor
    TS_FILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/TextEditor_en.ts
        ${CMAKE_CURRENT_SOURCE_DIR}/TextEditor_ru.ts
    TARGETS 
    TextEditor
)

install(TARGETS TextEditor
    RUNTIME DESTINATION bin
    COMPONENT TextEditor
)
