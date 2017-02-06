/**
* LICENSE PLACEHOLDER
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

StringHelper::StringHelper()
{

}

StringHelper::~StringHelper()
{

}

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
