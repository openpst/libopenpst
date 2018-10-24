SOURCES=$(shell find . -name "*.cpp")
SAMPLES=$(SOURCES:%.cpp=bin/%)

.PHONY: all clean

all: $(SAMPLES)

clean:
	rm $(SAMPLES)

bin/%: %.cpp ../build/release/libopenpst.a
	@mkdir -p bin
	g++ -o $@ -std=c++14 $< -L../build/release/ -lopenpst -I../include -I../lib/serial/include