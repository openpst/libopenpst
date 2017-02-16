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
* @file sahara_send_image.cpp
* @package openpst/libopenpst
* @brief This is an example of how to use the SaharaSerial class to send an image 
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "qualcomm/sahara_serial.h"
#include <iostream>

using OpenPST::QC::SaharaSerial;
using OpenPST::QC::SaharaSerialError;
using OpenPST::QC::SaharaState;


int main(int argc, char**argv) {

	if (argc < 3) {
		std::cout << "Usage: program [port] [file]" << std::endl;
		return 0;
	}

	SaharaSerial port("");

	// alternately intitialize and open, no need to setPort() and open()
	// but may throw an exception
	// SaharaSerial port(argv[1]);
	// SaharaSerial port(argv[1], 115200, 1000); // baud and timeout

	try {

		port.setPort(argv[1]);

		port.open();

		if (!port.available()) {
			// no data is waiting, meaning this is not Sahara or device needs to be reset
			// Sahara will always sent a packet on connection
			return 0;
		}

		// Sahara hello contains some configuration parameters
		// as well as what mode it is currently in
		SaharaHelloRequest hello = port.readHello();

		// from this point, we send back a hello response packet
		// we either send the same mode back to continue with the
		// devices request, or we can switch modes
		// you can only switch to command mode. 
		// Sahara will generally be in kSaharaModeImageTxPending or kSaharaModeMemoryDebug

		uint32_t mode = hello.mode;

		if (mode == kSaharaModeImageTxPending) {
			// this example handles sending an image
			std::cout << "Device is requesting an image file." << std::endl;
		} else if (mode == kSaharaModeMemoryDebug) {
			// if it were in memory debug mode, then we would need to handle
			// it seperately using kSaharaCommandMemoryDebug and kSaharaCommandMemoryRead
			// command packets
			std::cerr << "This example does not support memory debug" << std::endl;
			return 1;
		} else {
			// SaharaSerial has a helper method to get a named version of the mode
			std::cerr << "This example does not support mode " << port.getNamedMode(mode) << std::endl;
			return 1;
		}


		// we send the hello with the mode we wish to enter
		// We could check the version the device has against what the library supports
		// for this example we just say we are using the same version
		SaharaState state = port.sendHello(mode, hello.version, hello.minVersion, 0);

		// after hello established, device will send (in kSaharaModeImageTxPending mode) an image id and amount
		// to send it
		// the process starts by requesting the header of the file (usually 80 bytes), then requesting the rest
		// in larger chunks
		// for memory debug mode, the device would send a kSaharaCommandMemoryDebug request with a location in memory
		// to read that contains a list of additional memory locations to read with debug information in them
		// again this example is just sending an image
		SaharaReadDataRequest readRequest = port.readNextImageOffset();

		std::cout << "Device is requesting " << std::dec << readRequest.size 
			<< " from offset " << readRequest.offset 
			<< " of image id " << readRequest.imageId << std::endl;

		// SaharaSerial has a helper method to get a named version of the image id
		std::cout << "Image: " << port.getNamedRequestedImage(readRequest.imageId);

		// we can use the imageId to match an entry in a sahara.xml file to automatically handle selecting 
		// the image. For reading a sahara XML you can use the SaharaXmlReader class povided in this library
		
		// In this example we will just use the user provided argument argv[2] and send whatever that is

		// Lets send the image
		// libopenpst has two sendImage methods which you can use. One handles sending the whole image for you
		// just pass it a path to the file and the initial read request we received after establishing hello
		SaharaReadDataRequest nextImageRequest = port.sendImage(argv[2], readRequest);

		// alternately you can open the file yourself, use your own looping logic to send the file(s) request
		// for example to work with updating a progress bar
		// For this see:
		// 			SaharaReadDataRequest sendImage(std::ifstream& file, uint32_t offset, size_t size);

		if (nextImageRequest.imageId && nextImageRequest.imageId != readRequest.imageId) {
			// device is requesting another image
			std::cerr << "Error encounted. This example only supports sendng one file but the device requested a 2nd" << std::endl;
			std::cerr << "First Image: " << port.getNamedRequestedImage(readRequest.imageId);
			std::cerr << "Second Image: " << port.getNamedRequestedImage(nextImageRequest.imageId);
			return 0;
		}

		// when no more images requested, send the done command
		port.sendDone();

		port.close();

		// if the requested image was a programmer to restore flash, you would then transfer to that
		// programmers protocol on next connection if the programmer was executed successfully on the device
		// libopenpst currently supports streaming dload in these cases

	} catch(SaharaSerialError& e) {
		std::cerr << e.what() << std::endl;
		port.close();
	}

	return 0;
}