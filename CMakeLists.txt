project(evdipp)
cmake_minimum_required(VERSION 2.8.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ExternalProject)
ExternalProject_Add(github_evdi
 GIT_REPOSITORY https://github.com/DisplayLink/evdi.git
 GIT_TAG main
 BUILD_IN_SOURCE 1
 CONFIGURE_COMMAND ""
 BUILD_COMMAND cd library && make
 INSTALL_COMMAND ""
)

ExternalProject_Get_Property(github_evdi SOURCE_DIR)
set(LIBEVDI_DIR ${SOURCE_DIR}/library)
install(FILES ${LIBEVDI_DIR}/libevdi.so DESTINATION lib RENAME libevdi.so.0)
install(FILES ${LIBEVDI_DIR}/evdi_lib.h DESTINATION include)

add_library(libevdi STATIC IMPORTED)
set_target_properties(libevdi PROPERTIES
  IMPORTED_LOCATION ${LIBEVDI_DIR}/libevdi.so
)
add_dependencies(libevdi github_evdi)

include_directories(${LIBEVDI_DIR})
add_subdirectory(libevdipp)

add_subdirectory(example)
add_subdirectory(monitorsim)
