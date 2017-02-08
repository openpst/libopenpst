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
* @file sahara_xml_reader.cpp
* @class SaharaXmlReader
* @package openpst/libopenpst
* @brief Parses a sahara.xml file format for use with sahara automation
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "qualcomm/sahara_xml_reader.h"
#include <iostream>

using namespace OpenPST::QC;

SaharaXmlReader::SaharaXmlReader()
{

}

SaharaXmlReader::~SaharaXmlReader()
{

}

std::vector<SaharaXmlEntry> SaharaXmlReader::parse(const std::string& filePath)
{
	pugi::xml_document doc;
	std::vector<SaharaXmlEntry> ret;

    if (!doc.load_file(filePath.c_str())) {
    	throw SaharaXmlReaderError("Unable to load XML file");
    }

	try {
    	pugi::xpath_query query_images("/sahara_config/images/image");

        pugi::xpath_node_set images = query_images.evaluate_node_set(doc);

        for (auto &image : images) {
        	SaharaXmlEntry e;

            for (auto &attribute : image.node().attributes()) {
                std::string name(attribute.name());
                std::string value(attribute.value());
                
                StringHelper::toUpper(name);

                if (name.compare("IMAGE_ID") == 0){
                    if (!value.size()) {
                        continue;
                    }
                    e.imageId = StringHelper::toInt<uint32_t>(value);
                } else if (name.compare("IMAGE_PATH") == 0) {
                    if (!value.size()) {
                        continue;
                    }
                    e.imagePath = value;
                } else if (name.compare("PROGRAMMER") == 0) {
                    StringHelper::toUpper(value);

                    e.programmer = value.compare("TRUE") == 0;
                }
            }

        	ret.push_back(e);
        }

    } catch (std::out_of_range& e) {
        std::stringstream ss;
        ss << "A numeric value failed conversion: " << e.what();
        throw SaharaXmlReaderError(ss.str());
    } catch (...) {
        throw SaharaXmlReaderError("Unhandled Exception Encountered");
    }
    
    return ret;
}