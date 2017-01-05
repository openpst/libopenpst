all: default


default:
	if [ ! -d "./build/linux" ]; then mkdir -p ./build/linux;  fi	
	if [ ! -d "./lib/serial/include" ]; then git submodule init && git submodule update;  fi
	qmake -makefile -o ./build/linux/Makefile libopenpst.pro
	$(MAKE) -C ./build/linux/

clean:
	rm -rf build/*