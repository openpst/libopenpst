#include <iostream>
#include "qualcomm/sahara_xml_reader.h"

using OpenPST::QC::SaharaXmlReader;
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
	} catch (...) {

	}

	for (auto &entry : entries) {
		std::cout << entry.imageId << " - " << entry.imagePath << " Programmer: " << (entry.programmer ? "True" : "False") << std::endl;
	}

	return 0;
}
