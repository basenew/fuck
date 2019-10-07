#-------------------------------------------------
#
# Project created by QtCreator 2019-05-15T11:09:40
#
#-------------------------------------------------

QT       -= core gui

TARGET = UBTTaskMgr
TEMPLATE = lib

DEFINES += UBTTASKMGR_LIBRARY

CONFIG += c++11
CONFIG+=warn_off

INCLUDEPATH += \
    $$PWD/../../../Common \
    $$PWD/../../Common \
    $$PWD/../Common \
    $$PWD/../../../../trd \
    $$PWD/../../../../trd/gsoap/xml-rpc-json

SOURCES += ubttaskmgr.cpp \
    ubttask.cpp \
    ubtstatemachine.cpp \
    ubtactiontask.cpp \
    ubtvstask.cpp \
    ubtrecotask.cpp \
    ubtmsgmapping.cpp \
    ubttaskfactory.cpp \
    ubtjsonparser.cpp \
    ubtttstask.cpp \
    ubtmusictask.cpp

HEADERS += ubttaskmgr.h\
        ubttaskmgr_global.h \
    ubttask.h \
    ubtstatemachine.h \
    ubtactiontask.h \
    ubtvstask.h \
    ubtmsg.h \
    ubtrecotask.h \
    ubtmsgmapping.h \
    ubttaskfactory.h \
    ubttasktype.h \
    ubtjsonparser.h \
    ubtttstask.h \
    ubtmusictask.h

if(contains(DEFINES,IMX6ULL)){
    unix {
        target.path = $$MYDEPLOYDIR
        INSTALLS += target

        QMAKE_RPATHDIR += :\'\$\$ORIGING\'
        QMAKE_RPATHDIR += :./libavs
    }
}else{
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../UBTCore/release/ -lUBTCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../UBTCore/debug/ -lUBTCore
else:unix: LIBS += -L$$OUT_PWD/../../../UBTCore/ -lUBTCore

INCLUDEPATH += $$PWD/../../../UBTCore
DEPENDPATH += $$PWD/../../../UBTCore

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../zxLib/release/ -lzxLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../zxLib/debug/ -lzxLib
else:unix: LIBS += -L$$OUT_PWD/../../../zxLib/ -lzxLib

INCLUDEPATH += $$PWD/../../../zxLib
DEPENDPATH += $$PWD/../../../zxLib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../UBTUtil/release/ -lUBTUtil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../UBTUtil/debug/ -lUBTUtil
else:unix: LIBS += -L$$OUT_PWD/../../../UBTUtil/ -lUBTUtil

INCLUDEPATH += $$PWD/../../../UBTUtil
DEPENDPATH += $$PWD/../../../UBTUtil

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../Alpha1E_SDK/release/ -lAlpha1E_SDK
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../Alpha1E_SDK/debug/ -lAlpha1E_SDK
else:unix: LIBS += -L$$OUT_PWD/../../../Alpha1E_SDK/ -lAlpha1E_SDK

INCLUDEPATH += $$PWD/../../../Alpha1E_SDK
DEPENDPATH += $$PWD/../../../Alpha1E_SDK

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../UBTData/release/ -lUBTData
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../UBTData/debug/ -lUBTData
else:unix: LIBS += -L$$OUT_PWD/../../../UBTData/ -lUBTData

INCLUDEPATH += $$PWD/../../../UBTData
DEPENDPATH += $$PWD/../../../UBTData

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../trd/libavs/lib/release/ -lAVSCommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../trd/libavs/lib/debug/ -lAVSCommon
else:unix: LIBS += -L$$PWD/../../../../trd/libavs/lib/ -lAVSCommon

INCLUDEPATH += $$PWD/../../../../trd/libavs/include
DEPENDPATH += $$PWD/../../../../trd/libavs/include

INCLUDEPATH += $$PWD/../../../../trd/libavs/thirdparty/rapidjson-1.1.0/include


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTEventTrackingModule/release/ -lUBTEventTrackingModule
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTEventTrackingModule/debug/ -lUBTEventTrackingModule
else:unix: LIBS += -L$$OUT_PWD/../UBTEventTrackingModule/ -lUBTEventTrackingModule

INCLUDEPATH += $$PWD/../UBTEventTrackingModule
DEPENDPATH += $$PWD/../UBTEventTrackingModule


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTMscModule/release/ -lUBTMscModule
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTMscModule/debug/ -lUBTMscModule
else:unix: LIBS += -L$$OUT_PWD/../UBTMscModule/ -lUBTMscModule

INCLUDEPATH += $$PWD/../UBTMscModule
DEPENDPATH += $$PWD/../UBTMscModule

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../UBTConfigModule/release/ -lUBTConfigModule
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../UBTConfigModule/debug/ -lUBTConfigModule
else:unix: LIBS += -L$$OUT_PWD/../UBTConfigModule/ -lUBTConfigModule

INCLUDEPATH += $$PWD/../UBTConfigModule
DEPENDPATH += $$PWD/../UBTConfigModule
#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}
