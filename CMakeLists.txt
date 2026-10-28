cmake_minimum_required(VERSION 3.16)

project(DataBaseApp1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Этот блок сам определит, Qt5 у тебя или Qt6
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

add_executable(DataBaseApp1
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    database.h
)

target_link_libraries(DataBaseApp1 PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)
