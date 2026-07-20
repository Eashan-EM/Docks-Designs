cmake_minimum_required(VERSION 3.30.6)
project(Gladiator_Glance)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)
pkg_check_modules(GTK_LAYER_SHELL REQUIRED IMPORTED_TARGET gtk4-layer-shell-0)

set(CMAKE_CXX_STANDARD 23)
add_executable(glance main.cpp include/window.cpp include/components.cpp include/chrono.cpp)
target_include_directories(glance PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/headers")
target_link_libraries(glance PRIVATE PkgConfig::GTKMM PkgConfig::GTK_LAYER_SHELL)