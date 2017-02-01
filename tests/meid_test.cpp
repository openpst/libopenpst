#include "util/meid_converter.h"

using namespace OpenPST;

int main() {
	MeidConverter converter;

	std::string t1 = "270113181116019285";
	std::string t2 = "A1000023F46F55";
	std::string t3 = "12806266744";
	std::string t4 = "805F9F78";

	std::cout << t1.size() << " " << t2.size() << " " << t3.size() << " " << t4.size() << std::endl;

	try {
		std::cout << "From MEID DEC :" << t1 << std::endl;
		MeidInfo test1 = converter.convert(t1);
		std::cout << test1.getMeidDec() << std::endl;
		std::cout << test1.getMeidHex() << std::endl;
		std::cout << test1.getEsnDec() << std::endl;
		std::cout << test1.getEsnHex() << std::endl << std::endl;

		std::cout << "From MEID HEX :" << t2 << std::endl;
		MeidInfo test2 = converter.convert(t2);
		std::cout << test2.getMeidDec() << std::endl;
		std::cout << test2.getMeidHex() << std::endl;
		std::cout << test2.getEsnDec() << std::endl;
		std::cout << test2.getEsnHex() << std::endl << std::endl;

		std::cout << "From ESN DEC :" << t3 << std::endl;
		MeidInfo test3 = converter.convert(t3);
		std::cout << test3.getMeidDec() << std::endl;
		std::cout << test3.getMeidHex() << std::endl;
		std::cout << test3.getEsnDec() << std::endl;
		std::cout << test3.getEsnHex() << std::endl << std::endl;

		std::cout << "From ESN HEX :" << t4 << std::endl;
		MeidInfo test4 = converter.convert(t4);
		std::cout << test4.getMeidDec() << std::endl;
		std::cout << test4.getMeidHex() << std::endl;
		std::cout << test4.getEsnDec() << std::endl;
		std::cout << test4.getEsnHex() << std::endl << std::endl;


	} catch (...) {
		std::cerr << "Unhandled Exception" << std::endl;
	}

	return 0;
}
