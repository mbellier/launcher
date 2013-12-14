QT += core gui widgets printsupport


# Add more folders to ship with the application, here
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    statistics.cpp \
    database.cpp \
    link.cpp \
    element.cpp \
    statistics_dialog.cpp \
    qcustomplot.cpp \
    launcher.cpp \
    settings.cpp \
    launchbutton.cpp

HEADERS += \
    statistics.h \
    database.h \
    link.h \
    element.h \
    statistics_dialog.h \
    qcustomplot.h \
    launcher.h \
    settings.h \
    launchbutton.hpp

RESOURCES += \
    txt.qrc

OTHER_FILES += \
    launcher.rc \
    launcher.ico

RC_FILE = launcher.rc

FORMS += \
    statistics_dialog.ui \
    settings.ui
