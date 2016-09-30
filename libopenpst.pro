#-------------------------------------------------
# PLACEHOLDER
#-------------------------------------------------

CONFIG += C++11

TARGET = libopenpst

TEMPLATE = lib

INCLUDEPATH += $$PWD/include $$PWD/lib/serial/include

DEPENDPATH += $$PWD/

VPATH += $$PWD/

SOURCES += \
    $$PWD/lib/serial/src/serial.cc \
    $$PWD/lib/serial/src/impl/unix.cc \
    $$PWD/lib/serial/src/impl/list_ports/list_ports_linux.cc \
    $$PWD/src/android/adb_device_manager.cpp \
    $$PWD/src/lg/laf_serial.cpp \
    $$PWD/src/qualcomm/hdlc.cpp \
    $$PWD/src/qualcomm/hdlc_encoder.cpp \
    $$PWD/src/qualcomm/dm_efs_manager.cpp \
    $$PWD/src/qualcomm/dm_efs_node.cpp \
    $$PWD/src/qualcomm/hdlc_serial.cpp \
    $$PWD/src/qualcomm/qcdm_serial.cpp \
    $$PWD/src/qualcomm/sahara_serial.cpp \
    $$PWD/src/qualcomm/streaming_dload_serial.cpp \
    $$PWD/src/samsung/akseed_calculator.cpp \
    $$PWD/src/serial/generic_serial.cpp \
    $$PWD/src/util/convert.cpp \
    $$PWD/src/util/endian.cpp \
    $$PWD/src/util/hexdump.cpp \
    $$PWD/src/util/sleep.cpp
