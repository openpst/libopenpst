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
* @file meid_converter.cpp
* @package openpst/libopenpst
* @brief Converts MEID from hex and dec 
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "util/meid_converter.h"

using namespace OpenPST;

MeidConverter::MeidConverter(){

}

MeidConverter::~MeidConverter(){

}

MeidInfo MeidConverter::convert(const std::string& input)
{
	std::stringstream ss;
	MeidConverterInputType type;
	Meid meid = {};
	Esn esn = {};
	
	if (input.size() == 18) {
		type = kMeidInputMeidDec;
	} else if (input.size() == 14) {
		type = kMeidInputMeidHex;
	} else if (input.size() == 11) {
		type = kMeidInputEsnDec;
	} else if (input.size() == 8) {
		type = kMeidInputEsnHex;
	} else {
		throw std::invalid_argument("String does not appear to be a valid MEID or ESN in HEX or DEC format");
	}

	switch(type) {
		case kMeidInputMeidDec:
			meid.p1 = StringHelper::toInt<uint32_t>(input.substr(0,10));
			meid.p2 = StringHelper::toInt<uint32_t>(input.substr(10));			
			break;
		case kMeidInputMeidHex:
			meid.p1 = StringHelper::toInt<uint32_t>(input.substr(0,8), true);
			meid.p2 = StringHelper::toInt<uint32_t>(input.substr(8), true);
			break;
		case kMeidInputEsnDec:
			esn.p1 = StringHelper::toInt<uint32_t>(input.substr(0,8));
			esn.p2 = StringHelper::toInt<uint32_t>(input.substr(8));
			break;
		case kMeidInputEsnHex:
			esn.p1 = StringHelper::toInt<uint32_t>(input.substr(0,2), true);
			esn.p2 = StringHelper::toInt<uint32_t>(input.substr(2), true);
			break;
	}

	MeidInfo ret(input, type, meid, esn);
	
	if (type == kMeidInputMeidDec || type == kMeidInputMeidHex) {
		ret.setEsn(calculatePesn(ret.getMeidHex()));
	}

	return ret;
}

Esn MeidConverter::calculatePesn(const std::string& meidHex)
{
	std::stringstream ss;
	uint32_t hash[5];
	uint8_t  data[7];
	Esn 	 ret = {};

	for(int i = 0, k = 0; i < meidHex.size(); i+=2, k++){
		data[k] = StringHelper::toInt<uint32_t>(meidHex.substr(i, 2), true);
	}
	
	boost::uuids::detail::sha1 sha1;
	sha1.process_bytes(data, sizeof(data));
	sha1.get_digest(hash);

	ss << std::hex << hash[4];

	std::string pesn = ss.str();

	pesn.replace(pesn.begin(), pesn.begin() + 2, "80");

	ret.p1 = StringHelper::toInt<uint32_t>(pesn.substr(0,2), true);
	ret.p2 = StringHelper::toInt<uint32_t>(pesn.substr(2), true);

	return ret;
}


MeidInfo::MeidInfo(const std::string& input, MeidConverterInputType type) :
	input(input), type(type) {
}

MeidInfo::MeidInfo(const std::string& input, MeidConverterInputType type, Meid meid, Esn esn) :
	input(input), type(type), meid(meid), esn(esn) {
}

MeidInfo::MeidInfo(const std::string& input, MeidConverterInputType type, Meid meid) :
	input(input), type(type), meid(meid) {
}

MeidInfo::MeidInfo(const std::string& input, MeidConverterInputType type, Esn esn) :
	input(input), type(type), esn(esn) {
}

MeidInfo::~MeidInfo() {

}

void MeidInfo::setEsn(const Esn& esn) 
{
	this->esn = esn;
}

void MeidInfo::setEsn(uint32_t p1, uint32_t p2) 
{
	esn.p1 = p1;
	esn.p2 = p2;
}

void MeidInfo::setMeid(const Meid& meid) {
	this->meid = meid;
}

void MeidInfo::setMeid(uint32_t p1, uint32_t p2) 
{
	meid.p1 = p1;
	meid.p2 = p2;
}

const std::string& MeidInfo::getInput() 
{
	return input;
}

MeidConverterInputType MeidInfo::getType() 
{
	return type;
}

std::string MeidInfo::getMeidDec() 
{
	if (getType() == kMeidInputEsnDec || getType() == kMeidInputEsnHex) {
		return "000000000000000000";
	}

	std::stringstream ss1;
	std::stringstream ss2;

	ss1 << std::dec << meid.p1;
	ss2 << std::dec << meid.p2;

	std::string p1 = ss1.str();
	std::string p2 = ss2.str();

	while(p1.size() < 10) {
		p2.insert(p2.begin(), 0x30);
	}

	while(p1.size() < 8) {
		p2.insert(p2.begin(), 0x30);
	}

	return std::string(p1 + p2);
}

std::string MeidInfo::getMeidHex() 
{
	if (getType() == kMeidInputEsnDec || getType() == kMeidInputEsnHex) {
		return "00000000000000";
	}

	std::stringstream ss;
	ss << std::hex << meid.p1 << meid.p2;
	return ss.str();
}

std::string MeidInfo::getEsnDec() 
{
	std::stringstream ss1;
	std::stringstream ss2;

	ss1 << std::dec << esn.p1;
	ss2 << std::dec << esn.p2;
	
	std::string p1 = ss1.str();
	std::string p2 = ss2.str();
	while(p2.size() + p1.size() < 11) {
		p2.insert(p2.begin(), 0x30);
	}
	return std::string(p1 + p2);
}

std::string MeidInfo::getEsnHex() {
	std::stringstream ss;
	ss << std::hex << esn.p1 << esn.p2;
	return ss.str();
}