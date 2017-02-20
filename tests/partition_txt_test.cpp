#include "lg/partition_txt_reader.h"
#include <iostream>

using OpenPST::LG::PartitionTxtReader;
using OpenPST::LG::PartitionTxtReaderError;
using OpenPST::LG::PartitionTxtEntry;


int main(int argc, char** argv) 
{
	
	if (argc < 2) {
		std::cout << "Usage: program [partition.txt]" << std::endl;
		return 0;
	}

	PartitionTxtReader reader;

	std::vector<PartitionTxtEntry> entries;

	try {
		entries = reader.parse(argv[1]);

	} catch(PartitionTxtReaderError& e) {

		std::cerr << "ERROR " << e.what() << std::endl;

	}

	int i = 0;
	for (auto &e : entries) {
		std::cout << "Entry " 		<< std::dec <<  (i+1) << std::endl;
		std::cout << "\tlba: " 		<< std::dec << e.lba << " 0x" << std::hex << e.lba << std::endl;
		std::cout << "\tsectors: " 	<< std::dec << e.sectors << " 0x" << std::hex << e.sectors << std::endl;
		std::cout << "\tunk1: " 	<< std::dec << e.unk1 << " 0x" << std::hex << e.unk1 << std::endl;
		std::cout << "\tunk2: " 	<< std::dec << e.unk2 << " 0x" << std::hex << e.unk2 << std::endl;
		std::cout << "\tname: " 	<< (const char*)&e.name << std::endl;
		std::cout << "\tfile: " 	<< (const char*)&e.file << std::endl;
		i++;
	}

	return 0;
}
