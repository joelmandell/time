
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .

QT += xml phonon

CONFIG += CONSOLE

# Input
HEADERS += ttimer.h
SOURCES += main.cpp ttimer.cpp
TRANSLATIONS = lang/en_US.ts \
lang/fi_FI.ts
