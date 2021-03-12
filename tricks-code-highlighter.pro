QT += core gui widgets
CONFIG += c++17
TARGET = tricks-code-highlighter

INCLUDEPATH += /usr/include/KF5/KSyntaxHighlighting
LIBS += -lKF5SyntaxHighlighting

SOURCES += \
    codeeditor.cpp \
    editordialog.cpp \
    main.cpp \
    highlighter.cpp

HEADERS += \
    codeeditor.h \
    editordialog.h \
    highlighter.h \
    linenumberarea.h


frames.path = /usr/share/tricks-code-highlighter
frames.files = frames

target.path = /usr/bin
INSTALLS += target frames

FORMS += \
    editordialog.ui
