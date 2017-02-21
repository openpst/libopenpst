#include <iostream>
#include "qualcomm/raw_program_xml_reader.h"
#include <vector>

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
		entries = reader.parse(argv[1], 128);
	} catch (...) {

	}

	for (auto &entry : entries) {
		std::cout << entry.label << " - " << entry.fileName << std::endl;
		
		std::cout << "\tSector Size: " << std::dec << entry.sectorSize << std::endl;
		
		std::cout << "\tFile Sector Offset: " << std::dec << entry.fileSectorOffset << std::endl;
		
		std::cout << "\tNumber of Parition Sectors: " << std::dec << entry.numPartitionSectors << std::endl;
		
		std::cout << "\tPhysical Partition Number: " << std::dec << entry.physicalPartitionNumber << std::endl;
		
		std::cout << "\tStart Byte: " << std::dec << entry.startByte << " (0x" << std::hex << entry.startByte << ")" << std::endl;
		std::cout << "\tStart Sector: " << std::dec << entry.startSector << std::endl;
		std::cout << "\tSize: " << std::dec << entry.size << std::endl;

		std::cout << "\tPart of Single Image: " << std::boolalpha << entry.partOfSingleImage << std::endl;
		std::cout << "\tRead Back Verify: " << std::boolalpha << entry.readBackVerify << std::endl;
		std::cout << "\tSparse: " << std::boolalpha << entry.sparse << std::endl;

	}
	return 0;
}
