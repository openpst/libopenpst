#-------------------------------------------------
# QMake Build Script for: openpst/libopenpst
#-------------------------------------------------

lessThan(QT_MAJOR_VERSION, 5): error("At least Qt 5.0 is required")

CONFIG    += C++11 debug_and_release build_all
CONFIG    -= qt        # we do not need QT libs for this
TARGET    = openpst
TEMPLATE  = lib
VER_MAJ   = 1
VER_MIN   = 0
VER_PAT   = 0

!defined(BUILD_SHARED) {
    CONFIG += staticlib
}

BASE_DIR = $$PWD

CONFIG(release, debug|release){
    RELEASE = release
} else {
    RELEASE = debug
}

BUILD_DIR = $$BASE_DIR/build/$$RELEASE/

CONFIG(debug, debug|release){
    DEFINES   += DEBUG
    DEFINES   += HEXDUMP_PORT_RX
    DEFINES   += HEXDUMP_PORT_TX
}

INCLUDEPATH  += $$BASE_DIR/include \
                $$BASE_DIR/lib/serial/include
DEPENDPATH   += $$BASE_DIR
VPATH        += $$BASE_DIR
OBJECTS_DIR  += $$BUILD_DIR/obj
MOC_DIR      += $$BUILD_DIR/moc
RCC_DIR      += $$BUILD_DIR/rcc
UI_DIR       += $$BUILD_DIR/ui
DESTDIR      += $$BUILD_DIR

message("------------ $$TARGET ------------ !!!")
message("ARC         $$QT_ARCH")
message("BASE_DIR    $$BASE_DIR")
message("BUILD_DIR   $$BUILD_DIR")
message("INCLUDEPATH $$INCLUDEPATH")
message("DEPENDPATH  $$DEPENDPATH")
message("VPATH       $$VPATH")
message("OBJECTS_DIR $$OBJECTS_DIR")
message("MOC_DIR     $$MOC_DIR")
message("RCC_DIR     $$RCC_DIR")
message("UI_DIR      $$UI_DIR")
message("DESTDIR     $$DESTDIR")
message("------------ $$TARGET ------------ ")

SOURCES += \
    $$BASE_DIR/lib/serial/src/serial.cc \
    $$BASE_DIR/lib/serial/src/impl/unix.cc \
    $$BASE_DIR/lib/serial/src/impl/list_ports/list_ports_linux.cc \
    #$$BASE_DIR/src/android/adb_device_manager.cpp \
    $$BASE_DIR/src/lg/laf_serial.cpp \
    $$BASE_DIR/src/qualcomm/mbn_reader.cpp \
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
