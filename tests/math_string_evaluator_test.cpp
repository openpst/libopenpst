#include <iostream>
#include "util/math_string_evaluator.h"

using OpenPST::MathStringEvaluator;

int main(int argc, char** argv) {
	
	MathStringEvaluator eval;
	std::vector<std::string> tests;

	tests.push_back("1+2");
	tests.push_back("(1+2)");
	tests.push_back("(512*128)-16896");
	tests.push_back("(512*128)/16896");
	tests.push_back("3+4*2/(1-5)^2^3");
	tests.push_back("3+4*2/(1-5)");

	for (auto &test : tests) {
		std::string res;
		std::cout << "Test: " << test;
		try {
			res = eval.evaluate(test);
			std::cout << " Value: " << res << std::endl;
		} catch(std::invalid_argument& e) {
			std::cout << " Failed: " << e.what() << std::endl;	
		} catch (...) {
			std::cout << " Failed: Unexpected Error" << std::endl;
			continue;
		}

	}
	return 0;
}