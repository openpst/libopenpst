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
		std::cout << "Entry " << std::dec <<  (i+1) << std::endl;
		std::cout << "\taddress: " << std::dec << e.address << " " << swapEndian<uint32_t>(e.address) << " 0x" << std::hex << e.address << " 0x" << swapEndian<uint32_t>(e.address) << std::endl;
		std::cout << "\tunk1: " << std::dec << e.unk1 << " " << swapEndian<uint32_t>(e.unk1) << " 0x" << std::hex << e.unk1 << " 0x" << swapEndian<uint32_t>(e.unk1) << std::endl;
		std::cout << "\tunk2: " << std::dec << e.unk2 << " 0x" << std::hex << e.unk2 << std::endl;
		std::cout << "\tunk3: " << std::dec << e.unk3 << " 0x" << std::hex << e.unk3 << std::endl;
		std::cout << "\tname: " << (const char*)&e.name << std::endl;
		std::cout << "\tfile: " << (const char*)&e.file << std::endl;
		i++;
	}

	return 0;
}
