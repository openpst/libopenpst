#include "util/gpt_parser.h"

using namespace OpenPST;

GptParser::GptParser()
{
	
}


GptParser::~GptParser()
{
	
}

GptInfo GptParser::parse(std::string filePath, int flags, off_t start)
{
	std::stringstream ss;
    GptInfo gpt = {};

    std::ifstream file(filePath, std::ios_base::in | std::ios_base::binary);

    if (!file.is_open()) {
        ss << "Could not open " << filePath;
    	throw std::invalid_argument(ss.str());
    }

    file.seekg(0, file.end);

    size_t fileSize = file.tellg() - start;

    file.seekg(start, file.beg);

    if (flags & kGptParserFlagEntries && !flags & kGptParserFlagHeader) {
    	flags |= kGptParserFlagHeader;
    }

    if (flags & kGptParserFlagMbr) {
    	if (fileSize < sizeof(gpt.mbr)) {
    		throw std::invalid_argument("File is too small for legacy mbr");
    	}

    	file.read(reinterpret_cast<char*>(&gpt.mbr), sizeof(gpt.mbr));
    }

    if (flags & kGptParserFlagHeader) {
    	if (fileSize < sizeof(gpt.mbr) + sizeof(gpt.header)) {
    		throw std::invalid_argument("File is too small for header");
    	}

    	if (!flags & kGptParserFlagMbr) {
    		file.seekg(sizeof(gpt.mbr), file.beg);
    	}
    	
    	file.read(reinterpret_cast<char*>(&gpt.header), sizeof(gpt.header));
    }

 	if (flags & kGptParserFlagEntries) {
	    if ((file.tellg() + (gpt.header.numPartitionEntries * sizeof(GptEntry))) > fileSize) {
	    	throw std::out_of_range("File is too small for the amount of declared entries");
	    }

	    for (int i = 0; i < gpt.header.numPartitionEntries; i++) {
	        GptEntry entry;
	        file.read(reinterpret_cast<char*>(&entry), sizeof(GptEntry));
	        gpt.entries.push_back(entry);
	    }
 	}
    
	return gpt;
}

std::wstring GptParser::getEntryNameW(GptEntry* entry)
{
	std::wstring ret;

	for (int i = 0; i < sizeof(entry->partitionName)/sizeof(uint16_t); i++) {
		if (!entry->partitionName[i]) {
			break;
		}
		ret.push_back(entry->partitionName[i]);
	}

    return ret;
}


std::string GptParser::getEntryName(GptEntry* entry)
{
    std::string ret;

    for (int i = 0; i < sizeof(entry->partitionName)/sizeof(uint16_t); i++) {
        if (!entry->partitionName[i]) {
            break;
        }
        ret.push_back((char)entry->partitionName[i]);
    }

    if (!ret.size()) {
        return "[Not Specified]";
    }
    return ret;
}

std::string GptParser::getUUID(GptUuid* uuid)
{
    std::stringstream ss;

    ss << std::hex << uuid->timeLow << "-";
    ss << std::hex << uuid->timeMid << "-";
    ss << std::hex << uuid->timeHiAndVersion << "-";
    ss << std::hex << uuid->clockSeq << "-";

    for (int i = 0; i < 6; i++) {
         ss << std::hex << (int)uuid->node[i];
    }

    return ss.str();
}