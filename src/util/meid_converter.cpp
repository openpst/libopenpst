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
			meid.p1 = strtoul(input.substr(0,10).c_str(), nullptr, 10);
			meid.p2 = strtoul(input.substr(10).c_str(), nullptr, 10);			
			break;
		case kMeidInputMeidHex:
			meid.p1 = strtoul(input.substr(0,8).c_str(), nullptr, 16);
			meid.p2 = strtoul(input.substr(8).c_str(), nullptr, 16);
			break;
		case kMeidInputEsnDec:
			esn.p1 = strtoul(input.substr(0,8).c_str(), nullptr, 10);
			esn.p2 = strtoul(input.substr(8).c_str(), nullptr, 10);
			break;
		case kMeidInputEsnHex:
			esn.p1 = strtoul(input.substr(0,2).c_str(), nullptr, 16);
			esn.p2 = strtoul(input.substr(2).c_str(), nullptr, 16);
			break;
	}

	MeidInfo ret(input, type, meid, esn);
	
	if (type == kMeidInputMeidDec || type == kMeidInputMeidHex) {
		ret.setEsn(calculatePesn(ret.getMeidHex()));
	}

	return ret;
}

MeidInfo MeidConverter::convert(uint8_t* input, size_t size)
{
	MeidInfo ret("", kMeidInputMeidDec);

	return ret;
}

Esn MeidConverter::calculatePesn(const std::string& meidHex)
{
#ifdef _WIN32
    /*//https://msdn.microsoft.com/en-us/library/windows/desktop/aa382380.aspx
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
    }*/
#else
	std::stringstream ss;
	uint32_t hash[5];
	uint8_t  data[7];
	Esn 	 ret = {};

	for(int i = 0, k = 0; i < meidHex.size(); i+=2, k++){
		data[k] = strtol(meidHex.substr(i, 2).c_str(), nullptr, 16);
	}
	
	boost::uuids::detail::sha1 sha1;
	sha1.process_bytes(data, sizeof(data));
	sha1.get_digest(hash);

	ss << std::hex << hash[4];

	std::string pesn = ss.str();

	pesn.replace(pesn.begin(), pesn.begin() + 2, "80");

	ret.p1 = strtoul(pesn.substr(0,2).c_str(), nullptr, 16);
	ret.p2 = strtoul(pesn.substr(2).c_str(), nullptr, 16);

	return ret;
#endif
}
