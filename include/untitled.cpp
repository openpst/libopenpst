#if defined(_WIN32) && !defined(__MINGW32__)
	#include "win_stdint.h"
	#include "win_inttypes.h"
	#include  <windows.h>
	#define PACKED( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )

#else
	#include <stdint.h>
	#include <unistd.h>
	#define PACKED( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

PACKED(typedef struct { // 0x02
	uint8_t  command;
	uint8_t  magic[STREAMING_DLOAD_MAGIC_SIZE];
	uint8_t  version;
	uint8_t  compatibleVersion;
	uint32_t maxPreferredBlockSize;
	uint32_t baseFlashAddress;
	uint8_t  flashIdLength;
}) StreamingDloadHelloResponseHeader;

PACKED(typedef struct { // 0x02
	uint8_t  command;
	uint8_t  magic[STREAMING_DLOAD_MAGIC_SIZE];
	uint8_t  version;
	uint8_t  compatibleVersion;
	uint32_t maxPreferredBlockSize;
	uint32_t baseFlashAddress;
	uint8_t  flashIdLength;
	uint8_t  flashIdenfier[STREAMING_DLOAD_FLASH_ID_MAX_SIZE];
	uint16_t windowSize;
	uint16_t numberOfSectors;
	uint32_t sectorSizes[STREAMING_DLOAD_MAX_SECTORS];
	uint8_t  featureBits;
}) StreamingDloadHelloResponse;