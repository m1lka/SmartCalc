#-------------------------------------------------
#
# Project created by QtCreator 2013-03-18T15:16:38
#
#-------------------------------------------------

QT       += core gui

TARGET = SmartCalc
TEMPLATE = app

INCLUDEPATH += include

SOURCES += src/main.cpp \
		src/mainwindow.cpp \
		src/helpdialog.cpp \
		src/parser.cpp \
		src/log.cpp

HEADERS  += include/mainwindow.h \
			include/helpdialog.h \
			include/math/math_func.h \
			include/log.h \
			include/parser.h

FORMS    += ui/mainwindow.ui \
    ui/helpdialog.ui
