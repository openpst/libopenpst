/**
* LICENSE PLACEHOLDER
*
* @file mbn_reader.h
* @class OpenPST::QC::MbnHeader
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

		class MbnReader {
			
			protected:
				std::string filePath;
				size_t fileSize = 0;
				MbnHeader header = {};

			public:
				/**
				* @brief - Constructor
				*/
				MbnReader(std::string filePath);

				/**
				* @brief - Deconstructor
				*/
				~MbnReader();

				const MbnHeader& getHeader();

				int getHeaderSize();
		};
	}
}
