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
* @file meid_converter.h
* @package openpst/libopenpst
* @brief Converts MEID from hex and dec 
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#pragma once

#include "definitions.h"
#include <iostream>
#include <sstream>
#include <boost/uuid/detail/sha1.hpp>
#include "util/string_helper.h"

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
