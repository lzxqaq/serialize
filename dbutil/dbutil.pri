QT += sql xml
DEFINES += DBUTIL_LIBRARY

HEADERS += \
    $$PWD/dbutil.h \
    $$PWD/dbutil_global.h \
    $$PWD/dbutilconfig.h \
    $$PWD/sqlhandler.h

SOURCES += \
    $$PWD/dbutil.cpp \
    $$PWD/dbutilconfig.cpp \
    $$PWD/sqlhandler.cpp

RESOURCES += \
    $$PWD/dbutil.qrc

OTHER_FILES += \
    $$PWD/res/dbutil.json \
    $$PWD/res/sqls/mainwindow/log.xml \
    $$PWD/res/sqls/mainwindow/errorlog.xml \
    $$PWD/res/sqls/systree/tree.xml \



