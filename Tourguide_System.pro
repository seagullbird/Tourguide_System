#-------------------------------------------------
#
# Project created by QtCreator 2016-04-28T19:42:10
#
#-------------------------------------------------

QT       += core gui

LIBS += -lsqlite3

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tourguide_System
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    change_pass.cpp \
    clock_thread.cpp \
    database.cpp \
    history.cpp \
    searcher.cpp \
    sign_up.cpp \
    small_funs.cpp \
    user.cpp \
    welcome.cpp \
    sqlite3.c

HEADERS  += mainwindow.h \
    change_pass.h \
    clock_thread.h \
    database.h \
    history.h \
    searcher.h \
    sign_up.h \
    small_funs.h \
    sqlite3.h \
    user.h \
    welcome.h

FORMS    += mainwindow.ui \
    change_pass.ui \
    sign_up.ui \
    welcome.ui
