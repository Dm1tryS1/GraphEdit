QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphEdit
TEMPLATE = app


SOURCES += main.cpp mainwindow.cpp edge.cpp graphwidget.cpp node.cpp \
    tablewindow.cpp

HEADERS  += mainwindow.h edge.h graphwidget.h node.h HelpBrowser.h node.h \
    tablewindow.h

FORMS    += mainwindow.ui \
    tablewindow.ui

RESOURCES += icons.qrc

RC_FILE = icons/icon.rc
