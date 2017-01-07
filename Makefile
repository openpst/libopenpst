####
## Makefile for openpst/libopenpst
####

all: default

default:
	if [ ! -d "./build" ]; then mkdir -p build;  fi
	if [ ! -d "./lib/serial/include" ]; then git submodule init && git submodule update;  fi
	qmake -makefile -o ./build/Makefile libopenpst.pro 
	$(MAKE) -C build

clean:
	rm -rf build/*