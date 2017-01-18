####
## Makefile for openpst/libopenpst
####

all: default

default:
	if [ ! -d "./build" ]; then mkdir -p build;  fi
	$(CXX)  -I./../include -std=gnu++11 $(CXX_FLAGS) \
		./../src/qualcomm/hdlc_encoder.cpp ./../src/util/hexdump.cpp ./hdlc_test.cpp -o build/hdlc_test 

clean:
	rm -rf build/*