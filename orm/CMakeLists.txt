find_package(PostgreSQL REQUIRED)

set(ORM_SRC
        db.cpp
        modelregistry.cpp
        columntype.cpp
        miscrequest.cpp
)

add_library(orm STATIC ${ORM_SRC})

target_include_directories(orm PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PostgreSQL_INCLUDE_DIRS})

target_link_libraries(orm PUBLIC
        Soci::postgresql_static
        Soci::core_static
        PostgreSQL::PostgreSQL)