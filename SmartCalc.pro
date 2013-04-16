#-------------------------------------------------
#
# Project created by QtCreator 2013-03-18T15:16:38
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = SmartCalc
TEMPLATE = app

INCLUDEPATH += include

SOURCES += src/main.cpp \
		src/mainwindow.cpp \
		src/helpdialog.cpp \
		src/parser.cpp \
		src/log.cpp \
    src/glwindow.cpp \
    src/grafic_window.cpp

HEADERS  += include/mainwindow.h \
			include/helpdialog.h \
			include/math/math_func.h \
			include/log.h \
			include/parser.h \
    include/glwindow.h \
    include/grafic_window.h

FORMS    += ui/mainwindow.ui \
    ui/helpdialog.ui \
    ui/grafic_window.ui
