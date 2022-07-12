project(RoundedShadowWidget LANGUAGES CXX)

set(SOURCES
    main.cpp
    shadowwidget.h
    shadowwidget.cpp
)

add_executable(RoundedShadowWidget ${SOURCES})

target_link_libraries(RoundedShadowWidget
    Qt5::Core 
    Qt5::Gui 
    Qt5::Widgets
)