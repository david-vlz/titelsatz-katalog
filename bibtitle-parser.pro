TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    charbufferlib.cpp \
    categorylib.cpp \
    datasetlib.cpp \
    querylib.cpp \
    cataloguelib.cpp

HEADERS += \
    charbuffer.h \
    dataset.h \
    category.h \
    query.h \
    catalogue.h

