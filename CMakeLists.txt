cmake_minimum_required(VERSION 3.0.0)
project(btvccntrl VERSION 1.0)

include(CTest)
enable_testing()

add_executable(btvccntrl main.cpp lib/botvac-controller.hpp lib/botvac-controller.cpp lib/pathfinder.hpp lib/pathfinder.cpp lib/visualisation.hpp lib/visualisation.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(CURL)
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
pkg_check_modules(SERIAL libserial)
pkg_check_modules(CURLPP curlpp)
include_directories(${CURL_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS} ${SERIAL_INCLUDE_DIRS} ${CURLPP_INCLUDE_DIRS})
target_link_libraries(btvccntrl ${CURL_LIBRARIES} ${GTKMM_LIBRARIES} ${SERIAL_LDFLAGS} ${CURLPP_LIBRARIES})