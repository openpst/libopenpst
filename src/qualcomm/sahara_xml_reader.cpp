/**
* LICENSE PLACEHOLDER
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
    	throw std::invalid_argument("Unable to load XML file");
    }
	
	pugi::xpath_query query_images("/sahara_config/images/image");

    pugi::xpath_node_set images = query_images.evaluate_node_set(doc);

    for (auto &image : images) {
    	SaharaXmlEntry e;

        for (auto &attribute : image.node().attributes()) {
            std::string name(attribute.name());
            std::string value(attribute.value());
            
            stringHelper.toUpper(name);

            if (name.compare("IMAGE_ID") == 0){
                if (!value.size()) {
                    continue;
                }
                e.imageId = std::strtoul(value.c_str(), nullptr, 10);
            } else if (name.compare("IMAGE_PATH") == 0) {
                if (!value.size()) {
                    continue;
                }
                e.imagePath = value;
            } else if (name.compare("PROGRAMMER") == 0) {
                stringHelper.toUpper(value);

                e.programmer = value.compare("TRUE") == 0;
            }
        }

    	ret.push_back(e);
    }

    return ret;
}