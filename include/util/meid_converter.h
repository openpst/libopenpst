#pragma once

#include <stdint.h>
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
			MeidInfo(const std::string& input, MeidConverterInputType type) :
				input(input), type(type) {
			}

			MeidInfo(const std::string& input, MeidConverterInputType type, Meid meid, Esn esn) :
				input(input), type(type), meid(meid), esn(esn) {
			}
			
			MeidInfo(const std::string& input, MeidConverterInputType type, Meid meid) :
				input(input), type(type), meid(meid) {
			}

			MeidInfo(const std::string& input, MeidConverterInputType type, Esn esn) :
				input(input), type(type), esn(esn) {
			}

			~MeidInfo() {

			}

			void setEsn(const Esn& esn) {
				this->esn = esn;
			}

			void setEsn(uint32_t p1, uint32_t p2) {
				esn.p1 = p1;
				esn.p2 = p2;
			}

			void setMeid(const Meid& meid) {
				this->meid = meid;
			}

			void setMeid(uint32_t p1, uint32_t p2) {
				meid.p1 = p1;
				meid.p2 = p2;
			}

			const std::string& getInput() {
				return input;
			}

			MeidConverterInputType getType() {
				return type;
			}

			std::string getMeidDec() {
				std::stringstream ss;
				ss << std::dec << meid.p1 << meid.p2;
				return ss.str();
			}

			std::string getMeidHex() {
				std::stringstream ss;
				ss << std::hex << meid.p1 << meid.p2;
				return ss.str();
			}

			std::string getEsnDec() {
				std::stringstream ss;
				ss << std::dec << esn.p1 << esn.p2;
				return ss.str();
			}

			std::string getEsnHex() {
				std::stringstream ss;
				ss << std::hex << esn.p1 << esn.p2;
				return ss.str();
			}
	};
	

	class MeidConverter
	{

		public:
			MeidConverter();
			~MeidConverter();
			MeidInfo convert(const std::string& input);
			MeidInfo convert(uint8_t* input, size_t size);
			Esn calculatePesn(const std::string& meidHex);
	};
}