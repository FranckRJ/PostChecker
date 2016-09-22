TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    postNode.cpp \
    postNodeHandler.cpp \
    postStringLoader.cpp

HEADERS += \
    postNode.hpp \
    postNodeHandler.hpp \
    postStringLoader.hpp
