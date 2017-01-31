#pragma once

#include <stdint.h>
#include <iostream>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#include <Wincrypt.h>
#else
#include "openssl/sha.h"
#endif

#define SHA_DIGEST_LENGTH 20

struct MeidInfo {
	std::string meidDec;
	std::string meidHex;
	std::string pEsnDec;
	std::string pEsnHex;
};

enum MeidConverterInputType {
	kMeidInputMeidDec = 1,
	kMeidInputMeidHex,
	kMeidInputEsnDec,
	kMeidInputEsnHex
};

class MeidConverter
{



	public:
		MeidConverter();
		~MeidConverter();
		MeidInfo convert(const std::string& input);
		MeidInfo convert(uint8_t* input, size_t size);
		std::string sha1(std::string data);
		/*
		std::string convertToMeidHex(char input[]);
		std::string convertToMeidDec(char input[]);
		std::string convertToEsnHex(char input[]);
		std::string convertToEsnDec(char input[]);
		std::string calculatePesn(char input[]);
*/
	/*private:
		int mInputType;
		char *mInput;
		MEID result;
		void setType(int type);
		std::string transformSerial(char* input, int srcBase, int dstBase, int p1Width, int p1Padding, int p2Padding);
		*/
};