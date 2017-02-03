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
    auto tokens = tokenize(expr);

    outstack.empty();
    opstack.empty();
    
    for (auto &token : tokens) {
    	if (token.type == kMathStringEvaluatorTokenTypeValue) {
            int v = std::stoul(token.value.c_str(), nullptr, 10);
    		pushOut(v);
    		continue;
    	} else if (token.type == kMathStringEvaluatorTokenTypeOperator) {
    		
            if (token.value[0] == '(') {
                pushOp(getOperator(token));
    			continue;
    		} else if (token.value[0] == ')') {
                if (!opstack.size()) {
                    throw std::invalid_argument("Bad expression");
                }

                while(opstack.size() && opstack.top()->op != '(') {                    
    				execOp(popOp());
    			}
                continue;			
    		}

            auto op = getOperator(token);

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
    	}
    }

    while(opstack.size()) {
        execOp(popOp());
    }

    return std::to_string(popOut());
}

void MathStringEvaluator::execOp(MathStringEvaluatorOp* op)
{
    if (op->op == '(') {            
        return;
    }

    int a1 = popOut();

    if(op->unary) {
        pushOut(op->evaluate(a1, 0));
    } else {        
        int a2 = popOut();
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
            throw std::invalid_argument("Empty Token");
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
		throw std::invalid_argument("Token is not an operator");
	}

    for (int i = 0; i < sizeof(operators)/sizeof(MathStringEvaluatorOp); i++) {
    	if (token.value[0] == operators[i].op) {
    		return &operators[i];
    	}
    }

    throw std::invalid_argument("Operator not found");
}

MathStringEvaluatorOp* MathStringEvaluator::popOp()
{
    
    if (!opstack.size()) {
        throw std::invalid_argument("Empty output stack");
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

int MathStringEvaluator::popOut()
{
    if (!outstack.size()) {
        throw std::invalid_argument("Empty output stack");
    }
    int ret = outstack.top();
    outstack.pop();
    return ret;
}

size_t MathStringEvaluator::pushOut(int v)
{
    outstack.push(v);
    return outstack.size();
}

int MathStringEvaluator::evalAdd(int a, int b) {
    return a + b;
}

int MathStringEvaluator::evalSubtract(int a, int b) {
    return a - b;
}

int MathStringEvaluator::evalMultiply(int a, int b) {
    return a * b;
}

int MathStringEvaluator::evalDivide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a / b;
}

int MathStringEvaluator::evalPow(int a, int b) {
    return pow(a,b);
}

int MathStringEvaluator::evalMod(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("Division by zero");
    }
    return a % b;
}

int MathStringEvaluator::evalUminus(int a, int b) {
    return -a;
}

