#include "qualcomm/sahara_xml_reader.h"
#include <iostream>

using OpenPST::QC::SaharaXmlReader;
using OpenPST::QC::SaharaXmlReaderError;
using OpenPST::QC::SaharaXmlEntry;

int main(int argc, char** argv) {
	
	SaharaXmlReader reader;
	std::vector<SaharaXmlEntry> entries;
	
	if (argc < 2) {
		std::cerr << "Provide path to xml as argument" << std::endl;
		return 1;
	}

	try {
		entries = reader.parse(argv[1]);
	} catch (SaharaXmlReaderError& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	// iterate over the resulted entries
	// you would generally iterate over this result set
	// and match an imageId to a requeted imageId from SaharaReadRequest
	// and send requested bytes from file which you would need to resolve
	// either relative path or absolute to imagePath structure member

	for (auto &entry : entries) {
		std::cout << entry.imageId << " - " << entry.imagePath << " Programmer: " << (entry.programmer ? "True" : "False") << std::endl;
	}

	return 0;
}
