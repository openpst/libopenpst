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

namespace OpenPST {

	enum SimpleMathEvaluatorTokenType : int {
		kSimpleMathEvaluatorTokenTypeInt = 0,
		kSimpleMathEvaluatorTokenTypeFloat,
		kSimpleMathEvaluatorTokenTypeOperator,
		kSimpleMathEvaluatorTokenTypeParenthesisLeft,
		kSimpleMathEvaluatorTokenTypeParenthesisRight ,
	};

	enum SimpleMathEvaluatorOperatorType : int {
		kSimpleMathEvaluatorOperatorTypeNone = 0,
		kSimpleMathEvaluatorOperatorTypeAdd,
		kSimpleMathEvaluatorOperatorTypeSubtract,
		kSimpleMathEvaluatorOperatorTypeMultiply,
		kSimpleMathEvaluatorOperatorTypeDivide
	};

	struct SimpleMathEvaluatorToken {
		int type;
		std::string value;
	};

	class SimpleMathEvaluator {

		protected:
			std::stack<SimpleMathEvaluatorToken> op;
			std::queue<SimpleMathEvaluatorToken> output;						
		public:
			SimpleMathEvaluator();
			~SimpleMathEvaluator();
			
			std::string evaluate(const std::string& expr);
		protected:
			SimpleMathEvaluatorToken readNextToken(std::string::iterator& cur, std::string::iterator& end);
			bool isOperator(char v);
			int getOperatorType(char v);
	};
}