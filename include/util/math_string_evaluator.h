/**
*
* (c) Gassan Idriss <ghassani@gmail.com>
* 
* This file is part of libopenpst.
* 
* libopenpst is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* libopenpst is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with libopenpst. If not, see <http://www.gnu.org/licenses/>.
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

		class MathStringEvaluatorError : public std::exception
		{
			const MathStringEvaluatorError& operator=(MathStringEvaluatorError);
			std::string _what;
			public:
				MathStringEvaluatorError(std::string message) : _what(message)  { }
				MathStringEvaluatorError(const MathStringEvaluatorError& second) : _what(second._what) {}
				virtual ~MathStringEvaluatorError() throw() {}
				virtual const char* what() const throw () {
					return _what.c_str();
				}

		};	
}