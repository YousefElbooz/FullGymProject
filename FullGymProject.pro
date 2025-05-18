QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    member.cpp \
    staff.cpp \
    coach.cpp \
    manger.cpp \
    receptionists.cpp \
    gymclass.cpp \
    filehandler.cpp \
    animations.cpp \
    HelperFunction.cpp \
    BookingLoader.cpp \
    BookingCourt.cpp \
    Court.cpp \
    CourtLoader.cpp \
    PaddleCourtWindow.cpp

HEADERS += \
    mainwindow.h \
    member.h \
    staff.h \
    coach.h \
    manger.h \
    receptionists.h \
    gymclass.h \
    filehandler.h \
    animations.h \
    HelperFunction.h \
    BookingLoader.h \
    BookingCourt.h \
    Court.h \
    CourtLoader.h \
    PaddleCourtWindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    bookings.txt \
    classes.txt \
    members.txt \
    padel_courts.txt \
    staffs.txt \
    waitlist.txt

RESOURCES += \
    resource.qrc