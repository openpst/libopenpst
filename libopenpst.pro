#-------------------------------------------------
# PLACEHOLDER
#-------------------------------------------------
lessThan(QT_MAJOR_VERSION, 5): error("At least Qt 5.0 is required")

CONFIG += C++11
CONFIG -= qt        # we do not need QT libs for this
TARGET = openpst
TEMPLATE = lib
VER_MAJ=1
VER_MIN=0
VER_PAT=0

equals(BASE_DIR, ""): BASE_DIR = $$PWD

contains(QT_ARCH, i386) {
    unix {
        equals(BUILD_DIR, ""): BUILD_DIR = $$BASE_DIR/build/linux/i386
    }
} contains(QT_ARCH, x86_64) { 
    unix {
        equals(BUILD_DIR, ""): BUILD_DIR = $$BASE_DIR/build/linux/x86_64
    }
} else {
    error("Arch $$QT_ARCH not supported")
}

equals(BASE_DIR, ""): error("Missing BASE_DIR")
equals(BUILD_DIR, ""): error("Missing BUILD_DIR")


INCLUDEPATH     += $$BASE_DIR/include $$BASE_DIR/lib/serial/include
DEPENDPATH      += $$BASE_DIR/
VPATH           += $$BASE_DIR/
OBJECTS_DIR     += $$BUILD_DIR/obj
MOC_DIR         += $$BUILD_DIR/moc
DESTDIR         += $$BUILD_DIR

message("$$TARGET ARC         $$QT_ARCH")
message("$$TARGET BASE_DIR    $$BASE_DIR")
message("$$TARGET BUILD_DIR   $$BUILD_DIR")
message("$$TARGET INCLUDEPATH $$INCLUDEPATH")
message("$$TARGET DEPENDPATH  $$DEPENDPATH")
message("$$TARGET VPATH       $$VPATH")
message("$$TARGET OBJECTS_DIR $$OBJECTS_DIR")
message("$$TARGET MOC_DIR     $$MOC_DIR")
message("$$TARGET DESTDIR     $$DESTDIR")


SOURCES += \
    $$BASE_DIR/lib/serial/src/serial.cc \
    $$BASE_DIR/lib/serial/src/impl/unix.cc \
    $$BASE_DIR/lib/serial/src/impl/list_ports/list_ports_linux.cc \
    #$$BASE_DIR/src/android/adb_device_manager.cpp \
    $$BASE_DIR/src/lg/laf_serial.cpp \
    $$BASE_DIR/src/qualcomm/hdlc_encoder.cpp \
    $$BASE_DIR/src/qualcomm/dm_efs_manager.cpp \
    $$BASE_DIR/src/qualcomm/dm_efs_node.cpp \
    $$BASE_DIR/src/qualcomm/hdlc_serial.cpp \
    $$BASE_DIR/src/qualcomm/qcdm_serial.cpp \
    $$BASE_DIR/src/qualcomm/sahara_serial.cpp \
    $$BASE_DIR/src/qualcomm/streaming_dload_serial.cpp \
    $$BASE_DIR/src/samsung/akseed_calculator.cpp \
    $$BASE_DIR/src/serial/generic_serial.cpp \
    $$BASE_DIR/src/util/convert.cpp \
    $$BASE_DIR/src/util/endian.cpp \
    $$BASE_DIR/src/util/hexdump.cpp \
    $$BASE_DIR/src/util/sleep.cpp
