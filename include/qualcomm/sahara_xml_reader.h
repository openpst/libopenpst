/**
* LICENSE PLACEHOLDER
*
* @file sahara_xml_reader.h
* @class SaharaXmlReader
* @package openpst/libopenpst
* @brief Parses a sahara.xml file format for use with sahara automation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/
#pragma once

#include <vector>
#include "definitions.h"
#include "util/string_helper.h"
#include "pugixml.hpp"

using OpenPST::StringHelper;

namespace OpenPST {
	namespace QC {
		
		struct SaharaXmlEntry {
			uint32_t imageId;
			bool programmer;
			std::string imagePath;
		};

		class SaharaXmlReader
		{
			protected:
				StringHelper stringHelper;
			public:
				SaharaXmlReader();
				~SaharaXmlReader();
				std::vector<SaharaXmlEntry> parse(const std::string& filePath); 
		};
	}
}