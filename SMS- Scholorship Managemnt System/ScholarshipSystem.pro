QT += core gui widgets

CONFIG += c++17

TARGET = ScholarshipSystem
TEMPLATE = app

SOURCES += \
    main.cpp \
    models/Student.cpp \
    models/Scholarship.cpp \
    models/Application.cpp \
    services/FileManager.cpp \
    services/EligibilityService.cpp \
    ui/LoginWindow.cpp \
    ui/StudentDashboard.cpp \
    ui/AdminDashboard.cpp

HEADERS += \
    models/Student.h \
    models/Scholarship.h \
    models/Application.h \
    services/FileManager.h \
    services/EligibilityService.h \
    ui/LoginWindow.h \
    ui/StudentDashboard.h \
    ui/AdminDashboard.h

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
