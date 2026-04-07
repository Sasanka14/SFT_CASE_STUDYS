cmake_minimum_required(VERSION 3.16)

project(ScholarshipSystem LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(ScholarshipSystem
    main.cpp
    models/Student.h
    models/Student.cpp
    models/Scholarship.h
    models/Scholarship.cpp
    models/Application.h
    models/Application.cpp
    services/FileManager.h
    services/FileManager.cpp
    services/EligibilityService.h
    services/EligibilityService.cpp
    ui/LoginWindow.h
    ui/LoginWindow.cpp
    ui/StudentDashboard.h
    ui/StudentDashboard.cpp
    ui/AdminDashboard.h
    ui/AdminDashboard.cpp
    ui/StudentRegistrationWindow.h
    ui/StudentRegistrationWindow.cpp
)

target_link_libraries(ScholarshipSystem
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
