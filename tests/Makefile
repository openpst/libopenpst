####
## Makefile for openpst/libopenpst
####

all: default

default: hdlc meid gpt sahara_xml raw_program_xml simple_math_evaluator
	
hdlc:
	if [ ! -d "./build" ]; then mkdir -p build;  fi
	$(CXX)  -I./../include -std=gnu++11 $(CXX_FLAGS) \
		./../src/qualcomm/hdlc_encoder.cpp ./../src/util/hexdump.cpp ./hdlc_test.cpp -o build/hdlc_test 
meid:
	if [ ! -d "./build" ]; then mkdir -p build;  fi
	$(CXX)  -I./../include -I./../lib/boost/uuid/include \
		-I./../lib/boost/throw_exception/include \
		-I./../lib/boost/assert/include \
		-I./../lib/boost/static_assert/include \
		-std=gnu++11 $(CXX_FLAGS) \
		./../src/util/meid_converter.cpp \
		./../src/util/hexdump.cpp \
		./meid_test.cpp -o build/meid_test 
gpt:
	if [ ! -d "./build" ]; then mkdir -p build;  fi
	$(CXX)  -I./../include -std=gnu++11 $(CXX_FLAGS) \
		./../src/util/gpt_parser.cpp \
		./../src/util/hexdump.cpp \
		./gpt_test.cpp -o build/gpt_test 
sahara_xml:
	if [ ! -d "./build" ]; then mkdir -p build;  fi
	$(CXX)  -I./../include -I./../lib/pugixml/src -std=gnu++11 $(CXX_FLAGS) \
		./../lib/pugixml/src/pugixml.cpp \
		./../src/qualcomm/sahara_xml_reader.cpp \
		./../src/util/string_helper.cpp \
		./sahara_xml_test.cpp -o build/sahara_xml_test 

raw_program_xml:
	if [ ! -d "./build" ]; then mkdir -p build;  fi
	$(CXX)  -I./../include -I./../lib/pugixml/src -std=gnu++11 $(CXX_FLAGS) \
		./../lib/pugixml/src/pugixml.cpp \
		./../src/qualcomm/raw_program_xml_reader.cpp \
		./../src/util/string_helper.cpp \
		./../src/util/simple_math_evaluator.cpp \
		./raw_program_xml_test.cpp -o build/raw_program_xml_test 

simple_math_evaluator:
	if [ ! -d "./build" ]; then mkdir -p build;  fi
	$(CXX)  -I./../include -I./../lib/pugixml/src -std=gnu++11 $(CXX_FLAGS) \
		./../src/util/simple_math_evaluator.cpp \
		./simple_math_evaluator_test.cpp -o build/simple_math_evaluator_test 

clean:
	rm -rf build/*