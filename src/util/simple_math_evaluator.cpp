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

#include "util/simple_math_evaluator.h"
#include <iostream>
#include <sstream>

using namespace OpenPST;

SimpleMathEvaluator::SimpleMathEvaluator()
{

}

SimpleMathEvaluator::~SimpleMathEvaluator()
{

}

std::string SimpleMathEvaluator::evaluate(const std::string& expr)
{   
    std::string e = expr;

    std::string::iterator cur = e.begin();
    std::string::iterator end = e.end();

    output.empty();
    op.empty();


    while (cur != end) {
        SimpleMathEvaluatorToken token = readNextToken(cur, end);
        
        if (token.value.size()) {
            if (token.type == kSimpleMathEvaluatorTokenTypeOperator) {
                if (op.size()) {
                    auto top = op.top();
                    if (top.type == kSimpleMathEvaluatorTokenTypeOperator) {
                        int topOpType = getOperatorType(top.value[0]);
                        int curOpType = getOperatorType(token.value[0]);
                        if (
                            (curOpType == kSimpleMathEvaluatorOperatorTypeMultiply &&
                            topOpType == kSimpleMathEvaluatorOperatorTypeDivide) ||
                            (curOpType ==  kSimpleMathEvaluatorOperatorTypeDivide &&
                            topOpType == kSimpleMathEvaluatorOperatorTypeMultiply)
                        ) {
                            output.push(top);
                            op.pop();
                        } else if (
                            (curOpType == kSimpleMathEvaluatorOperatorTypeAdd &&
                            topOpType == kSimpleMathEvaluatorOperatorTypeSubtract) ||
                            (curOpType ==  kSimpleMathEvaluatorOperatorTypeSubtract &&
                            topOpType == kSimpleMathEvaluatorOperatorTypeAdd)
                        ) {
                            output.push(top);
                            op.pop();
                        }
                    }
                }
                op.push(token);
            } else if(token.type == kSimpleMathEvaluatorTokenTypeParenthesisLeft) {
                op.push(token);
            } else if(token.type == kSimpleMathEvaluatorTokenTypeParenthesisRight) {
                
                auto top = op.top();
                
                while (top.type != kSimpleMathEvaluatorTokenTypeParenthesisLeft) {
                    output.push(top);
                    op.pop();
                    if (!op.size()) {
                        throw std::invalid_argument("Bad Expression");
                    }
                    top = op.top();
                }

                op.pop();
            } else {
                output.push(token);
            }

        } else {
            throw std::invalid_argument("Empty Token");
        }
    }

    while(op.size()) {
        output.push(op.top());
        op.pop();
    }

    /*if (output.back().type == kSimpleMathEvaluatorTokenTypeParenthesisLeft) {
        throw();
    }*/

    int total = 0;
    while(output.size()) {
        int l;
        int r;

        auto left  = output.front();
        
        output.pop(); 
        
        if (left.type == kSimpleMathEvaluatorTokenTypeInt) {
            if (left.value.find("0x") != std::string::npos) {
                l = std::strtoul(left.value.c_str(), nullptr, 16);
            } else {
                l = std::strtoul(left.value.c_str(), nullptr, 10);
            }            
        } else if (left.type == kSimpleMathEvaluatorTokenTypeOperator) {
            throw std::invalid_argument("Invalid Expression");
        }

        auto right = output.front();
        
        output.pop(); 

        if (right.type == kSimpleMathEvaluatorTokenTypeInt) {
            if (right.value.find("0x") != std::string::npos) {
                r = std::strtoul(right.value.c_str(), nullptr, 16);
            } else {
                r = std::strtoul(right.value.c_str(), nullptr, 10);
            }    
        } else if (right.type == kSimpleMathEvaluatorTokenTypeOperator) {
            r = l;
            l = total;
        }

        SimpleMathEvaluatorToken op;

        if (right.type == kSimpleMathEvaluatorTokenTypeOperator){
            op = right;
        } else {
             
            op = output.front();   
            output.pop();   
        }
    
        if (op.type != kSimpleMathEvaluatorTokenTypeOperator) {
            throw std::invalid_argument("Invalid Expression");
        }
        
        switch(getOperatorType(op.value[0])) {
            case kSimpleMathEvaluatorOperatorTypeAdd:
                total = l + r;
                break;
            case kSimpleMathEvaluatorOperatorTypeSubtract:
                total = l - r;
                break;
            case kSimpleMathEvaluatorOperatorTypeMultiply:
                total = l * r;
                break;
            case kSimpleMathEvaluatorOperatorTypeDivide:
                total = l / r;
                break;
        }
    }
    
    return std::to_string(total);  
}

SimpleMathEvaluatorToken SimpleMathEvaluator::readNextToken(std::string::iterator& cur, std::string::iterator& end)
{
    SimpleMathEvaluatorToken ret;

    while(cur != end) {
        char v = *cur;
        if (isOperator(v)) {
            if (ret.value.size()) {
                break;
            }
            ret.type   = kSimpleMathEvaluatorTokenTypeOperator;
            ret.value.append(&v, sizeof(v));
            cur++;
            break;
        } else if (v == ')' || v == '(') {            
            if (ret.value.size()) {
                break;
            }
            ret.type = v == '(' ? kSimpleMathEvaluatorTokenTypeParenthesisLeft : kSimpleMathEvaluatorTokenTypeParenthesisRight;
            ret.value.append(&v, sizeof(v));
            cur++;
            break;
        } else {
            ret.type = kSimpleMathEvaluatorTokenTypeInt;
        }

        if (v != ' ') {
            ret.value.append(&v, sizeof(v));
        } 
        cur++;
    }    

    return ret;
}


bool SimpleMathEvaluator::isOperator(char v)
{   
    std::string operators = "*/+-";
    for (int i = 0; i < operators.size(); i++) {
        char c = operators[i];
        if (c == v) {
            return true;
        }
    }
    return false;
}

int SimpleMathEvaluator::getOperatorType(char v)
{   
    switch(v) {
        case '+':
            return kSimpleMathEvaluatorOperatorTypeAdd;
            break;
        case '-':
            return kSimpleMathEvaluatorOperatorTypeSubtract;
            break;
        case '*':
            return kSimpleMathEvaluatorOperatorTypeMultiply;
            break;
        case '/':
            return kSimpleMathEvaluatorOperatorTypeDivide;
            break;
    }
    return kSimpleMathEvaluatorOperatorTypeNone;
}