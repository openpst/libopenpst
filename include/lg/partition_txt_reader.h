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
* @file partition_txt_reader.h
* @class PartitionTxtReader
* @package openpst/libopenpst
* @brief Parses partition.txt from LG firmware packages with partition.txt file
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include "definitions.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#define LG_PARTITION_TXT_MAX_ENTRIES 1024

namespace OpenPST {
	namespace LG {

		struct PartitionTxtEntry {
			uint32_t lba;
			uint32_t sectors;
			uint32_t unk1;
			uint32_t unk2;
			char name[32];
			char file[32];
		};

		class PartitionTxtReader {
			protected:

			public:
				PartitionTxtReader();
				~PartitionTxtReader();
				std::vector<PartitionTxtEntry> parse(const std::string& filePath);
			private:

		};

		class PartitionTxtReaderError : public std::exception
		{
			const PartitionTxtReaderError& operator=(PartitionTxtReaderError);
			std::string _what;
			public:
				PartitionTxtReaderError(std::string message) : _what(message)  { }
				PartitionTxtReaderError(const PartitionTxtReaderError& second) : _what(second._what) {}
				virtual ~PartitionTxtReaderError() throw() {}
				virtual const char* what() const throw () {
					return _what.c_str();
				}

		};	
	}
}
