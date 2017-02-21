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
* @file gpt_parser.h
* @package openpst/libopenpst
* @brief Parses GPT information from dumped files
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#pragma once

#include "definitions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#define GPT_BLOCK_SIZE 512

#define GPT_MAX_SIZE sizeof(LegacyMbr) + 16384;

namespace OpenPST {
	struct GptMbr {
		uint8_t     bootIndicator;  /* unused by EFI, set to 0x80 for bootable */
		uint8_t     startHead;      /* unused by EFI, pt start in CHS */
		uint8_t     startSector;    /* unused by EFI, pt start in CHS */
		uint8_t     startTrack;
		uint8_t     osType;         /* EFI and legacy non-EFI OS types */
		uint8_t     endHead;        /* unused by EFI, pt end in CHS */
		uint8_t     endSector;      /* unused by EFI, pt end in CHS */
		uint8_t     endTrack;       /* unused by EFI, pt end in CHS */
		uint32_t  	startingLba;    /* used by EFI - start addr of the on disk pt */
	 	uint32_t  	lbaSize;    	/* used by EFI - size of pt in LBA */
	} __attribute__ ((packed));

	struct LegacyMbr {
		uint8_t 	bootCode[440];
		uint32_t 	mbrSignature;
		uint16_t 	unknown;
		GptMbr 		partition[4];
		uint16_t 	signature;
	} __attribute__ ((packed));

	struct GptUuid {
		uint32_t        timeLow;
		uint16_t        timeMid;
		uint16_t        timeHiAndVersion;
		uint16_t        clockSeq;
		uint8_t         node[6];
	}  __attribute__ ((packed));

	struct GptEntry {
		GptUuid typeUuid;
		GptUuid partitionUuid;
		uint64_t startingLba;
		uint64_t endingLba;
		uint64_t attributes;
		uint16_t partitionName[36];
	} __attribute__ ((packed));

	struct GptHeader {
		char signature[8];
		uint32_t revision;
		uint32_t headerSize;
		uint32_t headerCrc32;
		uint32_t reserved1;
		uint64_t myLba;
		uint64_t alternateLba;
		uint64_t firstUsableLba;
		uint64_t lastUsableLba;
		GptUuid guid;
		uint64_t partitionEntryLba;
		uint32_t numPartitionEntries;
		uint32_t sizeofPartitionEntry;
		uint32_t partitionEntryArrayCrc32;
		uint8_t reserved2[GPT_BLOCK_SIZE - 92];
	} __attribute__ ((packed));

	struct GptEntryAttributes {
		uint64_t requiredToFunction:1;
		uint64_t reserved:47;
	    uint64_t typeGuidSpecific:16;
	} __attribute__ ((packed));

	struct GptInfo {
		LegacyMbr mbr;
		GptHeader header;
		std::vector<GptEntry> entries;
	};

	enum GptParserFlags {
		kGptParserFlagMbr = 0x02,
		kGptParserFlagHeader = 0x04,
		kGptParserFlagEntries = 0x08,
		kGptParserFlagAll = kGptParserFlagMbr | kGptParserFlagHeader | kGptParserFlagEntries,
	};

	class GptParser {
		protected:
		public:
			GptParser();
			~GptParser();
			GptInfo parse(std::string filePath, int flags = kGptParserFlagAll, off_t startOffset = 0);
			std::string getEntryName(GptEntry* entry);
			std::wstring getEntryNameW(GptEntry* entry);
			std::string getUUID(GptUuid* uuid);
	};
}
