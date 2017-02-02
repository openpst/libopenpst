#include <iostream>
#include "qualcomm/raw_program_xml_reader.h"

using OpenPST::QC::RawProgramXmlReader;
using OpenPST::QC::RawProgramXmlEntry;


int main(int argc, char** argv) {
	
	RawProgramXmlReader reader;
	std::vector<RawProgramXmlEntry> entries;
	
	if (argc < 2) {
		std::cerr << "Provide path to xml as argument" << std::endl;
		return 1;
	}

	try {
		entries = reader.parse(argv[1]);
	} catch (...) {

	}


	return 0;
}
