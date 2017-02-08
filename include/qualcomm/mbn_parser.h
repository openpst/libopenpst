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
* @file mbn_parser.h
* @class OpenPST::QC::MbnParser
* @package openpst/libopenpst
* @brief Reads and parses various MBN information from a binary on disk
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include "definitions.h"
#include "qualcomm/mbn.h"

namespace OpenPST {
	namespace QC {

		enum MbnParserFlag {
			kMbnParserFlagNone 			   = 0x00,
			kMbnParserFlagExtractCode 	   = 0x01,
			kMbnParserFlagExtractSignature = 0x02,
			kMbnParserFlagExtractX509 	   = 0x04,
			kMbnParserFlagAll		   	   = kMbnParserFlagExtractCode | 
											kMbnParserFlagExtractSignature | kMbnParserFlagExtractX509
		};

		class Mbn {
			protected:
				std::string filePath;
				std::string imageName;
				int parserFlags;
				size_t fileSize;
				uint32_t imageId = 0;
				uint32_t imageSrc = 0;
				uint32_t imageDestPtr = 0;
				uint32_t imageSize = 0;
				uint32_t codeSize = 0;
				uint32_t signaturePtr = 0;
				uint32_t signatureSize = 0;
				uint32_t certChainPtr = 0;
				uint32_t certChainSize = 0;
				uint32_t oemRootCertSelection = 0;
				uint32_t oemNumberOfRootCerts = 0;
			public:
				std::vector<uint8_t> header;
				std::vector<uint8_t> code;
				std::vector<uint8_t> signature;
				std::vector<uint8_t> x509;

				Mbn(std::string filePath, int parserFlags);
				
				~Mbn();
				
				const std::string& getFilePath();
				
				int getParserFlags();

				size_t getFileSize();

				void setFileSize(size_t fileSize);

				std::string& getImageName();

				void setImageName(std::string imageName);

				uint32_t getImageId();

				void setImageId(uint32_t imageId);

				uint32_t getImageDestPtr();

				void setImageDestPtr(uint32_t imageDestPtr);

				uint32_t getImageSize();

				void setImageSize(uint32_t imagSize);

				uint32_t getCodeSize();

				void setCodeSize(uint32_t codeSize);

				uint32_t getSignaturePtr();

				void setSignaturePtr(uint32_t signaturePtr);

				uint32_t getSignatureSize();

				void setSignatureSize(uint32_t signatureSize);

				uint32_t getOemRootCertSelection();

				void setOemRootCertSelection(uint32_t oemRootCertSelection);

				uint32_t getOemNumberOfRootCerts();

				void setOemNumberOfRootCerts(uint32_t oemNumberOfRootCerts);

				void setImageSrc(uint32_t imageSrc);

				uint32_t getImageSrc();
	
				void setCertChainPtr(uint32_t certChainPtr);

				uint32_t getCertChainPtr();

				void setCertChainSize(uint32_t certChainSize);

				uint32_t getCertChainSize();

				std::vector<uint8_t>& getHeader();

				std::vector<uint8_t>& getCode();

				std::vector<uint8_t>& getSignature();

				std::vector<uint8_t>& getX509();
		};
			
		class MbnParser {
			
			public:
				/**
				* @brief - Constructor
				*/
				MbnParser();

				/**
				* @brief - Deconstructor
				*/
				~MbnParser();

				std::unique_ptr<Mbn> parse(std::string filePath, int flags = kMbnParserFlagNone);

				std::string getImageName(uint32_t imageId);

			protected:
				size_t readSegment(std::ifstream& file, int offset, int size, size_t maxSize, std::vector<uint8_t>& dest);

		};

		class MbnParserError : public std::exception
		{
			const MbnParserError& operator=(MbnParserError);
			std::string _what;
			public:
				MbnParserError(std::string message) : _what(message)  { }
				MbnParserError(const MbnParserError& second) : _what(second._what) {}
				virtual ~MbnParserError() throw() {}
				virtual const char* what() const throw () {
					return _what.c_str();
				}

		};		
	}
}