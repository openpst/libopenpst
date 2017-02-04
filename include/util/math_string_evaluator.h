/**
* LICENSE PLACEHOLDER
*
* @file math_string_evaluator.h
* @class MathStringEvaluator
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

	enum MathStringEvaluatorTokenType {
		kMathStringEvaluatorTokenTypeValue = 0,
		kMathStringEvaluatorTokenTypeOperator,
	};

	enum MathStringEvaluatorAssociativity {
		kMathStringEvaluatorAssociativityNone = 0,
		kMathStringEvaluatorAssociativityLeft,
		kMathStringEvaluatorAssociativityRight
	};

	struct MathStringEvaluatorOp {
		char op;
		int precedence;
		int associativity;
		int unary;
		float (*evaluate)(float, float);
	};

	struct MathStringEvaluatorToken {
		int type;
		std::string value;
	};

	class MathStringEvaluator {

		protected:			
			
			MathStringEvaluatorOp operators[9] = {
				{'_', 8,  kMathStringEvaluatorAssociativityRight, 1,  MathStringEvaluator::evalUminus},
				{'^', 6,  kMathStringEvaluatorAssociativityRight, 0,  MathStringEvaluator::evalPow},
				{'*', 4,  kMathStringEvaluatorAssociativityLeft,  0,  MathStringEvaluator::evalMultiply},
				{'/', 4,  kMathStringEvaluatorAssociativityLeft,  0,  MathStringEvaluator::evalDivide},
				{'%', 4,  kMathStringEvaluatorAssociativityLeft,  0,  MathStringEvaluator::evalMod},
				{'+', 2,  kMathStringEvaluatorAssociativityLeft,  0,  MathStringEvaluator::evalAdd},
				{'-', 2,  kMathStringEvaluatorAssociativityLeft,  0,  MathStringEvaluator::evalSubtract},
				{'(', 0,  kMathStringEvaluatorAssociativityNone,  0,  nullptr},
				{')', 0,  kMathStringEvaluatorAssociativityNone,  0,  nullptr}
			};
			std::stack<MathStringEvaluatorOp*> opstack;
			std::stack<float> outstack;
		public:
			MathStringEvaluator();
			~MathStringEvaluator();
			
			std::string evaluate(const std::string& expr);

			bool isOperator(char v);
		protected:			
			std::vector<MathStringEvaluatorToken> tokenize(const std::string& expr);

			MathStringEvaluatorToken nextToken(std::string::iterator& cur, std::string::iterator& end);
			
			void execOp(MathStringEvaluatorOp* op);
			
			
						
			MathStringEvaluatorOp* getOperator(const MathStringEvaluatorToken& token);

			MathStringEvaluatorOp* getOperator(char v);
			
			MathStringEvaluatorOp* popOp();

			size_t pushOp(MathStringEvaluatorOp* op);

			float popOut();

			size_t pushOut(float v);

			static float evalAdd(float a, float b);

			static float evalSubtract(float a, float b);

			static float evalMultiply(float a, float b);

			static float evalDivide(float a, float b);

			static float evalPow(float a, float b);

			static float evalMod(float a, float b);

			static float evalUminus(float a, float b);
	};
}