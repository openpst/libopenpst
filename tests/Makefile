####
## Makefile for openpst/libopenpst
####

all: default

default: hdlc meid
	
hdlc:
	if [ ! -d "./build" ]; then mkdir -p build;  fi
	$(CXX)  -I./../include -std=gnu++11 $(CXX_FLAGS) \
		./../src/qualcomm/hdlc_encoder.cpp ./../src/util/hexdump.cpp ./hdlc_test.cpp -o build/hdlc_test 
meid:
	if [ ! -d "./build" ]; then mkdir -p build;  fi
	$(CXX)  -I./../include -I./../lib/openssl/include -std=gnu++11 $(CXX_FLAGS) \
		-lssl -lcrypto \
		./../src/util/meid_converter.cpp \
		./../src/util/hexdump.cpp \
		/home/ghassani/Desktop/openpst/libopenpst/lib/openssl/crypto/sha/sha1_one.c \	

		./meid_test.cpp -o build/meid_test 

clean:
	rm -rf build/*