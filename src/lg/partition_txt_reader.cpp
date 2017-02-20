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
* @file partition_txt_reader.cpp
* @class PartitionTxtReader
* @package openpst/libopenpst
* @brief Parses partition.txt from LG firmware packages with partition.txt file
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#include "lg/partition_txt_reader.h"

using namespace OpenPST::LG;

PartitionTxtReader::PartitionTxtReader()
{

}

PartitionTxtReader::~PartitionTxtReader()
{

}

std::vector<PartitionTxtEntry> PartitionTxtReader::parse(const std::string& filePath)
{
	std::vector<PartitionTxtEntry> ret;

	std::ifstream file(filePath.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open()) {
		std::stringstream ss;
		ss << "Error opening " << filePath << " for reading";
		throw PartitionTxtReaderError(ss.str());
	}

	file.seekg(0, file.end);

	size_t fileSize = static_cast<size_t>(file.tellg());

	file.seekg(0, file.beg);


	for (int i = 0; i < (fileSize/sizeof(PartitionTxtEntry)) && i <= 1024; i++) {
		PartitionTxtEntry e = {};
		file.read(reinterpret_cast<char*>(&e), sizeof(e));       
		ret.push_back(e);
	}

	file.close();

	return ret;
}