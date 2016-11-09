set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(monitorsim_SOURCES
  monitorsim.cpp
  qevdiscreen.cpp
  screenpreview.cpp
)

add_executable(monitorsim ${monitorsim_SOURCES})
target_link_libraries(monitorsim Qt5::Widgets evdipp libevdi)

install(TARGETS monitorsim RUNTIME DESTINATION bin)
