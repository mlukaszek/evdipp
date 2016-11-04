set(libevdpp_SOURCES
  buffer.cpp
  evdi.cpp
  screen.cpp
)
add_library(evdipp STATIC ${libevdpp_SOURCES})

set(example_SOURCES
  driver.cpp
  example.cpp
)
add_executable(example ${example_SOURCES})

find_library(LIBEV_LIBRARY NAMES ev)
find_path(LIBEV_INCLUDE_DIR ev++.h PATH_SUFFIXES include/ev include) # Find header
find_package_handle_standard_args(libev DEFAULT_MSG LIBEV_LIBRARY LIBEV_INCLUDE_DIR)

find_package(Boost COMPONENTS system filesystem REQUIRED)

target_link_libraries(example
  evdipp
  libevdi
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${LIBEV_LIBRARY}
)

install(TARGETS evdipp example
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)
install(FILES buffer.hpp screen.hpp evdi.hpp
  DESTINATION include/libevdipp
)
