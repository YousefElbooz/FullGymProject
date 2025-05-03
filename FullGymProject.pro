QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    coach.cpp \
    filehandler.cpp \
    gymclass.cpp \
    main.cpp \
    mainwindow.cpp \
    manger.cpp \
    member.cpp \
    receptionists.cpp \
    staff.cpp

HEADERS += \
    coach.h \
    filehandler.h \
    gymclass.h \
    mainwindow.h \
    manger.h \
    member.h \
    receptionists.h \
    staff.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    classes.txt \
    images/108f39f4-48a7-476b-a645-39a3c04ad2cd.jpeg \
    images/267282ae-3300-45ae-82ce-d296517a826e.jpeg \
    images/486462912_18358780699183487_8907685704026824224_n.jpg \
    images/486525452_18358780720183487_8689780641527827007_n.jpg \
    images/486528368_18358780702183487_7876072232796722153_n.jpg \
    images/486558591_18358780675183487_8529278639721177876_n.jpg \
    images/486639959_18358780729183487_2395281786417293728_n.jpg \
    images/486761338_18358780648183487_518183920224791822_n.jpg \
    images/486781489_18458191648073759_5384791180746033223_n.jpg \
    images/486954623_18458191684073759_6188361622420108090_n.jpg \
    images/487106121_18358780660183487_738216244790693941_n.jpg \
    images/487241769_18358780711183487_4467916180267804207_n.jpg \
    images/487331601_18458191693073759_627849444980872012_n.jpg \
    images/487380404_18358780678183487_6712429437797112055_n.jpg \
    images/487396330_18358780606183487_3924480028177396507_n.jpg \
    images/487421743_18037488254535615_5469574134106876009_n.jpg \
    images/487794954_18037488260535615_7174140484398860324_n.jpg \
    images/487923262_18358780738183487_3064122802807129279_n.jpg \
    images/487931916_18458191666073759_5571215937298845863_n.jpg \
    images/5833ac0c066a34fc0ba67f3627c4a56d.jpg \
    images/93bd66f367106eaa00a77764796b1f77.jpg \
    images/ChatGPT Image Apr 15, 2025, 12_44_13 PM.png \
    images/Ever imagined what our Ayu Yoga instructors would….jpeg \
    images/Iconic 📸.jpeg \
    images/Maldini.jpg \
    images/MessiKiss.jpg \
    images/Mosalah.jpg \
    images/Padel Players - Matt Munday.jpeg \
    images/SheilYaTweelElOmer.jpg \
    images/Studio Ghibli aesthetics.jpeg \
    images/TailandKickBoxing.jpeg \
    images/exit.svg \
    images/fcf4f0ffe9b1e307a0b2f741bb6fcfc5.jpg \
    images/txt.txt \
    images/رونالدو.jpeg \
    images/🏋️_♂️Studio Ghibli, but make it gym edition….jpeg \
    members.txt \
    staffs.txt
