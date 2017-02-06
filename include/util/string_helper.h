/**
* LICENSE PLACEHOLDER
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
			StringHelper();
			~StringHelper();
			void toUpper(std::string& s);
			std::string toUpper(const std::string& s);
			void toLower(std::string& s);		
			std::string toLower(const std::string& s);
			bool isHex(const std::string& s);
			void replaceAll(std::string& haystack, const std::string& needle, const std::string& value);
			
			template <typename T>
			T toInt(const std::string& str, bool hex = false) {
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

			float toFloat(const std::string& str) {
				float ret = std::stof(str.c_str());

				if (errno == ERANGE){
					std::stringstream ss;
					ss << "Value " << str << " is out of range";
					throw std::out_of_range(ss.str());
				}

			}
	};
}