#include <iostream>
#include "util/simple_math_evaluator.h"

using OpenPST::SimpleMathEvaluator;

int main(int argc, char** argv) {
	
	SimpleMathEvaluator eval;
	std::vector<std::string> tests;

	tests.push_back("1+2");
	tests.push_back("(1+2)");
	tests.push_back("(512*128)-16896");
	tests.push_back("(512*128)/16896");


	for (auto &test : tests) {
		std::cout << "Testing: " << test << std::endl;

		eval.evaluate(test);
	}
	return 0;
}
