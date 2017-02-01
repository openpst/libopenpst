#pragma once

#include "definitions.h"
#include <iostream>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#include <Wincrypt.h>
#else
#include <boost/uuid/sha1.hpp>
#endif

enum MeidConverterInputType {
	kMeidInputMeidDec = 1,
	kMeidInputMeidHex,
	kMeidInputEsnDec,
	kMeidInputEsnHex
};

namespace OpenPST {
	
	struct Meid {
		uint32_t p1;
		uint32_t p2;
	};

	struct Esn {
		uint32_t p1;
		uint32_t p2;
		bool isPEsn;
	};

	class MeidInfo {
		protected:
			std::string input;
			MeidConverterInputType type;
			Meid meid = {};
			Esn esn = {};
		public:
			MeidInfo(const std::string& input, MeidConverterInputType type);
			MeidInfo(const std::string& input, MeidConverterInputType type, Meid meid, Esn esn);			
			MeidInfo(const std::string& input, MeidConverterInputType type, Meid meid);	
			MeidInfo(const std::string& input, MeidConverterInputType type, Esn esn);
			~MeidInfo();
			void setEsn(const Esn& esn);
			void setEsn(uint32_t p1, uint32_t p2);
			void setMeid(const Meid& meid);
			void setMeid(uint32_t p1, uint32_t p2);
			const std::string& getInput();
			MeidConverterInputType getType();
			std::string getMeidDec();
			std::string getMeidHex();
			std::string getEsnDec();
			std::string getEsnHex();
	};
	

	class MeidConverter
	{

		public:
			MeidConverter();
			~MeidConverter();
			MeidInfo convert(const std::string& input);
			Esn calculatePesn(const std::string& meidHex);
	};
}