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

add_executable(gimpsep_team_1 main.cpp)

target_link_libraries(gimpsep_team_1 PRIVATE opencv_ml opencv_dnn opencv_core opencv_flann)
target_link_libraries(gimpsep_team_1 PRIVATE wx::core wx::base)

set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS /SUBSYSTEM:WINDOWS
)
