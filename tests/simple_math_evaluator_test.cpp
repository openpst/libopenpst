#include <iostream>
#include "util/simple_math_evaluator.h"

using OpenPST::SimpleMathEvaluator;

int main(int argc, char** argv) {
	
	SimpleMathEvaluator eval;
	std::vector<std::string> tests;

	//tests.push_back("1+2");
	//tests.push_back("(1+2)");
	//tests.push_back("(512*128)-16896");
	//tests.push_back("(512*128)/16896");
	//tests.push_back("3+4*2/(1-5)^2^3");
	tests.push_back("3+4*2/(1-5)");

	for (auto &test : tests) {
		std::cout << "Testing: " << test << std::endl;
		std::string res;

		try {
			res = eval.evaluate(test);
			std::cout << "\tValue: " << res << std::endl;
		} catch(std::invalid_argument& e) {
			std::cout << "\tFailed: " << e.what() << std::endl;			
		} catch (...) {
			std::cout << "\tFailed: Unexpected Error" << std::endl;
			continue;
		}
		
	}
	return 0;
}
