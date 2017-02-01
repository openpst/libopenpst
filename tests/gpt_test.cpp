#include "definitions.h"
#include "util/gpt_parser.h"

using namespace OpenPST;

int main(int argc, char**argv) {
	GptParser parser;

	if (argc < 2) {
		std::cerr << "Provide path to file as argument" << std::endl;
		return 1;
	}

	try {
		GptInfo info = parser.parse(argv[1], kGptParserFlagAll, 0);

		for (auto &entry : info.entries) {
			std::cout << parser.getEntryUUID(&entry) << std::endl;
		}

	} catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	} catch (std::out_of_range& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "UNEXPECTED ERROR" << std::endl;
		return 1;
	}
	return 0;
}
