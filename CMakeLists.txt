cmake_minimum_required(VERSION 3.29)

project(rjmmo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    set(PostgreSQL_ROOT "C:/PostgreSQL/17")
    set(Qt6_DIR "C:/Qt/6.7.3/msvc2022_64/lib/cmake/Qt6")
endif ()

add_subdirectory(lib)
add_subdirectory(orm)
add_subdirectory(models)
add_subdirectory(gui)

add_executable(rjmmo main.cpp)

target_link_libraries(rjmmo gui)

