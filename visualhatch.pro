# Add more folders to ship with the application, here
folder_01.source = qml/visualhatch
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    hatchmessenger.cpp \
    messagereader.cpp \
    ShipData.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    hatchmessenger.h \
    common.h \
    messagereader.h \
    ShipData.h


Qt += network quick

CONFIG += c++11

OTHER_FILES += \
    qml/visualhatch/LoginBox.qml
