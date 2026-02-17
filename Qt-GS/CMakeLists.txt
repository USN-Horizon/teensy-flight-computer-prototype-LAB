cmake_minimum_required(VERSION 3.19)

#set(CMAKE_CXX_COMPILER "C:/msys64/mingw_64/bin")
project(Qt-GS LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Widgets
    SerialPort
)

qt_standard_project_setup()

qt_add_executable(Qt-GS
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    ui/mainwindow.ui
)

target_link_libraries(Qt-GS
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::SerialPort
)

include(GNUInstallDirs)

install(TARGETS Qt-GS
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Qt-GS
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})