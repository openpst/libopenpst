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
* @file string_helper.cpp
* @class String
* @package openpst/libopenpst
* @brief String helper
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "util/string_helper.h"

using namespace OpenPST;

std::string StringHelper::toUpper(const std::string& s)
{
	std::string ret = s;

    toUpper(ret);

    return ret;
}

void StringHelper::toUpper(std::string& s) 
{
    for (int i = 0; i < s.size(); i++) {
        s[i] = toupper(s[i]);
    }
}

void StringHelper::toLower(std::string& s) 
{
    for (int i = 0; i < s.size(); i++) {
        s[i] = tolower(s[i]);
    }
}

std::string StringHelper::toLower(const std::string& s)
{
	std::string ret = s;

    toLower(ret);

    return ret;
}

void StringHelper::replaceAll(std::string& haystack, const std::string& needle, const std::string& value) 
{
	off_t pos = haystack.find(needle);

	while(pos != std::string::npos) {
		haystack.replace(pos, needle.size(), value);
		pos = haystack.find(needle);
	}
}

bool StringHelper::isHex(const std::string& s)
{
    try {
        toInt<int>(s, true);
        return true;
    } catch (...) {
        return false;
    }

    return false;
}
