project(ArrowPopupWidget LANGUAGES CXX)

set(SOURCES
    main.cpp
    arrowpopupwidget.h
    arrowpopupwidget.cpp
)

add_executable(ArrowPopupWidget ${SOURCES})

target_link_libraries(ArrowPopupWidget
    Qt5::Core 
    Qt5::Gui 
    Qt5::Widgets
)