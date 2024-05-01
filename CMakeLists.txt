cmake_minimum_required(VERSION 3.28)
project(gimpsep_team_1)

find_package(wxWidgets CONFIG REQUIRED)

if(wxWidgets_FOUND)
    message("WXW found")
endif()

find_package(OpenCV CONFIG REQUIRED)

if(OpenCV_FOUND)
    message("OpenCV found")
endif()

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(gimpsep_team_1 main.cpp
        Manipulation.cpp
        MainFrame.cpp
        Manipulation.h
        GimpFrame.cpp
        GimpFrame.h)

target_link_libraries(gimpsep_team_1 PRIVATE wx::core wx::base ${OpenCV_LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)

