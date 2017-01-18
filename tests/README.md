####
## Makefile for openpst/libopenpst/tests
####

all: hdlc

hdlc:
	if [ ! -d "./build" ]; then mkdir -p build;  fi

clean:
	rm -rf build/*