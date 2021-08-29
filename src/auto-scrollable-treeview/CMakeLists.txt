project(AutoScrollableTreeView LANGUAGES CXX)

set(SOURCES
    main.cpp
    treeview.h
    treeview.cpp
)

add_executable(AutoScrollableTreeView ${SOURCES})

target_link_libraries(AutoScrollableTreeView
    Qt5::Core 
    Qt5::Gui 
    Qt5::Widgets
)