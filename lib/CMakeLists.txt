add_library(sqlite3 STATIC ${CMAKE_CURRENT_SOURCE_DIR}/sqlite/sqlite3.c)

target_include_directories(sqlite3 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
)

option(SOCI_SHARED "" OFF)
option(SOCI_STATIC "" ON)
option(WITH_BOOST "" OFF)
option(WITH_SQLITE3 "" ON)
option(WITH_POSTGRESQL "" ON)
option(WITH_MYSQL "" OFF)
option(WITH_DB2 "" OFF)
option(WITH_FIREBIRD "" OFF)
option(WITH_ODBC "" OFF)
option(WITH_ORACLE "" OFF)

set(SQLITE3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sqlite)
if (WIN32)
    set(SQLITE3_LIBRARY ${CMAKE_BINARY_DIR}/lib/sqlite3.lib)
elseif (APPLE)
    set(SQLITE3_LIBRARY ${CMAKE_BINARY_DIR}/lib/libsqlite3.a)
elseif (UNIX)
    set(SQLITE3_LIBRARY ${CMAKE_BINARY_DIR}/lib/libsqlite3.a)
endif ()

add_subdirectory(soci)
