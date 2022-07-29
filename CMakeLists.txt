cmake_minimum_required(VERSION 3.0.0)
project(btvccntrl VERSION 1.0)

include(CTest)
enable_testing()

add_executable(btvccntrl main.cpp lib/botvac-controller.hpp lib/botvac-controller.cpp lib/pathfinder.hpp lib/pathfinder.cpp lib/visualisation.hpp lib/visualisation.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
pkg_check_modules(SERIAL libserial)
include_directories(${GTKMM_INCLUDE_DIRS} ${SERIAL_INCLUDE_DIRS})
target_link_libraries(btvccntrl ${GTKMM_LIBRARIES} ${SERIAL_LDFLAGS})