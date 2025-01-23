set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (WIN32)
    set(Qt6_DIR "C:/Qt/6.7.3/msvc2022_64/lib/cmake/Qt6")
endif ()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(GUI_SOURCES
        mainwindow.cpp
        datamanager.cpp
        form.h
        utils.cpp
)

add_library(gui STATIC ${GUI_SOURCES})

target_include_directories(gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gui PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        models
)
