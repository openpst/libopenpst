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
* @file string_helper.h
* @class String
* @package openpst/libopenpst
* @brief String helper
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#pragma once

#include "definitions.h"
#include <string>
#include <sstream>
#include <cassert>
#include <errno.h>
#include <cstdlib>
#include <stdexcept>

namespace OpenPST {
	class StringHelper {
		public:
			static void toUpper(std::string& s);
			static std::string toUpper(const std::string& s);
			static void toLower(std::string& s);		
			static std::string toLower(const std::string& s);
			static bool isHex(const std::string& s);
			static void replaceAll(std::string& haystack, const std::string& needle, const std::string& value);
			
			template <typename T>
			static T toInt(const std::string& str, bool hex = false) {
				assert(std::is_fundamental<T>::value);
				
				unsigned long long res = std::strtoull(str.c_str(), nullptr, hex ? 16 : 10);
				
				if (errno == ERANGE){
					std::stringstream ss;
					ss << "Value " << str << " is out of range";
					throw std::out_of_range(ss.str());
				}

				T ret = static_cast<T>(res);

				return ret;
			}

			static float toFloat(const std::string& str) {
				float ret = std::stof(str.c_str());

				if (errno == ERANGE){
					std::stringstream ss;
					ss << "Value " << str << " is out of range";
					throw std::out_of_range(ss.str());
				}

			}
	};
}