####
## Makefile for openpst/libopenpst
####
QMAKE = qmake

all: default

default: check debug release

debug: check _debug

release: check _release

check:
	if [ ! -d "./build" ]; then mkdir -p build;  fi
	if [ ! -d "./lib/serial/include" ]; then git submodule init && git submodule update;  fi

_debug:
	$(QMAKE) -makefile -o ./build/Makefile libopenpst.pro  "CONFIG+=debug" "CONFIG-=release"
	$(MAKE) -C build


_release:
	$(QMAKE) -makefile -o ./build/Makefile libopenpst.pro  "CONFIG-=debug" "CONFIG+=release"
	$(MAKE) -C build

clean:
	rm -rf build/*
