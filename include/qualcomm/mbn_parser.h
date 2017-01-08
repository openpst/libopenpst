/**
* LICENSE PLACEHOLDER
*
* @file mbn_parser.h
* @class OpenPST::QC::MbnParser
* @package OpenPST
* @brief Reads and parses various MBN information from a binary on disk
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include "definitions.h"
#include "qualcomm/mbn.h"

namespace OpenPST {
	namespace QC {

		struct MbnHeader {
			bool is80byte;
			union {
				MbnHeader80byte header80;
				MbnHeader40byte header40;
			};
		};

		class MbnParser {
			
			protected:
				std::string filePath;
				size_t fileSize = 0;
				MbnHeader header = {};
			public:
				/**
				* @brief - Constructor
				*/
				MbnParser(std::string filePath);

				/**
				* @brief - Deconstructor
				*/
				~MbnParser();

				const MbnHeader& getHeader();

				int getHeaderSize();

				std::string getImageName();
				
				std::string getImageName(uint32_t imageId);
		};
	}
}