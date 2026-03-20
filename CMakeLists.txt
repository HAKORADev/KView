cmake_minimum_required(VERSION 3.16)

project(kview VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Suppress Qt QML policy warnings
set(QT_POLICY_QTP0001 NEW)

set(QT_MIN_VERSION "6.6.0")
set(KF_MIN_VERSION "6.0.0")

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
    Multimedia
    Pdf
    PdfQuick
)

find_package(KF6CoreAddons ${KF_MIN_VERSION} REQUIRED)
find_package(KF6I18n ${KF_MIN_VERSION} REQUIRED)
find_package(KF6KIO ${KF_MIN_VERSION} REQUIRED)
find_package(KF6Kirigami ${KF_MIN_VERSION} REQUIRED)
find_package(KF6Archive ${KF_MIN_VERSION} REQUIRED)
find_package(KF6FileMetaData ${KF_MIN_VERSION} REQUIRED)

add_executable(kview
    src/main.cpp
    src/FileHandler.cpp
)

target_link_libraries(kview
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Multimedia
    Qt6::Pdf
    Qt6::PdfQuick
    KF6::CoreAddons
    KF6::I18n
    KF6::KIOWidgets
    KF6::Kirigami
    KF6::Archive
    KF6::FileMetaData
)

# QML Resources
qt6_add_qml_module(kview
    URI KView
    VERSION 1.0
    QML_FILES
        src/Main.qml
)

install(TARGETS kview DESTINATION bin)
