#ifndef _GPT_H
#define _GPT_H_

#include "include/definitions.h"
#include "stdint.h"
#include <wchar.h>

#define GPT_BLOCK_SIZE 512

PACKED(typedef struct GptHeader {
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
}) GptHeader;

PACKED(typedef struct GptEntryAttributes {
	uint64_t requiredToFunction:1;
	uint64_t reserved:47;
    uint64_t typeGuidSpecific:16;
}) GptEntryAttributes;

PACKED(typedef struct GptEntry {
	uint64_t partitionTypeGuid1;
	uint64_t partitionTypeGuid2;
	uint64_t uniquePartitionGuid1;
	uint64_t uniquePartitionGuid2;
	uint64_t startingLba;
	uint64_t endingLba;
	GptEntryAttributes attributes;
	wchar_t partitionName[36];
}) GptEntry;

#endif // _GPT_H_