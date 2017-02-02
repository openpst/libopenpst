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

namespace OpenPST {
	class StringHelper {
		public:
			StringHelper();
			~StringHelper();
			void toUpper(std::string& s);
			std::string toUpper(const std::string& s);
			void toLower(std::string& s);		
			std::string toLower(const std::string& s);
			void replaceAll(std::string& haystack, const std::string& needle, const std::string& value);
			
	};
}