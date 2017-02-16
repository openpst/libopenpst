## libopenpst
`libopenpst` is a multi-platform C++ library for working with (mostly) Qualcomm SoC devices.

### Features

   - Serial device wrappers for:
      - Qualcomm Sahara Protocol
      - Qualcomm Streaming DLOAD Protocol
      - Qualcomm Diagnostic Monitor (DM,QCDM) Protocol
      
   - Definitions and packet data structures for:
      - Qualcomm DLOAD Protocol
      - Qualcomm Sahara Protocol
      - Qualcomm Streaming DLOAD Protocol
      - Qualcomm Diagnostic Monitor (DM,QCDM) Protocol

### Dependencies


 - QT5 [https://www.qt.io/developers/]
 - Serial Communication Library [https://github.com/wjwwood/serial]
 - Boost UUID module

On ubuntu you can install everything you should need with:

    sudo apt-get install build-essential qt5-default qt5-qmake libboost-dev
 
### Compiling on Linux
    git clone https://github.com/openpst/libopenpst --recursive
    
    make

This will build both debug and release libs. You can build just debug or release with `make debug` or `make release`

### Compiling on Windows

Todo


### Project Include Folders

You will need to add the following include search folders for your project:

These are relative to the root directory of `libopenpst`.

 - include/
 - lib/serial/include/
 - lib/pugixml/src/

### Docs & Examples

Documenattion is not complete but will come at some point.

For now, you can reference any of the GUI applications which implement this library, or you can check out the few examples available in the `examples` directory.