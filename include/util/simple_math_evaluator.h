/**
* LICENSE PLACEHOLDER
*
* @file simple_math_evaluator.h
* @class SimpleMathEvaluator
* @package openpst/libopenpst
* @brief Parses simple string math expressions. 
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#pragma once

#include "definitions.h"
#include <string>
#include <queue>
#include <stack>
#include <math.h>

namespace OpenPST {

	enum SimpleMathEvaluatorTokenType : int {
		kSimpleMathEvaluatorTokenTypeValue = 0,
		kSimpleMathEvaluatorTokenTypeOperator,
	};

	enum SimpleMathEvaluatorAssociativity : int {
		kSimpleMathEvaluatorAssociativityNone = 0,
		kSimpleMathEvaluatorAssociativityLeft,
		kSimpleMathEvaluatorAssociativityRight
	};

	struct SimpleMathEvaluatorOp {
		char op;
		int precedence;
		int associativity;
		int unary;
		int (*evaluate)(int a1, int a2);
	};

	struct SimpleMathEvaluatorToken {
		int type;
		std::string value;
	};

	class SimpleMathEvaluator {

		protected:
			std::stack<SimpleMathEvaluatorOp*> opstack;
			std::stack<int> outstack;
			
			SimpleMathEvaluatorOp operators[9] = {
				{'_', 10, kSimpleMathEvaluatorAssociativityRight, 1,  SimpleMathEvaluator::evalUminus},
				{'^', 9,  kSimpleMathEvaluatorAssociativityRight, 0,  SimpleMathEvaluator::evalPow},
				{'*', 8,  kSimpleMathEvaluatorAssociativityLeft,  0,  SimpleMathEvaluator::evalMultiply},
				{'/', 8,  kSimpleMathEvaluatorAssociativityLeft,  0,  SimpleMathEvaluator::evalDivide},
				{'%', 8,  kSimpleMathEvaluatorAssociativityLeft,  0,  SimpleMathEvaluator::evalMod},
				{'+', 5,  kSimpleMathEvaluatorAssociativityLeft,  0,  SimpleMathEvaluator::evalAdd},
				{'-', 5,  kSimpleMathEvaluatorAssociativityLeft,  0,  SimpleMathEvaluator::evalSubtract},
				{'(', 0,  kSimpleMathEvaluatorAssociativityNone,  0,  nullptr},
				{')', 0,  kSimpleMathEvaluatorAssociativityNone,  0,  nullptr}
			};

		public:
			SimpleMathEvaluator();
			~SimpleMathEvaluator();
			
			std::string evaluate(const std::string& expr);
		protected:
			std::vector<SimpleMathEvaluatorToken> tokenize(const std::string& expr);

			SimpleMathEvaluatorToken readNextToken(std::string::iterator& cur, std::string::iterator& end);
			
			bool isOperator(char v);
						
			SimpleMathEvaluatorOp* getOperator(const SimpleMathEvaluatorToken& token);

			SimpleMathEvaluatorOp* popOp()
			{
				auto ret = opstack.top();
				opstack.pop();
				return ret;
			}

			size_t pushOp(SimpleMathEvaluatorOp* op)
			{
				opstack.push(op);
				return opstack.size();
			}

			int popOut()
			{
				auto ret = outstack.top();
				outstack.pop();
				return ret;
			}

			size_t pushOut(int v)
			{
				outstack.push(v);
				return outstack.size();
			}

			static int evalAdd(int a, int b) {
				return a + b;
			}

			static int evalSubtract(int a, int b) {
				return a - b;
			}

			static int evalMultiply(int a, int b) {
				return a * b;
			}

			static int evalDivide(int a, int b) {
				return a / b;
			}

			static int evalPow(int a, int b) {
				return pow(a,b);
			}

			static int evalMod(int a, int b) {
				return a % b;
			}

			static int evalUminus(int a, int b) {
				return a;
			}
	};
}