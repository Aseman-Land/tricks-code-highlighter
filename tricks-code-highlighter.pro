QT += core gui widgets
CONFIG += c++17
TARGET = tricks-code-highlighter

exists($$[QT_INSTALL_HEADERS]/KF5SyntaxHighlighting/SyntaxHighlighter): {
    message(KF5SyntaxHighlighting founded)
    DEFINES += KSYNTAX_HIGHLIGHTER
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]/KF5SyntaxHighlighting
    android: {
        LIBS += -L$$[QT_INSTALL_LIBS] -lKF5SyntaxHighlighting_$${ANDROID_TARGET_ARCH}
    } else {
        LIBS += -L$$[QT_INSTALL_LIBS] -lKF5SyntaxHighlighting
    }
} else {
    exists(/usr/include/KF5/KSyntaxHighlighting/KSyntaxHighlighting) {
        message(KF5SyntaxHighlighting founded)
        DEFINES += KSYNTAX_HIGHLIGHTER
        INCLUDEPATH += /usr/include/KF5/KSyntaxHighlighting/KSyntaxHighlighting/

        LIBS += -lKF5SyntaxHighlighting
    } else {
        exists(/usr/include/KF5/KSyntaxHighlighting/SyntaxHighlighter) {
                DEFINES += KSYNTAX_HIGHLIGHTER
                INCLUDEPATH += /usr/include/KF5/KSyntaxHighlighting
                LIBS += -lKF5SyntaxHighlighting
        } else {
            message(Warning: KF5SyntaxHighlighting not found)
        }
    }
}


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
