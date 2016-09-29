all: as_so

as_so:
	$(CXX) \
		-I"./lib/serial/include" \
		-I"./include" \
	    -std=gnu++11 -Wall -g3 -shared -fPIC  \
	    ./lib/serial/src/serial.cc \
	    ./lib/serial/src/impl/unix.cc \
	    ./lib/serial/src/impl/list_ports/list_ports_linux.cc \
	    ./src/android/adb_device_manager.cpp \
	    ./src/lg/laf_serial.cpp \
	    ./src/qualcomm/hdlc_encoder.cpp \
	    ./src/qualcomm/dm_efs_manager.cpp \
	    ./src/qualcomm/dm_efs_node.cpp \
	    ./src/qualcomm/hdlc_serial.cpp \
	    ./src/qualcomm/qcdm_serial.cpp \
	    ./src/qualcomm/sahara_serial.cpp \
	    ./src/qualcomm/streaming_dload_serial.cpp \
	    ./src/samsung/akseed_calculator.cpp \
	    ./src/serial/generic_serial.cpp \
	    ./src/util/convert.cpp \
	    ./src/util/endian.cpp \
	    ./src/util/hexdump.cpp \
	    ./src/util/sleep.cpp \
		-o ./build/libopenpst.so  

clean:
	rm -rf build/*