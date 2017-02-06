/**
* LICENSE PLACEHOLDER
*
* @file math_string_evaluator.cpp
* @class MathStringEvaluator
* @package openpst/libopenpst
* @brief Parses simple string math expressions. 
*
* @author Gassan Idriss <ghassani@gmail.com>
*/

#include "util/math_string_evaluator.h"
#include <iostream>
#include <sstream>

using namespace OpenPST;

MathStringEvaluator::MathStringEvaluator()
{

}

MathStringEvaluator::~MathStringEvaluator()
{

}

std::string MathStringEvaluator::evaluate(const std::string& expr)
{   
    // https://en.wikipedia.org/wiki/Shunting-yard_algorithm
    //std::cout << "Evaluating: " << expr << std::endl;

    auto tokens = tokenize(expr);

    outstack.empty();
    opstack.empty();
    
    MathStringEvaluatorToken lastToken;

    for (auto &token : tokens) {    	

    	if (token.type == kMathStringEvaluatorTokenTypeValue) {
    		pushOut(std::strtoul(token.value.c_str(), nullptr, 10));
            lastToken = token;
    		continue;
    	} else if (token.type == kMathStringEvaluatorTokenTypeOperator) {
    		
    		auto op = getOperator(token);
				

    		/*
            TODO: Fix unary negate
            if (
    			(lastToken.value.size() && isOperator(lastToken.value[0]) && opstack.top()->op !=')' && op->op == '-' ) ||
    			(!opstack.size() && op->op == '-')
    		) {
    			op = getOperator('_');    			
            }*/

            if (token.value[0] == '(') {
                pushOp(op);
                lastToken = token;
    			continue;
    		} else if (token.value[0] == ')') {
                if (!opstack.size()) {
                    throw MathStringEvaluatorError("Bad expression");
                }

                while(opstack.size() && opstack.top()->op != '(') {                    
    				execOp(popOp());
    			}
                continue;			
    		}

            if(opstack.size() && op->associativity == kMathStringEvaluatorAssociativityRight) {
                while(opstack.size() && op->precedence < opstack.top()->precedence) {
                    execOp(popOp());
                } 
            } else if(opstack.size() && op->associativity == kMathStringEvaluatorAssociativityLeft) {
                while(opstack.size() && op->precedence <= opstack.top()->precedence) {
                    execOp(popOp());
                }
            }

            pushOp(op);
            lastToken = token;
    	}
    }

    while(opstack.size()) {
        execOp(popOp());
    }

    float result = popOut();

    //std::cout << "Result: " << result << std::endl;

    if (result == (int)result) {
    	return std::to_string((int)result);
    }
    return std::to_string(result);
}

void MathStringEvaluator::execOp(MathStringEvaluatorOp* op)
{
    if (op->op == '(') {            
        return;
    }

    float a1 = popOut();

    if(op->unary) {
        pushOut(op->evaluate(a1, 0));
    } else {        
        float a2 = popOut();
        pushOut(op->evaluate(a2, a1));
    }
}

std::vector<MathStringEvaluatorToken> MathStringEvaluator::tokenize(const std::string& expr)
{
    std::string e = expr;
    std::vector<MathStringEvaluatorToken>  ret;

    std::string::iterator cur = e.begin();
    std::string::iterator end = e.end();

    // parse the string, token by token
    while (cur != end) {
        auto token = nextToken(cur, end);
        
        if (!token.value.size()) {
            throw MathStringEvaluatorError("Empty Token");
        }

        ret.push_back(token);
    }

    return ret;
}

MathStringEvaluatorToken MathStringEvaluator::nextToken(std::string::iterator& cur, std::string::iterator& end)
{
    MathStringEvaluatorToken ret;

    while(cur != end) {
        char v = *cur;
        if (isOperator(v)) {            
            if (ret.value.size()) {
                break;
            }
            ret.type = kMathStringEvaluatorTokenTypeOperator;
            ret.value.append(&v, sizeof(v));
            cur++;
            break;        
        } else {            
            ret.type = kMathStringEvaluatorTokenTypeValue;
        }

        if (!std::isspace(v)) {
            ret.value.append(&v, sizeof(v));
        } 
        cur++;
    }    

    return ret;
}

bool MathStringEvaluator::isOperator(char v)
{   
    for (int i = 0; i < sizeof(operators)/sizeof(MathStringEvaluatorOp); i++) {
    	if (v == operators[i].op) {
    		return true;
    	}
    }
    return false;
}

MathStringEvaluatorOp* MathStringEvaluator::getOperator(const MathStringEvaluatorToken& token)
{
	if (!token.value.size() || !isOperator(token.value[0])) {
		throw MathStringEvaluatorError("Token is not an operator");
	}

    for (int i = 0; i < sizeof(operators)/sizeof(MathStringEvaluatorOp); i++) {
    	if (token.value[0] == operators[i].op) {
    		return &operators[i];
    	}
    }

    throw MathStringEvaluatorError("Operator not found");
}

MathStringEvaluatorOp* MathStringEvaluator::getOperator(char v)
{
    for (int i = 0; i < sizeof(operators)/sizeof(MathStringEvaluatorOp); i++) {
    	if (v == operators[i].op) {
    		return &operators[i];
    	}
    }

    throw MathStringEvaluatorError("Operator not found");
}

MathStringEvaluatorOp* MathStringEvaluator::popOp()
{
    
    if (!opstack.size()) {
        throw MathStringEvaluatorError("Empty output stack");
    }
    auto op = opstack.top();
    opstack.pop();
    return op;
}

size_t MathStringEvaluator::pushOp(MathStringEvaluatorOp* op)
{
    opstack.push(op);
    return opstack.size();
}

float MathStringEvaluator::popOut()
{
    if (!outstack.size()) {
        throw MathStringEvaluatorError("Empty output stack");
    }
    float ret = outstack.top();
    outstack.pop();
    return ret;
}

size_t MathStringEvaluator::pushOut(float v)
{
    outstack.push(v);
    return outstack.size();
}

float MathStringEvaluator::evalAdd(float a, float b) {
    return a + b;
}

float MathStringEvaluator::evalSubtract(float a, float b) {
    return a - b;
}

float MathStringEvaluator::evalMultiply(float a, float b) {
    return a * b;
}

float MathStringEvaluator::evalDivide(float a, float b) {
    if (b == 0) {
        throw MathStringEvaluatorError("Division by zero");
    }
    return a / b;
}

float MathStringEvaluator::evalPow(float a, float b) {
    return pow(a,b);
}

float MathStringEvaluator::evalMod(float a, float b) {
    if (b == 0) {
        throw MathStringEvaluatorError("Division by zero");
    }
    return fmod(a, b);
}

float MathStringEvaluator::evalUminus(float a, float b) {
    return -a;
}

