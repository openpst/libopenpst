#-------------------------------------------------
# PLACEHOLDER
#-------------------------------------------------

CONFIG += C++11 staticlib

TARGET = libopenpst

TEMPLATE = lib

INCLUDEPATH += $$PWD/src $$PWD/lib/serial/include

DEPENDPATH += $$PWD/

VPATH += $$PWD/

HEADERS  += \
    src/include/definitions.h \
    src/include/win_inttypes.h \
    src/include/win_stdint.h \
    src/qualcomm/dm.h \
    src/qualcomm/dm_nv.h \
    src/qualcomm/dm_efs.h \
    src/qualcomm/dm_efs_manager.h \
    src/qualcomm/dm_efs_node.h \
    src/qualcomm/dm_nv.h \
    src/qualcomm/dload.h \
    src/qualcomm/hdlc.h \
    src/qualcomm/hdlc_encoder.h \
    src/qualcomm/mbn.h \
    src/qualcomm/qcdm_nv_responses.h \
    src/qualcomm/qcdm_packet_types.h \
    src/qualcomm/sahara.h \
    src/qualcomm/streaming_dload.h \
    src/qualcomm/hdlc_serial.h \
    src/qualcomm/qcdm_serial.h \
    src/qualcomm/sahara_serial.h \
    src/qualcomm/streaming_dload_serial.h \
    src/samsung/akseed_calculator.h \
    src/util/convert.h \
    src/util/endian.h \
    src/util/hexdump.h \
    src/util/sleep.h 

SOURCES += \
    src/qualcomm/dm_efs_manager.cpp \
    src/qualcomm/dm_efs_node.cpp \
    src/qualcomm/hdlc.cpp \
    src/qualcomm/hdlc_encoder.cpp \
    src/qualcomm/hdlc_serial.cpp \
    src/qualcomm/qcdm_serial.cpp \
    src/qualcomm/sahara_serial.cpp \
    src/qualcomm/streaming_dload_serial.cpp \
    src/samsung/akseed_calculator.cpp \
    src/util/convert.cpp \
    src/util/endian.cpp \
    src/util/hexdump.cpp \
    src/util/sleep.cpp 

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/release/ -lserial
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/debug/ -lserial
else:unix: LIBS += -L$$OUT_PWD/ -lserial