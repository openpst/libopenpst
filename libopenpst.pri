#-------------------------------------------------
# QMake Build Include for: openpst/libopenpst
#-------------------------------------------------

QMAKE_EXTRA_TARGETS += libopenpst
PRE_TARGETDEPS      += libopenpst
LIBS 				+= -L$$BUILD_DIR/lib -lopenpst

CONFIG(release, debug|release){
	libopenpst.commands = cd $$BASE_DIR/lib/libopenpst && make clean && make $$RELEASE && mkdir -p $$BUILD_DIR/lib && cp build/release/*.a $$BUILD_DIR/lib
} else {
	libopenpst.commands = cd $$BASE_DIR/lib/libopenpst && make clean && make $$RELEASE && mkdir -p $$BUILD_DIR/lib && cp build/debug/*.a $$BUILD_DIR/lib
}