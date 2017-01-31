#include "definitions.h"
#include "util/meid_converter.h"

int main() {
	MeidConverter conv;

	std::string t1 = "270113181116019285";
	std::string t2 = "A1000023F46F55";
	std::string t3 = "12806266744";
	std::string t4 = "805F9F78";


	try {
		MeidInfo test1 = conv.convert(t1);

		std::cout << "MEID (DEC)" << test1.meidDec << std::endl;
		std::cout << "MEID (HEX)" << test1.meidHex << std::endl;
		std::cout << "pESN (DEC)" << test1.pEsnDec << std::endl;
		std::cout << "pESN (HEX)" << test1.pEsnHex << std::endl;
	} catch (...) {
		std::cerr << "Unhandled Exception" << std::endl;
	}

	return 0;
}
