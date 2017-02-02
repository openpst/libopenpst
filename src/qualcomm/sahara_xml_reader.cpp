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

    	std::string imageId(image.node().attribute("image_id").value());
    	std::string programmer(image.node().attribute("programmer").value());
    	std::string imagePath(image.node().attribute("image_path").value());

    	if (!imageId.size() || !imagePath.size()) {
    		continue;
    	}

    	e.imageId    = strtoul(imageId.c_str(), nullptr, 10);
    	e.programmer = programmer.compare("true") == 0;
    	e.imagePath  = imagePath;

    	ret.push_back(e);
    }

    return ret;
}