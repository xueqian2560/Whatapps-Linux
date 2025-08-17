cmake_minimum_required(VERSION 3.16)

project(Whatapps VERSION 0.1 LANGUAGES CXX)

# 确保使用完整安装路径
set(CMAKE_INSTALL_DATAROOTDIR "/usr/share")

# 强制使用 DEB 格式
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION TRUE)

# 必须设置的 DEB 包属性
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name <your.email@example.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5core5a, libqt5gui5, libqt5widgets5, libqt5webenginewidgets5")

# 可选但推荐的设置
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://example.com")
set(CPACK_DEBIAN_PACKAGE_SECTION "net")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 关键修改1：添加 WebEngineWidgets 模块
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets WebEngineWidgets)  # 增加 WebEngineWidgets
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets WebEngineWidgets)  # 增加 WebEngineWidgets

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Whatapps MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
    add_executable(Whatapps ${PROJECT_SOURCES})
endif()

# 关键修改2：链接 WebEngineWidgets 库
target_link_libraries(Whatapps PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets  # 必须添加
)

# 其他配置保持不变...
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Whatapps)
endif()

# 放在文件最后
include(CPack)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Whatapps.desktop
    DESTINATION share/applications  # 注意这里是相对路径
)
