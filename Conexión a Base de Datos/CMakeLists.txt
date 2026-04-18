cmake_minimum_required(VERSION 3.16)
project(cpp_postgres_demo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Buscar libpqxx y PostgreSQL
find_package(PostgreSQL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

include_directories(
    include
    ${PostgreSQL_INCLUDE_DIRS}
    ${PQXX_INCLUDE_DIRS}
)

add_executable(demo
    src/main.cpp
    src/Database.cpp
    src/UserRepository.cpp
)

target_link_libraries(demo
    ${PostgreSQL_LIBRARIES}
    ${PQXX_LIBRARIES}
)

target_compile_options(demo PRIVATE ${PQXX_CFLAGS_OTHER})
