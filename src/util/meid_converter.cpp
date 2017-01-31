#include "util/meid_converter.h"
#include "openssl/sha.h"

/**
* MeidConverter
*/
MeidConverter::MeidConverter(){

}

/**
* MeidConverter
*/
MeidConverter::~MeidConverter(){

}


MeidInfo MeidConverter::convert(const std::string& input)
{
	std::stringstream ss;

	MeidConverterInputType type;
	MeidInfo ret = {};
	
	if (input.size() == 18) {
		type = kMeidInputMeidDec;
		ret.meidDec = input;
	} else if (input.size() == 14) {
		type = kMeidInputMeidHex;
		ret.meidHex = input;
	} else if (input.size() == 11) {
		type = kMeidInputEsnDec;
		ret.pEsnDec = input;
	} else if (input.size() == 8) {
		type = kMeidInputEsnHex;
		ret.pEsnHex = input;
	} else {
		throw std::invalid_argument("String does not appear to be a valid MEID or ESN in HEX or DEC format");
	}

	uint32_t p1 = 0;
	uint32_t p2 = 0;

	if (type == kMeidInputMeidDec) {
		p1 = strtoul(input.substr(0,10).c_str(), nullptr, 10);
		p2 = strtoul(input.substr(10).c_str(), nullptr, 10);
		
		ss << std::hex << p1 << p2;
		
		ret.meidHex = ss.str();
	}

	return ret;
}

MeidInfo MeidConverter::convert(uint8_t* input, size_t size)
{
	MeidInfo ret = {};

	return ret;
}

std::string MeidConverter::sha1(std::string data) 
{
#ifdef _WIN32
    //https://msdn.microsoft.com/en-us/library/windows/desktop/aa382380.aspx
	HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    uint32_t dwStatus = 0;
    uint8_t hash[SHA_DIGEST_LENGTH];

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        dwStatus = GetLastError();
        throw std::runtime_error("Error acquiring crypto context");
    }
    if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash)) {
        dwStatus = GetLastError();
        CryptReleaseContext(hProv, 0);
        throw std::runtime_error("Error creating hash");
    }

    if (!CryptHashData(hHash, reinterpret_cast<const uint8_t*>(data.c_str()), data.size(), 0)) {
        dwStatus = GetLastError();
        CryptReleaseContext(hProv, 0);
        CryptDestroyHash(hHash);
        throw std::runtime_error("Error hasing data");
    }
	
	hashSize = SHA_DIGEST_LENGTH;
    if (CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashSize, 0))  {
        for (DWORD i = 0; i < hashSize; i++) {

        }
    }
#else
	unsigned char hash[SHA_DIGEST_LENGTH];
	SHA1(reinterpret_cast<const uint8_t*>(data.c_str()), data.size(), hash);
	std::string ret = "";
	return ret;
#endif
}


 // == 20

/*

MEID (DEC): 270113181116019285 - 18
MEID (HEX): A1000023F46F55 - 14
ESN (DEC): 12806266744 - 11 ?
ESN (HEX): 805F9F78 - 8

MeidConverter::MEID MeidConverter::convert(char input[]){

	if(!this->isValid(input)){
		throw std::string("Invalid Input");
	}

	this->mInput = input;

	MEID output;
	output.meidDec = this->convertToMeidDec(input);
	output.meidHex = this->convertToMeidHex(input);

	if(this->getInputType() == MEID_DEC || this->getInputType() == MEID_HEX){
		output.pEsnHex = this->calculatePesn((char*)output.meidHex.c_str());
		output.pEsnDec = this->convertToEsnDec((char*)output.pEsnHex.c_str());
	} else {
		output.pEsnDec = this->convertToEsnDec(input);
		output.pEsnHex = this->convertToEsnHex(input);
	}
	return output;
}

std::string MeidConverter::convertToMeidHex(char input[]){
	if(this->getInputType() == MEID_HEX){
		return std::string(input);
	}
	return this->transformSerial(input, 10, 16, 10, 8, 6);
}


std::string MeidConverter::convertToMeidDec(char input[]){
	if(this->getInputType() == MEID_DEC){
		return std::string(input);
	}
	return this->transformSerial(input, 16, 10, 8, 10, 8);
}


std::string MeidConverter::convertToEsnHex(char input[]){
	if(this->getInputType() == ESN_HEX){
		return std::string(input);
	}
	return this->transformSerial(input, 10, 16, 3, 2, 6);
}


std::string MeidConverter::convertToEsnDec(char input[]){
	if(this->getInputType() == ESN_DEC){
		return std::string(input);
	}
	return this->transformSerial(input, 16, 10, 2, 3, 8);
}

std::string MeidConverter::calculatePesn(char meidHex[]){
	if( this->getInputType() == ESN_DEC  || this->getInputType() == ESN_HEX ){ //ESN has no pESN
		return NULL;
    }

	unsigned char hash[20];
	char hexstring[41];
	unsigned int key = 0;
	char buff[7];

	for(unsigned int i = 0; i < strlen(meidHex); i+=2){
		buff[key] = strtol(std::string(meidHex).substr(i, 2).c_str(),nullptr,16);
		key++;
	}

    sha1::calc(buff, sizeof(buff), hash);
	sha1::toHexString(hash, hexstring);
	
	std::string out = "80";
	out.append(std::string(hexstring).substr((strlen(hexstring) -6),(strlen(hexstring))));
	return out;    
}

bool MeidConverter::isValid(char input[]){
	std::string s = input;

	std::regex meidDecTest ("[0-9]{18}");
	std::regex meidHexTest ("[a-fA-F0-9]{14}");
	std::regex esnDecTest ("[0-9]{11}");
	std::regex esnHexTest ("[a-fA-F0-9]{8}");

	if( std::regex_match (s,meidDecTest) ){
		this->setType(MEID_DEC);
		return true;
	}

	if( std::regex_match (s,meidHexTest) ){
		this->setType(MEID_HEX);
		return true;
	}

	if( std::regex_match (s,esnDecTest) ){
		this->setType(ESN_DEC);
		return true;
	}

	if( std::regex_match (input,esnHexTest) ){
		this->setType(ESN_HEX);
		return true;
	}

	return false;
}
	return this->transformSerial(input, 16, 10, 8, 10, 8);

std::string MeidConverter::transformSerial(char input[], int srcBase, int dstBase, int p1Width, int p1Padding, int p2Padding){
	std::string inputAsString = input;
	bool toHex = srcBase == 10 && dstBase == 16;

	std::string part1_s = inputAsString.substr(0,p1Width);
	std::string part2_s = inputAsString.substr(p1Width);
		
	unsigned long part1_l = strtoul(part1_s.c_str(),nullptr,srcBase);
	unsigned long part2_l = strtoul(part2_s.c_str(),nullptr,srcBase);
	
	char outputBuffer[50];
	if(toHex)
		sprintf(outputBuffer, "%lX%lX", part1_l, part2_l );
	else
		sprintf(outputBuffer, "%lu%lu", part1_l, part2_l );

	return std::string(outputBuffer);
}


int MeidConverter::getInputType(){
	return this->mInputType;
}

void MeidConverter::setType(int type){
	this->mInputType = type;
}
*/