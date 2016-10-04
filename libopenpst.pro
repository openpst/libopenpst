#-------------------------------------------------
# PLACEHOLDER
#-------------------------------------------------

equals(LIBOPENPST_DIR, ""):	LIBOPENPST_DIR = $$PWD

CONFIG += C++11

TARGET = openpst

TEMPLATE = lib

INCLUDEPATH += $$LIBOPENPST_DIR/include $$LIBOPENPST_DIR/lib/serial/include

DEPENDPATH += $$LIBOPENPST_DIR/

VPATH += $$LIBOPENPST_DIR/

SOURCES += \
    $$LIBOPENPST_DIR/lib/serial/src/serial.cc \
    $$LIBOPENPST_DIR/lib/serial/src/impl/unix.cc \
    $$LIBOPENPST_DIR/lib/serial/src/impl/list_ports/list_ports_linux.cc \
    $$LIBOPENPST_DIR/src/android/adb_device_manager.cpp \
    $$LIBOPENPST_DIR/src/lg/laf_serial.cpp \
    $$LIBOPENPST_DIR/src/qualcomm/hdlc.cpp \
    $$LIBOPENPST_DIR/src/qualcomm/hdlc_encoder.cpp \
    $$LIBOPENPST_DIR/src/qualcomm/dm_efs_manager.cpp \
    $$LIBOPENPST_DIR/src/qualcomm/dm_efs_node.cpp \
    $$LIBOPENPST_DIR/src/qualcomm/hdlc_serial.cpp \
    $$LIBOPENPST_DIR/src/qualcomm/qcdm_serial.cpp \
    $$LIBOPENPST_DIR/src/qualcomm/sahara_serial.cpp \
    $$LIBOPENPST_DIR/src/qualcomm/streaming_dload_serial.cpp \
    $$LIBOPENPST_DIR/src/samsung/akseed_calculator.cpp \
    $$LIBOPENPST_DIR/src/serial/generic_serial.cpp \
    $$LIBOPENPST_DIR/src/util/convert.cpp \
    $$LIBOPENPST_DIR/src/util/endian.cpp \
    $$LIBOPENPST_DIR/src/util/hexdump.cpp \
    $$LIBOPENPST_DIR/src/util/sleep.cpp