#pragma once

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define GPT_BLOCK_SIZE 512

namespace OpenPST {
	struct GptMbr {
		uint8_t      boot_indicator; /* unused by EFI, set to 0x80 for bootable */
		uint8_t      start_head;     /* unused by EFI, pt start in CHS */
		uint8_t      start_sector;   /* unused by EFI, pt start in CHS */
		uint8_t      start_track;
		uint8_t      os_type;        /* EFI and legacy non-EFI OS types */
		uint8_t      end_head;       /* unused by EFI, pt end in CHS */
		uint8_t      end_sector;     /* unused by EFI, pt end in CHS */
		uint8_t      end_track;      /* unused by EFI, pt end in CHS */
		uint32_t  	starting_lba;   /* used by EFI - start addr of the on disk pt */
	 	uint32_t  	size_in_lba;    /* used by EFI - size of pt in LBA */
	} __attribute__ ((packed));

	struct LegacyMbr {
		uint8_t 	boot_code[440];
		uint32_t 	unique_mbr_signature;
		uint16_t 	unknown;
		GptMbr 		partition_record[4];
		uint16_t 	signature;
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
		uint64_t guid1;
		uint64_t guid2;
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

	struct GptUuid {
		uint32_t        timeLow;
		uint16_t        timeMid;
		uint16_t        timeHiAndVersion;
		uint8_t         clockSeqHigh;
		uint8_t         clockSeqLow;
		uint8_t         node[6];
	}  __attribute__ ((packed));

	struct GptEntry {
		GptUuid partitionTypeGuid1;
		GptUuid partitionTypeGuid2;
		uint64_t startingLba;
		uint64_t endingLba;
		uint64_t attributes;
		uint16_t partitionName[36];
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
			std::string getEntryUUID(GptEntry* entry);
	};
}
