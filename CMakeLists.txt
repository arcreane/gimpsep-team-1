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
        GimpIsep.cpp
        GimpIsep.h
        MainFrame.cpp
        MainFrame.h
        EditorPanel.cpp
        EditorPanel.h
        StitchPanel.cpp
        StitchPanel.h
        MenuPanel.cpp
        MenuPanel.h
        MyMenuBar.cpp
        MyMenuBar.h
        Manipulation.cpp
        Manipulation.h
)

target_link_libraries(gimpsep_team_1 PRIVATE wx::core wx::base ${OpenCV_LIBS})

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS /SUBSYSTEM:WINDOWS)
