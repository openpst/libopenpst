all: shared

qmake:
	if [ ! -d "./build" ]; then mkdir -p ./build;  fi
	if [ ! -d "./lib/serial/include" ]; then git submodule init && git submodule update;  fi
	qmake -makefile -o ./build/Makefile libopenpst.pro 
	$(MAKE) -C build

shared:
	if [ ! -d "./build" ]; then mkdir -p ./build;  fi
	if [ ! -d "./lib/serial/include" ]; then git submodule init && git submodule update;  fi
	$(CXX) \
		-I"./lib/serial/include" \
		-I"./include" \
	    -std=gnu++11 -Wall -g3 -shared -fPIC  \
	    ./lib/serial/src/serial.cc \
	    ./lib/serial/src/impl/unix.cc \
	    ./lib/serial/src/impl/list_ports/list_ports_linux.cc \
	    ./src/android/adb_device_manager.cpp \
	    ./src/lg/laf_serial.cpp \
	    ./src/qualcomm/hdlc.cpp \
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

#travis:
#	/opt/qt54/bin/qt54-env.sh 
#	/opt/qt54/bin/qmake -makefile -o ./build/Makefile OpenPST.pro 

clean:
	rm -rf build/*
	rm -rf lib/serial/build/*
