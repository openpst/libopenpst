/**
*
* (c) Gassan Idriss <ghassani@gmail.com>
* 
* This file is part of libopenpst.
* 
* libopenpst is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* libopenpst is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with libopenpst. If not, see <http://www.gnu.org/licenses/>.
*
* @file streaming_dload_read_write.cpp
* @package openpst/libopenpst
* @brief This is an example of how to use the StreamingDloadSerial class to read and write flash
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "qualcomm/streaming_dload_serial.h"
#include <iostream>

using OpenPST::QC::StreamingDloadSerial;
using OpenPST::QC::StreamingDloadSerialError;


int main(int argc, char**argv) {

	if (argc < 2) {
		std::cout << "Usage: program [port]" << std::endl;
		return 0;
	}

	StreamingDloadSerial port("");

	// alternately intitialize and open, no need to setPort() and open()
	// but may throw an exception
	// StreamingDloadSerial port(argv[1]);
	// StreamingDloadSerial port(argv[1], 115200, 1000); // baud and timeout

	try {

		port.setPort(argv[1]);

		port.open();

		// always send hello request
		// to get device parameters and establish a session

		// we need to send a magic string to start the session
		// default is QCOM fast download protocol host but OEMs
		// DO have the ability to change this so I left it open
		std::string magic = "QCOM fast download protocol host";
		uint8_t ver = 5; // max version we support
		uint8_t minVer = 2; // min version we support
		uint8_t featureBits = STREAMING_DLOAD_FEATURE_ALL; // check for features, we check for all known

		StreamingDloadHelloResponse hello = port.sendHello(magic, ver, minVer, featureBits);
		// you can also just do:
		//		StreamingDloadHelloResponse hello = port.sendHello(); // will populate defaults

		// dump some info about the device received in the hello
		std::cout << "[!] Version: " 			 << (int)hello.version << std::endl;
		std::cout << "[!] Compatible Version: "  << (int)hello.compatibleVersion << std::endl;
		std::cout << "[!] Preferred Block Size " << hello.maxPreferredBlockSize << std::endl;
		std::cout << "[!] Base Flash Address "   << hello.baseFlashAddress << std::endl;
		std::cout << "[!] Flash ID " 			 << hello.flashIdLength << std::endl;
		std::cout << "[!] Number of Sectors " 	 << hello.numberOfSectors << std::endl;
		std::cout << "[!] Feature Bits 0x" 		 << std::hex << (int)hello.featureBits << std::endl;
		
		// check what features are actually supported
		if (hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_UNCOMPRESSED_DOWNLOAD) {
			std::cout << "[!] Device requires an uncompressed download" << std::endl;
		}

		if (hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_NAND_BOOTABLE_IMAGE) {
			std::cout << "[!] Device features NAND Bootable Image" << std::endl;
		}

		if (hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_NAND_BOOT_LOADER) {
			std::cout << "[!] Device features NAND Bootloader" << std::endl;
		}

		if (hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_MULTI_IMAGE) {
			std::cout << "[!] Supports multi-image" << std::endl;
		}

		if (hello.featureBits & STREAMING_DLOAD_FEATURE_BIT_SECTOR_ADDRESSES) {
			std::cout << "[!] Device features sector addresses" << std::endl;
		}

		
		// hello populates some internal things as well, such as buffer sizes in case
		// a device supports larger than documented buffers

		// we can now send other available commends

		// for least errors, set security mode to untrusted first
		port.setSecurityMode(kStreamingDloadSecurityModeUntrusted);


		// now we open EMMC User partition image, which is the only thing you really
		// need to open to restore the flash on modern devices
		// older devices have seperate modes you may need to open to restore the device
		// but libopenpst has not tested older devices but includes ability to open them
		// should you need them

		if (!port.openMultiImage(kStreamingDloadOpenModeMultiEmmcUser)) {
			std::cerr << "Error opening mode " << port.getNamedMultiImage(kStreamingDloadOpenModeMultiEmmcUser) << std::endl;
			port.close();
			return 0;
		}

		// now we have established our session, set untrusted mode to give us more control
		// and opened the flash device for reading and writing

		// lets read the first 10000 bytes from the flash device
		std::vector<uint8_t> flashData;

		size_t read = port.readFlash(0x00000000, 10000, flashData);

		// you can also read directly into a out file stream
		// see:
		// 		size_t StreamingDloadSerial::readFlash(uint32_t address, size_t amount, std::ofstream& out);

		std::cout << "Read " << std::dec << read << "/10000 bytes starting from 0x00000000" << std::endl;


		// Now lets write it back

		size_t wrote = port.writeFlash(0x00000000, flashData);

		// you can also write from a raw pointer and size
		// see:
		// 			size_t StreamingDloadSerial::writeFlash(uint32_t address, uint8_t* data, size_t length);

		std::cout << "Wrote back " << wrote << "/" << read << " bytes starting at 0x00000000" << std::endl;

		port.close();

		// this example just shows the process of reading and writing
		// but what you would typically do is write each partition required to boot
		// like: sbl1, rpmb, tz, modem, etc
		// you can use the RawProgramXmlReader to read a XML file cotaining information
		// about the device flash partition layout and automate the write process

	} catch(StreamingDloadSerialError& e) {
		std::cerr << e.what() << std::endl;
		port.close();
	}

	return 0;
}